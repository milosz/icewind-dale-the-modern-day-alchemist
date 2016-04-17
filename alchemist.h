#include <string.h>

#define FILE_TYPE "GAMEV2.0"

int verify_file_type(FILE *fp);

// verify Icewind Dale: EE save game
int verify_file_type(FILE *fp) {
  char buffer[8];
  
  // store current file position
  long int position = ftell(fp);

  // rewind file to the beginning
  rewind(fp);

  // read 8 characters
  fread(&buffer,1,8,fp);

  // restore file position  
  fseek(fp, position, SEEK_SET);

  if(strcmp(buffer, FILE_TYPE) == 0)
    return 0;
  else
    return 1;
}
