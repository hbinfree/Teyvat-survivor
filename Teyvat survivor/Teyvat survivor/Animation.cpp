#include "Animation.h"
#include "global.h"
#include <string>
#include <vector>
#include <memory>



Animation::Animation(Altas* altas, int interval)
{
	anim_altas = altas;
	interval_ms = interval;  //����֡���ʱ�䣬����45����ÿ45ms����֡����+1
}

Animation::~Animation() = default;

void Animation::Play(int x, int y, int delta)
{
	timer += delta;          //delta��֡���������1000/144��ÿ��144֡������ÿ֡���1000/144
	if (timer >= interval_ms)
	{
		idx_frame = (idx_frame + 1) % anim_altas->frame_list.size();
		timer = 0;
	}

	putimage_alpha(x, y, anim_altas->frame_list[idx_frame].get());
}