#ifndef _MODEL_H_
#define _MODEL_H_

// 模块间连接函数 
void Go_Button_M(void); 				// 在 Ctrl中调用，实现空格使方块直接落实 
void Model_Flow(void);					// 在 Main中调用，Timer回调函数组成部分，表明游戏流程 
void Delete_Row(void);					// 在 Draw中调用，方便实现消除动画 

// 模块内部函数 
static void Creat_Block(void);			// 生成下个方块 
static int Fall_Block(void);			// 方块下落，是否已经落地，是1否0 
static void Fixed_Block(void);			// 将已经固定的方块加入游戏区，固定方块 
static int Is_Touch(void);				// 是否接触到底部 ，是1否0 
static int Is_End_Game(void);			// 结束游戏 是1否0 
static void Move_Area(int row);			// 将1-row-1行向下移动一行，最上行填零 
static void Change_Score(int ret);		// 分数改变，ret 为消除行数  
static void Timer_Model(void); 			// 难度控制函数，通过管理Timer控制速度 
static void End_Game(void);				// 游戏结束后的处理，返回到开始菜单 
static void Print_Area(void); 			// 在 Consule输出数组地图，调试 

#endif
