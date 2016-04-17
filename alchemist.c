#include <stdio.h>     // io functions
#include <stdlib.h>    // EXIT codes
#include <ctype.h>     // getopt, isprint functions
#include <unistd.h>    // getopt options

#define MAX_PATH_LEN 1024
#define MAX_GOLD     10000000

int main(int argc, char *argv[]) {
  // file
  FILE *fp;
  char *filename = "BALDUR.gam";
  
  // options
  int opt;
  extern int opterr;
  opterr= 0; // make getopt silent

  // flags
  int pflag, aflag, dflag;

  // initial values
  char     *path   = "";  
  long int  amount = 0;

  // full file path
  char path_buffer[MAX_PATH_LEN] = "";

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
    default:
      fprintf (stderr, "aaa\n");
      exit(EXIT_FAILURE);
      break;
    }
  }
  
  // print debug information
  if(dflag == 1) {
    printf("Defined file path   = %s\n", path);
    printf("Defined gold amount = %ld\n", amount);
  }

  exit(EXIT_SUCCESS);  
}
