#pragma once
#include <windows.h>
#include "Button.h"

//��ʼ��Ϸ��ť
class StartGameButton : public Button
{
public:
	StartGameButton(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed);
	~StartGameButton();

protected:
	void OnClick();
};