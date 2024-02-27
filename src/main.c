#include <curses.h>
#include <stdio.h>
#include <stdlib.h>

#include "./keypress/keypress.h"
#include "./erow/erow.h"
#include "./lib.h"

#define COLD_VERSION "0.0.1"

void te_DrawRows(struct TextEditor te) {
  /* move(0,0); */
  int y = te.height;
  for (int j = 0; j < y; j++){
    int filerow = j + te.rowoff;
    move(j,0);
    if (filerow >= te.numrows) {
    if (0 ==  te.numrows && (j == y/3)) {
  	char welcome[80];
	int welcomelen = snprintf(welcome,sizeof(welcome),
				  "Cold editor -- version %s",
				  COLD_VERSION);
	if (welcomelen > te.width) welcomelen = te.width;
	int padding = (te.width - welcomelen)/2;
	if (padding) {
	  printw("%c",'~');
	}
	while (padding--) printw(" ");
	welcome[welcomelen] = '\0';
	printw("%s",welcome);
      } else {
	printw("%c",'~'); 
      }
    } else {
      int len = te.row[filerow].size;
      if (len > te.width) len = te.width;
      /* te.row[j].chars[len] = '\0'; */
      printw("%s",te.row[filerow].chars);
    }
  }
}

void te_Scroll(struct TextEditor *te){
  if (te->cy < te->rowoff){
    te->rowoff = te->cy;
    /* te->rowoff = 0; */
  }

  if (te->cy >= te->rowoff + te->height){
    te->rowoff = te->cy - te->height + 1;
  }
}

void te_RefreshScreen(const struct TextEditor te){
  refresh();
  clear();
  te_DrawRows(te);
  move(te.cy-te.rowoff,te.cx);
}

void te_initEditor(struct TextEditor *te){
  te->cx = 0;
  te->cy = 0;
  te->row = NULL;
  
  int width,height;
  getmaxyx(stdscr,height,width);
  
  te->height = height;
  te->width = width;
  te->numrows = 0;
  te->rowoff = 0;
}

int main1(int argc, char *argv[]){
  initscr();
  raw();
  noecho();
  cbreak();
  keypad(stdscr, TRUE);

  clear();
  endwin();
  return 0;
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
  
  while(1){
    te_Scroll(&te);
    te_RefreshScreen(te);
    if(te_ProcessKeypress(&te)) break;
  }
  endwin();
  return 0;
}
