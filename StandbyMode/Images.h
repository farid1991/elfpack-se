#ifndef _IMAGES_H_
  #define _IMAGES_H_

#define IMAGE_COUNT 6

/*
extern const int *cfg_image_show[IMAGE_COUNT];
extern const int *cfg_image_not_show_if_player[IMAGE_COUNT];
extern int image_show[IMAGE_COUNT];
*/
void ShowImages(int y, int location);
void UpdateImagesShowParams(void);
void UpdateImagesParams(void);
void FreeImagesParams(void);

#endif //_IMAGES_H_
