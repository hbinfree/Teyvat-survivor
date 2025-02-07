#pragma once
#include <graphics.h>
#include "Animation.h"

extern Altas* atlas_player_left;
extern Altas* atlas_player_right;

class Player
{
public:
	const int FRAME_WIDTH = 80;   // 玩家宽度
	const int FRAME_HEIGHT = 80;  // 玩家高度

public:
	Player();

	~Player();

	// 处理玩家消息
	void ProcessEvent(const ExMessage& msg);

	//处理玩家移动
	void Move();

	//绘制玩家
	void Draw(int delta);

	const POINT& GetPosition() const;

private:
	const int SPEED = 3;          // 玩家速度
	const int SHADOW_WIDTH = 32;  // 阴影宽度

private:
	POINT position = { 500,500 };
	IMAGE img_shadow;              //玩家底下的阴影动画
	Animation* anim_left;
	Animation* anim_right;
	bool is_move_up = false;
	bool is_move_down = false;
	bool is_move_left = false;
	bool is_move_right = false;

};