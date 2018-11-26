#include <stdio.h>
#include <time.h>
#include <malloc.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/select.h>


#define ROW 5
#define COLUMN 5


enum Vector{
	UP, DOWN, LEFT, RIGHT
};


typedef struct element{
	int row;
	int column;
}

typedef struct snake{

	element data;

	struct snake * next;
}snake;

int layer[ROW][COLUMN];
int total_layer[ROW][COLUMN];

void main(void){

	

}

