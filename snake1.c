
 #include <stdio.h>
 #include <time.h>
 #include <malloc.h>
 #include <unistd.h>
 #include <fcntl.h>
 #include <stdlib.h>
 #include <sys/select.h>
 
 void setIO(int flag)
 {
         if(flag)
                 system("stty cbreak -echo");
         else
                 system("stty cooked echo");
 }
 
 #define high 20
 #define wide 30
 
 #define up 1
 #define down 2
 #define left 3
 #define right 4
 
 int score  = 0;
 int Level = 1;
 int direction = 1;
 int IsEat=0;
 int FoodH=5,FoodW=10;

 char Picture[high][wide];  //游戏框图
 
 typedef struct snake{
       int x;
       int y;
       struct snake* next;
 }Node,*PSnake;
 
 PSnake Init()           //生成一条蛇  坐标为中心点向下5个格，蛇头向上 
 {
    printf("SnakeMake start!\n");
    int len=5;
    PSnake head=(PSnake)malloc(sizeof(Node));
    if(head == NULL)
          printf("Snake head make failed!\n");
    head->x=wide/2;
    head->y=high/2+5;
    head->next=NULL;
 
    int i=0;
   for(;i<5;i++)
   {
      PSnake P=(PSnake)malloc(sizeof(Node));
      if(P==NULL)
      {     printf("Frog king is dead!\n");
             break;
       }
       P->x=wide/2;
       P->y=high/2-i+4;
       P->next=head;
       head=P;
    }
    printf("Snake is alive!\n");
    return head;
 }
 
 PSnake Eat(int x,int y,PSnake snake)
 {
    PSnake p=(PSnake)malloc(sizeof(Node));
    if(p==NULL)
    {
      printf("New head make failed!");
   }
   p->x = x;
    p->y = y;
    p->next=snake;
    score += 100;
    return p;
 }
 
 void Walk(int x,int y,PSnake snake)
 {
    PSnake p=snake;
    int a,b, c=x, d=y;
    while(p!=NULL)
    {
       a=p->x;
       b=p->y;
       p->x = c;
       p->y = d;
       c=a;
      d=b;
       p=p->next;
   }
 }
 
 int Serch(int x,int y,PSnake snake)     //用于检验移动后的蛇头是否吃了自身
{
    PSnake q=snake->next;
    while(q!= NULL)
    {
        if( ( (q->x) == x ) && ( (q->y) == y ) )
                return 1;
        q=q->next;
    }
    return 0;
 }
 
 void WriteSnake(PSnake snake)   //把蛇写入打印数组
 {
    PSnake   p=snake;
    while(p != NULL)
    {
       Picture[p->y][p->x]='*';
       p=p->next;
    }
 }
 
 void Paint(void)       //Init picture 初始化以及还原空图
 {
    int y=high,x=wide,i,j;
    for(i=0; i<y; i++)
          for(j=0; j<x; j++)
                Picture[i][j]=' ';
 }
 void Print(char* p,int score,int Lev)       //打印框架，生成画面
 {
    int a=high,b=wide,i=0,j;
    static int cnt=1;
    printf("\033c Test\n");
    printf("The Snake Game is On!! The Write times is %d ！\n",cnt);
    cnt++;
    printf("Player:%s   Score:%d   Level:%d \n",p,score,Lev);
    while(i<b*2+2)
    {
       printf("-");
       i++;
    }
    printf("\n");
    for (i=0; i<a; i++)
    {
       printf("|");
       for(j=0; j<b; j++)
       {
           printf("%c ",Picture[i][j]);
       }
       printf("|");
       printf("\n");
    }
    for(i=0;i<=b*2+1;i++)
    {
       printf("-");
    }
    printf("\n");
    printf("-------------AAAAAAAA\n");
    printf("----fdsfsdfsd3-------------------------------\n");
   printf("===========++++++++This is bottom!!!!+++++++++++++++++++++\n");
 }
 int MakeFood(void)       //创造食物
 {
    static int MC=0;         //MC =  FoodMake Count
    while(1)    //  食物不能生成在已是snake的部分，确认此处运行在蛇写入的部分之后！！！
   {
       if(MC > ((high * wide)/2 ) )      //限制食物生成数量，留待考虑
             return 0;
       srand((int)time(0));
      FoodH=rand()%high;
       FoodW=rand()%wide;
       if(Picture[FoodH][FoodW] == ' ')
               break;
    }
    //Picture[FoodH][FoodW]='*';
    MC++;
    return 1;
 }
 
 PSnake MakeMove(PSnake s)    //蛇的动作
 {
    int x,y;
    PSnake p=s;
    x=s->x,y=s->y;      //x,y为下一步的坐标
 
    if(direction == up)
            y = y - 1;
    if(direction == down)
            y = y + 1;
   if(direction == right)
            x = x + 1;
    if(direction == left)
            x = x - 1;
    //check if the snake crash the wall
    if( (y>(high-1)) || ((y<0)) || ((x)<0) || (x>(wide-1)) )   //撞墙检测
    {
       printf("x=%d y=%d s.x=%d s.y=%d \n",x,y,s->x,s->y);
       printf("The snake break the wall!");
       return NULL;
    }
    if(Serch(x,y,s))                                 //吃掉自身检测
    {
       printf("x=%d y=%d \n",x,y);
       while(p != NULL)
      {
          printf("p->x= %d p->y= %d \n",p->x,p->y);
          p=p->next;
       }
       printf("Your snake eat itsself!");
       return NULL;
  }
 
   if( (x==FoodW) && (y==FoodH) )     //如果下一步是食物，则吃食物
    {
       s=Eat(x,y,s);
       IsEat=1;
    }
    else                               //不是食物则前进一步
    {
      Walk(x,y,s);
    }
    return s;
 }
 
 int kbhit(void)                        //是否按下检测
 {
         struct timeval tv;
         fd_set rdfs;
         tv.tv_sec = 0;
         tv.tv_usec = 0;
         FD_ZERO(&rdfs);
         FD_SET(STDIN_FILENO,&rdfs);
         select(STDIN_FILENO+1,&rdfs,NULL,NULL,&tv);
         return FD_ISSET(STDIN_FILENO,&rdfs);
 }
 void InputCTL(int level)        //控制输入部分，并根据level控制移动速度
 {
    int Dir=direction;
    int timeUse;
    struct timeval start,end;    
    gettimeofday(&start,NULL);
    setIO(1);
    char c,n;
    while(1)
    {
            gettimeofday(&end,NULL);
            timeUse = 1000000*(end.tv_sec - start.tv_sec) +
                    end.tv_usec - start.tv_usec;
            if(timeUse > 1000000- level*100000)    //通过设定时间段结束循环，达到控制速度的目的
                    break;
            if(kbhit())                           //记录按键信息
               c=getchar();
    }
    setIO(0);
    if( c == 'w')
    {
        Dir=1;
 }
    else if( c == 's')
    {
        Dir=2;
    }
    else if( c == 'a')
    {
        Dir=3;
    }
    else if( c == 'd')
    {
        Dir=4;
    }
    else;
    if( ((Dir == 1) && (direction == down) ) || ((Dir == 2) && (direction == up))
                    || ((Dir == 3) && (direction == right)) || ((Dir == 4) && (direction == left) ) )               //如若与行进方向相反则保持原来方向
    {
            //printf("Dir = %d \n",Dir);
    }
    else
    {
       direction = Dir;
    }
    // printf("The Direction'value is %d !\n",direction);
 }
 
 int CheckLevel(int score)                  //return level
 {
         static int change=0;
         if(((score - change) >= 300) && (Level < 9) )
         {
                 Level ++;
                 change += 300;
         }
         return Level;
 }
 void GameRun(void)
 {
    int GameState=1;
    score=0;
    Level=1;
    char Name[10];
   printf("Please input your name:");
    scanf("%s",&Name);
    PSnake jack=Init();
 
    PSnake p=jack;
    while(GameState)
    {                            
       Paint();             //恢复空的数组
      WriteSnake(jack);    //写入蛇
       if(IsEat)            //判断是否生成新的食物
       {
             if(MakeFood())
                IsEat=0;
       }
       Picture[FoodH][FoodW]='*';   //写入食物
       //system("clear");
       //sleep(2);
       Print(Name,score,CheckLevel(score));                      //打印游戏画面
       InputCTL(Level);           //方向控制、速度控制
       jack = MakeMove(jack);     //蛇的行动
       if( jack == NULL )         //如果蛇头不存在，则游戏结束！
       {
          GameState=0;
          printf("\033c");
          printf("Game Over!\n");
      }
    }
 }
 int main(void)
 {
    GameRun();
         return 0;
 }
