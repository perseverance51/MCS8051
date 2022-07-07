#include <stdio.h>
int main()
{
  FILE *fp;
  fp = fopen("24c04.bin","wb");
  fwrite("654321",1,6,fp);
  fclose(fp);
  return 0;
}