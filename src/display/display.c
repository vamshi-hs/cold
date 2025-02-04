#include <curses.h>
#include <string.h>
#include "../lib.h"

void displayDrawStatusBar(const struct TextEditor te){
  char status[80],rstatus[80];
  int len = snprintf(status, sizeof(status), "%.20s - %d lines %s",
		     te.filename ? te.filename : "[No Name]",te.numrows
		     ,te.dirty ? "(modified)" : "");
  int rlen = snprintf(rstatus, sizeof(rstatus), "%d/%d",
		      te.cy+1,te.numrows);
  move(te.height,len);
  if (len > te.width) len = te.width;
  start_color();
  init_pair(1,COLOR_BLACK,COLOR_WHITE);
  attron(COLOR_PAIR(1));
  // printw("%s","emacs");
  while(len < te.width){
    if (te.width - len == rlen) {
      printw("%.*s",rlen,rstatus);
      break;
    } else {
      printw(" ");
      len++;
    }     
  }
   
  move(te.height,0);
  printw("%.*s",len,status);
  attroff(COLOR_PAIR(1));
}

void editorDrawMessage(const struct TextEditor te){
  int msglen = strlen(te.statusmsg);
  if (msglen > te.width) msglen = te.width;
  if (msglen && time(NULL) - te.statusmsg_time < 5)
    printw("%.*s",msglen,te.statusmsg);
}

void editorSetStatusMessage(struct TextEditor *te,const char *fmt, ...){
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(te->statusmsg,sizeof(te->statusmsg),fmt,ap);
  va_end(ap);
  te->statusmsg_time = time(NULL);
}

void te_DrawRows(const struct TextEditor te){
  int y = te.height;
  for (int j = 0; j < y; j++){
    int filerow = j + te.rowoff;
    move(j,0);
    if (filerow >= te.numrows) {
    if (0 ==  te.numrows && (j == y/3)) {
  	char welcome[80];
	int welcomelen = snprintf(welcome,sizeof(welcome),"Cold editor -- version %s",COLD_VERSION);
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
      int len = te.row[filerow].rsize - te.coloff;
      if (len < 0) len = 0;
      if (len > te.width) len = te.width;
      printw("%.*s",len,&te.row[filerow].render[te.coloff]);
    }
  }
  move(te.height,0);
  displayDrawStatusBar(te);
  move(te.height+1,0);
  editorDrawMessage(te);
}

void te_Scroll(struct TextEditor *te){
  te->rx = te->cx;
  if (te->cy < te->numrows)
    te->rx = editorRowCxtoRx(&te->row[te->cy],te->cx);
  
  if (te->cy < te->rowoff){
    te->rowoff = te->cy;
  }

  if (te->cy >= te->rowoff + te->height){
    te->rowoff = te->cy - te->height + 1;
  }

  if (te->rx < te->coloff){
    te->coloff = te->rx;
  }

  if (te->rx >= te->coloff + te->width){
    te->coloff = te->rx - te->width + 1;
  }
}

void te_RefreshScreen(const struct TextEditor te){
  refresh();
  clear();
  te_DrawRows(te);
  move(te.cy-te.rowoff,te.rx-te.coloff);
}
