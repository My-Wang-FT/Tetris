#include "data.h"
#include "model.h"
#include "ctrl.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// M 代指在 Model 中的函数 ，和Ctrl中的函数区分开 
void Go_Button_M(void)
{
	while ( !Fall_Block() );
	Timer_Model();
	if ( Is_End_Game() ){ 
		End_Game();
	} else {
		Creat_Block();
	}
}

// 流程函数 
void Model_Flow(void)
{
	if ( Inf[In_Game] ) {
		if ( Fall_Block() ) {
			Timer_Model();
			if ( Is_End_Game() ){
				End_Game();
			} else {
				Creat_Block();
			}
		}
	} 
}
 
// 随机产生一个新的方块，种类代号 0-18，颜色代号为 1-7 
static void Creat_Block(void)
{
	srand(time(NULL));
	Now_Block = Next_Block;
	Next_Block = rand()%BLOCK_TYPE;
	Now_Color = Next_Color;
	Next_Color = ( (Next_Color+1)%7 ) ? (Next_Color+1)%7 : 7; 
	// 方块初始坐标 
	X = 0;
	Y = WIDTH/2-2; 
}

//下落，到底停止 
static int Fall_Block(void)
{
	int ret = 0; 
	if ( Is_Touch() ){
		Change_Score(0); 
		Fixed_Block();
		ret = 1; 
	} else {
		X++;
	} 
	return ret;
}

// 将方块信息写到 Area数组中， 实现移动方块向固定方块的转变 
static void Fixed_Block(void)
{
	int i,j;
	for(i=0; i<SIZE; i++){
		for(j=0; j<SIZE; j++){
			if ( Bck_block[Now_Block].a[i][j] ) {
				Area[X-SIZE+1+i][Y+j] = Now_Color;
			}
		}
	}
}

//判断方块是否到底 
static int Is_Touch(void)
{
	int i, j;
	int ret = 0;
	for(j=0; j<SIZE; j++){
		for(i=SIZE-1; i>=0; i--){
			if( Bck_block[Now_Block].a[i][j] ){ // 对方块模型数据的引用 
				if( X-SIZE+1+i+1 >=0 ){//保证数组引用不越界 
					if( (X-SIZE+1+i+1)==(HEIGHT) || Area[X-SIZE+1+i+1][Y+j] ){
						// 如果当前方块下还有方块或者已经超出边界 
						ret = 1;
						return ret; 
					} else {
						break;
					}
				}
			}
		}
	}
	return ret;
}

static int Is_End_Game(void)
{
	int ret = 0,i;
	//游戏区中最上面一行区域 
	for(i=0; i<WIDTH; i++){
		if ( Area[0][i] ) {  // 如果此区域有方块即结束游戏 
			ret = 1;
			break;
		}
	}
	return ret;
} 

// 判断是否满行，满行删除，同时修改Inf[Del_Gif]以显示动画 
void Delete_Row(void)
{
	int cnt = 0, ret, i, j, k;
	for ( i=0; i<HEIGHT; i++ ) {
		ret = 1;
		Del_Line[i] = 0; 
		for ( j=0; j<WIDTH; j++ ) {
			if ( !Area[i][j] ) {
				ret = 0;
				break;
			}
		}
		if ( ret ) {
			Del_Line[i] = 1;	// 记录需要消除的行 
			Move_Area(i);		// 游戏区数据消除行 
			C_Line++;			// 消除的行加一 
			cnt++;
		}
	}
	if ( cnt ) {
		Change_Score(cnt);		// 修改分数 
		Inf[Del_Gif] = 1;		// 绘制动画 
	}
} 

// 将游戏区 0 ~ (row-1) 行整体向下平移一行 
static void Move_Area(int row)
{
	int i,j;
	for(i=row-1; i>=0; i--){
		for(j=0; j<WIDTH; j++){
			Area[i+1][j] = Area[i][j];
		}
	}
}

// 改变分数 
static void Change_Score(int ret)
{
	switch(ret){
		case 0:G_Score +=   7 + Level *  3 + S_Line;	break;	// 下落方块 
		case 1:G_Score +=  90 + Level * 10 + S_Line;	break;	// 消除一行 
		case 2:G_Score += 280 + Level * 20 + S_Line;	break;	// 消除两行 
		case 3:G_Score += 570 + Level * 30 + S_Line;	break;	// 消除三行 
		case 4:G_Score += 960 + Level * 40 + S_Line;	break;	// 消除四行 
	}
} 

// 修改Timer， 调整游戏难度 
static void Timer_Model(void)
{
	static int ret = 0; 
	if( (C_Line-2)/6 != ret ){
		ret++;
		if(ret > (C_Line-2)/6 ){
			ret = (C_Line-2)/6;
		} 	
		if(Level != 20){
			Level++;
		}
	}
	
	if(Now_Timer != Level){ 
		cancelTimer(Now_Timer);
		startTimer(Level, (21-Level)*40);
		Now_Timer = Level;
	}
}

// 结束游戏判断 
static void End_Game(void)
{
	printf("游戏结束！\n"); 
	cancelTimer(Now_Timer);
	// 更新排行榜链表 
	Head = Con_Input( Head, Con_Create() );
	Num++;
	// 排行榜链表排序 
	Con_Sort(Head); 
	// 调试语句
	Con_Fileprint(Head, Num); 
	printf("Num = %d", Num);
	// 再次初始化游戏 
	Init_Game(); 
	Inf[Is_Menu] = 1; // 回到游戏界面 
	Inf[In_Game] = 0; // 不在游戏中 
	Inf[Is_Rank] = 1; // 显示排行榜 
} 

// 仅调试用 ，输出地图固定方块，不包括正在下落方块 
static void Print_Area(void)
{
	int i, j;
	for(i=0; i<HEIGHT; i++){
		for(j=0; j<WIDTH; j++){
			printf("%3d ",Area[i][j]); 
		}
		printf("\n");
	}
	printf("\n");
} 
