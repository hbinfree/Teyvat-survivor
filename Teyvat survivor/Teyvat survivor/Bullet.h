#pragma once
#include <graphics.h>

class Bullet
{
public:
	POINT position = { 0,0 };

public:
	Bullet();
	~Bullet();
	void Draw() const;

private:
	const int RADIUS = 10;
};