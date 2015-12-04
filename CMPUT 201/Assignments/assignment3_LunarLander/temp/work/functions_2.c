/*
    name:               Mohammad Sirajee
    ONE Card number:    1255986
    Unix id:            sirajee
    lecture section:    EB1
    instructor's name:  Jacqueline Smith
    lab section:        EH02
    TA's name:          Nicolas Barriga, Shida He,
                        and Rong Feng 
*/

#include "lander.h"

void drawLander(FILE* executable, double *landerAddress, int *drawThruster) {
  double *lander = landerAddress;
  //draw the lander
  fprintf(executable, "drawSegment %ld %ld %ld %ld\n", lround(lander[0]), 
      lround(lander[1]), lround(lander[2]), lround(lander[3])); 
  fprintf(executable, "drawSegment %ld %ld %ld %ld\n", lround(lander[2]), 
      lround(lander[3]), lround(lander[4]), lround(lander[5]));
  fprintf(executable, "drawSegment %ld %ld %ld %ld\n", lround(lander[4]), 
      lround(lander[5]), lround(lander[6]), lround(lander[7]));
  fprintf(executable, "drawSegment %ld %ld %ld %ld\n", lround(lander[6]), 
      lround(lander[7]), lround(lander[0]), lround(lander[1]));
  
  //draw the thrusters
  if (*drawThruster) {
    fprintf(executable, "drawSegment %ld %ld %ld %ld\n", lround(lander[0]), 
        lround(lander[1]), lround(lander[8]), lround(lander[9]));
    fprintf(executable, "drawSegment %ld %ld %ld %ld\n", lround(lander[6]), 
        lround(lander[7]), lround(lander[8]), lround(lander[9])); 
  }
}

void updateParameters() {

}

void setupCurses()
{
  // use return values.  see man pages.  likely just useful for error
  // checking (NULL or non-NULL, at least for init_scr)
  initscr();
  cbreak();
  noecho();
  // needed for cursor keys (even though says keypad)
  keypad(stdscr, true);
}

void unsetCurses()
{
  keypad(stdscr, false);
  nodelay(stdscr, false);
  nocbreak();
  echo();
  endwin();
}

// called when SIGALRM is sent.
// could be called for other signals, so signal parameter indicates
// which one.
void updateGraphics(int signal)
{
  static int called = 0;

  called++;

  // called because of SIGALRM
  if (signal == SIGALRM)
  {

    // gets timer, puts it into timer (man 2 getitimer)
    struct itimerval timer;
    if (getitimer(ITIMER_REAL, &timer) == -1)
      exit(EXIT_FAILURE);

    printf("called: %d\n", called);

    // stops timer on 5th call.  In asn3, you should do this when the
    // game is done (e.g., ship has crashed or landed)
    if (called == 5)
    {
      // in asn3, you should use tv_usec
      timer.it_interval.tv_sec = 0;
      timer.it_value.tv_sec = 0;
      exit(EXIT_SUCCESS);
    }

    if (setitimer(ITIMER_REAL, &timer, NULL) == -1)
      exit(EXIT_FAILURE);
  }

}

// critical code is code that must not be interrupted.  In your asn3,
// examples will include code that updates global data structures, and
// code that erases an old ship and draws a new ship (i.e., moves the
// ship).
//
// The only way code can be interrupted in asn3 code is via our timer
// (i.e., by SIGALRM).
//
// We protect critical code by blocking the signal.
//
// The following is just an example of signal blocking; there is not
// critical code to block.
void drawWithoutBlock()
{
  // remember old list of signals to block (none, in our case)
  sigset_t old_mask;
  // set blocked signal set to block_mask_g (man 2 sigprocmask).
  // so now block SIGALRM.
  if (sigprocmask(SIG_BLOCK, &block_mask_g, &old_mask) < 0)
    exit(EXIT_FAILURE);

  // CRITICAL CODE GOES HERE ... can call other functions here, and
  // they will not be interrupted

  // unblock signal by setting mask to old value (which in our
  // case wasn't blocking anything)
  if (sigprocmask(SIG_SETMASK, &old_mask, NULL) < 0)
    exit(EXIT_FAILURE);
}
