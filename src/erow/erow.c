#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <curses.h>

#define COLD_VERSION "0.0.1"
#define COLD_TAB_STOP 8

#include "../lib.h"

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
    editorAppendRow(te,line,linelen);
  }
  free(line);
  fclose(fp);
}
