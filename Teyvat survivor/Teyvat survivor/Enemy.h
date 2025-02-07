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

	// ����Ƿ����ӵ���ײ
	bool CheckBulletCollision(const Bullet& bullet);

	// ����Ƿ��������ײ
	bool CheckPlayerCollision(const Player& player);

	// ׷������ƶ�
	void Move(const Player& player);

	// ���Ƶ���
	void Draw(int delta);

	// �յ�����
	void Hurt();

	// �Ƿ���
	bool CheckAlive();

private:
	const int SPEED = 2;          // �ٶ�
	const int FRAME_WIDTH = 80;   // ���˿��
	const int FRAME_HEIGHT = 80;  // ���˸߶�
	const int SHADOW_WIDTH = 48;  // ������Ӱ���

private:
	POINT position = { 0,0 };
	IMAGE img_shadow;              //������µ���Ӱ����
	Animation* anim_left;
	Animation* anim_right;
	bool facing_left = false;
	bool alive = true;

};