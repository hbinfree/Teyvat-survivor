#pragma once
#include <graphics.h>
#include <string>
#include <vector>
#include <memory>

class Altas
{
public:
	Altas(LPCTSTR path, int num);
	~Altas();

public:
	//std::vector<IMAGE*> frame_list;
	std::vector<std::unique_ptr<IMAGE>> frame_list; // 使用智能指针
};