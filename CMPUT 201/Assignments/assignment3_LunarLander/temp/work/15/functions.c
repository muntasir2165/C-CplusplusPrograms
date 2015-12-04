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

/*declare the global variables as extern variables to use in the
functions defined in this file*/
extern FILE* landscapeFile;
extern FILE* executable;
extern struct landerInfo lander;
extern double gravity;
extern double thrust;
extern double velocity;
// extern double gVelocity;
// extern double thrusterVelocity;
extern int thrusterOn;
extern int rotationDirection;
extern char fileName[PROG_LEN];
extern int gameOver;

void processInput(int *argcAddress, char *argv[]) {
  int argc = *argcAddress;
  for (int i = 1; i < argc-1; i++) {
        char flag;
        sscanf(argv[i], "-%c", &flag);

        switch(flag) {
            case 'g':
                sscanf(argv[i+1], "%lg", &gravity);
                break;
            case 't':
                sscanf(argv[i+1], "%lg", &thrust);
                break;
            case 'f':
                sscanf(argv[i+1], "%s", fileName);
                if (i < argc-2) {
                    i++;
                }
                break;
        }
    }
    // printf("gravity: %lg\n", gravity);
    // printf("thrust: %lg\n", thrust);
    // printf("filename: %s\n", fileName);
}

void drawLandscape() {
    double x = 0;
    double y = 0;
    double currentX = x;
    double currentY = y;
    char line[LINE_LEN];
    fgets(line, LINE_LEN, landscapeFile);
    sscanf(line, "%lg%lg", &currentX, &currentY);
    
    //draw the landscape
     while(!feof(landscapeFile)) {
        fgets(line, LINE_LEN, landscapeFile);
        sscanf(line, "%lg%lg", &x, &y);
        fprintf(executable, "drawSegment %ld %ld %ld %ld\n", lround(currentX), 
            lround(currentY), lround(x), lround(y));
        currentX = x;
        currentY = y;
    }

    //flush the executable
    fflush(executable);
}

void initializeLander() {
    //define the parameters needed to draw the lander
    lander.bottomLeftX = 300;
    lander.bottomLeftY = 40;
    lander.topLeftX = 310;
    lander.topLeftY = 5;
    lander.topRightX = 330;
    lander.topRightY = 5;
    lander.bottomRightX = 340;
    lander.bottomRightY = 40;
    lander.thrusterX = 320;
    lander.thrusterY = 48;
    lander.a = lander.topRightX - lander.topLeftX;
    lander.b = lander.bottomRightX - lander.bottomLeftX;
    lander.height = lander.bottomLeftY - lander.topLeftY;
    lander.centreX = lander.topLeftX + ((lander.topRightX-lander.topLeftX) / 2);
    lander.centreY = ( (lander.b+(2*lander.a)) / (3*(lander.a+lander.b)) ) * lander.height;
}

void drawLander() {
    //draw the lander
    fprintf(executable, "drawSegment %ld %ld %ld %ld\n", lround(lander.bottomLeftX), 
        lround(lander.bottomLeftY), lround(lander.topLeftX), lround(lander.topLeftY)); 
    fprintf(executable, "drawSegment %ld %ld %ld %ld\n", lround(lander.topLeftX), 
        lround(lander.topLeftY), lround(lander.topRightX), lround(lander.topRightY));
    fprintf(executable, "drawSegment %ld %ld %ld %ld\n", lround(lander.topRightX), 
        lround(lander.topRightY), lround(lander.bottomRightX), lround(lander.bottomRightY));
    fprintf(executable, "drawSegment %ld %ld %ld %ld\n", lround(lander.bottomRightX), 
        lround(lander.bottomRightY), lround(lander.bottomLeftX), lround(lander.bottomLeftY));

    //flush the executable
    fflush(executable);
}

void eraseLander() {
    //erase the lander
    fprintf(executable, "eraseSegment %ld %ld %ld %ld\n", lround(lander.bottomLeftX), 
        lround(lander.bottomLeftY), lround(lander.topLeftX), lround(lander.topLeftY)); 
    fprintf(executable, "eraseSegment %ld %ld %ld %ld\n", lround(lander.topLeftX), 
        lround(lander.topLeftY), lround(lander.topRightX), lround(lander.topRightY));
    fprintf(executable, "eraseSegment %ld %ld %ld %ld\n", lround(lander.topRightX), 
        lround(lander.topRightY), lround(lander.bottomRightX), lround(lander.bottomRightY));
    fprintf(executable, "eraseSegment %ld %ld %ld %ld\n", lround(lander.bottomRightX), 
        lround(lander.bottomRightY), lround(lander.bottomLeftX), lround(lander.bottomLeftY));

    //flush the executable
    fflush(executable);
}

double rotateX(double x, double y) {
    double angle = rotationDirection * ANGLE;
    double angle_radians = angle * PI / 180.0;
    double rotated_x = x * cos(angle_radians) - y * sin(angle_radians);
    return rotated_x;
}

double rotateY(double x, double y) {
    double angle = rotationDirection * ANGLE;
    double angle_radians = angle * PI / 180.0;
    double rotated_y = x * sin(angle_radians) + y * cos(angle_radians);
    return rotated_y;
}

void rotateLander() {
    eraseLander();
    //store the current centre in temporary variables
    double tempCentreX = lander.centreX;
    double tempCentreY = lander.centreY;

    //translate the individual points in the lander such that
    //they are aligned with respect to the origin (0, 0)
    lander.bottomLeftX -= tempCentreX;
    lander.bottomLeftY -= tempCentreY;
    lander.topLeftX -= tempCentreX;
    lander.topLeftY -= tempCentreY;
    lander.topRightX -= tempCentreX;
    lander.topRightY -= tempCentreY;
    lander.bottomRightX -= tempCentreX;
    lander.bottomRightY -= tempCentreY;
    lander.thrusterX -= tempCentreX;
    lander.thrusterY -= tempCentreY;

    //rotate the points with respect to the origin (0, 0)
    lander.bottomLeftX = rotateX(lander.bottomLeftX, lander.bottomLeftY);
    lander.bottomLeftY = rotateY(lander.bottomLeftX, lander.bottomLeftY);
    lander.topLeftX = rotateX(lander.topLeftX, lander.topLeftY);
    lander.topLeftY = rotateY(lander.topLeftX, lander.topLeftY);
    lander.topRightX = rotateX(lander.topRightX, lander.topRightY);
    lander.topRightY = rotateY(lander.topRightX, lander.topRightY);
    lander.bottomRightX = rotateX(lander.bottomRightX, lander.bottomRightY);
    lander.bottomRightY = rotateY(lander.bottomRightX, lander.bottomRightY);
    lander.thrusterX = rotateX(lander.thrusterX, lander.thrusterY);
    lander.thrusterY = rotateY(lander.thrusterX, lander.thrusterY);

    //translate the individual points in the lander such that
    //they are aligned with respect to their original centre
    lander.bottomLeftX += tempCentreX;
    lander.bottomLeftY += tempCentreY;
    lander.topLeftX += tempCentreX;
    lander.topLeftY += tempCentreY;
    lander.topRightX += tempCentreX;
    lander.topRightY += tempCentreY;
    lander.bottomRightX += tempCentreX;
    lander.bottomRightY += tempCentreY;
    lander.thrusterX += tempCentreX;
    lander.thrusterY += tempCentreY;

    // //update the lander centre
    // lander.a = lander.topRightX - lander.topLeftX;
    // lander.b = lander.bottomRightX - lander.bottomLeftX;
    // lander.height = lander.bottomLeftY - lander.topLeftY;
    // lander.centreX = lander.topLeftX + ((lander.topRightX-lander.topLeftX) / 2);
    // lander.centreY = ( (lander.b+(2*lander.a)) / (3*(lander.a+lander.b)) ) * lander.height;
}

void drawThruster() {
    // fprintf(executable, "setColor 255 0 0\n");
    
    //draw the thruster
    fprintf(executable, "drawSegment %ld %ld %ld %ld\n", lround(lander.bottomLeftX), 
      lround(lander.bottomLeftY), lround(lander.thrusterX), lround(lander.thrusterY));
    fprintf(executable, "drawSegment %ld %ld %ld %ld\n", lround(lander.bottomRightX), 
      lround(lander.bottomRightY), lround(lander.thrusterX), lround(lander.thrusterY)); 
    
    // fprintf(executable, "setColor 0 0 0\n");
    //flush the executable
    fflush(executable);
}

void eraseThruster() {
    //erase the thruster
    fprintf(executable, "eraseSegment %ld %ld %ld %ld\n", lround(lander.bottomLeftX), 
      lround(lander.bottomLeftY), lround(lander.thrusterX), lround(lander.thrusterY));
    fprintf(executable, "eraseSegment %ld %ld %ld %ld\n", lround(lander.bottomRightX), 
      lround(lander.bottomRightY), lround(lander.thrusterX), lround(lander.thrusterY)); 

    //thrusterOn = 0;
    //flush the executable
    fflush(executable);
}

double newY(double oldY, double oldVelocity) {
    double theNewY = oldY + (oldVelocity*t) + ((1.0/2)*gravity*t*t);
    return theNewY;
}

double newVelocity(double oldVelocity) {
    // double thrusterAcceleration = thrusterOn * thrust;
    // double theNewVelocity = oldVelocity + ((gravity+thrusterAcceleration)*t);
    double theNewVelocity = oldVelocity + (gravity* t);
    return theNewVelocity;
}

void updateParameters() {
    double oldVelocity = velocity;
    lander.bottomLeftY = newY(lander.bottomLeftY, oldVelocity);
    lander.topLeftY = newY(lander.topLeftY, oldVelocity);
    lander.topRightY = newY(lander.topRightY, oldVelocity);
    lander.bottomRightY = newY(lander.bottomRightY, oldVelocity);
    lander.thrusterY = newY(lander.thrusterY, oldVelocity);
    velocity = newVelocity(oldVelocity);

    //update the lander centre
    lander.a = lander.topRightX - lander.topLeftX;
    lander.b = lander.bottomRightX - lander.bottomLeftX;
    lander.height = lander.bottomLeftY - lander.topLeftY;
    lander.centreX = lander.topLeftX + ((lander.topRightX-lander.topLeftX) / 2);
    lander.centreY = ( (lander.b+(2*lander.a)) / (3*(lander.a+lander.b)) ) * lander.height;
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

void unsetCurses() {
    keypad(stdscr, false);
    nodelay(stdscr, false);
    nocbreak();
    echo();
    endwin();
}

void initializeTimer(sigset_t block_mask_g) {
    // just initializes so it's empty
    sigemptyset(&block_mask_g);
    
    // block_mask_g will now indicate to signal functions that something
    // (i.e., blocking) must be to SIGALRM signal
    sigaddset(&block_mask_g, SIGALRM); 

    // will store information for our alarm
    struct sigaction handler;

  // updateGraphics is function to call when signal (in our case an
  // alarm) is sent
  handler.sa_handler = updateGraphics;

  // empties sa_mask, meaning no signals will be blocked when our
  // signal_handler (updateGraphics) is executing
  sigemptyset(&handler.sa_mask);

  // if non-zero, indicates various options ... we don't want to use
  // them
  handler.sa_flags = 0;

  // sets up a signal handler as specified in handler struct.
  // when SIGALRM occurs (set up below), updateGraphics will now be
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
    time_delay.tv_sec = 0;
    time_delay.tv_usec = 50000;

    timer.it_interval = time_delay;

    // value for initial time (before 1st alarm is triggered)
    struct timeval start;
    start.tv_sec = 0;
    start.tv_usec = 50000;

    timer.it_value = start;

  // will execute repeatedly, sending SIGALRM every timer.it_interval
  // microseconds, who initial value is timer.it_value.
  // should handle error better (and should use a macro).  should use
  // errno.  note it may save you some hassle if you do this, too
  if (setitimer(ITIMER_REAL, &timer, NULL) == -1)
    exit(EXIT_FAILURE);
    
    //initialize curses
    setupCurses();

  move(5, 10);
  printw("Press any key to start.");
  refresh();
  int c = getch();

  nodelay(stdscr, true);
  erase();

  move(5, 10);
  printw("Press arrow keys, 'q' to quit.");
  refresh();

  c = getch();

  while(!gameOver) {
      if (c != ERR) {
          erase();
          move(5,10);
          printw("Press arrow keys or spacebar, 'q' to quit.");
          move(6, 10);
          // printw("left key:%d\n", has_key(260));
          // move(7, 10);
          // printw("RIGHT key:%d\n", has_key(261));
          // move(8, 10);
          // printw("SPACE key:%d\n", has_key(32));
          // move(9, 10);
          if (c == KEY_LEFT) {
              printw("left key pressed");
              rotationDirection = 1;
              rotateLander();
              thrusterOn = 0;
          } else if (c == KEY_RIGHT) {
                printw("right key pressed");
                rotationDirection = -1;
                rotateLander();
                thrusterOn = 0;
          } else if (c == ' ') {
                printw("spacebar pressed");
                // while (!has_key(' ')) {
                drawThruster();
                velocity = velocity + ((gravity+thrust) * t);
                thrusterOn = 1;
                // while (space == ' ') {
                //     drawThruster();
                //     velocity = velocity + ((gravity+thrust) * t);
                //     space = getch();
                // }
                // while (getch() == ' ') {
                //     printw("spacebar pressed:%d", c);
                //     thrusterOn = 1;
                //     drawThruster();
                // }
                // thrusterOn = 0;
                //eraseThruster();
          } else if (c == 'q') {
                gameOver = 1;
                break;
          }
          refresh();
      }
      // eraseThruster();
      c = getch();
  }

  // must do this or else Terminal will be unusable
    unsetCurses();

  // should never execute
  //exit(EXIT_SUCCESS);
}

// called when SIGALRM is sent.
// could be called for other signals, so signal parameter indicates
// which one.
void updateGraphics(int signal) {
  static int called = 0;

  called++;

  // called because of SIGALRM
  if (signal == SIGALRM)
  {

    // gets timer, puts it into timer
    struct itimerval timer;
    if (getitimer(ITIMER_REAL, &timer) == -1)
      exit(EXIT_FAILURE);

    //printf("called: %d\n", called);

    executeWithoutBlock();
    
    // stops timer on 5th call.  In asn3, you should do this when the
    // game is done (e.g., ship has crashed or landed)
    /*if (called == 15)
    {
      // in asn3, you should use tv_usec
      timer.it_interval.tv_sec = 0;
      timer.it_value.tv_sec = 0;
      exit(EXIT_SUCCESS);
    }*/

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
void executeWithoutBlock()
{
    // remember old list of signals to block (none, in our case)
    sigset_t old_mask;

    // set blocked signal set to block_mask_g (man 2 sigprocmask).
    // so now block SIGALRM.
    if (sigprocmask(SIG_BLOCK, &block_mask_g, &old_mask) < 0)
      exit(EXIT_FAILURE);

    // CRITICAL CODE GOES HERE ... can call other functions here, and
    // they will not be interrupted
    if (thrusterOn) {
    eraseThruster();
    }
    eraseLander();
    updateParameters();
    drawLander();
    // unblock signal by setting mask to old value (which in our
    // case wasn't blocking anything)
    if (sigprocmask(SIG_SETMASK, &old_mask, NULL) < 0)
      exit(EXIT_FAILURE);
}
