#include "../input/input.h"
#include <string.h>
#include <stdlib.h>
#include "../lib.h"

void editorFindCallBack(struct TextEditor *te,char *query,int key){
  if (key == '\n' || key == '\x1b'){
    return;
  }

  for (int i = 0; i < te->numrows;i++){
    erow *row = &te->row[i];
    char *match = strstr(row->render, query);
    if (match){
      te->cy = i;
      te->cx = editorRowRxtoCx(row,match - row->render);
      /* te->cx = match - row->render; */
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
  char *query = editorPrompt(te,"Search: %s (ESC to Cancel)",editorFindCallBack);

  if (query){
    free(query);
  } else {
    te->cx = saved_cx;
    te->cy = saved_cy;
    te->coloff = saved_coloff;
    te->rowoff = saved_rowoff;
  }
}

