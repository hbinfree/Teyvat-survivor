#pragma once
#include <graphics.h>
#include "Animation.h"

extern Altas* atlas_player_left;
extern Altas* atlas_player_right;

class Player
{
public:
	const int FRAME_WIDTH = 80;   // ��ҿ��
	const int FRAME_HEIGHT = 80;  // ��Ҹ߶�

public:
	Player();

	~Player();

	// ���������Ϣ
	void ProcessEvent(const ExMessage& msg);

	//��������ƶ�
	void Move();

	//�������
	void Draw(int delta);

	const POINT& GetPosition() const;

private:
	const int SPEED = 3;          // ����ٶ�
	const int SHADOW_WIDTH = 32;  // ��Ӱ���

private:
	POINT position = { 500,500 };
	IMAGE img_shadow;              //��ҵ��µ���Ӱ����
	Animation* anim_left;
	Animation* anim_right;
	bool is_move_up = false;
	bool is_move_down = false;
	bool is_move_left = false;
	bool is_move_right = false;

};