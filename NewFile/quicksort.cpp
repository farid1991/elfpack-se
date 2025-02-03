#include "quicksort.h"

void qs(wchar_t *items, int left, int right);

void QSort(wchar_t items[], int count)
{
  qs(items, 0, count-1);
}

void qs(wchar_t items[], int left, int right)
{
  register int i, j;
  wchar_t x, y;

  i = left; j = right;
  x = items[(left+right)/2];

  do {
    while((items[i] < x) && (i < right)) i++;
    while((x < items[j]) && (j > left)) j--;

    if(i <= j) {
      y = items[i];
      items[i] = items[j];
      items[j] = y;
      i++; j--;
    }
  } while(i <= j);

  if(left < j) qs(items, left, j);
  if(i < right) qs(items, i, right);
}
