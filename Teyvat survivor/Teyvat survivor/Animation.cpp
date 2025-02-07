#include "Animation.h"
#include "global.h"
#include <string>
#include <vector>
#include <memory>



Animation::Animation(Altas* altas, int interval)
{
	anim_altas = altas;
	interval_ms = interval;  //动画帧间隔时间，例如45就是每45ms动画帧索引+1
}

Animation::~Animation() = default;

void Animation::Play(int x, int y, int delta)
{
	timer += delta;          //delta是帧间隔，例如1000/144，每秒144帧动画，每帧间隔1000/144
	if (timer >= interval_ms)
	{
		idx_frame = (idx_frame + 1) % anim_altas->frame_list.size();
		timer = 0;
	}

	putimage_alpha(x, y, anim_altas->frame_list[idx_frame].get());
}