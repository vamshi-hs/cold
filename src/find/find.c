#include "../input/input.h"
#include <string.h>
#include <curses.h>
#include <stdlib.h>
#include "../lib.h"

void editorFindCallBack(struct TextEditor *te,char *query,int key){
  static int last_match = -1;
  static int direction = 1;
  
  if (key == '\n' || key == '\x1b'){
    last_match = -1;
    direction = 1;
    return;
  } else if (key == KEY_RIGHT || key == KEY_DOWN){
    direction = 1;
  } else if (key == KEY_LEFT || key == KEY_UP){
    direction = -1;
  } else {
    last_match = -1;
    direction = 1;
  }

  if (last_match == -1) direction = 1;
  int current = last_match;
  
  for (int i = 0; i < te->numrows;i++){
    current += direction;
    if (current == -1) current = te->numrows - 1;
    else if (current == te->numrows) current = 0;
    
    erow *row = &te->row[current];
    char *match = strstr(row->render, query);
    if (match){
      last_match = current;
      te->cy = current;
      te->cx = editorRowRxtoCx(row,match - row->render);
      te->rowoff = te->numrows;
      break;
    }
    }
}

void editorFind(struct TextEditor *te){
  int saved_cx = te->cx;
  int saved_cy = te->cy;
  int saved_coloff = te->coloff;
  int saved_rowoff = te->rowoff;
  char *query = editorPrompt((struct TextEditor *)te,"Search: %s (ESC/Arrows/Enter)",editorFindCallBack);

  if (query){
    free(query);
  } else {
    te->cx = saved_cx;
    te->cy = saved_cy;
    te->coloff = saved_coloff;
    te->rowoff = saved_rowoff;
  }
}
