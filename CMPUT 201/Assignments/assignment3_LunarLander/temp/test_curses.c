#include <curses.h>
#include <stdio.h>
#include <stdlib.h>

// brief example of using curses.
// man 3 ncurses for introductory man page, and man 3 function name
// for more information on that function.

void setup_curses();
void unset_curses();

int main()
{

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
      if (c == KEY_DOWN)
        printw("down key pressed");
      else if (c == KEY_LEFT)
        printw("left key pressed");
      else if (c == KEY_RIGHT) 
        printw("right key pressed");
      else if (c == KEY_UP)
        printw("up key pressed");
      else if (c == 'q')
        break;
      refresh();

    }

    c = getch();

  }

  // must do this or else Terminal will be unusable
  // (if there are problems, it's not that big a deal though ... just
  // close the Terminal, and open a new one.)
  unset_curses();

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
