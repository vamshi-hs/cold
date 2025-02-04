#include "../lib.h"

void editorInsertChar(struct TextEditor *te, int c);
char *editorRowsToString(struct TextEditor,int *buflen);
void editorInsertNewline(struct TextEditor *te);
void editorSave(struct TextEditor *te);
void editorDelChar(struct TextEditor *te);
void editorDelRow(struct TextEditor *te,int at);
void editorInsertRow(struct TextEditor *te,int at,char *s,size_t len);
