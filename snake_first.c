
#include <curses.h>
#include <unistd.h>

#define LEFTEDGE 20
#define RIGHTEDGE 20

void main(int argc, char ** argv)
{

	int initrow = 20;
	int initcol = 20;
	int row, col;
	
	initscr(); // /* turn of curses */

	clear(); /* send requests */

	move(10,20); /* row10, col20 */
	addstr("Hello, world! Hello, curses!");
	sleep(5);

	
	move(1,1);
	standout();
	addstr("haha");
	move(2,2);
	standend();
	
	refresh();
	sleep(10);
	clear();
	
	addstr("hahaha");
	move(LINES-1,0);


	refresh(); /* update the screen */
	getch(); /* wait for user input */
	//sleep(10);
	


	clear();
	move(10,20);
	standout();
	addstr("                        ");
	refresh();
	
	move(20,20);
	standend();
	addstr("!                  !");
	refresh();

	sleep(10);




	endwin(); /* turn off curses */

}
