// Illustrates how to use a timer to print something every x seconds,
// and how to block code from being interrupted by this timer

// need to define this macro as functions we're using are not C99
// compliant
#define _POSIX_C_SOURCE 200112L

// must include these 2 header files
#include <signal.h>
#include <sys/time.h>

#include <stdio.h>
#include <stdlib.h>

// our function which we will have called whenever the timer expires
void handle_timeout(int signal);

// will store alarm signal to indicate we want it blocked
sigset_t block_mask_g;

// note, ideally the error handling should be better done than below,
// and style isn't necessarily great
int main()
{
  // just initializes so it's empty (man 3 sigemptyset)
  sigemptyset(&block_mask_g); 
  // block_mask_g will now indicate to signal functions that something
  // (i.e., blocking) must be to SIGALRM signal
  sigaddset(&block_mask_g, SIGALRM); 

  // set up signal handler (man 2 sigaction)
  /* *********************************************************** */

  // will store information for our alarm
  struct sigaction handler;

  // handle_timeout is function to call when signal (in our case an
  // alarm) is sent
  handler.sa_handler = handle_timeout;
  // empties sa_mask, meaning no signals will be blocked when our
  // signal_handler (handle_timeout) is executing
  sigemptyset(&handler.sa_mask);
  // if non-zero, indicates various options ... we don't want to use
  // them
  handler.sa_flags = 0;

  // sets up a signal handler as specified in handler struct.
  // when SIGALRM occurs (set up below), handle_timeout will now be
  // called.
  if (sigaction(SIGALRM, &handler, NULL) == -1)
    exit(EXIT_FAILURE);

  // set up timer
  /* *********************************************************** */

  // sets up values for timer (intervals between sending SIGALRM).
  // times in microseconds (1/1 millionth of a second).
  // note although close enough for us, this is only approximate.
  // (man 2 setitimer)
  struct itimerval timer;

    // value for time between sending SIGALRM
    // The sample program set tv_usec to be 50000 (and tv_sec to be
    // zero), which meant the ship was redrawn approximately every
    // 50000 microseconds
    struct timeval time_delay;
    time_delay.tv_sec = 1;
    //time_delay.tv_usec = 50000;

    timer.it_interval = time_delay;

    // value for initial time (before 1st alarm is triggered)
    struct timeval start;
    start.tv_sec = 3;
    //start.tv_usec = 50000;

    timer.it_value = start;

  // will execute repeatedly, sending SIGALRM every timer.it_interval
  // microseconds, who initial value is timer.it_value.
  // should handle error better (and should use a macro).  should use
  // errno.  note it may save you some hassle if you do this, too
  if (setitimer(ITIMER_REAL, &timer, NULL) == -1)
    exit(EXIT_FAILURE);

  // infinite loop
  for ( ; ; )
    ;

  // should never execute
  exit(EXIT_SUCCESS);
}


// called when SIGALRM is sent.
// could be called for other signals, so signal parameter indicates
// which one.
void handle_timeout(int signal)
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
void example_of_blocking_a_signal()
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
