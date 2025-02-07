#pragma once
#include <graphics.h>
#include <string>
#include <vector>
#include <memory>

// 全局常量
constexpr int PLAYER_ANIM_NUM = 6;   // 玩家角色动画帧总数
constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;
constexpr int BUTTON_WIDTH = 192;
constexpr int BUTTON_HEIGHT = 75;

// 全局状态
extern bool running;
extern bool is_game_started;
extern int idx_current_anim;

// 全局函数声明
inline void putimage_alpha(int x, int y, IMAGE* img)
{
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(NULL), x, y, w, h,
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });
}
