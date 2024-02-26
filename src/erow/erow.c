#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <curses.h>

#include "../lib.h"

void editorAppendRow(struct TextEditor *te,char *s,size_t len){
  te->row = realloc(te->row,sizeof(erow) * (te->numrows+1));
  int at = te->numrows;
  te->row[at].size = len;
  te->row[at].chars = malloc(len+1);
  memcpy(te->row[at].chars,s,len);
  te->row[at].chars[len] = '\0';
  te->numrows++;
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
