#pragma once
#pragma comment(lib,"MSIMG32.LIB")
#include "Altas.h"
#include "global.h"

class Animation
{
public:
	Animation(Altas* altas, int interval);
	~Animation();

	void Play(int x, int y, int delta);

private:
	int timer = 0;              // ������ʱ��
	int idx_frame = 0;          // ����֡����
	int interval_ms = 0;
	//std::vector<std::unique_ptr<IMAGE>> frame_list; // ʹ������ָ��

private:
	Altas* anim_altas;
};