#include <curses.h>
#include "../lib.h"
#include "../erow/erow.h"

void te_move_cursor(struct TextEditor *te,const int key){
  erow *row = (te->cy >= te->numrows) ? NULL : &te->row[te->cy];
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
    if (te->cy < te->numrows) te->cy++;
    break;
  case KEY_RIGHT:
  case 'd':
    if (row && te->cx < row->size) {
      te->cx++;
    }
    else if (row && te->cx == row->size){
      te->cy++;
      te->cx = 0;
    }
    break;
  }

  row = (te->cy >= te->numrows) ? NULL : &te->row[te->cy];
  int rowlen = row ? row->size : 0;
  if (te->cx > rowlen) te->cx = rowlen;
}

int te_ProcessKeypress(struct TextEditor *te){
      int c = getch();
      switch(c){
      case CTRL_KEY('q'):
	return 1;
	break;
      case KEY_NPAGE:
      case KEY_PPAGE:
	{
	  if (c == KEY_PPAGE){
	    te->cy = te->rowoff;
	  } else if (c == KEY_NPAGE){
	    te->cy = te->rowoff + te->height - 1;
	    if (te->cy > te->numrows) te->cy = te->numrows;
	  }
	  int times = te->height;
	  while(times--)
	    te_move_cursor(te, c== KEY_PPAGE ? KEY_UP: KEY_DOWN);
	  /* if (c = KEY_PPAGE){ */
	    /* te->cy = te->rowoff; */
	  /* } */
	  /* int times = te->height; */
	  /* while(times--) */
	    /* te_move_cursor(te,c == KEY_PPAGE ? KEY_UP : KEY_DOWN); */
	}
	break;
      case KEY_HOME:
	te->cx = 0;
	break;
      case KEY_END:
	if (te->cy < te->numrows)
	  te->cx = te->row[te->cy].size;
	/* te->cx = te->width - 1; */
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
      return 0;
}
