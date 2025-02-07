#pragma once
#include <windows.h>
#include "Button.h"

// ÍË³öÓÎÏ·°´Å¥
class QuitGameButton : public Button
{
public:
	QuitGameButton(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed);
	~QuitGameButton();

protected:
	void OnClick();
};