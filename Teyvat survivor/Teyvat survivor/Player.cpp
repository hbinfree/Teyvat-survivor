#include <graphics.h>
#include "Animation.h"
#include "Player.h"
#include "Altas.h"

Player::Player()
{
	loadimage(&img_shadow, _T("img/shadow_player.png"));
	anim_left = new Animation(atlas_player_left, 45);     //玩家向左移动的动画对象
	anim_right = new Animation(atlas_player_right, 45);   //玩家向右移动的动画对象	
}

Player::~Player()
{
	delete anim_left;
	delete anim_right;
}

// 处理玩家消息
void Player::ProcessEvent(const ExMessage& msg)
{
	switch (msg.message)
	{
	case WM_KEYDOWN:
		switch (msg.vkcode)
		{
		case VK_UP:
			is_move_up = true;
			break;
		case VK_DOWN:
			is_move_down = true;
			break;
		case VK_LEFT:
			is_move_left = true;
			break;
		case VK_RIGHT:
			is_move_right = true;
			break;
		}
		break;
	case WM_KEYUP:
		switch (msg.vkcode)
		{
		case VK_UP:
			is_move_up = false;
			break;
		case VK_DOWN:
			is_move_down = false;
			break;
		case VK_LEFT:
			is_move_left = false;
			break;
		case VK_RIGHT:
			is_move_right = false;
			break;
		}
		break;
	}
}

//处理玩家移动
void Player::Move()
{
	//归一化处理，避免斜方向上速度比正常快
	int dir_x = is_move_right - is_move_left;
	int dir_y = is_move_down - is_move_up;
	double len_dir = sqrt(dir_x * dir_x + dir_y * dir_y);
	if (len_dir != 0)
	{
		double normalized_x = dir_x / len_dir;
		double normalized_y = dir_y / len_dir;
		position.x += (int)(SPEED * normalized_x);
		position.y += (int)(SPEED * normalized_y);
	}

	//玩家位置校准，防止超出范围
	if (position.x < 0) position.x = 0;
	if (position.y < 0) position.y = 0;
	if (position.x + FRAME_WIDTH > WINDOW_WIDTH) position.x = WINDOW_WIDTH - FRAME_WIDTH;
	if (position.y + FRAME_HEIGHT > WINDOW_HEIGHT) position.y = WINDOW_HEIGHT - FRAME_WIDTH;
	/*
	if (is_move_up)player_pos.y -= PLAYER_SPEED;
	if (is_move_down)player_pos.y += PLAYER_SPEED;
	if (is_move_left)player_pos.x -= PLAYER_SPEED;
	if (is_move_right)player_pos.x += PLAYER_SPEED;
	*/
}

//绘制玩家
void Player::Draw(int delta)
{
	static int counter = 0;
	if (++counter % 5 == 0)
		idx_current_anim++;
	idx_current_anim = idx_current_anim % PLAYER_ANIM_NUM;

	int pos_shadow_x = position.x + (FRAME_WIDTH / 2 - SHADOW_WIDTH / 2); //让阴影居中
	int pos_shadow_y = position.y + FRAME_HEIGHT - 8;                     //让阴影放置在玩家的脚下（8个像素）
	putimage_alpha(pos_shadow_x, pos_shadow_y, &img_shadow);

	static bool facing_left = false;
	int dir_x = is_move_right - is_move_left;
	if (dir_x < 0)
		facing_left = true;
	else if (dir_x > 0)
		facing_left = false;

	if (facing_left)
		anim_left->Play(position.x, position.y, delta);
	else
		anim_right->Play(position.x, position.y, delta);

}

const POINT& Player::GetPosition() const
{
	return position;
}
	
