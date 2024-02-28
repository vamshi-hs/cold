#include <ncurses.h>

int main() {
    initscr(); // Initialize the screen
    cbreak(); // Line buffering disabled, pass on everything to me
    noecho(); // Don't echo() while we do getch
    keypad(stdscr, TRUE); // Enable the use of function keys

    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    /* WINDOW *win = newwin(rows - 1, cols, 0, 0); // Create a new window */
    /* scrollok(win, TRUE); // Allow scrolling in window */
    /* wrefresh(win); */
    // Print some lines to the window
    // Loop to handle user input
    /* wmove(win,10,0); */
    /* displayTest(&win); */
  /* wrefresh(win); */
  /* for (int i = 0; i < 100; i++) { */
    /* wprintw(win, "Line %d\n", i); */
  /* } */

    char ch;
    while ((ch = getch()) != 'q') {
      printw("%c",ch);
      
  /* wrefresh(win); */
  /* for (int i = 0; i < 100; i++) { */
    /* wprintw(win, "Line %d\n", i); */
  /* } */

      /* displayTest(&win); */
      int key = ch;
      switch(key) {
      case '\n':
	printw("pressed enter");
      case 'e':
	printw("pressed e");

      /* case 'd': */
	/* wscrl(win, -1); */
	/* break; */
      /* case 'f': */
	/* wscrl(win, 1); */
	/* break; */
      /* case KEY_UP: */
	// Scroll up by one line
	/* if (!wscrl(win, -1)) break; */
	/* break; */
      /* case KEY_DOWN: */
	// Scroll down by one line
	/* wscrl(win, 1); */
	/* break; */
      }
      // Refresh the window to show changes after scrolling
    }
    /* delwin(win); */
    endwin(); // End curses mode
    return 0;
}


void displayTest(WINDOW **win){
}
