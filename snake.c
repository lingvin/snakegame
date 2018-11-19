#include<stdio.h>
#include<curses.h>
#define high 40
#define weight 80






int main()
{
        initscr();
        clear();
        move(0,0);
        addstr("Weclome to snake game");
        move(1,0);
        addstr("Player:");
        move(1,8);
        addstr("Score:");
        move(1,15);
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

refresh();
getch();
endwin();

}

