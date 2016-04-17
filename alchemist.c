#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>


#include "alchemist.h"

#define MAX_PATH_LEN 1024
#define MAX_GOLD     10000000   // you can even set more then 1 000 000 000

int main(int argc, char *argv[]) {
  // file
  FILE *fp;
  char *filename = "BALDUR.gam";
  
  // options
  int opt;
  extern int opterr;
  opterr= 0; // make getopt silent

  // flags
  int pflag = 0, aflag = 0, dflag = 0;

  // initial values
  char     *path   = "";  
  unsigned long int  amount = 0;

  // full file path
  char path_buffer[MAX_PATH_LEN] = "";

  // gold amount stored inside save file
  unsigned int current_gold;
  
  // temporary variables;
  char *str_end;
  
  // parse options
  while((opt = getopt(argc, argv, "p:a:d")) != -1) {
    switch(opt) {
    case 'p':
      path = optarg;
      
      // check path
      snprintf(path_buffer, sizeof(path_buffer), "%s%s", path, filename);
      fp=fopen(path_buffer,"r");
      if(fp == NULL){
       	fprintf (stderr, "Cannot open file: %s\n", path_buffer); 
       	exit(EXIT_FAILURE); 
      }
      fclose(fp);      
      pflag=1; // path is set
      break;
    case 'a':
      amount = strtol(optarg, &str_end, 0);

      // check for additional characters
      if(*str_end != '\0') {
	fprintf(stderr,"Amount contains additional characters: %s\n", str_end);
	exit(EXIT_FAILURE);
      }

      // check for max value
      if(amount > MAX_GOLD){
	fprintf(stderr, "Too much gold.\n");
	exit(EXIT_FAILURE);	
      }
      aflag=1; // amount is set
      break;
    case 'd':
      dflag = 1; // debug is set
      break;
    case '?':
      if(optopt == 'p')
	fprintf (stderr, "Option -%c requires an path argument.\n", optopt);
      else if(optopt == 'a')
	fprintf (stderr, "Option -%c requires an amount argument.\n", optopt);
      else if (isprint (optopt))
	fprintf (stderr, "Unknown option `-%c'.\n", optopt);
      else
	fprintf (stderr, "Unknown option `\\x%x'.\n", optopt);
      exit(EXIT_FAILURE);
      break;
    }
  }

  // require correct path and amount
  if(aflag == 0 || pflag == 0) {
      fprintf (stderr, "Parameters: -p directory-path -a gold-amount [-d]\n");
      fprintf (stderr, "Example: -a 200000 -p \"/home/milosz/.local/share/Icewind Dale - Enhanced Edition/save/000000008-2/\"\n");
      exit(EXIT_FAILURE);
  }
  
  // print debug information
  if(dflag == 1) {
    printf("Defined file path   = %s\n", path);
    printf("Defined gold amount = %ld\n", amount);
  }

  // open save file
  fp=fopen(path_buffer,"r+");
  if(fp != NULL){
    if(verify_file_type(fp) == 0) {
      // read gold amount 
      fseek(fp, 24, SEEK_SET);
      fread(&current_gold,1,4,fp);

      // print debug information
      if(dflag == 1)
	printf("Old gold amount     = %i\n", current_gold);

      // write new walue
      fseek(fp, 24, SEEK_SET);
      fwrite(&amount, 1, sizeof(unsigned int), fp);
    }
    fclose(fp);  
  }
  exit(EXIT_SUCCESS);  
}
