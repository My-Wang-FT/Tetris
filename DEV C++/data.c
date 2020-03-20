#include "data.h"
#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
 
//基础方块模型，以左下角为底，一共十九种方块 
const Block Bck_block[BLOCK_TYPE] = {
	{	  // 方块 00 
	  {	{ 0, 0, 0, 0 },
		{ 0, 1, 1, 0 },
		{ 0, 1, 1, 0 },
		{ 0, 0, 0, 0 }, } 
	} , { // 长条 01 
	  {	{ 0, 1, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 1, 0, 0 }, } 
	} , { // 长条 02 
	  {	{ 0, 0, 0, 0 },
		{ 1, 1, 1, 1 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }, } 
	} , { // 正 Z 03 
	  {	{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 1, 1, 0, 0 },
		{ 0, 1, 1, 0 }, } 
	} , { // 正 Z 04
	  {	{ 0, 0, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 1, 1, 0 },
		{ 0, 1, 0, 0 }, } 
	} , { // 反 Z 05 
	  {	{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 1, 1 },
		{ 0, 1, 1, 0 }, } 
	} , { // 反 Z 06
	  {	{ 0, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 1, 1, 0 },
		{ 0, 0, 1, 0 }, } 
	} , { // 正 L 07
	  {	{ 0, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 1, 1, 0 }, } 
	} , { // 正 L 08
	  {	{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 1, 1, 1, 0 },
		{ 1, 0, 0, 0 }, } 
	} , { // 正 L 09
	  {	{ 0, 0, 0, 0 },
		{ 1, 1, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 1, 0, 0 }, } 
	} , { // 正 L 10
	  {	{ 0, 0, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 1, 1, 1, 0 },
		{ 0, 0, 0, 0 }, } 
	} , { // 反 L 11
	  {	{ 0, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 1, 1, 0, 0 }, } 
	} , { // 反 L 12
	  {	{ 0, 0, 0, 0 },
		{ 1, 0, 0, 0 },
		{ 1, 1, 1, 0 },
		{ 0, 0, 0, 0 }, } 
	} , { // 反 L 13
	  {	{ 0, 0, 0, 0 },
		{ 0, 1, 1, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 1, 0, 0 }, } 
	} , { // 反 L 14
	  {	{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 1, 1, 1, 0 },
		{ 0, 0, 1, 0 }, } 
	} , { // T 型 15
	  {	{ 0, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 1, 1, 1, 0 },
		{ 0, 0, 0, 0 }, } 
	} , { // T 型 16
	  {	{ 0, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 1, 1, 0 },
		{ 0, 1, 0, 0 }, } 
	} , { // T 型 17
	  {	{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 1, 1, 1, 0 },
		{ 0, 1, 0, 0 }, } 
	} , { // T 型 18
	  {	{ 0, 0, 0, 0 },
		{ 0, 1, 0, 0 }, 
		{ 1, 1, 0, 0 },
		{ 0, 1, 0, 0 }, } 
	},
};

// 全局变量 
int Now_Timer;				// 记录当前游戏Timer
int Area[HEIGHT][WIDTH];	// 游戏区 
int X, Y;					// 正在下落的方块位置坐标 
int Next_Block;				// 下一个方块类型 
int Next_Color;				// 下一个方块颜色 
int Now_Block;				// 当前方块类型 
int Now_Color;				// 当前方块颜色 
int S_Line;					// 起始行数	 
int G_Score;				// 得分 
int Level;					// 难度等级 
int C_Line; 				// 消除行数 
int Del_Line[HEIGHT];		// 当前即将消除的行，从游戏区上往下计数 
int Num;					// 排行榜人数 
char Con_Name[NUB];			// 当前游戏用户名 
Con* Head = NULL;			// 当前游戏排行榜用户头指针 

// 综合信号变量区域，使用方法详见data.h文件宏定义部分 
int Inf[NUMBER];

// 用户链实现函数 

//新建 适用于每一局游戏结束时根据全局变量创建新节点 
Con* Con_Create(void)
{
	Con *p;
	p = (Con*)malloc( sizeof (Con) );			
	p->num = 0; 
	p->score = G_Score;
	strcpy(p->name, Con_Name);
	p->level = Level;
	p->c_line = C_Line;
	p->next = NULL;		
	return p;
}


//插入新用户进入排行榜 
Con* Con_Input(Con *head, Con *consumer)
{
	Con *ptr, *ptr1, *ptr2;
	
	ptr2 = head;			 
	ptr  = consumer;			//ptr指向待插入的新的学生记录结点 
	//原链表为空的插入 
	if (head == NULL){
		head = ptr;				//新插入的结点成为头结点 
		head->next = NULL;	
	} 
	//原链表不为空的插入 
	else {
		while( (ptr->num > ptr2->num) && (ptr2->next != NULL) ){
			//ptr1和ptr2各后移一个结点 
			ptr1 = ptr2;
			ptr2 = ptr2->next;
		}
		if ( ptr->num <= ptr2->num ) {
			if ( head == ptr2 )  head = ptr;
			else   ptr1->next = ptr;
			ptr->next = ptr2;
		} else {				//新插入的结点成为尾结点 
			ptr2->next = ptr;
			ptr->next  = NULL;
		}
	}
	return head;
}

//遍历并且输出链表信息（调试用） 
void Con_Print ( Con *head )
{
	Con *ptr;
	if ( head == NULL ) {
		printf("No records found\n");
	} else {
		printf("排名\t用户名\t得分\t难度\t消除行数\n");
		for( ptr = head; ptr != NULL; ptr = ptr->next){
			printf("%d\t%s\t%d\t%d\t%d\n", ptr->num, ptr->name, ptr->score, ptr->level, ptr->c_line);
		}	
	}
}

//对排行榜链表排序 
void Con_Sort(Con *head)
{
    Con *ptr1, *ptr2;
    int temp;
    char exname[NUB];
	for (ptr1 = head; ptr1 != NULL; ptr1 = ptr1->next){
		for (ptr2 = ptr1; ptr2 != NULL; ptr2 = ptr2->next){
			//当位于后面的结点的score较大时，交换其与前一个结点除num外的所有值 
			if (ptr1->score < ptr2->score){
				temp = ptr1->score;
				ptr1->score = ptr2->score;
				ptr2->score = temp;
				temp = ptr1->c_line;
				ptr1->c_line = ptr2->c_line;
				ptr2->c_line = temp;
				
				temp = ptr1->level;
				ptr1->level = ptr2->level;
				ptr2->level = temp;
				
				strcpy(exname, ptr1->name);
				strcpy(ptr1->name, ptr2->name);
				strcpy(ptr2->name, exname);
			}
		}
	}
	// 再次遍历，书写排名序号
	int cnt = 1;
	for (ptr1 = head; ptr1 != NULL; ptr1 = ptr1->next){
		ptr1->num = cnt++;
	}
}

//读入排行榜文件，形成链表结构 
Con* Con_Fileopen(void){
	Con *head = NULL, *p;
	int num, score, level, c_line;
	char name[NUB];
	int i;
	int size = sizeof (Con);
	FILE *fp = fopen ("Rank.txt","r");
	
	if (fp==NULL){
		printf("No File!\n");
		return NULL; 
	} else {
		fscanf(fp,"%d",&Num);
		for (i=0; i<Num; i++){
			fscanf(fp,"%d%s%d%d%d", &num, name, &score, &level, &c_line);
			p = (Con*)malloc(size);			
			p->num = num; 
			p->score = score;
			strcpy(p->name, name);
			p->level = level;
			p->c_line = c_line;
			head = Con_Input(head, p);
			p = p->next;
		
		}
		if(fclose(fp)){
			printf("Can not close the file!\n");
			exit(0);
		}
	}
	return head;
}

//读取链表写入到文件中
void Con_Fileprint(Con *head, int n)
{
	Con *ptr;
	int num, score, level, c_line;
	char name [NUB];
	int i;
	int size = sizeof (Con);
	FILE *fp = fopen("Rank.txt","w");
	fprintf(fp,"%d\n",n);
	for(ptr = head; ptr != NULL; ptr = ptr->next){
		fprintf(fp, "%d\t%s\t%d\t%d\t%d\n", ptr->num, ptr->name, ptr->score, ptr->level, ptr->c_line);
	}
	fclose(fp);
} 
		
//保存游戏数据并且写入文件中 
void Con_Save(void)
{
	int i, j, n; 
	//分别记录要保存的数据 
	int SArea[HEIGHT][WIDTH];
	int SNow_Block, SNow_Color; 
	int SNext_Block, SNext_Color;
	int SX, SY; 
	char SCon_Name[NUB];
	int SG_Score, SLevel, SC_Line; 
	
	//将要保存的数据存储到相对应的S中 
	for(i=0; i<HEIGHT; i++){
		for(j=0; j<WIDTH; j++){
			SArea[i][j] = Area[i][j];
		}
	}
	SNow_Block = Now_Block;
	SNow_Color = Now_Color;
	SNext_Block = Next_Block;
	SNext_Color = Next_Color;
	SX = X;
	SY = Y;
	strcpy(SCon_Name, Con_Name);
	SG_Score = G_Score;
	SLevel = Level;
	SC_Line = C_Line;
	
	//将保存的数据输出到Con_Save.txt中	 
	FILE *fp;
	fp = fopen("Con_Save.txt","w");

	for(i=0; i<HEIGHT; i++){
		for(j=0; j<WIDTH; j++){
			fprintf(fp, "%d\t", SArea[i][j]);
		}
		fprintf(fp, "\n");
	}
	fprintf(fp, "%s\n", SCon_Name);
	fprintf(fp, "%d\t%d\n", SNow_Block, SNow_Color);	
	fprintf(fp, "%d\t%d\n", SNext_Block, SNext_Color);
	fprintf(fp, "%d\t%d\n", SX, SY);
	fprintf(fp, "%d\t%d\t%d\n", SG_Score, SLevel, SC_Line);
	
	//关闭文件 
	fclose(fp);
} 

//读取文件载入游戏 	,直接载入并且修改全局变量 
void Con_Continue(void)
{
	int i, j, n; 
	//从Con_Save.txt 读取数据 
	FILE *fp;
	fp = fopen("Con_Save.txt","r");
	
	for(i=0; i<20; i++){
		for(j=0; j<10; j++){
			fscanf(fp, "%d", &Area[i][j]);
		}
	}
	
	fscanf(fp, "%s", Con_Name);
	fscanf(fp, "%d%d", &Now_Block, &Now_Color);
	fscanf(fp, "%d%d", &Next_Block, &Next_Color);
	fscanf(fp, "%d%d", &X, &Y);
	fscanf(fp, "%d%d%d", &G_Score, &Level, &C_Line);
	fclose(fp);
	
	Now_Timer = Level;
	
	startTimer(Now_Timer, (21-Level)*40); //重启当前Timer 
} 

// 删除掉之前的存档，并且写入一个  '#' 表明文档无效 
void Con_DestorySave(void)
{
	FILE *fp = fopen("Con_Save.txt","w");
	fprintf(fp,"%c",'#');
	fclose(fp);
} 

// 判断是否存在存档文件 
int Con_Is_Save(void)
{
	FILE *fp = NULL;
	fp = fopen("Con_Save.txt","r");
	char ret;
	if( fp ){ // 读取文件指针 
		fscanf(fp, "%c", &ret);
		if( ret == '#'){ // 文件第一个字符为 # ，没有存档 
			printf("没有存档！\n"); 
			return 0;
		} else { // 否则有存档 
			fclose(fp);
			return 1;
		}	
	} else { // 文件指针为空， 没有存档 
		printf("没有存档！\n");
		return 0;
	}
} 
