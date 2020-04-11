# 总体设计

## 1 功能模块设计

按照MVC设计模式，我们将代码分为四个主要模块，分别对应不同需求实现不同功能：

- `Data`：储存方块和用户信息，以及游戏的保存读取，数据的合理性等
- `Model`：方块的运动（包括平移，旋转，下落，加速等），计分，行消除和结束等
- `Draw`：菜单图形，绘制游戏界面，以及消除结束动画等
- `Ctrl`：菜单点击的鼠标回调，游戏的操作栏，以及游戏过程中的操作等

## 2 数据结构设计 

### 2.1 宏

1. 信号变量名称代号

   > 对应信号变量数组`Inf[]`
   >
   > 每一个信号变量的下标对应一个宏名，表明此信号变量指示的功能

   ```c
   #define NUMBER 23		// 信号指示变量个数宏 
   //信号变量，判断是否在此页面
   #define Is_Menu 0		// 图形界面指示变量，1 表示菜单界面，0 表示游戏界面 
   #define In_Game 1		// 游戏信号变量，1 表示正在进行游戏，0 表示游戏未运行 
   #define Read_Name 2		// 用户名信号变量，1 表示读入用户名，0 表示开始游戏
   #define Is_Tool 3		// 工具栏指示变量 ，1 表示在工具栏界面，0 表示不在工具栏界面
   #define Is_End 4		// 游戏结束信号变量，1 表示游戏结束，0 表示游戏没有结束
   #define Is_Rank 5		// 排行榜指示变量，1 表示在排行榜界面，0 表示不在排行榜界面 
   #define Is_Help 6		// 帮助指示变量，1 表示在帮助界面，0 表示不在帮助界面
   
   // 菜单交互交互绘图变量 , 对应菜单界面五个选项框 
   #define Menu_Ret1 7		// 开始游戏 
   #define Menu_Ret2 8		// 继续游戏 
   #define Menu_Ret3 9		// 排行榜 
   #define Menu_Ret4 10	// 帮助 
   #define Menu_Ret5 11	// 退出游戏
   
   //工具栏界面交互绘图变量，对应工具栏界面五个选项框
   #define Tool_Ret1 12	// 新游戏
   #define Tool_Ret2 13	// 继续游戏
   #define Tool_Ret3 14	// 排行榜
   #define Tool_Ret4 15	// 帮助
   #define Tool_Ret5 16	// 保存并退出 
   
   #define Back_Ret 17		// 共用交互绘图变量，对应排行榜和帮助界面的返回按钮
   
   //其余宏
   #define Del_Gif 18		// 是否展示消除动画
   #define Name_Ret 19		// 输入交互信号变量 
   #define Name_Remand 20	// 提示用户输入用户名 
   #define Is_Save 21		// 是否有存档 
   #define Is_Quit	22		// 是否退出游戏 
   ```

2. 常数

   > 宏名体现常数的具体含义

   ```c
   #define HEIGHT 20		// 游戏区高度 
   #define WIDTH 10		// 游戏区宽度 
   #define BLOCK_TYPE 19	// 基础方块种类 
   #define SIZE 4			// 每个方块存储区域大小 
   #define NUB 9			// 用户名数组大小
   // 键盘回调所用key值 
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
   ```

3. 文本替换

   ```c
   // 用于鼠标回调函数，表示在某个坐标范围内
   #define In(x1,y1,x2,y2) ( x>=(x1) && x<=(x2) && y>=(y1) && y<=(y2) )
   ```


### 2.2 数据结构体

1. 方块结构体

   - 结构定义：

     ```c
     typedef struct Block_{
     	int a[SIZE][SIZE]; 
     }Block;
     ```

   - 结构成员描述：

     - `Block`表示一个方块，其中`SIZE` 为宏，定义值为4；
     - 数据`a[SIZE][SIZE]`存储方块形状，0表示没有方块，1表示有方块。

2. 用户结构体

   - 结构体定义

     ```c
     typedef struct Con_node{
     	int num;					// 排名      
     	char name[NUB];				// 用户名 
     	int score;					// 分数 
     	int level;					// 难度 
     	int c_line;					// 消除行数 
     	struct Con_node * next;		// 下一个
     } Con；
     ```

   - 结构成员描述：

     - `Con`存储每个用户的必要数据，并且定义为链表节点形式，方便之后用链表组织排行榜数据，其中`NUB`为宏，定义值为9；
     - `num`确定用户在排行榜中的排名；
     - `name[NUB]`存储用户用户名，字符长度不超过8个；
     - `score`存储用户游戏最终得分；
     - `level`存储用户最终游戏排名；
     - `c_line (Clear Line)` 存储用户总共消除的行数；
     - `*next`用于指向下一个用户节点。 

### 2.3 全局变量

1. 全局信号变量

   - 变量定义：`int Inf[NUMBER];`
   - 变量说明：
     - `NUMBER`为宏，定义值为23，代表一共23个全局信号变量；

     - `Inf[NUMBER]`表示Information，其中每一个变量都是一个信号指示变量，通过改变信号变量的值，确定当前游戏进度，完成和用户的图形交互；

     - 其中每个下标都用相对应的宏名进行替换，如`Inf[Is_Menu]`代表引用了信号数组`Inf[0]`的值（`Is_Menu`宏值为0），通过访问此变量的值，程序可以知道当前游戏是处于菜单界面（`Inf[0]=1`）或者是游戏界面（`Inf[0]=0`）。

     - 全局信号变量条件流程：

2. 全局游戏数据变量

   - `int Now_Timer;`

     记录当前游戏模型Timer的ID，用于方块下落速度随游戏难度提升（其值应该与Level值相同）。

   - `const Block Bcl_block[BLOCK_TYPE];`

     基础方块，一共`BLOCK_TYPE`（19）种，每个方块有具体编号（0-18）通过`Bcl_block[i]`来访问第i种类型方块。

   - `int Now_Block, Next_Block;`

     记录当前方块和下一个方块的类型编号。

   - `int Now_Color, Next_Color;`

     记录当前方块和下一个方块的颜色编号。

   - `int Area[HEIGHT][WIDTH];`

     记录游戏方块区域，从图上来，原点在左上角，向右WIDTH正向，向下为HEIGHT正向。游戏区域中0代表没有方块，1-7代表对应颜色代号的方块。通过对Area数组值的修改，实现方块位置和颜色的对应。

   - `int X, Y;`

     记录当前方块在游戏区域的横纵坐标（即相对于游戏区域的位置）。

   - `int Del_Line[HEIGHT];`

     记录本次消除中被消除的行号，以便于显示消除动画。

   - `char Con_Name[NUB];`

     记录用户名，不超过NUB-1个字符。

   - `int G_Score;`

     记录用户得分。

   - `int Level;`

     记录用户当前难度。

   - `int C_Line;`

     记录用户已经消除的行数。

   - `int S_Line;`

     记录用户开局时起始有方块的行数。

   - `int Num;`

     记录排行榜用户个数。

   - `Con *Head;`

     排行榜链表头结点。

## 3 函数功能描述

### 3.1 `Main`

1. - 函数原型：`void Main();`

   - 功能描述：
     - 初始化游戏数据；
     - 初始化窗口；
     - 向系统声明回调函数；
     - 启动绘图Timer。

2. - 函数原型：`void TimeListener(int id);`
   - 功能描述：时间回调函数；
   - 参数描述：当前调用此函数的`TimerID`；
   - 函数算法描述：
     - ID为0的是绘图`Timer`，每20毫秒调用一次`Draw()`进行绘图；
     - 其他ID为游戏模型`Timer`，调用频率随游戏难度增加，调用`Model_Flow()`，修改全局数据。

3. - 函数原型：`void InitGame(void);`
   - 功能描述：
     - 初始化部分全局信号变量；
     - 读入文件`Rank.txt`数据形成排行榜链表，储存在`Head`指针里；
     - 尝试读取`Con_Save.txt`（用户存档数据）以此判断是否具有存档。

### 3.2 `Data`

#### 3.2.1 模块连接函数

> 由于`Data`是模块之间连接的桥梁，其特殊性决定`Data`内部的函数都为全局函数，可以在全局调用。

1. - 函数原型：`Con* Con_Create(void);`
   - 功能描述：新建适用于每一局游戏结束时根据全局变量创建新节；
   - 返回值描述：指针p；
   - 重要局部变量定义：Con *p；
   - 重要局部变量用途描述：指针p用来指向待储存的位置。

2. - 函数原型：`Con* Con_Input(Con *head, Con *consumer);`
   - 功能描述：插入新用户进入排行榜；
   - 参数描述：头指针`head`指向链表第一项；指针`consumer`指向待插入的新的学生记录点；
   - 返回值描述：头指针`head`；
   - 重要局部变量定义：`Con *ptr， *ptr1， *ptr2`；
   - 重要局部变量用途描述：三个指针用来移动结点位置。

3. - 函数原型：`void Con_Print(Con *head);`
   - 功能描述：遍历并且输出链表信息（调试用）；
   - 参数描述：头指针`head`；
   - 重要局部变量定义：`Con *ptr`；
   - 重要局部变量用途描述：用于遍历链表。 

4. - 函数原型：`void Con_Sort(Con *head);`
   - 功能描述：对排行榜链表排序；
   - 参数描述：头指针`head`；
   - 重要局部变量定义：`Con *ptr1， *ptr2`；
   - 重要局部变量用途描述：用于指向待比较的两组数据。 

5. - 函数原型：`Con* Con_Fileopen(void);`
   - 功能描述：读入排行榜文件，形成链表结构；
   - 返回值描述：头指针`head`；
   - 重要局部变量定义：`FILE *fp`；
   - 重要局部变量用途描述：用于进行文件的处理。

6. - 函数原型：`void Con_Fileprint(Con *head, int n);`
   - 功能描述：读取链表写入文件中；
   - 参数描述：头指针`head`；用户个数`n`。 

7. - 函数原型：`void Con_Save(void);`
   - 功能描述：保存游戏数据并且写入文件中； 

8. - 函数原型：`void Con_Continue(void);`
   - 功能描述：读取文件载入游戏，直接载入并修改全局变量；
   - 重要局部变量定义：`int i，j`；`FILE *fp`；
   - 重要局部变量用途描述：
     - `i，j`用于遍历游戏界面储存的数组；
     - `*fp`用于进行文件操作。

9. - 函数原型：`void Con_DestorySave(void);`
   - 功能描述：删除掉之前的存档，并且写入一个`#`表明文档无效 。

10. - 函数原型：`int Con_Is_Save(void);`
    - 功能描述：判断是否存在存档文件；
    - 返回值描述：
      - `0` 无存档文件；
      - `1` 有存档文件；
    - 重要局部变量定义：`char ret`；
    - 重要局部变量用途描述：用于判断文件内是否为`#`。 

### 3.3 `Model`

####  3.3.1 模块间连接函数

1. - 函数原型：`void Go_Button_M(void);`
   - 功能描述：在 `Ctrl`中调用，实现空格使方块直接落下；
   - 函数算法描述：
     - 当`Fall_Block()`值为0的时候，会持续移动使方块下落；
     - 根据`Is_End_Game()`返回的值，判断是否结束游戏`Is_End_Game()`或者形成下一个方块`Creat_Block()`。

2. - 函数原型：`void Model_Flow(void);`
   - 功能描述：在`Main`中调用，`Timer`回调函数组成部分，表明游戏流程； 
   - 函数算法描述：
     - 通过判断是否在游戏中以及方块是否落下，选择相应的操作；
     - 如果在游戏中`Inf[In_Game]`和`Fall_Block()`均为1的时候，调用`Timer_Model()`，判断是否需调整难度；
     - 根据`Is_End_Game()`返回的值，判断是否结束游戏`Is_End_Game()`或者形成下一个方块`Creat_Block()`。

3. - 函数原型：`void Delete_Row(void);`
   - 功能描述：在`Draw`中调用，方便实现消除动画； 
   - 函数算法描述：
     - 判断是否满行，满行删除将`Del_Line[i]`赋值为1，调用`Move_Area(i)`，并修改游戏区已消除的行，使`C_Line++`；
     - 调用`Change_Score()`，修改`Inf[Del_Gif]`为1以显示动画。

####  3.3.2 模块内部函数

1. * 函数原型：`static void Creat_Block(void)`
   * 功能描述：生成方块； 
   * 函数算法描述：确定方块种类、颜色、初始坐标。

2. - 函数原型：`static int Fall_Block(void);`
   - 功能描述：判断方块下落，是否已经落地，是1否0；
   - 返回值描述：`int ret`，返回值0或1；
   - 函数算法描述：
     - 对下落的方块处理，将其加入已落下的方块中`Fixed_Block()`；
     - 修改分数，单纯地落下方块调用`Change_Score(0)`。

3. - 函数原型：`static void Fixed_Block(void);`
   - 功能描述：将已经固定的方块加入游戏区，固定方块。 

4. - 函数原型：`static int Is_Touch(void);`
   - 返回值描述：`int ret`，返回值0或1；
   - 功能描述：
     - 判断是否接触到底部，是1否0。
     - 如果当前方块下还有方块或者已经超出边界，结果返回1。

5. - 函数原型：`static int Is_End_Game(void);`
   - 功能描述：判断结束游戏，是1否0； 
   - 返回值描述：`int ret`，返回值0或1。
   - 函数算法描述：如果游戏区中最上面一行区域，有方块`Area[0][i]`为1，即结束游戏。

6. - 函数原型：`static void Move_Area(int row);`
   - 功能描述：将游戏区 `0 ~ (row-1)` 行整体向下平移一行； 
   - 参数描述：`int`型`row`为所删除的那一行；
   - 函数算法描述：将`1-row-1`行向下移动一行，最上行填零。

7. - 函数原型：`static void Change_Score(int ret);`
   - 功能描述：改变分数；
   - 参数描述：`int`型`ret `为消除行数； 
   - 函数算法描述：通过`switch`来实现。

8. - 函数原型：`static void Timer_Model(void);`
   - 功能描述：难度控制函数，通过管理`Timer`控制速度； 
   - 函数算法描述：根据`Level`和`C_Line`调整难度，先将`Now_Timer`取消，再新建`startTimer(Level, (21-Level)*40)`，并且使得`Now_Timer = Level`。

9. - 函数原型：`static void End_Game(void);`
   - 功能描述：游戏结束后的处理，返回到开始菜单； 
   - 函数算法描述：取消当前`Now_Timer`，通过`data`中函数`Con_Input()`、`Con_Sort()`添加并排序排行榜的信息，同时初始化游戏界面,使得`Inf[Is_Menu]`为1，`Inf[In_Game]`为0，`Inf[Is_Rank]`为1。 

 

### 3.4 `Ctrl`

#### 3.4.1 模块全局变量

- 变量定义：`static int Name_cnt;`
- 变量说明：记录用户名字字符长度，防止字符数组引用越界引起的严重后果。

#### 3.4.2 模块间连接函数

1. - 函数原型：`void MouseCtrl(int x, int y, int button, int event);`
   - 功能描述：鼠标回调函数，对应用户鼠标操作，需要在`Main`中使用；
   - 参数描述：`x`,`y`为鼠标坐标，`button`为鼠标对应按键，`event`为鼠标操作；
   - 函数算法描述：通过分析全局信号变量，决定具体调用哪一个子函数进行相关鼠标控制。

2. - 函数原型：`void KeyBoardCtrl(int key, int event);`
   - 功能描述：键盘回调函数，对应用户键盘操作，需要在`Main`中使用；
   - 参数描述：`key`为键盘按钮编号，`event`为键盘操作事件
   - 函数算法描述：通过分析全局信号变量，决定具体调用哪一个子函数进行相关键盘控制。

3. - 函数原型：`void Init_Data(void);`
   - 功能描述：
     - 初始化由用户输入的数据，包括起始难度等级，起始游戏区域行数，并且将游戏设置为进行状态；
   - 函数算法描述：
     - 在初始化起始游戏区域随机方块的时候，采用赋值语句：
       - `Area[i][j] = !(rand()%4)*(rand()%7+1);`
     - 这句话中前半句`!(rand()%4)` 表示1/4概率形成方块（值为1）；
     - 后半句`(rand()%7+1)`随机形成颜色代码`(1-7)`；
     - 两者相乘，表达含义：25%概率生成一个随机颜色的方块填充到游戏区。

4. - 函数原型：`void Init_Game(void);`
   - 功能描述：
     - 初始化非用户选择的数据，包括初始难度，行数的设定，清除上一次游戏用户的数据(消除行数，分数等等)，设定方块位置坐标，初始和下一个方块种类，用户名清零以及一些游戏和信号变量的初始化。
   - 函数算法描述：下一个方块是采用随机数生成的方块代号。

#### 3.4.3 模块内部函数

1. - 函数原型：`static void Go_Button(void);`
   - 功能描述：
     - 对应空格落实方块的操作；
     - 因为具体功能要大量调用`Model`中的函数，为减少模块耦合度，此函数的具体功能在`Model`中同名函数`Go_Button_M()`中实现，再将`Go_Button_M()`声明给`Ctrl`，`Go_Button()`直接调用即可。具体功能的实现详见`Model`中关于`Go_Button_M()`的说明。

2. - 函数原型：`static void MenuKeyCtrl(int key, int event);`
   - 功能描述：菜单界面的键盘回调函数，实现菜单界面快捷键功能；
   - 参数描述：同`KeyBoardCtrl()`的参数； 
   - 函数算法描述：函数分为两个部分，用户交互部分和功能实现部分：
     - 用户交互部分：当用户按键不放的时候，对应不同按键使不同交互信号变量值为1；通知`Draw`在相应位置绘制长方形（用户交互）直到用户松开按键。
     - 功能实现部分：在用户松开按键时，将交互变量修改回0。根据不同的按键实现相关功能，其中涉及到比较复杂的条件判断。在此举例两个：
       - 按下`C`：表明用户想继续之前的存档；此时访问信号变量`Inf[Is_Save]`判断是否有存档。如果有存档，则会调用子函数读取存档，读取完成之后销毁当前存档（防止反复读档），之后修改信号变量`Inf[In_Game]`，`Inf[Is_Menu]`，`Inf[Read_Name]`等指示`Draw`绘制新的图片。
       - 按下`R`：表明用户想读取排行榜数据；通过修改信号变量`Inf[Is_Rank]`来通知`Draw`绘制或者擦除排行榜图案。

3. - 函数原型：`static void GameKeyCtrl(int key, int event);`

   - 功能描述：游戏过程中的键盘回调函数，为控制方块移动的键盘按键；

   - 参数描述：同`KeyBoardCtrl()`的参数；

   - 重要局部变量定义：`static int ret;`

   - 重要局部变量用途描述：

     - 专为按键次数定义，对应实现一直按键只需要一次操作的函数，如在旋转方块时，按住上键不放，方块一直旋转显得比较鬼畜，因此设计按键指示变量`ret`，确保上键按住不放只会旋转方块一次，直到松开按键，下次按键才能再次旋转。

   - 函数算法描述：

     - 采用`switch-case`语句，判断不同的按键情况，从而继续调用子函数实现方块的左右移动，旋转，加速下落和直接落实；

     - 其中，按下键所需要实现的功能即为`Model`中实现的自动下落，因此当游戏未处于暂停状态的时候，直接调用`TimerListener(1)`即可实现方块加速下落。 

4. - 函数原型：`static void NameKeyCtrl(int key, int event);`
   - 功能描述：用户输入按键回调函数，实现用户输入用户名等操作的键盘控制；
   - 参数描述：同`KeyBoardCtrl()`的参数；
   - 函数算法描述：
     - 通过读取用户按键，修改全局游戏变量`Con_Name[]`，`S_Line`，`Level`的值，实现用户可选难度和输入用户名的功能，
     - 在按下回车之后，通过修改通信变量，改变游戏图标为游戏界面，同时初始化用户可选择的游戏，并且开始游戏。 

5. - 函数原型：`static void HotKeyCtrl(int key, int event);`
   - 功能描述：用户游戏界面键盘回调，实现暂停和工具栏调用；
   - 参数描述：同`KeyBoardCtrl()`的参数； 
   - 重要局部变量定义：`static int ret;`
   - 重要局部变量用途描述：
     - 专为按键次数定义，对应实现一直按键只需要一次操作的函数。详见上面第三个函数；
   - 函数算法描述：通过修改通信变量，通知`Model`暂停和继续，通知`Draw`绘制不同的窗口。

6. - 函数原型：`static void ToolKeyCtrl(int key, int event);`
   - 功能描述：工具栏键盘回调函数；
   - 参数描述：同`KeyBoardCtrl()`的参数；
   - 函数算法描述：
     - 通过判断按键，修改通信变量控制`Draw`绘制不同窗口图案，或者调用不同的函数实现存档等功能。 

7. - 函数原型：`static void MenuMouseCtrl(int x, int y, int button, int event);`
   - 功能描述：菜单界面的鼠标回调，实现用户在菜单界面的鼠标点击
   - 参数描述：同`MouseCtrl()`的参数；
   - 函数算法描述：
     - 依旧使用了用户交互变量和全局通信变量，点击相应部分实现的功能和键盘控制一样，在此不再赘述。

8. - 函数原型：`static void HotMouseCtrl(int x, int y, int button, int event);`
   - 功能描述：游戏中暂停和工具栏鼠标回调，实现鼠标点击工具和暂停按钮的功能；
   - 参数描述：同`MouseCtrl()`的参数；
   - 函数算法描述：同`HotKeyCtrl()`。

9. - 函数原型：`static void ToolMouseCtrl(int x, int y, int button, int event);`
   - 功能描述：工具栏鼠标回调函数，对应实现工具栏的鼠标点击功能
   - 参数描述：同`MouseCtrl()`的参数； 
   - 函数算法描述：相关功能描述和实现同`ToolKeyCtrl()`。 

10. - 函数原型：`static void NameMouseCtrl(int x, int y, int button, int event);`
    - 功能描述：用户名输入界面的鼠标回调，实现用户输入届面的鼠标点击功能；
    - 参数描述：同`MouseCtrl()`的参数；
    - 函数算法描述：
      - 难度增减，初始行数增减实现方法同`NameMouseCtrl()`；
      - 除此之外，当鼠标移动到`Go`图标的时候，修改另一个通信变量`Inf[Name_Ret]`，通知`Draw`绘制彩色`Go`。

11. - 函数原型：`static int Is_Touch(void);`

    - 功能描述：判断方块是否和游戏区已有方块重叠，或者方块是否已经超出游戏边界； 

    - 返回值描述：当方块超出游戏区，或者已经重叠，则返回值为真，否则为假；

    - 函数算法描述：此函数有两个比较复杂的表达式：

      - 第一个：`Bck_block[Now_Block].a[i][j]`
        - 此表达式表示当前方块种类对应的方块4*4区域中第`i`行第`j`列的方块位置是否有方块填充，如果有则值为1，否则值为0；

      - 第二个：`Area[X-SIZE+1+i][Y+j]`
        - 此表达式为坐标位置为`X`，`Y`的方块`a[i][j]`的位置对应在游戏区域的位置，这是通过坐标变换得到的。

12. - 函数原型：`static void Turn_Block(void);`
    - 功能描述：通过修改当前方块代号，实现方块的旋转；
    - 重要局部变量定义：`int Old_Block;`
    - 重要局部变量用途：记录旋转之前的方块种类
    - 函数算法描述：
      - 在改变方块代号之后，需要检测方块是否发生不必要的重叠等等，如果发生重叠，证明不应旋转，此时通过将`Old_Block`的值可以将方块复原回去，相当于不旋转。 

13. - 函数原型：`static void Left_Move(void);`
    - 功能描述：将方块位置左移；
    - 函数算法描述：
      - 如果并未处于消除动画（消除动画时一切静止），那么左移只需要将`Y`坐标减一，但是之后要判断左移后是否出现重叠现象，如果重叠则将`Y`值修改回来。

14. - 函数原型：`static void Right_Move(void);`
    - 功能描述：将方块位置右移；
    - 函数算法描述：同`Left_Move()`。

### 3.5 `Draw`

#### 3.5.1 模块全局变量

1. - 变量定义：`static int i,j;`
   - 变量说明：`for`循环变量参数，不在函数内重复定义。

2. - 变量定义：`static int Areaa[HEIGHT][WIDTH];`
   - 变量说明：绘图数据数组，从数据库中更新，记录游戏区10*20个小块的颜色与状态。

3. - 变量定义：`static double Pen_X, Pen_Y;`
   - 变量说明：记录画笔位置。

4. - 变量定义：`static int Agl; `
   - 变量说明：记录当前角度。

5. - 变量定义：`const static int drawdown[6][10];`
   - 变量说明：主菜单下方方块图案数组，记录6*10个小块的颜色与状态。

6. - 变量定义：`static int tetris[5][22];`
   - 变量说明：主菜单上方 Tetris 方块，记录5*22个小块的颜色与状态。

#### 3.5.2 模块间连接函数

1. - 函数原型：`void Draw(void);`
   - 功能描述：供`Timer`调用，在此函数中实现所有`draw`相关函数的选择与调用； 
   - 函数算法描述：
     - 先调用`DisplayClear()`进行清屏，同时形成整个游戏边框。
     - 如果`Inf[Is_Menu]`为1，显示主菜单`MenuInterface()`，在这种情况下，当`Inf[Is_Quit]`为1的时候，退出游戏。
     - 如果`Inf[Is_Menu]`为0，说明不在菜单界面，进入游戏页面。`GameInterface()`，`Delete_Gif()`，`Show_Time()`，实现基本显示。
     - 如果`Inf[Is_Tool]`为1，显示工具栏`ToolInterface()`。
     - 如果在游戏中，`[In_Game]`为1，此时，显示开始按钮，`DrawStart()`。
     - 如果不在游戏中，`[In_Game]`为0，根据`Inf[Is_Rank]`和`Inf[Is_Help]`的值，调用工具栏中的功能。为了做出闪动的效果，通过`Cnt`控制定时闪动组件，绘制`DrawPause()`。

#### 3.5.3 模块内部函数

1. - 函数原型：`static void DrawPixel(int x1, int y1, int x2, int y2);`
   - 功能描述：根据输入的像素点画矩形边框；
   - 参数描述：`x1`，`x2`，`y1`，`y2`为对角线上像素点的坐标；
   - 重要局部变量定义：`double xx1, yy1, xx2, yy2;`
   - 重要局部变量用途描述：将输入的像素点转化为`double`类型。 

2. - 函数原型：`static string Choose_Color(int x);`
   - 功能描述：设定画笔的颜色；
   - 参数描述：`x`表示几号颜色；
   - 返回值描述：返回颜色指针；
   - 重要局部变量定义：`char *color;`
   - 重要局部变量用途描述：将库里的字符串赋给指针；
   - 函数算法描述：一个`switch`语句。

3. - 函数原型：`static void ClearScreen(int x1, int y1, int x2, int y2);`
   - 功能描述：将局部的区域清屏；
   - 参数描述：输入由像素点构成的区域。 

4. - 函数原型：`static void ShowStr(int x, int y, char *str);`
   - 功能描述：在`(x,y)`处显示字符串`str`；
   - 参数描述：`(x,y)`为像素点表示的坐标，`*str`为字符串指针。 

5. - 函数原型：`static void DrawPixelLine(int x1, int y1, int x2, int y2);`
   - 功能描述；由给定的像素位置画线段；
   - 参数描述：`x1`，`x2`，`y1`，`y2`为对角线上像素点的坐标；
   - 重要局部变量定义：`double xx1, yy1, xx2, yy2;`
   - 重要局部变量用途描述：将输入的像素点转化为`double`类型。 

6. - 函数原型：`static void ToolInterface(void);`
   - 功能描述：绘制工具栏界面； 
   - 函数算法描述：设定画笔颜色，画出所选功能的边框，输出相应的指针。 

7. - 函数原型：`static void GameInterface(void);`
   - 功能描述：绘制游戏界面；
   - 函数算法描述：
     - 当`Inf[Read_Name]=1`的时候，显示输入用户名界面，否则形成左边游戏区背景。形成右边显示区功能以及时间、得分等信息。

8. - 函数原型：`static void MenuInterface(void);`
   - 功能描述：绘制主菜单界面； 
   - 函数算法描述：绘制背景图片，根据用户选择切换到不同界面。

9. - 函数原型：`static void CreateNewMap(void);`
   - 功能描述：形成新的图形数组数据；
   - 函数算法描述：将已经落下的方块数据赋给`Areaa[i][j]`，同时将正在下落方块也一同赋值。

10. - 函数原型：`static void DrawNewMap(void);`
    - 功能描述：参照形成的新数据绘图；
    - 函数算法描述：画正在掉落的方块和下一个方块。 

11. - 函数原型：`static void Interact_Window(void);`
    - 功能描述：用户鼠标交互图形，鼠标移动指示框； 
    - 函数算法描述：如果鼠标移动到相应的框内，显示更大的指示框。 

12. - 函数原型：`static void DrawPause(void);`
    - 功能描述：绘制暂停符号。 

13. - 函数原型：`static void DrawStart(void);`
    - 功能描述：绘制开始符号。

14. - 函数原型：`static void NameFace(void);`
    - 功能描述：用户数据读入窗口，显示用户名、难度选择、初始行数。 

15. - 函数原型：`static void Draw_Go(void);`
    - 功能描述：绘制`Go`图案，如果用户输完用户名之后，鼠标移至上方会变色；
    - 重要局部变量定义：`int go[5][9]`，赋初值，表示颜色序号。 

16. - 函数原型：`static void DrawTet(void);`
    - 功能描述：绘制`Tetris`图案，同时每隔一段时间刷新颜色。 

17. - 函数原型：`static void DrawRank(void);`
    - 功能描述：绘制排行榜；
    - 函数算法描述：通过遍历用户数据链表，显示相关信息。

18. - 函数原型：`static void DrawHelp(void);`
    - 功能描述：绘制帮助菜单。 

19. - 函数原型：`static void DrawMenuChoice(void);`
    - 功能描述：绘制主菜单开始界面。 

20. - 函数原型：`static void Delete_Gif(void);`

    - 功能描述：实现消除方块的动画；

    - 函数算法描述：

      - 根据全局变量`Inf[Del_Gif]`调用`switch`语句，实现动画效果；

      - 当值为0的时候，调用`Delete_Row()`，根据`model`中返回的值，若为1，说明需要删除，此时`Inf[Del_Gif]`为1，取消当前时间`cancelTimer(Now_Timer)`，同时`Inf[Del_Gif]++`，这时满足`default`的条件，调用`Delete_Line()`，在里面给`Areaa`赋值0，实现清除效果，这样不断`default`直到`Inf[Del_Gif]`为`WIDTH+4`，恢复游戏模型调用`startTimer(Level, (21-Level)*40 )`，赋值`Inf[Del_Gif]`为0，退出动画模块。 

21. - 函数原型：`static void Delete_Line(void);`
    - 功能描述：行消除动画数据更改，将需要删除的行中每一项都变为0。

22. - 函数原型：`static void Draw_Line(int len, int ret);`
    - 功能描述：绘制一条线
    - 参数描述：`len`代表线的长度（单位：像素），`ret`表示是否绘制这条线；
    - 函数算法描述：
      - 通过访问全局变量`Pen_X`和`Pen_Y`，将笔挪到相应位置，参考`Agl`的方向，选择绘制线条或者不绘制，无论是否绘制线段，笔的位置都会随之改变。

23. - 函数原型：`static void SignalDigitalNub(int len, int nub, int space);`
    - 功能描述：绘制七段数码管风格的0-9中的一个数字；
    - 参数描述：`len`为一划的长度，`nub`为待绘制数字，不超过10，`space`为笔画之间空格； 
    - 重要局部变量定义：`int ret[7];`
    - 重要局部变量用途：`ret[7]`存储了所需绘制的数字哪些笔画需要绘制`（1）`，哪些不需要绘制`（0）`；
    - 函数算法描述：
      - 七段数码管从数码管中间的横开始绘制，按照顺时针方向继续绘制剩余六个笔画，在这个过程中，会调用七次`Draw_Line()`，并且将`ret[i]`的值作为参数传递进去，通过控制`ret`的值来控制最终绘制的效果，显示不同的数字。

24. - 函数原型：`static void Draw_Space(int len);`
    - 功能描述：绘制一小段空格，形成七段数码管的风格；
    - 参数描述：`len`表示空格长度（像素）；
    - 函数算法描述：根据`Agl`的方向，更改`Pen_X`或者`Pen_Y`的值，形成一段空格。

25. - 函数原型：`static void Draw_Nub(int x, int y, int nub, int len, int space, int size);`
    - 功能描述：绘制一个七段数码管风格的数字；
    - 参数描述：
      - `x`，`y`为绘制位置的起始坐标，`nub`为待绘制的数字，`len`为数字笔画长度，`space`为空格间距，`size`为笔粗细；
    - 函数算法描述：
      - 将所得到的数字拆分，调用`SignalDigitalNub()`绘单个数字，从低位到高位逐位绘制，绘制一段之后将笔的位置前移，再次绘制新的数字，直到绘制完成。

26. - 函数原型：`static void Show_Time(void);`
    - 功能描述：结合七段数码管函数绘制时间；
    - 重要局部变量定义：`int hour, min, sec;`
    - 重要局部变量用途：其地址作为`GetTime()`的参数传入，以获取时间；
    - 函数算法描述：
      - 判断时间可能出现的不同情况，并在指定位置绘制时间，同时制作出时间冒号随秒闪动的效果。 

27. - 函数原型：`static void GetTime(int *hour, int *min, int *sec);`
    - 功能描述：获得当前系统时间，并且写入指针所指的变量里；
    - 参数描述：`*hour`，`*min`，` *sec`分别为等待写入数据的小时，分钟，秒的变量地址；
    - 函数算法描述：
      - 通过系统库函数`localtime()`获得系统时间，并且将小时，分钟，秒这三个数的数值写入对应变量里。

28. - 函数原型：`static void Draw_Point(int x, int y, int size);`
    - 功能描述；绘制一个点；
    - 参数描述：`x`，`y`为绘制点的坐标，`size`为绘制点的大小；
    - 函数算法描述：转门为了显示时间的冒号而设计的函数。 
