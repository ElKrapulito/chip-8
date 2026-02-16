#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

uint16_t fixopposition(uint16_t val) {
  return ((val << 8) & 0xFF00 | (val >> 8) & 0xFF);
}

int main() {
  struct stat filestat;
  stat("./test/1-chip8-logo.ch8", &filestat);
  size_t filesize = filestat.st_size;

  FILE *fptr;
  uint16_t data[filesize];

  fptr = fopen("./test/1-chip8-logo.ch8", "rb");

  if (fptr == NULL) {
    printf("file failed to open\n");
    return 1;
  }
  size_t binary = fread(data, sizeof(data), filesize, fptr);

  printf("file size is: %u bytes\n", (unsigned int)filestat.st_size);

  for (int n = 0; n != filesize; ++n) {
    int pos = (n + 1) % 8;
    uint16_t corrected = fixopposition(data[n]);
    if (pos == 0) {
      printf("%04x\n", corrected);
    } else {
      printf("%04x ", corrected);
    }
  }
  printf("\n");
  uint16_t firstcode = data[0];
  uint16_t correct =
      ((firstcode >> 24) & 0xFF | (firstcode << 8) & 0xFF0000 |
       (firstcode >> 8) & 0xFF00 | (firstcode << 24) & 0xFF000000);
  int opcodefirst = correct & 0xFFFF0000;
  printf("%04x\n", firstcode);
  printf("%04x\n", correct);
  printf("%04x\n", opcodefirst);

  fclose(fptr);
  return 0;
}
