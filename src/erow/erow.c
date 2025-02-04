#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include "../editor/editor.h"
#include <stdio.h>
#include <curses.h>
#include "../lib.h"

void erow_insertChar(erow *row,int at,int c){
  if (at < 0 || at > row->size) at = row->size;
  row->chars = realloc(row->chars,row->size+2);
  memmove(&row->chars[at+1],&row->chars[at],(row->size-at)+1);
  row->size++;
  row->chars[at]=c;
  editorUpdateRow(row);
}

void erow_freeRow(erow *row){
  free(row->chars);
  free(row->render);
}

void erow_appendString(struct TextEditor *te,erow *row,char *s,size_t len){
  row->chars = realloc(row->chars,row->size+len+1);
  memcpy(&row->chars[row->size],s,len);
  row->size += len;
  row->chars[row->size] = '\0';
  editorUpdateRow(row);
  te->dirty++;
}

void erow_delChar(struct TextEditor *te,erow *row,int at){
  if (at < 0 || at >= row->size) return;
  
  memmove(&row->chars[at],&row->chars[at+1], row->size-at);
  row->size--;
  editorUpdateRow(row);
  te->dirty++;
}

void editorAppendRow(struct TextEditor *te,char *s,size_t len){
  te->row = realloc(te->row,sizeof(erow) * (te->numrows+1));
  int at = te->numrows;
  te->row[at].size = len;
  te->row[at].chars = malloc(len+1);
  memcpy(te->row[at].chars,s,len);
  te->row[at].chars[len] = '\0';

  te->row[at].rsize = 0;
  te->row[at].render = NULL;

  editorUpdateRow(&te->row[at]);
  te->numrows++;
  te->dirty++;
}

int editorRowCxtoRx(erow *row, int cx){
  int rx = 0;
  for (int j = 0; j < cx; j++){
    if (row->chars[j] == '\t')
      rx += (COLD_TAB_STOP - 1) - (rx % COLD_TAB_STOP);
    rx++;
  }
  return rx; 
}

int editorRowRxtoCx(erow *row, int rx){
  int cur_rx = 0;
  int cx = 0;
  for (cx = 0; cx < row->size; cx++){
    if (row->chars[cx] == '\t')
      cur_rx += (COLD_TAB_STOP - 1) - (rx % COLD_TAB_STOP);
    cur_rx++;

    if (cur_rx > rx) return cx;
  }
  return cx; 
}

void editorUpdateRow(erow *row){
  int tabs = 0;
  for (int j = 0; j < row->size;j++)
    if (row->chars[j] == '\t') tabs++;
  
  free(row->render);
  row->render = malloc(row->size + tabs * (COLD_TAB_STOP -1)  + 1);

  int idx = 0;
  for (int j = 0; j < row->size;j++){
    if (row->chars[j] == '\t') {
      row->render[idx++] = ' ';
      while (idx % COLD_TAB_STOP != 0) row->render[idx++] = ' ';
    } else {
      row->render[idx++] = row->chars[j];
    }
  }
  row->render[idx] = '\0';
  row->rsize = idx;
}

void editorOpen(struct TextEditor *te,char *filename){
  free(te->filename);
  te->filename = strdup(filename);
  
  FILE *fp = fopen(filename,"r");
  if (!fp) {
    refresh();
    endwin();
    exit(0);
  }

  char *line = NULL;
  size_t linecap = 0;
  ssize_t linelen;
  while((linelen = getline(&line,&linecap,fp)) != -1) {
    while (linelen > 0 && (line[linelen - 1] == '\n' ||
			   line[linelen - 1] == '\r'))
      linelen--;
    editorInsertRow(te,te->numrows,line,linelen);
  }
  free(line);
  fclose(fp);
  te->dirty = 0;
}
