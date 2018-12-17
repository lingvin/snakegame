#include<stdio.h>
#include<curses.h>
#include<stdlib.h>
#include<malloc.h>
#include<sys/time.h>
#include<unistd.h>
#include<signal.h>
#include<time.h>


#define N 60
#define weight 40 // == length
#define high 20 // == height
#define TRUE 1
#define FALSE 0
#define RAISE_UP 1


typedef struct coord
{
        int x;
        int y;
}coord;
typedef struct node
{
        int x;
        int y;
        struct node *pre;
        struct node *next;
}node;




struct coord direct;
struct coord food;
struct node *head;
struct node *tail;
struct node *temp;
struct node *temp2;
void initsnake();
void interface();
void getInput();
void CreatFood();
void creatLink();
void insertNode(int x , int y);
void deleteNode();
void deleteLink();
int set_ticker(int);
void printS_b();
void printrefresh();
void snakeInformation();
void gameover(int i);



int life=1;
int ch;
int time1;
int level;
int hour;
int minute;
int second;
int length;
int max_row;
int max_col;

int end_flag = 0; // for game end check

int main()
{
        initscr();
        initsnake();
		signal(SIGALRM,printrefresh);
        getInput();
	       
	endwin();
	
	return 0;
}



void initsnake()
{
	
 	noecho(); 
	curs_set(0);


    interface();
	srand((unsigned int)time(0));
	direct.x=1;
	direct.y=0;
	ch='d';
	level=0;
	time1=0;
	food.x=rand()%(weight-2)+1;
	food.y=rand()%(high-2)+4;
	creatLink();
    set_ticker(10);

}



void interface()
{
        move(0,0);
        addstr("Weclome to snake game");
        move(1,0);
        addstr("time:");
        move(1,18);
        addstr("Score:");
        move(1,30);
        addstr("Level:");
        move(2,0);
        int i=0,j=0;
        for(;i<weight+1;i++)
           addstr("-");
           addstr("\n");
           for(;j<high;j++)
        {
                addstr("|");
                for(i=1;i<weight;i++)
                        addstr(" ");
                addstr("|");
                addstr("\n");
        }

         for(i=0;i<weight+1;i++)
		 {
			 standout();
			 addstr(" ");
			 standend();
		 }
           addstr("\n");


}



int set_ticker(int n_msecs)
{
    struct itimerval new_timeset;
    long    n_sec, n_usecs;

    n_sec = n_msecs / 1000 ;
    n_usecs = ( n_msecs % 1000 ) * 1000L ;
    new_timeset.it_interval.tv_sec  = n_sec;
    new_timeset.it_interval.tv_usec = n_usecs;

    new_timeset.it_value.tv_sec     = n_sec  ;
    new_timeset.it_value.tv_usec    = n_usecs ;
    return setitimer(ITIMER_REAL, &new_timeset, NULL);
}



void snakeInformation()
{
	time1++;
    	if(time1 >= 1000000)       
         time1 = 0;
    	if(1 != time1 % 50)
         return;
    	move(1,6);
   	 printw("%d:%d:%d", hour, minute, second);
    	second++;
    	if(second > N)
    	 {
        	second = 0;
       	 	minute++;
    	 }
    	if(minute > N)
   	 {
        	minute = 0;
        	hour++;
    	}
    	move(1, 26);
    	printw("%d", length);
    	move(1,37);
    	level = length / 3 + 1; // level setting
    	printw("%d", level);
}


void  printS_b()
{
	 if(1 != time1 % (50-level))
        return;

        bool lenChange = false; 
    	move(food.y, food.x); // food point set
    	standout();
		printw("*"); // food shape define
		standend();
	if(life==0) // life for re-game
	{
		sleep(5);
		endwin();
		exit(0);
	}
	else
	{
		if((weight-1==head->next->x && 1==direct.x)
        || (1==head->next->x && -1==direct.x)
        || (high+2==head->next->y && 1==direct.y)
        || (3==head->next->y && -1==direct.y) )
		{
			gameover(1); // meet wall so end game
			return;
		}
		
		if('#' == mvinch(head->next->y+direct.y, head->next->x+direct.x) )
		{
			gameover(2); // meet body so end game
			return;
		}
		
		insertNode(head->next->x+direct.x, head->next->y+direct.y);
		
		if(head->next->x==food.x && head->next->y==food.y)
		{
			lenChange = true;
			length++;
			if(length >= 100)
			{ // snake's max length is 100
				gameover(3); // too long so end game
            return;
			}
			
			food.x=rand()%(weight-2)+1;
			food.y=rand()%(high-2)+4;
		}
		
		if(!lenChange)
		{
        move(tail->pre->y, tail->pre->x);
        printw(" ");
        deleteNode();
		}
		
		move(head->next->y, head->next->x);
		printw("#");
	}



}


void printrefresh()
{
        signal(SIGALRM, printrefresh);
		snakeInformation();
        printS_b();
        refresh(); // view total images();

		//if (end_flag == TRUE)
		//	signal(SIGALRM, printrefresh);

}

void getInput()
{
        while(1)
         {
                ch= getch();
                
				if('a'==ch)
                {
					if(direct.x!=1)
					{
                 	 	 direct.x=-1;
                 		 direct.y=0;
					}
                }
                else if('s'==ch)
                {
					if(direct.y!=-1)
					{
                 		 direct.x=0;
                		 direct.y=1;
					}
                }
                 else if('d'==ch)
				 {
					 if(direct.x!=-1)
					 {
                		 direct.x=1;
                		 direct.y=0;
					 }
                }
                 else if('w'==ch)
                {
					if(direct.y!=1)
					{
                		 direct.x=0;
                		 direct.y=-1;
					}
                }
                 set_ticker(5);
 
        }
}



void gameover(int i)
{
  	 move(0, 0);
    int j;
    for(j=0;j<weight;j++)
        addstr(" ");
    move(high/2,weight/2);
    if(1 == i)
        addstr("Game over");
    else if(2 == i)
        addstr("Game over");
    else if(3 == i)
        addstr("YOU WIN");
	      
    	deleteLink(); 
 	set_ticker(5);  

}


void creatLink()
{
    node *temp = (node *)malloc( sizeof(node) );
    head = (node *)malloc( sizeof(node) );
    tail = (node *)malloc( sizeof(node) );
    temp->x = weight/2;
    temp->y = high/2;
    head->pre = tail->next = NULL;
    head->next = temp;
    temp->next = tail;
    tail->pre = temp;
    temp->pre = head;
}
void insertNode(int x, int y)
{
    node *temp = (node *)malloc( sizeof(node) );
    temp->x = x;
    temp->y = y;
    temp->next = head->next;
    head->next = temp;
    temp->pre = head;
    temp->next->pre = temp;
}
void deleteNode()
{
    node *temp = tail->pre;
    node *temp2 = temp->pre;
    temp2->next = tail;
    tail->pre = temp2;
    temp->next = temp->pre = NULL;
    free(temp);
    temp = NULL;
}
void deleteLink()
{
    while(head->next != tail)
        deleteNode();
    head->next = tail->pre = NULL;
    free(head);
    free(tail);
	life -=1;
	
}
