#include "erow/erow.h"
#ifndef TE
#define TE

/* typdef struct; erow; */

typedef struct erow {
  int size;
  char *chars;
}erow;


struct TextEditor{
  int cx,cy;
  int height;
  int width;
  erow *row;
  
  int numrows;
};

#define CTRL_KEY(x)  ((x) & 0x1f)

#endif //TE
