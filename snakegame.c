#include<stdio.h>
#include<curses.h>
#include<stdlib.h>
#include<malloc.h>
#include<sys/time.h>
#include<unistd.h>
#include<signal.h>
#include<time.h>


#define N 60 // for 1 hour = 3600 sec check
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




struct coord direct; // for next direction
struct coord food; // for food coordinate
struct node *head; // for snake's head
struct node *tail; // for snake's tail
struct node *temp; // for temp
struct node *temp2; // for temp2

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



int life=1; // for cheking snake alive
int ch; //
int time1; // know for time 
int level; // level count
int hour; // for hour
int minute; // for minutes
int second; // for seconds
int length; // for snake body's length
int max_row; 
int max_col;

int end_flag = 0; // for game end check

int main()
{
        initscr(); // start curses
        initsnake(); // initialize snkae
		signal(SIGALRM,printrefresh); // check SIGALRM, run printrefresh
        getInput(); // get user stdin
	    endwin(); // end curses
	
	return 0;
}



void initsnake()
{
	
 	noecho();  // no echo mode
	curs_set(0);  // curse point set 0

    interface(); // image making for interface
	srand((unsigned int)time(0)); // for random number
	direct.x=1; // initialize next cols = 1
	direct.y=0; // initialize next rows = 0
	ch='d'; // initialize inputching ch = 'd'
	level=0; // initialize start level = 0
	time1=0; // initialize time1 = 0
	food.x=rand()%(weight-2)+1; // food x-coordinates
	food.y=rand()%(high-2)+4; // food y-coordinates
	creatLink(); //
    set_ticker(10); // 

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
{ // view game information status at top line
	time1++; // end time
    	if(time1 >= 1000000)       
         time1 = 0;
    	if(1 != time1 % 50)
         return;
    	move(1,6);
   	 printf("%d:%d:%d", hour, minute, second); // view time status bar
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
    	printf("%d", length); // view users score status bar
    	move(1,37);
    	level = length / 3 + 1; // game level difficulty setting
    	printf("%d", level); // view now game difficulty
}


void  printS_b()
{ 
/* 
1. setting for food view 
2. check snake's life to game end
3. setting for game over image view
4. modify snake's moving using insertnode, deletenode
5. check snake eat food. if eating, grow up snake 

*/
	 if(1 != time1 % (50-level))
        return;

        bool lenChange = false; 
    	move(food.y, food.x); // food point set
    	standout();
		printf("*"); // food shape define
		standend();
	if(life==0) // life for re-game
	{
		sleep(5); // crying..
		endwin(); // end curses
		exit(0); // this game break!!!!!!!!
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
		{ // snake meet itself, uroboroth?
			gameover(2); // meet body so end game
			return;
		}
		
		insertNode(head->next->x+direct.x, head->next->y+direct.y);
		
		if(head->next->x==food.x && head->next->y==food.y)
		{ // congratuation!! snake eat food!!
			lenChange = true; // snake growing!!
			length++; // snake grow up!
			if(length >= 100)
			{ // snake's max length is 100
				gameover(3); // too long so end game
            return;
			}
			
			food.x=rand()%(weight-2)+1; // create next food x-coordinate
			food.y=rand()%(high-2)+4; // create next food y-coordinate
		}
		
		if(!lenChange) // only use snake hungry
		{
        move(tail->pre->y, tail->pre->x);
        printf(" "); 
        deleteNode(); // snake keep moving....
		}
		
		move(head->next->y, head->next->x);
		printf("#"); // set snake's image
	}



}


void printrefresh()
{
        signal(SIGALRM, printrefresh);
		snakeInformation(); // setting for view game information status at top line
        printS_b();
        refresh(); // view total images();

}

void getInput()
{
        while(1)
         {
                ch= getch();
                
                switch(ch)
                {
                    case 'a' : 
                        if(direct.x!=1)
                        {
                            direct.x=-1;
                            direct.y=0;
                        }
                        break;

                    case 's' :
                        if(direct.y!=-1)
                        {
                            direct.x=0;
                            direct.y=1;
                        }
                        break;

                    case 'd' :
                        if(direct.x!=-1)
                        {
                            direct.x=1;
                            direct.y=0;
                        }
                        break;

                    case 'w' :
                        if(direct.y!=1)
                        {
                            direct.x=0;
                            direct.y=-1;
                        }
                        break;

                    default :
                        break;
                }
                 set_ticker(5);
 
        }
} // check user input, user control snake



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
    head = (node *)malloc( sizeof(node) ); // dynamic allocate memory to head
    tail = (node *)malloc( sizeof(node) ); // dynamic allocate memory to tail
    temp->x = weight/2; // set starting x-coordinate point
    temp->y = high/2; // set starting y-coordinate point
    head->pre = tail->next = NULL; // 
    head->next = temp; // head link to starting point
    tail->pre = temp; 
    temp->next = tail; 
    temp->pre = head;
} // snake body node making

void insertNode(int x, int y)
{
    node *temp = (node *)malloc( sizeof(node) );
    temp->x = x; // save snake's future head x-coordinate
    temp->y = y; // save snake's future head y-coordinate
    temp->next = head->next; // head->next = unknown world
    head->next = temp; 
    temp->pre = head;
    temp->next->pre = temp;
    //link snake's next vector and snake's head coordinate finished
}

void deleteNode()
{ // snake removing more and more...
    node *temp = tail->pre;
    node *temp2 = temp->pre;
    temp2->next = tail;
    tail->pre = temp2;
    temp->next = temp->pre = NULL;
    free(temp);
    temp = NULL;
}

void deleteLink()
{ // snake destroy
    while(head->next != tail)
        deleteNode();
    head->next = tail->pre = NULL;
    free(head);
    free(tail);
	life -=1; // snake die!!
}
