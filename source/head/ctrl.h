#ifndef _CTRL_H_
#define _CTRL_H_

#define In(x1,y1,x2,y2) ( x>=(x1) && x<=(x2) && y>=(y1) && y<=(y2) )  

// 键盘key值 
#define Key_Up 38
#define Key_Down 40
#define Key_Left 37
#define Key_Right 39
#define Space 32 
#define Key_Enter 13
#define Key_A 65
#define Key_B 66
#define Key_C 67
#define Key_H 72
#define Key_N 78
#define Key_P 80
#define Key_R 82
#define Key_S 83
#define Key_T 84
#define Key_Z 90 
#define Key_Q 81
#define Key_Backspace 8

// 模块连接函数 
void MouseCtrl(int x, int y, int button, int event);// 在 Main中调用，鼠标回调  
void KeyBoardCtrl(int key, int event);				// 在 Main中调用，键盘回调 
void Init_Data(void);								// 用户可选游戏数据初始化
void Init_Game(void); 								// 非用户数据初始化

// 模块内函数 
static void Go_Button(void);					// 调用 Modle函数，实现空格落实方块 
static void MenuKeyCtrl(int key, int event);	// 菜单快捷键回调函数 
static void GameKeyCtrl(int key, int event);	// 游戏键盘控制函数 
static void NameKeyCtrl(int key, int event);	// 游戏用户输入控制函数 
static void HotKeyCtrl(int key, int event);		// 快捷键控制函数 
static void ToolKeyCtrl(int key, int event);	// 工具栏回调函数 

static void MenuMouseCtrl(int x, int y, int button, int event); // 菜单鼠标控制交互函数
static void HotMouseCtrl(int x, int y, int button, int event);	// 快捷键鼠标控制交互函数 
static void ToolMouseCtrl(int x, int y, int button, int event);	// 工具栏鼠标控制交互函数 
static void NameMouseCtrl(int x, int y, int button, int event);	// 用户输入数据鼠标回调函数 

static void Turn_Block(void);				// 改变方块种类 
static int Is_Touch(void);					// 判断方块是否和已有方块重叠或者超出边界 
static void Left_Move(void);				// 向左移动方块 
static void Right_Move(void);				// 向右移动方块 

#endif 
