#include "../lib.h"
#include "../erow/erow.h"
#include "../input/input.h"
#include "../display/display.h"
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

void editorInsertRow(struct TextEditor *te,int at,char *s,size_t len){
  if (at < 0 || at > te->numrows) return;

  te->row = realloc(te->row,sizeof(erow) * (te->numrows+1));
  memmove(&te->row[at+1],&te->row[at],sizeof(erow) * (te->numrows - at));
  te->row[at].chars = malloc(len+1);
  te->row[at].size = len;
  memcpy(te->row[at].chars,s,len);
  te->row[at].chars[len] = '\0';

  te->row[at].rsize = 0;
  te->row[at].render = NULL;
  editorUpdateRow(&te->row[at]);
  te->numrows++;
  te->dirty++;
}

void editorInsertChar(struct TextEditor *te, int c){
  if (te->cy == te->numrows)
    editorInsertRow(te,te->numrows,"",0);
  erow_insertChar(&te->row[te->cy],te->cx,c);
  te->cx++;
  te->dirty++;
}

void editorDelRow(struct TextEditor *te,int at){
  if (at < 0 || at > te->numrows) return;
  erow_freeRow(&te->row[at]);
  memmove(&te->row[at],&te->row[at+1],sizeof(erow) * (te->numrows+1));
  te->numrows--;
  te->dirty++;
}

void editorDelChar(struct TextEditor *te){
  if (te->cy == te->numrows) return;
  if (te->cy == 0 && te->cx == 0) return;

  erow *row = &te->row[te->cy];
  if (te->cx > 0){
    erow_delChar(te,row,te->cx-1);
    te->cx--;
  } else {
    te->cx = te->row[te->cy-1].size;
    erow_appendString(te,&te->row[te->cy-1],row->chars,row->size);
    editorDelRow(te,te->cy);
    te->cy--;
  }
}

void editorInsertNewline(struct TextEditor *te){
  if (te->cx == 0){
    editorInsertRow(te,te->cy,"",0);
  } else {
    erow *row = &te->row[te->cy];
    editorInsertRow(te,te->cy+1,&row->chars[te->cx],row->size - te->cx);
    row = &te->row[te->cy];
    row->size = te->cx;
    row->chars[row->size] ='\0';
    editorUpdateRow(row);
  }
  te->cy++;
  te->cx = 0;
}

char *editorRowsToString(struct TextEditor te,int *buflen){
  int totlen = 0;
  for (int j = 0; j < te.numrows; j++)
    totlen += te.row[j].size + 1;
  *buflen = totlen;

  char *buf = malloc(totlen);
  char *p = buf;
  for (int j = 0; j < te.numrows; j++){
    memcpy(p,te.row[j].chars,te.row[j].size);
    p += te.row[j].size;
    *p = '\n';
    p++;
  }

  return buf;
}

void editorSave(struct TextEditor *te){
  if (te->filename == NULL) {
    te->filename = editorPrompt(te,"Save as %s (ESC to cancel)",NULL);
    if (te->filename == NULL){
      editorSetStatusMessage(te,"Save aborted");
      return;
    }
  }

  int len;
  char *buf = editorRowsToString(*te,&len);

  int fd = open(te->filename, O_RDWR | O_CREAT,0644);
  if (fd != -1){
    if (ftruncate(fd,len) != -1) {
      if (write(fd,buf,len) == len) {
	close(fd);
	free(buf);
	te->dirty = 0;
	editorSetStatusMessage(te,"%d bytes written to disk", len);
	return;
      }
    }
    close(fd);
  }

  free(buf);
  editorSetStatusMessage(te,"Can't save! I/O erroe: %s", strerror(errno));
  return;
}
