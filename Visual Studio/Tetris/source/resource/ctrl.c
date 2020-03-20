#include "data.h"
#include "ctrl.h"
#include "model.h"
#include <stdio.h>

static int Name_cnt = 0;	// 记录用户名的字符长度 

// 鼠标回调函数 
void MouseCtrl(int x, int y, int button, int event)
{
//	printf("x=%d,y=%d,button=%d,event=%d\n",x,y,button,event);
	if( Inf[Is_Menu] == 1 ){	 				// 菜单回调函数 
		MenuMouseCtrl(x,y,button,event);
	} else if ( Inf[Is_Menu] == 0 && !Inf[Is_Gif] ) {			// 游戏界面 
		if ( Inf[Read_Name] ) {					// 用户名输入，难度选择等 
			NameMouseCtrl(x,y,button,event);
		} else{
			HotMouseCtrl(x,y,button,event);		// 游戏中的快捷键 
		} 
		if ( Inf[Is_Tool] ) {					// 工具栏的快捷键 
			ToolMouseCtrl(x,y,button,event);
		}
	}
}

// 键盘回调 
void KeyBoardCtrl(int key, int event)
{
//	printf("key=%d, event=%d\n", key, event);
	if ( Inf[Is_Menu] == 1) { 			// 主菜单 
		MenuKeyCtrl(key, event);
	} else if ( Inf[Is_Menu] == 0 && !Inf[Is_Gif] ){	// 游戏界面 
		if( Inf[Read_Name] ){			// 用户输入键盘回调 
			NameKeyCtrl(key, event);
		} else {						// 游戏界面
			HotKeyCtrl(key, event);		// 快捷键回调 
			if( Inf[In_Game] ){ 		// 游戏中的键盘回调 
				GameKeyCtrl(key, event); 
			} else {					// 暂停 
				if(Inf[Is_Tool]){		// 工具栏回调 
					ToolKeyCtrl(key,event);
				}
			}	
		}
	}
}

// 菜单键盘回调 
static void MenuKeyCtrl(int key, int event)
{
	 
	if (!event) {	// 按住不放 
		switch(key){	// 用户交互信号变量 
			case Key_S:	Inf[Menu_Ret1] = 1; break;
			case Key_C:	Inf[Menu_Ret2] = 1; break;
			case Key_R:	Inf[Menu_Ret3] = 1; break;
			case Key_H:	Inf[Menu_Ret4] = 1; break;
			case Key_Q:	Inf[Menu_Ret5] = 1; break;
		}
	} else {
		// 交互结束 
		Inf[Menu_Ret1] = 0;
		Inf[Menu_Ret2] = 0;
		Inf[Menu_Ret3] = 0;
		Inf[Menu_Ret4] = 0;
		Inf[Menu_Ret5] = 0;
		// 按键判别 
		switch(key){
			case Key_S:
				if ( !Inf[Is_Rank] && !Inf[Is_Help] ){ // 不在排行榜和帮助菜单， 下同 
					Inf[Is_Menu] = 0;		// 进入游戏 
					Init_Game();			// 初始化非用户修改数据	
				}
				break;
			case Key_C:
				if ( !Inf[Is_Rank] && !Inf[Is_Help] ){
					if(Inf[Is_Save]){  		// 如果有存档 
						Con_Continue(); 	// 读取存档 
						Con_DestorySave();	// 销毁存档 
						Inf[Is_Save] = Con_Is_Save(); 	// 存档有无判断
						// 跳转到游戏界面 
						Inf[In_Game] = 0;
						Inf[Is_Menu] = 0;
						Inf[Read_Name] = 0; 
					} else {
						printf("没有存档\n");
					}	
				}
				break;
			case Key_R: // 排行榜展示 
				if(Inf[Is_Rank]){
					Inf[Is_Rank] = 0;
				} else {
					if( !Inf[Is_Help] ){
						Con_Print(Head);
						Inf[Is_Rank] = 1;	
					}
				}
				break;
			case Key_H:	// 帮助展示 
				if(Inf[Is_Help]){
					Inf[Is_Help] = 0;
				} else {
					if( !Inf[Is_Rank] ) {
						Inf[Is_Help] = 1;
					}
				} 
				break;
			case Key_B:	// 退出排行榜和菜单 
				Inf[Is_Rank] = 0;
				Inf[Is_Help] = 0;
				break; 
			case Key_Q:	// 退出游戏 
				if( !Inf[Is_Rank] && !Inf[Is_Help] ){
					Inf[Is_Quit] = 1;
					break; 	
				} 
		}
	}
} 

// 工具栏键盘回调 
static void ToolKeyCtrl(int key, int event)
{
	if (Inf[Is_Tool] && !event) {
		switch(key){
			case Key_N: // 新游戏 
				if( !Inf[Is_Rank] && !Inf[Is_Help] ){
					Inf[Is_Menu] = 0;
					cancelTimer(Now_Timer); // 结束当前游戏Timer 
					Init_Game();
				}
				break;
			case Key_C:	// 继续游戏 
				if( !Inf[Is_Rank] && !Inf[Is_Help] ){
					Inf[Is_Tool] = 0;
					Inf[In_Game] = 1;
				}
				break;
			case Key_R:	// 排行榜 
				if( Inf[Is_Rank] ){
					Inf[Is_Rank] = 0;
				} else {
					if( !Inf[Is_Help] ){
						Inf[Is_Rank] = 1;
					}
				}
				break;
			case Key_H:	// 帮助 
				if( Inf[Is_Help] ){
					Inf[Is_Help] = 0;
				} else {
					if( !Inf[Is_Rank] ){
						Inf[Is_Help] = 1;
					}
				}
				break;
			case Key_B: // 返回 
				if( !Inf[Is_Rank] && !Inf[Is_Help] ){
					Con_Save();
					Inf[Is_Tool] = 0; 
					Inf[Is_Menu] = 1;
					Inf[Is_Save] = Con_Is_Save(); 
				}
				Inf[Is_Rank] = 0;
				Inf[Is_Help] = 0;
				break;
		}
	}
}

// 初始化新游戏数据，用户名等初始化 
void Init_Data(void)
{
	// 需要用户输入之后初始化 
	int i,j; 
	Now_Timer = Level; 
	startTimer(Now_Timer, (21-Now_Timer)*40); 
	
	for(i=0; i<HEIGHT-S_Line; i++){		// 起始区域初始化为空 
		for(j=0; j<WIDTH; j++){
			Area[i][j] = 0; 
		}
	}
	srand(time(NULL));
	for(i=HEIGHT-S_Line; i<HEIGHT; i++){	// 用户指定行数初始化为随机空格 
		for(j=0; j<WIDTH; j++){
			Area[i][j] = !(rand()%4)*(rand()%7+1);
		}
	} 
	Inf[In_Game] = 1;	// 游戏进行 
} 

// 初始化游戏， 非用户指定数据初始化 
void Init_Game(void)  
{
	int i,j;
	// 需要进入游戏就初始化 
	Level = 1;		// 初始化游戏难度为 1 
	G_Score = 0; 	// 初始化分数， 起始行数， 消除行数 
	S_Line = 0;
	C_Line = 0;
	// 初始化游戏区域 
	for(i=0; i<HEIGHT; i++){
		for(j=0; j<WIDTH; j++){
			Area[i][j] = 0; 
		}
	}
	// 初始化方块和颜色 
	Now_Color = 1; 
	Next_Color = 2; 
	srand(time(NULL));
	Next_Block = rand()%BLOCK_TYPE;
	// 初始化方块起始位置坐标
	X = -1;	 
	Y = WIDTH/2-2;	
	// 初始化用户名
	for(i=0; i<NUB; i++){ 
		Con_Name[i] = 0;
	} 
	// 初始化游戏内部信号变量 
	Inf[In_Game] = 0; 
	Inf[Is_Tool] = 0; 
	Inf[Read_Name] = 1;
	Inf[Name_Remand] = 0; 
	Inf[Is_End] = 0;
	Inf[Is_Help] = 0;
	Inf[Is_Rank] = 0;
	Inf[Del_Gif] = 0; 
}

// 游戏快捷键回调 
static void HotKeyCtrl(int key, int event)
{
	static int ret = 1;
	if(!event){
		switch(key){
			case Key_P:	// 暂停 
				// 确保长按不会反复在暂停和继续之间转换
				if(ret){ 
					ret = 0;
					if( Inf[In_Game] ){
						Inf[In_Game] = 0;
					} else {
						Inf[In_Game] = 1;
						Inf[Is_Tool] = 0;
					}
				}
			case Key_T:	// 工具栏 
				if(ret){ 
					ret = 0;
					if( Inf[In_Game] ){
						Inf[In_Game] = 0;
					}
					if( Inf[Is_Tool] == 0 ){
						Inf[Is_Tool] = 1;
					} else if(Inf[Is_Tool] == 1){
						Inf[Is_Tool] = 0;
						Inf[In_Game] = 1;
					}
				}
		}
	} else {
		ret = 1;
	}
}

// 游戏键盘控制函数 
static void GameKeyCtrl(int key, int event)
{
	static int ret; 
	// 专为按键次数定义，对应实现一直按键只需要一次操作的函数 
	if(!event){
		switch(key){
			case Key_Up: 	//旋转方块 
				if(ret){
					Turn_Block();
					ret = 0;	
				}
				break;
			case Key_Down:	//下降 
			// 在消除动画时不会调用模型函数
				if( !Inf[Del_Gif] ){ 
					TimeListener(1); 	
				}
				break;  
			case Key_Left:
				Left_Move();
				break;
			case Key_Right:
				Right_Move();
				break; 
			case Space:
				Go_Button();
				break;
		} 
	} else {
		ret = 1;
	}
} 

// 用户名输入回调函数 
static void NameKeyCtrl(int key, int event)
{
	if(!event){
		if ( key != Key_Enter ) { // 用户名输入 
			if ( key >= Key_A && key <= Key_Z  && Name_cnt < NUB-1 ) {
				Con_Name[Name_cnt++] = key;
			} else if( key == Key_Backspace && Name_cnt > 0 ){ // 删除 
				Con_Name[--Name_cnt] = 0;
			}
			switch(key){ // 难度和开始行数回调 
				case Key_Up:
					if(S_Line < 10){
						S_Line++;
					}
					break;
				case Key_Down:
					if(S_Line > 0){
						S_Line--;
					}
					break;
				case Key_Left:
					if(Level < 20){
						Level++;
					}
					break;
				case Key_Right:
					if(Level > 1){
						Level--;
					}
					break; 
				default:break;
			}
		} else { // 结束用户输入之后的操作 
			if(Name_cnt > 0){
				Inf[Read_Name] = 0; 	// 结束用户输入 
				Init_Data();
				Name_cnt = 0; 
			} else {
				Inf[Name_Remand] = 1;	// 用户名为空！ 提示输入用户名 
			}
		}
	}
} 

// 菜单鼠标回调 
static void MenuMouseCtrl(int x, int y, int button, int event)
{
	if( In(175,190,275,220) ){ 	// 如果移动到这个图形内 
		Inf[Menu_Ret1] = 1;		// 绘图变量修改为1 
		if ( button==1 && event==0 && !Inf[Is_Rank] && !Inf[Is_Help] ){
			Inf[Is_Menu] = 0;		// 进入游戏 
			Init_Game();			// 初始化非用户修改数据	
		}
	} else {
		Inf[Menu_Ret1] = 0;		// 如果移除当前图标 绘图变量修改为0 
	}
	// 下同
	if( In(175,230,275,260) ){
		Inf[Menu_Ret2] = 1;
		
		if ( !Inf[Is_Rank] && !Inf[Is_Help] && button==1 && event==0 ){
			if(Inf[Is_Save]){
				Inf[Is_Menu] = 0;
				Inf[Read_Name] = 0;
				Con_Continue();
				Inf[In_Game] = 0; 
				Con_DestorySave();
				Inf[Is_Save] = Con_Is_Save(); 
			} else {
				printf("没有存档\n");
			}	
		}
	} else {
		Inf[Menu_Ret2] = 0;
	}
	
	if( In(175,270,275,300) ){
		printf("排行榜\n"); 
		Inf[Menu_Ret3] = 1;
		if( button==1 && event==0){
			if( !Inf[Is_Rank]) {
				if( !Inf[Is_Help] ){
					Inf[Is_Rank] = 1;	
				}
			}
		}
	} else {
		Inf[Menu_Ret3] = 0;
	}
	
	if( In(175,310,275,340) ){ // 排行榜 
		Inf[Menu_Ret4] = 1;
		if( button==1 && event==0){
			if(!Inf[Is_Help]){
				if( !Inf[Is_Rank] ) {
					Inf[Is_Help] = 1;
				}
			}
		}
	} else {
		Inf[Menu_Ret4] = 0;
	}

	if( In(175,350,275,380) ){	// 帮助 
		Inf[Menu_Ret5] = 1;
		if( button==1 && event==0){
			if( !Inf[Is_Rank] && !Inf[Is_Help] ){
				Inf[Is_Quit] = 1;
			} 
		}
	} else {
		Inf[Menu_Ret5] = 0;
	}
	
	if( In(180,370,255,400) ){	// 退出游戏 
		Inf[Is_Back] = 1;
		if( button==1 && event==0 && Inf[Is_Help]==0){
			Inf[Is_Rank] = 0;
		}
		if( button==1 && event==0 && Inf[Is_Rank]==0){
			Inf[Is_Help] = 0;
		}
	}else{
		Inf[Is_Back] = 0;
	}
}

// 工具栏回调 
static void ToolMouseCtrl(int x, int y, int button, int event)
{
	if( In(175,190,275,220) ){ 	// 如果移动到这个图形内 
		Inf[Tool_Ret1] = 1;		// 绘图变量修改为1 
		if( button==1 && event==0){	// 如果点击 
			if( !Inf[Is_Rank] && !Inf[Is_Help] ){
				printf("新游戏\n"); 
				Inf[Is_Menu] = 0;
				cancelTimer(Now_Timer);
				Init_Game();
			}
		}
	} else {
		Inf[Tool_Ret1] = 0;		// 如果移除当前图标 绘图变量修改为0 
	}
	// 下同
	if( In(175,230,275,260) ){
		Inf[Tool_Ret2] = 1;
		if( button==1 && event==0){
			if( !Inf[Is_Rank] && !Inf[Is_Help] ){
				printf("继续游戏\n");
				Inf[Is_Tool] = 0;
				Inf[In_Game] = 1;
			}
		}
	} else {
		Inf[Tool_Ret2] = 0;
	}
	
	if( In(175,270,275,300) ){	// 排行榜 
		Inf[Tool_Ret3] = 1;
		if( button==1 && event==0){
			if ( !Inf[Is_Rank] ) {
				if( !Inf[Is_Help] ){
					printf("排行榜\n"); 
					Inf[Is_Rank] = 1;
					Con_Print(Head);
				}
			}
		}
	} else {
		Inf[Tool_Ret3] = 0;
	}
	
	if( In(175,310,275,340) ){	// 帮助 
		Inf[Tool_Ret4] = 1;
		if( button==1 && event==0){
			if ( !Inf[Is_Help] ) {
				if( !Inf[Is_Rank] ){
					printf("帮助\n");
					Inf[Is_Help] = 1;
				}
			}
		}
	} else {
		Inf[Tool_Ret4] = 0;
	}
	
	if( In(175,350,275,380) ){	// 保存并退出 
		Inf[Tool_Ret5] = 1;
		if( button==1 && event==0){
			if( !Inf[Is_Rank] && !Inf[Is_Help] ){
				printf("保存并退出\n");
				Con_Save();
				Inf[Is_Tool] = 0; 
				Inf[Is_Menu] = 1;
				Inf[Is_Save] = Con_Is_Save();
				Inf[Is_Rank] = 0;
				Inf[Is_Help] = 0; 
			}
		}
	} else {
		Inf[Tool_Ret5] = 0;
	}
	
	if( In(180,370,255,400) ){	// 排行榜和帮助回调 
		printf("f返回\n");
		Inf[Is_Back] = 1;
		if( button==1 && event==0 && Inf[Is_Help]==0){
			Inf[Is_Rank] = 0;
		}
		if( button==1 && event==0 && Inf[Is_Rank]==0){
			Inf[Is_Help] = 0;
		}
	}else{
		Inf[Is_Back] = 0;
	}
}

// 图标快捷键回调函数 
static void HotMouseCtrl(int x, int y, int button, int event)
{
	if( In(275,510,335,540) ){ 	// 如果移动到这个图形内 
		if( button==1 && event==0){	// 如果点击 
			printf("工具栏\n"); 
			if( Inf[In_Game] ){
				Inf[In_Game] = 0;
			}
			if( Inf[Is_Tool] == 0 ){
				Inf[Is_Tool] = 1;
			} else if(Inf[Is_Tool] == 1){
				Inf[Is_Tool] = 0;
				Inf[In_Game] = 1;
			}
		}
	}
	// 下同
	if( In(335,510,375,545) ){	// 暂停 
		if( button==1 && event==0){
			printf("暂停\n");
				// 确保长按不会反复在暂停和继续之间转换
			if( Inf[In_Game] ){
				Inf[In_Game] = 0;
			} else {
				Inf[In_Game] = 1;
				Inf[Is_Tool] = 0;
			}
		}
	}
}

// 用户名鼠标回调 
static void NameMouseCtrl(int x, int y, int button, int event)
{
	if ( In(170,225,190,245) ) {	// 难度增加 
		if ( !event && Level < 20 ) {
			Level++;
		}
	}
	if ( In(215,225,235,245) ) {	// 难度减少 
		if ( !event && Level > 1 ) {
			Level--;
		}
	}
	if ( In(170,265,190,285) ) {	// 初始行数增加
		if ( !event && S_Line < 10 ) {
			S_Line++;
		}
	}
	if ( In(215,265,235,285) ) {	// 初始行数减少 
		if ( !event && S_Line > 0 ) {
			S_Line--;
		}
	}
	if ( In(49, 315, 225, 413) ) {	// 开始游戏 
		Inf[Name_Ret] = 1;
		if( !event ){
			if( Name_cnt > 0 ) {
				Inf[Read_Name] = 0; // 结束用户输入 
				Init_Data();
				Name_cnt = 0;
			} else {
				Inf[Name_Remand] = 1;	
			} 
		} 
	} else {
		Inf[Name_Ret] = 0;
	} 
} 

//方块旋转 
static void Turn_Block(void)
{
	int Old_Block = Now_Block; 
	switch( Now_Block ) {	// 改变方块种类即可实现旋转 
		case 0 : Now_Block = 0;	break;
		case 1 : Now_Block++;	break;
		case 2 : Now_Block = 1;	break;
		case 3 : Now_Block++;	break;
		case 4 : Now_Block = 3;	break;
		case 5 : Now_Block++;	break;
		case 6 : Now_Block = 5;	break;
		case 7 : Now_Block++;	break;
		case 8 : Now_Block++;	break;
		case 9 : Now_Block++;	break;
		case 10: Now_Block = 7;	break;
		case 11: Now_Block++;	break;
		case 12: Now_Block++;	break;
		case 13: Now_Block++;	break;
		case 14: Now_Block = 11;	break;
		case 15: Now_Block++;	break;
		case 16: Now_Block++;	break;
		case 17: Now_Block++;	break;
		case 18: Now_Block = 15;	break;
	}
	//判断旋转后的方块是否和区域已有方块重叠 
	if ( Is_Touch() ) {
		//如果重叠就将方块类型改回来，相当于不旋转 
		Now_Block = Old_Block;
	}
}

// 判断方块是否和游戏区已有方块重叠
// 或者方块是否已经超出游戏边界 
static int Is_Touch()
{
	int i,j;
	int ret = 0;
	for(i=0; i<SIZE; i++){
		for(j=0; j<SIZE; j++){
			if( Bck_block[Now_Block].a[i][j] ) { //如果方块模型的该位置有方块 
				if( Y+j < 0 || Y+j >= WIDTH ) {
					//方块超出左右边界或者方块和其他方块重叠 
					ret = 1;
					return ret;
				}
				//如果方块还在外面，数组引用可能越界，加上判断 
				if ( X-SIZE+i+1 >= 0 && X-SIZE+i+1 < HEIGHT && Area[X-SIZE+1+i][Y+j] ){
					ret = 1;
					return ret;
				}//最低边界判断 
				if ( X-SIZE+i+1 >= HEIGHT ){
					ret = 1;
					return ret;
				}
			}
		}
	}
	return ret; 
}

// 左移 
static void Left_Move(void)
{
	if(!Inf[Del_Gif]){
		Y--;
		if( Is_Touch() ) {
			Y++;
		}	
	}
} 

// 右移 
static void Right_Move(void)
{
	if(!Inf[Del_Gif]){
		Y++;
		if( Is_Touch() ) {
			Y--;
		}	
	}
} 

// 空格降落函数 
static void Go_Button(void)
{
// 因为相关函数都是在Model中
// 为降低模块间的耦合度
// 将此函数具体内容在Model.c中实现 
	Go_Button_M();
}
