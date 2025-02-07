#include "Altas.h"

Altas::Altas(LPCTSTR path, int num)
{
	TCHAR path_file[256];
	for (size_t i = 0; i < num; i++)
	{
		_stprintf_s(path_file, path, i);

		/*IMAGE* frame = new IMAGE();
		loadimage(frame, path_file);
		frame_list.push_back(frame); // ��֡��ӵ��б���
		*/
		auto frame = std::make_unique<IMAGE>();
		loadimage(frame.get(), path_file);
		frame_list.push_back(std::move(frame)); // ʹ������ָ��
	}
}
Altas::~Altas() = default;

