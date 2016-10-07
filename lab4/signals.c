#include <stdio.h>     /* standard I/O functions                         */
#include <stdlib.h>    /* exit                                           */
#include <unistd.h>    /* standard unix functions, like getpid()         */
#include <signal.h>    /* signal name macros, and the signal() prototype */

/* first, define the Ctrl-C counter, initialize it with zero. */
int ctrl_c_count = 0;
int got_response = 0;
#define CTRL_C_THRESHOLD  5 

/* the Ctrl-C signal handler */
void catch_int(int sig_num)
{
  /* increase count, and check if threshold was reached */
  ctrl_c_count++;
  if (ctrl_c_count >= CTRL_C_THRESHOLD) {
    char answer[30];
    alarm(10);
    
    /* prompt the user to tell us if to really
     * exit or not */
    printf("\nReally exit? [Y/n]: ");
    fflush(stdout);
    fgets(answer, sizeof(answer), stdin);
    if (answer[0] == 'n' || answer[0] == 'N') {
    	alarm(0);
      printf("\nContinuing\n");
      fflush(stdout);
      /* 
       * Reset Ctrl-C counter
       */
      ctrl_c_count = 0;
    }
    else {
      printf("\nExiting...\n");
      fflush(stdout);
      exit(0);
    }
  }
}

/* the Ctrl-Z signal handler */
void catch_tstp(int sig_num)
{
  /* print the current Ctrl-C counter */
  printf("\n\nSo far, '%d' Ctrl-C presses were counted\n\n", ctrl_c_count);
  fflush(stdout);
}

/* the SIGALRM signal handler */
void catch_alrm(int sig_num) {
	printf("\nUser taking too long to response. Exiting . . .\n");
	exit(0);
}

int main(int argc, char* argv[])
{
  struct sigaction sa;
  sigset_t mask_set;  /* used to set a signal masking set. */

  /* setup mask_set */
  sigfillset(&mask_set);
  sigdelset(&mask_set, SIGALRM);
  sa.sa_mask = mask_set;

  /* set signal handlers */
  
  sa.sa_handler = catch_alrm;
  sigaction(SIGALRM, &sa, NULL);
  sa.sa_handler = catch_int;
  sigaction(SIGINT, &sa, NULL);
  sa.sa_handler = catch_tstp;
  sigaction(SIGTSTP, &sa, NULL);
  
  while(1) {
		pause();  
  }

  return 0;
}
