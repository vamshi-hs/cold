#include "erow/erow.h"
#ifndef TE
#define TE

/* typdef struct; erow; */

typedef struct erow {
  int size;
  int rsize;
  char *chars;
  char *render;
} erow;


struct TextEditor{
  int cx,cy;
  int rx;
  int rowoff;
  int coloff;
  int height;
  int width;
  erow *row;
  int numrows;
};

#define CTRL_KEY(x)  ((x) & 0x1f)

#endif //TE
