#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "random.h"
#include "strlib.h"
#include "conio.h"

#ifndef _DRAW_H_
#define _DRAW_H_

// 模块间连接函数
void Draw(void);				// 供Timer调用 

// 模块内部函数 
// 基本函数 
static void DrawPixel(int x1, int y1, int x2, int y2);		// 绘制长方形 
static string Choose_Color(int x);							// 选择颜色 
static void ClearScreen(int x1, int y1, int x2, int y2);	// 局部清屏 
static void ShowStr(int x, int y, char *str);				// 向图形窗口输出文字函数
static void Single_Pen(int ret);							// 单段绘制 
static void DrawPixelLine(int x1, int y1, int x2, int y2);	// 绘制一条线

// 以下为复合功能函数 
static void ToolInterface(void);				// 工具界面 
static void GameInterface(void);				// 游戏界面 
static void MenuInterface(void); 				// 菜单界面 
static void CreateNewMap(void);					// 形成新的图形数据 
static void DrawNewMap(void);					// 参照形成的新数据绘图  
static void Interact_Window(void);				// 用户鼠标交互图形，鼠标移动指示框 
static void DrawPause(void);					// 绘制暂停符号 
static void DrawStart(void);					// 绘制开始符号 
static void NameFace(void);						// 用户数据读入窗口 
static void Draw_Go(void);						// 画Go 
static void DrawTet(void);						// 画Tetris 
static void DrawRank(void);                     // 画排行榜
static void DrawHelp(void);                     // 画帮助 
static void DrawMenuChoice(void);				// 画开始界面选项 
static void Delete_Gif(void);					// 消除动画 
static void End_Gif(void);						// 结束游戏动画 
 
// 七段数码管专用变量及函数  
static void Draw_Line(int len, int ret);									// 画单独一条线，可以虚画 
static void SignalDigitalNub(int len, int nub, int space);					// 画出单个数字 
static void Draw_Space(int len);											// 画出数字间空隙 
static void Draw_Nub(int x, int y, int nub, int len, int space, int size);	// 画出整个数字串

// 显示时间 
static void Show_Time(void);							// 游戏界面时间显示 
static void GetTime(int *hour, int *min, int *sec);		// 获得当前系统时间，以指针形式返回 
static void Draw_Point(int x, int y, int size); 		// 绘制点（时间显示专用） 

#endif
