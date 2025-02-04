#ifndef EROW
#define EROW

#include "../lib.h"

void editorOpen(struct TextEditor *te,char *filename);
void editorAppendRow(struct TextEditor *te,char *s,size_t len);
void editorUpdateRow(erow *row);
int editorRowCxtoRx(erow *row, int cx);
void erow_insertChar(erow *row,int at,int c);
int editorRowRxtoCx(erow *row, int rx);
void erow_freeRow(erow *row);
void erow_appendString(struct TextEditor *te,erow *row,char *s,size_t len);
void erow_delChar(struct TextEditor *te,erow *row,int at);
#endif //EROW
