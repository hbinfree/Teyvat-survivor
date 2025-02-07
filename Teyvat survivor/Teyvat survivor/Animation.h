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
	int timer = 0;              // 动画计时器
	int idx_frame = 0;          // 动画帧索引
	int interval_ms = 0;
	//std::vector<std::unique_ptr<IMAGE>> frame_list; // 使用智能指针

private:
	Altas* anim_altas;
};