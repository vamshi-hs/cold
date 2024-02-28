#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
// rgv ddfdfkdlk
#include "./keypress/keypress.h"
#include "./erow/erow.h"
#include "./display/display.h"
#include "./lib.h"

void te_initEditor(struct TextEditor *te){
  te->cx = 0;
  te->cy = 0;
  te->row = NULL;
  
  int width,height;
  getmaxyx(stdscr,height,width);
  
  te->height = height-2;
  te->width = width;

  te->rowoff = 0;
  te->coloff = 0;
  te->rx = 0;
  te->dirty = 0;
  te->filename = NULL;
  te->statusmsg[0] = '\0';
  te->statusmsg_time = 0;
  te->numrows = 0;
}

int main(int argc, char *argv[]){
  struct TextEditor te;

  initscr();
  raw();
  noecho();
  cbreak();
  keypad(stdscr, TRUE);

  te_initEditor(&te);
  if (argc >= 2){
    editorOpen(&te,argv[1]);
  }
  
  editorSetStatusMessage(&te,"HELP: Ctrl-S = save | Ctrl-Q = quit");
  
  while(1){
    te_Scroll(&te);
    te_RefreshScreen(te);
    if(te_ProcessKeypress(&te)) break;
  }
  endwin();
  return 0;
}
