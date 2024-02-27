#ifndef EROW
#define EROW

#include "../lib.h"

void editorOpen(struct TextEditor *te,char *filename);
void editorAppendRow(struct TextEditor *te,char *s,size_t len);
void editorUpdateRow(erow *row);
int editorRowCxtoRx(erow *row, int cx);
#endif //EROW
