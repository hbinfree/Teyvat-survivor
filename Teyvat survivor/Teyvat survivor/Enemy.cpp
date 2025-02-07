#include <graphics.h>
#include "Animation.h"
#include "Enemy.h"

Enemy::Enemy()
{
	loadimage(&img_shadow, _T("img/shadow_enemy.png"));
	anim_left = new Animation(atlas_enemy_left, 45);     //向左移动的动画对象
	anim_right = new Animation(atlas_enemy_right, 45);   //向右移动的动画对象	

	enum class SpawnEdge
	{
		Up = 0,
		Down,
		Left,
		Right
	};

	// 将敌人放置在地图外边界处的随机位置
	SpawnEdge edge = (SpawnEdge)(rand() % 4);
	switch (edge)
	{
	case SpawnEdge::Up:
		position.x = rand() % WINDOW_WIDTH;     //上边界产生的敌人，x坐标随机
		position.y = -FRAME_HEIGHT;             //y坐标位置：自己的下边界位于地图的上边界
		break;
	case SpawnEdge::Down:
		position.x = rand() % WINDOW_WIDTH;
		position.y = WINDOW_HEIGHT;
		break;
	case SpawnEdge::Left:
		position.x = -FRAME_WIDTH;
		position.y = rand() % WINDOW_HEIGHT;
		break;
	case SpawnEdge::Right:
		position.x = WINDOW_WIDTH;
		position.y = rand() % WINDOW_HEIGHT;
		break;
	default:
		break;
	}
}

Enemy::~Enemy()
{
	delete anim_left;
	delete anim_right;
}

// 检查是否与子弹碰撞
bool Enemy::CheckBulletCollision(const Bullet& bullet)
{
	// 将子弹等效为点，判断点是否在敌人矩形内
	bool is_overlap_x = bullet.position.x >= position.x && bullet.position.x <= position.x + FRAME_WIDTH;
	bool is_overlap_y = bullet.position.y >= position.y && bullet.position.y <= position.y + FRAME_HEIGHT;
	return is_overlap_x && is_overlap_y;
}

// 检查是否与玩家碰撞
bool Enemy::CheckPlayerCollision(const Player& player)
{
	// 将敌人中心位置等效为点，判断点是否在玩家矩形内
	POINT check_position = { position.x + FRAME_WIDTH / 2,position.y + FRAME_HEIGHT / 2 };
	bool is_overlap_x = check_position.x >= player.GetPosition().x && check_position.x <= player.GetPosition().x + player.FRAME_WIDTH;
	bool is_overlap_y = check_position.y >= player.GetPosition().y && check_position.y <= player.GetPosition().y + player.FRAME_HEIGHT;
	return is_overlap_x && is_overlap_y;
}

// 追踪玩家移动
void Enemy::Move(const Player& player)
{
	const POINT& player_position = player.GetPosition();
	// 玩家位置与敌人位置作差，得到敌人的移动向量
	int dir_x = player_position.x - position.x;
	int dir_y = player_position.y - position.y;
	double len_dir = sqrt(dir_x * dir_x + dir_y * dir_y);
	if (len_dir != 0)
	{
		double normalized_x = dir_x / len_dir;
		double normalized_y = dir_y / len_dir;
		position.x += (int)(SPEED * normalized_x);
		position.y += (int)(SPEED * normalized_y);
	}

	// 更新敌人朝向
	if (dir_x < 0)
		facing_left = true;
	else if (dir_x > 0)
		facing_left = false;

}

// 绘制敌人
void Enemy::Draw(int delta)
{
	int pos_shadow_x = position.x + (FRAME_WIDTH / 2 - SHADOW_WIDTH / 2);  //让阴影居中
	int pos_shadow_y = position.y + FRAME_HEIGHT - 30;                     //让阴影放置在敌人的脚下（30个像素）
	putimage_alpha(pos_shadow_x, pos_shadow_y, &img_shadow);

	if (facing_left)
		anim_left->Play(position.x, position.y, delta);
	else
		anim_right->Play(position.x, position.y, delta);
}

// 收到攻击
void Enemy::Hurt()
{
	alive = false;
}

// 是否存活
bool Enemy::CheckAlive()
{
	return alive;
}