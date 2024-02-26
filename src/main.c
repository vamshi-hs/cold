#include <curses.h>
#include <stdio.h>
#include <stdlib.h>

struct TextEditor{
  int cx,cy;
  int height;
  int width;
};

#define COLD_VERSION "0.0.1"
#define CTRL_KEY(x)  ((x) & 0x1f)

void te_DrawRows(struct TextEditor te) {
  move(0,0);
  int y = te.height;
  for (int j = 0; j < y; j++){
    move(j,0);
    if (j == y/3){
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
  }
}

void te_RefreshScreen(const struct TextEditor te){
  refresh();
  te_DrawRows(te);
  move(te.cy,te.cx);
}

int te_ProcessKeypress(struct TextEditor *te){
      int c = getch();
      switch(c){
      case CTRL_KEY('q'):
	return 1;
	break;
      case 'w':
      case 's':
      case 'a':
      case 'd':
      case KEY_UP:
      case KEY_RIGHT:
      case KEY_DOWN:
      case KEY_LEFT:
	te_move_cursor(te,c);
      default:
	return 0;
	break;
      }
}

void te_move_cursor(struct TextEditor *te,int key){
  int ch = key;
  switch(ch) {
  case KEY_LEFT:
  case 'a':
    if (te->cx > 0) te->cx--;
    break;
  case KEY_UP:
  case 'w':
    if (te->cy > 0) te->cy--;
    break;
  case KEY_DOWN:
  case 's':
    if (te->cy < te->height-1) te->cy++;
    break;
  case KEY_RIGHT:
  case 'd':
    if (te->cx < te->width-1) te->cx++;
    break;
  }
}

void te_initEditor(struct TextEditor *te){
  te->cx = 0;
  te->cy = 0;
  int width,height;
  getmaxyx(stdscr,height,width);
  te->height = height;
  te->width = width;
}

int main(){
  struct TextEditor te;

  initscr();
  raw();
  noecho();
  cbreak();
  keypad(stdscr, TRUE);
  te_initEditor(&te);
  while(1){
    te_RefreshScreen(te);
    if(te_ProcessKeypress(&te)) break;
  }
  endwin();
  return 0;
}
