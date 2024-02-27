#include "erow/erow.h"
#include <time.h>
#ifndef TE
#define TE

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
  int dirty;
  int numrows;
  char *filename;
  char statusmsg[80];
  time_t statusmsg_time;
};

#define CTRL_KEY(x)  ((x) & 0x1f)
#define COLD_VERSION "0.0.1"
#define COLD_TAB_STOP 8
#endif //TE
