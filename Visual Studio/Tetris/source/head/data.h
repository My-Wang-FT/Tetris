#ifndef _DATA_H_
#define _DATA_H_

#define HEIGHT 20		// 游戏区高度 
#define WIDTH 10		// 游戏区宽度 
#define BLOCK_TYPE 19	// 基础方块种类 
#define SIZE 4			// 每个方块存储区域大小 
#define NUB 9			// 用户名数组大小 

// 如下宏为信号变量定义方式
// 如新添加绘图变量 In_Draw
// 在下面宏定义中添加宏：#define In_Draw Nub
// 其中 Nub 为当前 宏NUMBER+1 的结果
// 并且把宏 NUMBER 值也加一
// 可以采用 Inf[In_Draw] 的方式引用 
#define NUMBER 24		// 信号指示变量个数宏 
#define Is_Menu 0 		// 图形界面指示变量，0 表示游戏界面 ，1 表示菜单界面
#define In_Game 1 		// 游戏信号变量， 1 表示正在进行游戏，0 表示游戏未运行 
// 菜单交互交互绘图变量 , 对应菜单界面四个选项框 
#define Menu_Ret1 2		// 开始游戏 
#define Menu_Ret2 3		// 继续游戏 
#define Menu_Ret3 4		// 排行榜 
#define Menu_Ret4 5		// 帮助 
#define Menu_Ret5 6	// 退出游戏 
#define Read_Name 7 	// 是否读入用户名 
#define Del_Gif 8		// 是否展示消除动画
#define Name_Ret 9		// 输入交互信号变量 
#define Name_Remand 10	// 提示用户输入用户名 
#define Is_Tool 11		// 是否在工具栏界面 
#define Is_End 12		// 游戏是否结束 
#define Is_Save 13		// 是否有存档 
#define Is_Rank 14		// 是否显示排行榜 
#define Is_Help	15		// 是否显示帮助 
#define Is_Quit	16		// 是否退出游戏 
#define Tool_Ret1 17	// 新游戏
#define Tool_Ret2 18	// 继续游戏
#define Tool_Ret3 19	// 排行榜
#define Tool_Ret4 20	// 帮助
#define Tool_Ret5 21	// 保存并退出 
#define Is_Back 22		// 返回 
#define Is_Gif 23 
 
extern int Now_Timer;	// 记录当前游戏Timer 

// 方块结构体 
typedef struct Block_{
	int a[SIZE][SIZE]; 	// 存放方块形状 
}Block;

extern const Block Bck_block[BLOCK_TYPE]; // 基础方块模型 

extern int Now_Block;	// 当前方块 
extern int Next_Block;	// 下一个方块 
// 方块颜色代号为 1-7 
extern int Now_Color;	// 当前方块颜色 
extern int Next_Color;	// 下一个方块颜色 

extern int X,Y;			// 当前方块坐标 

extern int Area[HEIGHT][WIDTH];		// 游戏区域 
extern int Del_Line[HEIGHT];		// 当前即将消除的行，从上往下数 
 
// 游戏显示信息	
extern char Con_Name[NUB];	// 当前游戏用户名 
extern int G_Score;			// 当前得分 
extern int Level;			// 当前难度级别 
extern int C_Line; 			// 已经消除的行数 
extern int S_Line; 			// 初始游戏行数
extern int Inf[NUMBER];		// 声明信号变量集合

 // 用户结构体 
typedef struct Con_node{
	int num;					// 排名      
	char name[NUB];				// 用户名 
	int score;					// 分数 
	int level;					// 难度 
	int c_line;					// 消除行数 
	struct Con_node * next;
} Con;

extern int Num;		// 排行榜用户个数 
extern Con* Head;	// 游戏用户数据链表头指针 

Con* Con_Create(void);						// 新建 
Con* Con_Input(Con *head, Con *consumer);	// 插入 
void Con_Print(Con *head);					// 格式化Console输入 
void Con_Sort(Con *head);					// 排序 
Con* Con_Fileopen(void);					// 打开文件读取排行榜 
void Con_Fileprint(Con *head, int n);		// 形成文件保存排行榜 
void Con_Save(void);						// 存储用户数据 
void Con_Continue(void);					// 读入存档数据 
int Con_Is_Save(void);						// 判断是否有存档 
void Con_DestorySave(void);					// 使存档失效，文档不可以多次恢复 
 
#endif
