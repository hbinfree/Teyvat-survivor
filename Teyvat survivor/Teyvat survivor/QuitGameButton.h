#pragma once
#include <windows.h>
#include "Button.h"

// �˳���Ϸ��ť
class QuitGameButton : public Button
{
public:
	QuitGameButton(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed);
	~QuitGameButton();

protected:
	void OnClick();
};