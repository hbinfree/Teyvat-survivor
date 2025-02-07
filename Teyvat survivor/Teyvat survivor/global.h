#pragma once
#include <graphics.h>
#include <string>
#include <vector>
#include <memory>

// ȫ�ֳ���
constexpr int PLAYER_ANIM_NUM = 6;   // ��ҽ�ɫ����֡����
constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;
constexpr int BUTTON_WIDTH = 192;
constexpr int BUTTON_HEIGHT = 75;

// ȫ��״̬
extern bool running;
extern bool is_game_started;
extern int idx_current_anim;

// ȫ�ֺ�������
inline void putimage_alpha(int x, int y, IMAGE* img)
{
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(NULL), x, y, w, h,
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });
}
