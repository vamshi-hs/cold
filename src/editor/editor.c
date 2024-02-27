#include "../lib.h"
#include "../erow/erow.h"
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

void editorInsertChar(struct TextEditor *te, int c){
  if (te->cy == te->numrows)
    editorAppendRow(te,"",0);
  erow_insertChar(&te->row[te->cy],te->cx,c);
  te->cx++;
  te->dirty++;
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
  if (te->filename == NULL) return;

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
