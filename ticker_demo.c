

#include <stdio.h>
#include <sys/time.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

int main(){

	void countdown(int);
	int set_ticker(int);

	signal(SIGALRM, countdown);
	if ( set_ticker(300) == -1 )
		perror("set_ticker");
	else
		while( 1 )
			pause();
	return 0;
}

void countdown(int signum){
	static int num = 20;
	printf("%d ..", num--);
	fflush(stdout);
	if( num < 0 ){
		printf("DONE!\n");
		exit(0);
	}
}

int set_ticker( int n_msecs )
{
	struct itimerval new_timeset;
	long n_sec, n_usecs;

	n_sec = n_msecs / 1000;  /* int part */
	n_usecs = ( n_msecs % 1000 ) * 1000L;  /* remainder */

	new_timeset.it_interval.tv_sec = n_sec; /* set reload */
	new_timeset.it_interval.tv_usec = n_usecs; /* new ticker value */
	new_timeset.it_value.tv_sec = n_sec; /* store this */
	new_timeset.it_value.tv_usec = n_usecs; /* and this */

	return setitimer(ITIMER_REAL, &new_timeset, NULL);
}

