#pragma once
#include <graphics.h>
#include "Animation.h"
#include "Bullet.h"
#include "Player.h"

extern Altas* atlas_enemy_left;
extern Altas* atlas_enemy_right;

class Enemy
{
public:
	Enemy();

	~Enemy();

	// 检查是否与子弹碰撞
	bool CheckBulletCollision(const Bullet& bullet);

	// 检查是否与玩家碰撞
	bool CheckPlayerCollision(const Player& player);

	// 追踪玩家移动
	void Move(const Player& player);

	// 绘制敌人
	void Draw(int delta);

	// 收到攻击
	void Hurt();

	// 是否存活
	bool CheckAlive();

private:
	const int SPEED = 2;          // 速度
	const int FRAME_WIDTH = 80;   // 敌人宽度
	const int FRAME_HEIGHT = 80;  // 敌人高度
	const int SHADOW_WIDTH = 48;  // 敌人阴影宽度

private:
	POINT position = { 0,0 };
	IMAGE img_shadow;              //怪物底下的阴影动画
	Animation* anim_left;
	Animation* anim_right;
	bool facing_left = false;
	bool alive = true;

};