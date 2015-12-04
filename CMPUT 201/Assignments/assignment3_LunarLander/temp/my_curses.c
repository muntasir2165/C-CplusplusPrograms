#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// brief example of using curses.
// man 3 ncurses for introductory man page, and man 3 function name
// for more information on that function.

/*
Please compile and run this program from the command line as follows:
  gcc my_curses.c -lcurses -lm; ./a.out
*/

void setup_curses();
void unset_curses();

int main()
{
  //declare FILE pointer for the executable file
    FILE* executable = NULL;

    //flags for the landscape and executable pointers
    int executableOpen = 0;

  //create a new graphical window and draws images (or more precisely, line
  //segments) based on the input file
  const char exec_name[] = "java -jar Sketchpad.jar";
  
  //open the sketchpad executable to be run by the child process
  executable = popen(exec_name, "w");
  if (executable == NULL) {
      fprintf(stderr, "Cannot open the Sketchpad executable\n");
  } else {
      executableOpen = 1;
  }
  
  setup_curses();

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

  while(1)
  {
    if (c != ERR)
    {
      // in asn3, won't need to do any printing to screen.
      // instead, will rotate figure on left or right arrow keys, and
      // initiate thrust when space bar is pressed.
      erase();
      move(5,10);
      printw("Press arrow keys, 'q' to quit.");
      move(6, 10);
      if (c == KEY_DOWN) {
        printw("down key pressed\n");
        fprintf(executable, "drawSegment %ld %ld %ld %ld\n", lround(310), 
            lround(5), lround(330), lround(5));
        move(7, 10);
        printw("Done!");
      } else if (c == KEY_LEFT) {
          printw("left key pressed\n");
          fprintf(executable, "drawSegment %ld %ld %ld %ld\n", lround(300), 
            lround(40), lround(340), lround(40));
          move(7, 10);
          printw("Done!");
      } else if (c == KEY_RIGHT) {
          printw("right key pressed\n");
          fprintf(executable, "drawSegment %ld %ld %ld %ld\n", lround(310), 
            lround(5), lround(300), lround(40));
          move(7, 10);
          printw("Done!");
      } else if (c == KEY_UP) {
          printw("up key pressed\n");
          fprintf(executable, "drawSegment %ld %ld %ld %ld\n", lround(330), 
            lround(5), lround(340), lround(40));
          move(7, 10);
          printw("Done!");
      } else if (c == ' ') {
          printw("space key pressed\n");
          fprintf(executable, "drawSegment %ld %ld %ld %ld\n", lround(300), 
            lround(40), lround(320), lround(48)); 
          fprintf(executable, "drawSegment %ld %ld %ld %ld\n", lround(340), 
            lround(40), lround(320), lround(48));
          move(7, 10);
          printw("Done!");
      } else if (c == 'q') {
          //fprintf(executable, "%s\n", " pause 3");
          break;
        }
      refresh();

    }

    c = getch();

  }

  // must do this or else Terminal will be unusable
  // (if there are problems, it's not that big a deal though ... just
  // close the Terminal, and open a new one.)
  unset_curses();

  fprintf(executable, "%s\n", " pause 3");

  //close the pipe to Sketchpad and close the executable
  if (executableOpen) {
      //char* end = " end";
      fprintf(executable, "%s\n", " end");
      pclose(executable);
    }

  exit(EXIT_SUCCESS);
}

void setup_curses()
{
  // use return values.  see man pages.  likely just useful for error
  // checking (NULL or non-NULL, at least for init_scr)
  initscr();
  cbreak();
  noecho();
  // needed for cursor keys (even though says keypad)
  keypad(stdscr, true);
}

void unset_curses()
{
  keypad(stdscr, false);
  nodelay(stdscr, false);
  nocbreak();
  echo();
  endwin();
}
