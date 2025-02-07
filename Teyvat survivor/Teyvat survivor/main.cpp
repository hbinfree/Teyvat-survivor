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

// 生成新的敌人
void TryGenerateEnemy(std::vector<Enemy*>& enemy_list);

// 更新子弹位置
void UpdateBullets(std::vector<Bullet>& bullet_list, const Player& player);

// 绘制玩家得分
void DrawPlayerScore(int score);

// 全局变量定义
bool running = true;
bool is_game_started = false;
int idx_current_anim = 0; // 当前动画帧索引

IMAGE img_player_left[PLAYER_ANIM_NUM];//玩家向左移动的动画数组
IMAGE img_player_right[PLAYER_ANIM_NUM];//玩家向右移动的动画数组

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
	// 把mus日录下的bgm.mp3文件规载到程序串
	// alias bgm: 给这个音乐文件创建一个别名 bgm。
	mciSendString(_T("open mus/bgm.mp3 alias bgm"), NULL, 0, NULL);
	mciSendString(_T("open mus/hit.wav alias hit"), NULL, 0, NULL);
	// 播放音乐
	// repeat： 循环播放音乐，播放到结尾后会自动从头开始。
	// from 0： 从第 0 毫秒开始播放（即音频的起始位置）。

	int score = 0;
	Player player;
	ExMessage msg;
	IMAGE img_background;
	IMAGE img_menu;
	std::vector<Enemy*> enemy_list;
	std::vector<Bullet> bullet_list(3);

	// 初始化按钮
	RECT region_btn_start_game, region_btn_quit_game;
	// 把按钮放置在屏幕中央
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

		// 读取操作
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
			// 数据处理

			// 尝试生成新的敌人
			TryGenerateEnemy(enemy_list);
			// 玩家移动
			player.Move();
			// 更新子弹位置
			UpdateBullets(bullet_list, player);
			//更新敌人位置
			for (Enemy* enemy : enemy_list)
				enemy->Move(player);

			//检测敌人和玩家的碰撞
			for (Enemy* enemy : enemy_list)
			{
				if (enemy->CheckPlayerCollision(player))
				{
					static TCHAR text[128];
					_stprintf_s(text, _T("最终得分：%d !"), score);
					//MessageBox(GetHWnd(), _T("扣“1”观看战败CG"), _T("游戏结束"), MB_OK);
					MessageBox(GetHWnd(), text, _T("游戏结束"), MB_OK);
					running = false;
					break;
				}
			}

			// 检测子弹和敌人的碰撞
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

			// 移除生命值归零的敌人
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

		//绘图
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


// 生成新的敌人
void TryGenerateEnemy(std::vector<Enemy*>& enemy_list)
{
	const int INTERVAL = 100; // 生成间隔
	static int counter = 0;
	if ((++counter) % INTERVAL == 0)
		enemy_list.push_back(new Enemy());
}

// 更新子弹位置
void UpdateBullets(std::vector<Bullet>& bullet_list, const Player& player)
{
	const double RADIAL_SPEED = 0.0045;                                // 径向波动速度
	const double TANGENT_SPEED = 0.0055;                               // 切向波动速度
	double radian_interval = 2 * 3.14159 / bullet_list.size();         // 子弹之间的弧度间隔
	POINT player_position = player.GetPosition();
	double radius = 100 + 25 * sin(GetTickCount() * RADIAL_SPEED);
	for (size_t i = 0; i < bullet_list.size(); i++)
	{
		double radian = GetTickCount() * TANGENT_SPEED + radian_interval * i; //当前子弹所在弧度值
		bullet_list[i].position.x = player_position.x + player.FRAME_WIDTH / 2 + (int)(radius * sin(radian));
		bullet_list[i].position.y = player_position.y + player.FRAME_HEIGHT / 2 + (int)(radius * cos(radian));
	}
}

// 绘制玩家得分
void DrawPlayerScore(int score)
{
	static TCHAR text[64];  // 定义静态字符数组，用于存储得分文本
	_stprintf_s(text, _T("当前玩家得分:%d"), score);  // 格式化字符串，生成得分信息

	setbkmode(TRANSPARENT);  // 设置背景模式为透明
	settextcolor(RGB(255, 85, 185));  // 设置文本颜色为粉紫色
	outtextxy(10, 10, text);  // 在 (10,10) 坐标位置绘制文本
}
