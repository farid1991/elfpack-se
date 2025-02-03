#ifndef GUI_H
#define GUI_H

struct MyGUI : GUI
{
  char unk[0x2c];
};

GUI *GUI_Create();
void GUI_Destroy();

extern unsigned char *video_buffer;
extern MyGUI *MyGui;

void video2screen();

#endif
