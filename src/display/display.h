
void te_DrawRows(const struct TextEditor te);
void te_Scroll(struct TextEditor *te);
void te_RefreshScreen(const struct TextEditor te);
void displayDrawStatusBar(struct TextEditor te);
void editorSetStatusMessage(struct TextEditor *te,const char *fmt, ...);
