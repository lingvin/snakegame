#include<stdio.h>
#include<curses.h>
#define high 40
#define weight 80

void interface();



int main()
{
        initscr();
        clear();
        interface();
        refresh();
        getch();
        endwin();

}

void interface()
{
         move(0,0);
        addstr("Weclome to snake game");
        move(1,0);
        addstr("Player:");
        move(1,18);
        addstr("Score:");
        move(1,25);
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
           addstr("-");
           addstr("\n");

}

