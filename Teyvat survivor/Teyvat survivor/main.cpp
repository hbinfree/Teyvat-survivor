#include <graphics.h>
#include "Animation.h"
#include "Altas.h"
#include "Bullet.h"
#include "Button.h"
#include "Enemy.h"
#include "Player.h"
#include "global.h"
#include "StartGameButton.h"
#include "QuitGameButton.h"
#include <string>
#include <vector>
#include <memory>
#pragma comment(lib,"Winmm.lib")

// �����µĵ���
void TryGenerateEnemy(std::vector<Enemy*>& enemy_list);

// �����ӵ�λ��
void UpdateBullets(std::vector<Bullet>& bullet_list, const Player& player);

// ������ҵ÷�
void DrawPlayerScore(int score);

// ȫ�ֱ�������
bool running = true;
bool is_game_started = false;
int idx_current_anim = 0; // ��ǰ����֡����

IMAGE img_player_left[PLAYER_ANIM_NUM];//��������ƶ��Ķ�������
IMAGE img_player_right[PLAYER_ANIM_NUM];//��������ƶ��Ķ�������

Altas* atlas_player_left;
Altas* atlas_player_right;
Altas* atlas_enemy_left;
Altas* atlas_enemy_right;

int main()
{
	initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);

	atlas_player_left = new Altas(_T("img/player_left_%d.png"), 6);
	atlas_player_right = new Altas(_T("img/player_right_%d.png"), 6);
	atlas_enemy_left = new Altas(_T("img/enemy_left_%d.png"), 6);
	atlas_enemy_right = new Altas(_T("img/enemy_right_%d.png"), 6);

	// mci = Media Control Interface
	// ��mus��¼�µ�bgm.mp3�ļ����ص�����
	// alias bgm: ����������ļ�����һ������ bgm��
	mciSendString(_T("open mus/bgm.mp3 alias bgm"), NULL, 0, NULL);
	mciSendString(_T("open mus/hit.wav alias hit"), NULL, 0, NULL);
	// ��������
	// repeat�� ѭ���������֣����ŵ���β����Զ���ͷ��ʼ��
	// from 0�� �ӵ� 0 ���뿪ʼ���ţ�����Ƶ����ʼλ�ã���

	int score = 0;
	Player player;
	ExMessage msg;
	IMAGE img_background;
	IMAGE img_menu;
	std::vector<Enemy*> enemy_list;
	std::vector<Bullet> bullet_list(3);

	// ��ʼ����ť
	RECT region_btn_start_game, region_btn_quit_game;
	// �Ѱ�ť��������Ļ����
	region_btn_start_game.left = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
	region_btn_start_game.right = region_btn_start_game.left + BUTTON_WIDTH;
	region_btn_start_game.top = 430;
	region_btn_start_game.bottom = region_btn_start_game.top + BUTTON_HEIGHT;
	region_btn_quit_game.left = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
	region_btn_quit_game.right = region_btn_quit_game.left + BUTTON_WIDTH;
	region_btn_quit_game.top = 550;
	region_btn_quit_game.bottom = region_btn_quit_game.top + BUTTON_HEIGHT;

	StartGameButton btn_start_game = StartGameButton(region_btn_start_game,
		_T("img/ui_start_idle.png"), _T("img/ui_start_hovered.png"), _T("img/ui _start_pushed.png"));
	QuitGameButton btn_quit_game = QuitGameButton(region_btn_quit_game,
		_T("img/ui_quit_idle.png"), _T("img/ui_quit_hovered.png"), _T("img/ui_quit pushed.png"));

	loadimage(&img_menu, _T("img/menu.png"));
	loadimage(&img_background, _T("img/background.png"));

	BeginBatchDraw();


	while (running)
	{
		DWORD start_time = GetTickCount();

		// ��ȡ����
		while (peekmessage(&msg))
		{
			if (is_game_started)
				player.ProcessEvent(msg);
			else
			{
				btn_start_game.ProcessEvent(msg);
				btn_quit_game.ProcessEvent(msg);
			}
		}

		if (is_game_started)
		{
			// ���ݴ���

			// ���������µĵ���
			TryGenerateEnemy(enemy_list);
			// ����ƶ�
			player.Move();
			// �����ӵ�λ��
			UpdateBullets(bullet_list, player);
			//���µ���λ��
			for (Enemy* enemy : enemy_list)
				enemy->Move(player);

			//�����˺���ҵ���ײ
			for (Enemy* enemy : enemy_list)
			{
				if (enemy->CheckPlayerCollision(player))
				{
					static TCHAR text[128];
					_stprintf_s(text, _T("���յ÷֣�%d !"), score);
					//MessageBox(GetHWnd(), _T("�ۡ�1���ۿ�ս��CG"), _T("��Ϸ����"), MB_OK);
					MessageBox(GetHWnd(), text, _T("��Ϸ����"), MB_OK);
					running = false;
					break;
				}
			}

			// ����ӵ��͵��˵���ײ
			for (Enemy* enemy : enemy_list)
			{
				for (const Bullet& bullet : bullet_list)
				{
					if (enemy->CheckBulletCollision(bullet))
					{
						mciSendString(_T("play hit from 0"), NULL, 0, NULL);
						enemy->Hurt();
						score++;
					}
				}
			}

			// �Ƴ�����ֵ����ĵ���
			for (size_t i = 0; i < enemy_list.size(); i++)
			{
				Enemy* enemy = enemy_list[i];
				if (!enemy->CheckAlive())
				{
					std::swap(enemy_list[i], enemy_list.back());
					enemy_list.pop_back();
					delete enemy;
				}
			}
		}

		//��ͼ
		if (is_game_started)
		{
			cleardevice();
			putimage(0, 0, &img_background);
			player.Draw(1000 / 144);
			for (Enemy* enemy : enemy_list)
				enemy->Draw(1000 / 144);
			for (const Bullet& bullet : bullet_list)
				bullet.Draw();
			DrawPlayerScore(score);
		}
		else
		{
			putimage(0, 0, &img_menu);
			btn_start_game.Draw();
			btn_quit_game.Draw();
		}

		FlushBatchDraw();

		DWORD end_time = GetTickCount();
		DWORD delta_time = end_time - start_time;
		if (delta_time < 1000 / 144)
			Sleep(1000 / 144 - delta_time);
	}

	EndBatchDraw();
}


// �����µĵ���
void TryGenerateEnemy(std::vector<Enemy*>& enemy_list)
{
	const int INTERVAL = 100; // ���ɼ��
	static int counter = 0;
	if ((++counter) % INTERVAL == 0)
		enemy_list.push_back(new Enemy());
}

// �����ӵ�λ��
void UpdateBullets(std::vector<Bullet>& bullet_list, const Player& player)
{
	const double RADIAL_SPEED = 0.0045;                                // ���򲨶��ٶ�
	const double TANGENT_SPEED = 0.0055;                               // ���򲨶��ٶ�
	double radian_interval = 2 * 3.14159 / bullet_list.size();         // �ӵ�֮��Ļ��ȼ��
	POINT player_position = player.GetPosition();
	double radius = 100 + 25 * sin(GetTickCount() * RADIAL_SPEED);
	for (size_t i = 0; i < bullet_list.size(); i++)
	{
		double radian = GetTickCount() * TANGENT_SPEED + radian_interval * i; //��ǰ�ӵ����ڻ���ֵ
		bullet_list[i].position.x = player_position.x + player.FRAME_WIDTH / 2 + (int)(radius * sin(radian));
		bullet_list[i].position.y = player_position.y + player.FRAME_HEIGHT / 2 + (int)(radius * cos(radian));
	}
}

// ������ҵ÷�
void DrawPlayerScore(int score)
{
	static TCHAR text[64];  // ���徲̬�ַ����飬���ڴ洢�÷��ı�
	_stprintf_s(text, _T("��ǰ��ҵ÷�:%d"), score);  // ��ʽ���ַ��������ɵ÷���Ϣ

	setbkmode(TRANSPARENT);  // ���ñ���ģʽΪ͸��
	settextcolor(RGB(255, 85, 185));  // �����ı���ɫΪ����ɫ
	outtextxy(10, 10, text);  // �� (10,10) ����λ�û����ı�
}
