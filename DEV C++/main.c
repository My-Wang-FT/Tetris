#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "random.h"
#include "strlib.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>
 
#include "main.h"		// 主函数头文件 
#include "data.h"		// 数据库头文件 
#include "model.h" 		// 模型库头文件 
#include "ctrl.h"		// 控制库头文件 

void Main()
{
	//游戏数据初始化
	InitGame();
	// 图形窗口初始化设置 
	SetWindowTitle("Tetris"); 
	SetWindowSize( (double)451/GetXResolution(),(double)550/GetYResolution() );  
	InitGraphics();
	// 回调函数 
	registerMouseEvent(MouseCtrl);
	registerTimerEvent(TimeListener);
	registerKeyboardEvent(KeyBoardCtrl); 
	startTimer(0, 20);	// 绘图Timer 启动 
}

void TimeListener(int id)
{
	if ( id == 0 ) { 
		Draw(); 
	} else {
		Model_Flow();
	}  
}

//初始化游戏数据 
void InitGame(void)
{
	// 全局游戏信号变量初始化 
	Inf[Is_Menu]   = 1;	// 进入显示游戏界面 
	Inf[In_Game]   = 0;	// 游戏未进行 
	Inf[Name_Ret]  = 0; // 不输入用户名 
	Inf[Is_Help]   = 0;	// 不显示帮助 
	Inf[Is_Rank]   = 0; // 不显示排行榜 
	Inf[Is_Quit]   = 0; // 不退出游戏 
	
	// 图形交互变量初始化 
	Inf[Menu_Ret1] = 0; // 开始游戏 
	Inf[Menu_Ret2] = 0; // 读入存档 
	Inf[Menu_Ret3] = 0; // 排行榜 
	Inf[Menu_Ret4] = 0; // 帮助 
	Inf[Menu_Ret5] = 0; // 退出游戏 
	Inf[Tool_Ret1] = 0;	// 新游戏
	Inf[Tool_Ret2] = 0;	// 继续游戏
	Inf[Tool_Ret3] = 0;	// 排行榜
	Inf[Tool_Ret4] = 0;	// 帮助
	Inf[Tool_Ret5] = 0;	// 保存并退出
	
	Head = Con_Fileopen();		// 读入排行榜 
	Inf[Is_Save] = Con_Is_Save();  	// 判断是否有存档 
}
