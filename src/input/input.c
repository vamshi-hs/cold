#include <curses.h>
#include "../lib.h"
#include "../display/display.h"
#include <ctype.h>
#include <stdlib.h>

char *editorPrompt(struct TextEditor *te,char *prompt){
  size_t bufsize = 128;
  char *buf = malloc(bufsize);

  size_t buflen = 0;
  buf[0] = '\0';
  int prev_cx = te->cx;
  int prev_cy = te->cy;
  while(1) {
    te->cx = strlen(prompt) + buflen+1;
    te->cy = te->height;
    te_Scroll(te);
    editorSetStatusMessage(te,prompt,buf);
    te_RefreshScreen(*te);

    int c = getch();
    if (c == KEY_DC || c == CTRL_KEY('h') || c == KEY_BACKSPACE){
      if (buflen != 0) buf[--buflen] = '\0';
    } else  if (c == '\x1b') {
      te->cx = prev_cx;
      te->cy = prev_cy;
      editorSetStatusMessage(te,"");
      free(buf);
      return NULL;
    } else if (c == '\n'){
      if (buflen != 0) {
	editorSetStatusMessage(te,"");
	return buf;
      }
    } else if (!iscntrl(c) && c < 128) {
      if (buflen == bufsize - 1){
	bufsize *= 2;
	buf = realloc(buf,bufsize);
      }
      buf[buflen++] = c;
      buf[buflen] = '\0';
    }
  }
  te->cx = prev_cx;
  te->cy = prev_cy;
}
