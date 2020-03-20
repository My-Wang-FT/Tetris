#include <stdio.h>
#include <time.h> 
#include "data.h"
#include "draw.h" 

static int i,j;									// for循环变量，不在函数内重复定义 
static int Areaa[HEIGHT][WIDTH];				// 绘图数组数据，从数据库中更新 
// 七段数码管专用变量 
static double Pen_X, Pen_Y;						// 记录画笔位置 
static int Agl; 								// 记录当前角度

// 主菜单下面方块 
const static int drawdown[6][10]={
	{1,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,0,0,0,5},
	{1,0,0,0,0,0,0,0,0,5},
	{1,2,0,0,0,0,0,0,5,5},
	{2,2,2,0,0,0,0,4,6,6},
	{3,3,3,3,0,4,4,4,6,6},
};

// 主菜单上方 Tetris 文字 
static int tetris[5][22]={
	{1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,1,0,1,0,1,1,1},
	{0,1,0,0,1,0,0,0,0,1,0,0,1,0,0,1,0,1,0,1,0,0},
	{0,1,0,0,1,1,1,0,0,1,0,0,1,1,1,0,0,1,0,1,1,1},
	{0,1,0,0,1,0,0,0,0,1,0,0,1,0,1,0,0,1,0,0,0,1},
	{0,1,0,0,1,1,1,0,0,1,0,0,1,0,0,1,0,1,0,1,1,1},
};

void Draw(void){
	static int cnt;				// 函数调用计数器 
	DisplayClear();             // 清屏 
	
	// 形成整个游戏区域边框 
	SetPenColor(Choose_Color(8));
	DrawPixel(5,5,446,545);
	DrawPixel(4,4,447,546);
	DrawPixel(3,3,448,547);
	
	if ( Inf[Is_Menu]==1 ) {
		MenuInterface();
		if( Inf[Is_Quit] ) {
			ExitGraphics();      // 退出游戏 
		}
	} else {
		GameInterface();		// 游戏界面绘制 
		
		// 动画流程判断 
		switch(Inf[Del_Gif]){
			case 0:{ // 无动画 
				CreateNewMap();			// 形成方块数组 
				DrawNewMap();			// 方块绘制 
				Delete_Row();
				break;
			}
			case 14:{ // 动画结束 
				Now_Timer = Level; 
				startTimer(Level, (21-Level)*40 );
				Inf[Del_Gif] = 0;
				break;
			}
			case 1:{ // 动画开始 
				cancelTimer(Now_Timer);
				Inf[Del_Gif]++;
			}
			default:{ // 动画过程 
				Delete_Gif();
				Inf[Del_Gif]++;
				break;
			}
		}
		
		Show_Time();			// 时间展示 
		
		// 通过Cnt控制定时闪动组件 
		cnt++; 
		if ( Inf[Is_Tool] ){ 	// 在工具栏 
			ToolInterface(); 
		}
		if ( Inf[In_Game] ) {	// 游戏正在进行 
			DrawStart();
		} else {
			if(Inf[Is_Rank]){	// 显示排行榜 
				DrawRank();
			} else if(Inf[Is_Help]){	// 显示帮助 
				DrawHelp();
			}
			if( !Inf[Read_Name] ){		// 非读入名字界面的暂停闪动 
				if ( cnt<20 ){
					
				} else if(cnt < 40){
					DrawPause(); 
				} else {
					cnt = 0;
				}
			} else {
				DrawPause();
			}
		}
	}
	Interact_Window();		// 用户交互函数
}

// 输入像素点画线框 
static void DrawPixel(int x1, int y1, int x2, int y2)
{
	double xx1, yy1, xx2, yy2;
	xx1=ScaleXInches(x1);     // 将像素点转化为英寸单位 
	yy1=ScaleYInches(y1);
	xx2=ScaleXInches(x2);
	yy2=ScaleYInches(y2);
	
	MovePen(xx1,yy1);
	DrawLine(0,yy2-yy1);
	DrawLine(xx2-xx1,0);
	DrawLine(0,yy1-yy2);
	DrawLine(xx1-xx2,0);
}

// 画像素线段 
static void DrawPixelLine(int x1, int y1, int x2, int y2){
	double xx1, yy1, xx2, yy2;
	xx1=ScaleXInches(x1);
	yy1=ScaleYInches(y1);
	xx2=ScaleXInches(x2);
	yy2=ScaleYInches(y2);
	
	DrawLine(xx2-xx1,yy2-yy1);
}

// 涂色 
static string Choose_Color(int x){
	char *color;
	switch(x){
		case 1: color= "red"; break;
		case 2: color= "Orange"; break;
		case 3: color= "yellow"; break;
		case 4: color= "green"; break;
		case 5: color= "Cyan"; break;
		case 6: color= "blue"; break;
		case 7: color= "Magenta"; break;
		
		// 自定义了一些颜色，在库中已经修改 
		case 8: color= "Tomato"; break;
		case 9: color= "Goldenrod1"; break;
		case 10: color= "Moccasin"; break;
		case 11: color= "Khaki1"; break;
		default: color= "black"; break;
	}
	return color;
}

// 特定区域的清屏 
static void ClearScreen(int x1, int y1, int x2, int y2){
	StartFilledRegion(0.01);
	DrawPixel(x1,y1,x2,y2);
	EndFilledRegion();
}
 
// 形成主菜单界面
static void MenuInterface(void){
	
	DrawTet();				// 绘制背景图片  
	
	// 菜单功能的选择 
	if(Inf[Is_Rank]==0&&Inf[Is_Help]==0){
		DrawMenuChoice();
	} else if(Inf[Is_Help]){
		DrawHelp();
	} else if (Inf[Is_Rank]){
		DrawRank();
	} 
} 

// 画主菜单 
static void DrawMenuChoice(void)
{
	SetPenColor(Choose_Color(8));
	
	DrawPixel(80,160,370,405);
	DrawPixel(175,190,275,220);		// 开始游戏 
	DrawPixel(175,230,275,260);		// 游戏设置 
	DrawPixel(175,270,275,300);		// 排行榜 
	DrawPixel(175,310,275,340);		// 帮助
	DrawPixel(175,350,275,380);     // 退出游戏 
	
	// 显示具体功能 
	ShowStr(184, 210, "开始游戏 [S]");
	ShowStr(184, 250, "读入存档 [C]");
	ShowStr(192, 290, "排行榜 [R]");
	ShowStr(200, 330, "帮助 [H]");
	ShowStr(184, 370, "退出游戏 [Q]");
} 

// 画输入用户名界面 
static void NameFace(void){
	SetPenColor(Choose_Color(2));
	
	DrawPixel(6,120,274,440);
	ShowStr(30,144,"输入用户名 (仅限字母 直接按键)");
	DrawPixel(68,160,220,190);
	ShowStr(73,180,Con_Name);
	
	// 显示设定难度功能选择 
	ShowStr(40,240,"设定难度 [左右]");
	SetPenSize(3);
	DrawPixel(175,235,185,235);  // 画正号 
	DrawPixel(180,230,180,240);
	DrawPixel(219,235,229,235);  // 画负号 
	SetPenSize(1);
	DrawPixel(170,225,234,245);  // 大框 
	DrawPixel(190,225,214,245);	 // 小框 
	ShowStr(195,240,IntegerToString(Level));
	
	// 显示设定初始行数选择 
	ShowStr(40,280,"初始行数 [上下]");
	SetPenSize(3);
	DrawPixel(175,275,185,275);  // 画正号 
	DrawPixel(180,270,180,280);
	DrawPixel(219,275,229,275);  // 画负号 
	SetPenSize(1);
	DrawPixel(170,265,234,285);  // 大框 
	DrawPixel(190,265,214,285);	 // 小框 
	ShowStr(195,280,IntegerToString(S_Line));
	
	Draw_Go();
}

// 画出GO样式 
static void Draw_Go(void)
{
	int go[5][9]={
		{1,2,3,4,0,6,7,1,2},
		{2,0,0,0,0,7,0,0,3},
		{3,0,5,6,0,1,0,0,4},
		{4,0,0,7,0,2,0,0,5},
		{5,6,7,1,0,3,4,5,6},
	};
	
	int i,j;
	
	// 遍历数组进行方块涂色 
	for(i=0;i<5;i++){
		for(j=0;j<9;j++){
			if(go[i][j]){
				if( Inf[Name_Ret] ){
					SetPenColor(Choose_Color(go[i][j]));
				} else {
					SetPenColor(Choose_Color(12));
				}
				StartFilledRegion(0.9);
				DrawPixel(50+j*20,315+i*20,50+j*20+16,315+i*20+16);
				EndFilledRegion();
			}
		} 
	}
	
	// 未输入用户名显示提示 
	if (Inf[Name_Remand] ){
		SetPenColor("red");
		ShowStr(96, 210, "请输入用户名"); 
	}
} 

// 画排行榜
static void DrawRank(void){
	SetPenSize(1);
	SetPenColor(Choose_Color(8));
	ClearScreen(80,160,370,405); 
	DrawPixel(80,160,370,405);
	ShowStr(170,190,"排行榜(只取前5名)");
	ShowStr(100,220,"名次");
	ShowStr(200,220,"玩家");
	ShowStr(300,220,"得分");
	
	// 显示排行榜 
	int i;
	Con* tem = NULL;
	tem=Head;
	for(i=0;i<5;i++){
		if(tem){
			ShowStr(110,240+i*25,IntegerToString(tem->num));
			ShowStr(190,240+i*25,tem->name);
			ShowStr(300,240+i*25,IntegerToString(tem->score));
			tem=tem->next;
		} 
	}
	
	DrawPixel(180,370,255,400);
	ShowStr(190,390,"返回 [B]");
} 

// 画帮助菜单 
static void DrawHelp(void){
	SetPenSize(1);
	SetPenColor(Choose_Color(8));
	ClearScreen(80,160,370,405);
	DrawPixel(80,160,370,405);
	
	// 显示具体帮助内容 
	ShowStr(207,185,"帮助");
	ShowStr(127,205,"建议开启大写锁定进行游戏");
	ShowStr(104,225,"游戏支持鼠标点击以及快捷键操作");
	ShowStr(167,255,"游戏界面快捷键");
	ShowStr(125,275,"左右键：移动方块");
	ShowStr(125,295,"上键：旋转方块");
	ShowStr(125,315,"下键：加速方块下落");
	ShowStr(125,335,"空格键：方块落至底部");
	ShowStr(125,355,"P：暂停游戏      T ：工具栏");
	
	DrawPixel(180,370,255,400);
	ShowStr(190,390,"返回 [B]");
}

// 形成游戏界面 
static void GameInterface(void)
{
	int i, j;
	if (Inf[Read_Name]==1) {
		NameFace();
	} else {
		SetPenColor(Choose_Color(10));
		StartFilledRegion(0.2);
		DrawPixel(6,6,445,544);
		EndFilledRegion();	
		
		// 填充背景与划线 
		for(i=0;i<19;i++){
			DrawPixel(5,5+i*27+27,446,5+i*27+27);
		}
		
		for(i=0;i<9;i++){
			DrawPixel(5+i*27+27,5,5+i*27+27,545);
		}

		SetPenColor(Choose_Color(11));
		StartFilledRegion(0.4);
		DrawPixel(276,6,445,544);
		EndFilledRegion();	
	}
	
	// 形成游戏边框 
	SetPenColor(Choose_Color(8));
	DrawPixel(275,5,275,545);
	DrawPixel(275,175,446,175);
	DrawPixel(275,290,446,290);
	DrawPixel(275,400,446,400);
	DrawPixel(275,510,446,510);
	DrawPixel(335,510,335,545);
	DrawPixel(375,510,375,545);
	
	SetPenColor(Choose_Color(8));
	// 显示右侧界面
	ShowStr(280, 25, "下一个方块");
	// 显示已消除行数界面
	ShowStr(280, 195, "已消除行数");
	// 显示得分界面
	ShowStr(280, 310, "当前得分");
	// 显示当前难度等级
	ShowStr(280, 420, "当前难度等级");
	// 显示工具栏
	ShowStr(280, 533, "工具 [T]");
	
	DrawPixel(310,68-25,385+25,145+25-25);
	DrawPixel(310+25,68-25,385+25,145+25-25);
	DrawPixel(335+25,68-25,385+25,145+25-25);
	DrawPixel(360+25,68-25,385+25,145+25-25);
	
	DrawPixel(310,68-25,385+25,70+25-25);
	DrawPixel(310,70+25-25,385+25,95+25-25);
	DrawPixel(310,95+25-25,385+25,120+25-25);
	DrawPixel(310,120+25-25,385+25,145+25-25);
	  
	// 显示 Level，G_Score，C_Line 
	int size = 15;
	int Pen_Size = 2;
	int Space = 2; 
	Draw_Nub(410, 240, C_Line, size, Space, Pen_Size);
	Draw_Nub(410, 350, G_Score, size, Space, Pen_Size);
	Draw_Nub(410, 460, Level, size, Space, Pen_Size); 
}

// 形成新的数组 
static void CreateNewMap(void)
{
	// 刷新新数据
	for(i=0; i<HEIGHT; i++)
       for(j=0; j<WIDTH; j++)
   	       Areaa[i][j] = Area[i][j];
	
	// 记录正在下落方块 
	for(i=0; i<SIZE; i++){
		for(j=0; j<SIZE; j++){
			if ( Bck_block[Now_Block].a[i][j] && (X-SIZE+1+i) >= 0 ) {
				Areaa[X-SIZE+1+i][Y+j] = Now_Color;
			}
		} 
    }
}

// 画出新数组 
static void DrawNewMap(void)
{
	int xxx1, yyy1, xxx2, yyy2;
	
	// 画正在掉落的方块 
	for(i=0;i<HEIGHT;i++){
		for(j=0;j<WIDTH;j++){
			if( Areaa[i][j] ){
				SetPenColor(Choose_Color(Areaa[i][j]));
				StartFilledRegion(0.9);
				xxx1=7+j*27;
				yyy1=7+i*27;
				xxx2=7+j*27+23;
				yyy2=7+i*27+23;
				DrawPixel(xxx1,yyy1,xxx2,yyy2);
				EndFilledRegion();
			}
		}
   }
	
    // 画下一个方块 
	SetPenColor(Choose_Color(Next_Color));
	for(i=0;i<SIZE;i++){
		for(j=0;j<SIZE;j++){
			if(Bck_block[Next_Block].a[i][j]){
				StartFilledRegion(0.9);
				DrawPixel(312+j*25,72+i*25-25,312+j*25+21,72+i*25+21-25);
				EndFilledRegion();
			}
		} 
	}
}

// 形成工具栏界面
static void ToolInterface(void)
{
	SetPenColor(Choose_Color(8));
	ClearScreen(80,160,370,405);
	
	DrawPixel(175,190,275,220);		
	DrawPixel(175,230,275,260);		
	DrawPixel(175,270,275,300);		 
	DrawPixel(175,310,275,340);	
	DrawPixel(175,350,275,380);	
	
	ShowStr(192, 210, "新游戏 [N]");
	ShowStr(184, 250, "继续游戏 [C]");
	ShowStr(192, 290, "排行榜 [R]");
	ShowStr(200, 330, "帮助 [H]");
	ShowStr(175, 370, "保存并退出 [B]");
} 

// 在(x,y)处显示字符串str 
static void ShowStr(int x, int y, char *str)
{
	MovePen(ScaleXInches(x),ScaleYInches(y));
	DrawTextString(str);
}

// 相应菜单鼠标的交互
static void Interact_Window(void)  
{
	// 排行榜和帮助的返回框图 
	if ( Inf[Is_Rank] || Inf[Is_Help] ){
		
		// 帮助和排行榜的返回【B】的用户交互窗口 
		if ( Inf[Is_Back] ){		
			printf("In Interact  Is_Rank\n");
			DrawPixel(177,367,258,403);
		}
	} else if ( Inf[Is_Menu] ){ // 如果在菜单界面 
		if ( Inf[Menu_Ret1] ) {
			DrawPixel(172,187,278,223);
		}
		if ( Inf[Menu_Ret2] ) {
			DrawPixel(172,227,278,263);
		}
		if ( Inf[Menu_Ret3] ) {
			DrawPixel(172,267,278,303);
		}
		if ( Inf[Menu_Ret4] ) {
			DrawPixel(172,307,278,343);
		}	
		if ( Inf[Menu_Ret5] ) {
			DrawPixel(172,347,278,383);
		}	
	} 
	else { // 游戏界面 
		if ( Inf[Is_Tool] ) {
			// 工具栏用户交互 
			if ( Inf[Tool_Ret1] ) {
				DrawPixel(172,187,278,223);
			}
			if ( Inf[Tool_Ret2] ) {
				DrawPixel(172,227,278,263);
			}
			if ( Inf[Tool_Ret3] ) {
				DrawPixel(172,267,278,303);
			}
			if ( Inf[Tool_Ret4] ) {
				DrawPixel(172,307,278,343);
			}
			if ( Inf[Tool_Ret5] ) {
				DrawPixel(172,347,278,383);
			}		
		}  
	} 
}

// nub为所画数字， len 为数字大小，以像素为单位 
static void Draw_Nub(int x, int y, int nub, int len, int space, int size)
{
	SetPenColor(Choose_Color(8));
	Pen_X = x/GetXResolution();
	Pen_Y = (GetWindowHeight()*GetYResolution()-y)/GetYResolution();
	
	int Sgl_Nub;	// 单个数字 
	int o_pensize = GetPenSize();	// 记录之前画笔宽度 
	SetPenSize(size);			// 设置新的画笔宽度 
	if(nub){						// 画单个数字（从右往左） 
		while(nub){
			Sgl_Nub = nub % 10;
			nub /= 10;
			SignalDigitalNub(len, Sgl_Nub, space);
			Pen_X -= ScaleXInches(2*len+7*space);
		}
	} else {
		SignalDigitalNub(len, 0, space);		 
	}
	SetPenSize(o_pensize);			// 将画笔宽度修改回来 
}

// 画出单个数字 
static void SignalDigitalNub(int len, int nub, int space)
{
	SetPenColor(Choose_Color(8));
	int i, ret[7];
	
	switch (nub) {		// 0-9数字对应数码管亮灭 
		case 0:ret[0]=0;ret[1]=1;ret[2]=1;ret[3]=1;ret[4]=1;ret[5]=1;ret[6]=1;break;  
		case 1:ret[0]=0;ret[1]=1;ret[2]=0;ret[3]=0;ret[4]=0;ret[5]=0;ret[6]=1;break;
		case 2:ret[0]=1;ret[1]=0;ret[2]=1;ret[3]=1;ret[4]=0;ret[5]=1;ret[6]=1;break;
		case 3:ret[0]=1;ret[1]=1;ret[2]=1;ret[3]=0;ret[4]=0;ret[5]=1;ret[6]=1;break;
		case 4:ret[0]=1;ret[1]=1;ret[2]=0;ret[3]=0;ret[4]=1;ret[5]=0;ret[6]=1;break;
		case 5:ret[0]=1;ret[1]=1;ret[2]=1;ret[3]=0;ret[4]=1;ret[5]=1;ret[6]=0;break;
		case 6:ret[0]=1;ret[1]=1;ret[2]=1;ret[3]=1;ret[4]=1;ret[5]=1;ret[6]=0;break;
		case 7:ret[0]=0;ret[1]=1;ret[2]=0;ret[3]=0;ret[4]=0;ret[5]=1;ret[6]=1;break;
		case 8:ret[0]=1;ret[1]=1;ret[2]=1;ret[3]=1;ret[4]=1;ret[5]=1;ret[6]=1;break;
		case 9:ret[0]=1;ret[1]=1;ret[2]=1;ret[3]=0;ret[4]=1;ret[5]=1;ret[6]=1;break;	
	}
	// 画数码管单个笔画 
	for(i=0; i<4; i++){		
		Draw_Space(space);		// 开始空格 
		Draw_Line(len, ret[i]);	// 画线 
		Draw_Space(space);		// 结束空格 
		Agl += 90;				// 画笔转向 
	}
	Agl-= 90;
	for(i=4; i<7; i++){
		Draw_Space(space);
		Draw_Line(len, ret[i]);
		Draw_Space(space);
		Agl += 90;
	}
	Agl -= 180; 
}

// 画出数字间空隙 
static void Draw_Space(int len)
{
	double Len_X = ScaleXInches(len);
	double Len_Y = GetWindowHeight() - ScaleYInches(len);
	switch ( Agl % 360 ) {
		case 0: 	Pen_X += Len_X; break;
		case 90:	Pen_Y -= Len_Y; break;
		case 180:	Pen_X -= Len_X; break;
		case 270:	Pen_Y += Len_Y; break;
	}
} 

// 画单独一条线，可以虚画 
static void Draw_Line(int len, int ret)
{
	SetPenColor(Choose_Color(8));
	double Len_X = ScaleXInches(len);
	double Len_Y = GetWindowHeight() - ScaleYInches(len); 
	MovePen(Pen_X,Pen_Y);
	// 对应不同角度，画笔长度宽度可能不同 
	switch ( Agl % 360 ) {
		case 0:
			if(ret){
				DrawLine(Len_X, 0);
			} 
			Pen_X += Len_X;
			break;
		case 90:
			if(ret){
				DrawLine(0, -Len_Y); 
			} 
			Pen_Y -= Len_Y;
			break;
		case 180:
			if(ret){
				DrawLine(-Len_X, 0);
			}
			Pen_X -= Len_X;
			break;
		case 270:
			if(ret){
				DrawLine(0, Len_Y);
			}
			Pen_Y += Len_Y;
			break;
	}
}

// 获得当前系统时间，以指针形式返回 
static void GetTime(int *hour, int *min, int *sec)
{
	time_t timep;
	struct tm *p;
	time(&timep);
	p = localtime(&timep);
	*hour = p->tm_hour;
	*min = p->tm_min;
	*sec = p->tm_sec;
} 

// 游戏界面时间显示 
static void Show_Time(void)
{
	int hour, min, sec;
	string o_color = GetPenColor();
	GetTime(&hour, &min, &sec);
	if(hour<10){
		Draw_Nub(380,528,0,6,2,3);
	} 
	Draw_Nub(395,528,hour,6,2,3);
	if(min<10){
		Draw_Nub(415,528,0,6,2,3);
	}
	Draw_Nub(430,528,min, 6,2,3); 
	if(sec%2){
		Draw_Point(410,523,4);
		Draw_Point(410,533,4);
	}
	SetPenColor(o_color);
}

// 绘制点（时间显示专用）
static void Draw_Point( int x, int y, int size)
{
	double X = ScaleXInches(x);
	double Y = ScaleYInches(y);
	int o_size = GetPenSize();
	SetPenColor(Choose_Color(8));
	SetPenSize(size);
	MovePen(X,Y);
	DrawLine(0.0001,0.0001);
	SetPenSize(o_size);
}

// 画暂停按钮 
static void DrawPause(void){
	SetPenColor(Choose_Color(8));
	StartFilledRegion(0.9);
	MovePen(ScaleXInches(345),ScaleYInches(518));
	DrawPixelLine(365,515,385,525);
	DrawPixelLine(385,525,365,535);
	DrawPixelLine(365,535,365,515);
	EndFilledRegion();
}

// 画开始按钮
static void DrawStart(void){
	SetPenColor(Choose_Color(8));
	StartFilledRegion(0.9);
	DrawPixel(345,518,350,538);
	EndFilledRegion();
	StartFilledRegion(0.9);
	DrawPixel(360,518,365,538);
	EndFilledRegion();
} 

// 主菜单图形美化函数 
static void DrawTet(void)
{
	static int cnt;
	int i,j;
	
	// 图案的赋值 
	for(i=0;i<5;i++){
		for(j=0;j<22;j++){
			if(tetris[i][j]){
				if( !(cnt%10) ){
					tetris[i][j] = rand()%7+1;
				}
				SetPenColor(Choose_Color(tetris[i][j]));
				StartFilledRegion(0.9);
				DrawPixel(20+j*19,20+i*19,20+j*19+17,20+i*19+17);
				EndFilledRegion();
			}
		} 
	}
	
	// 绘制图案	
	for(i=0;i<6;i++){
		for(j=0;j<10;j++){
			if(drawdown[i][j]){
				SetPenColor(Choose_Color(drawdown[i][j]));
				StartFilledRegion(0.9);
				DrawPixel(20+j*42,290+i*42,20+j*42+39,290+i*42+39);
				EndFilledRegion();
			}
		} 
	}
	cnt++;
}

// 消除动画 
static void Delete_Gif(void)
{
	DisplayClear();
	DrawPixel(5,5,446,545);
	DrawPixel(4,4,447,546);
	DrawPixel(3,3,448,547);
	GameInterface();
	Show_Time();
	
	for(i=0; i<HEIGHT; i++){
		if( Del_Line[i] ){
			Areaa[i][Inf[Del_Gif]-2] = 0; 
		}
	}
	DrawNewMap();
}
