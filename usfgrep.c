/* To compile: gcc -o usfgrep usfgrep.c
 * To execute: ./usfgrep USF *.txt */

/* The purpose of this program is to implement the grep UNIX command 
 * with system functions and to include the line number next
 * to the file name. */
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"

int lines[10000];
int global_count;

/* The purpose of this function is:
 * to get the length of the line*/
int get_length(int fd){
  int len = 0;
  char c;
  int n;
  while((n = read(fd, &c, 1)) > 0){ //0 or 1 charecter at a time
    if(c != '\n'){
      len++;
    }
  }/* at the end of file and there is no \n */
  if(n == 0 && len == 0){
    len = -1 ;
  }
  return len;
}

/* The purposes of this function are:
 *  To read the line 1 charecter at a time and store it
 *  To store the line number every time there is \n  */
void read_line(int fd, char *arr){
  char c;
  int i = 0; int count = 0; int n;
  global_count = 0;
  
  while((n = read(fd, &c, 1)) > 0){
    if(c != '\n'){  
      arr[i]=c;
      count++;
      i++;
    }
    if(c == '\n'){
      lines[global_count] = count; // store the first line number
      global_count++;
    }
  }
  if(n == 0 && i == 0){
    arr[i] = -1;
  }
  return;	
}

/* The purposes of the function is: 
 * To retrive the line number 
 * that is being stored in the global array called lines*/
int get_line_number(int ln){
  int i;
  for(i = 0; i < global_count; i++){
    if(ln < lines[i]){
      break;
    }
  }
  return i + 1; 
}	

int main(int argc, char **argv)
{
  int fd;
  char *filename;
  char *query;
  
  if (argc < 3) {
    printf(1, "Insufficient arguments.\n");
    printf(1, "usage: usfgrep <string> <file1> [<file2> ...]\n");
    printf(1, "\n");
    exit();
  }
  query = argv[1];
  
  int d;
  for(d = 2; d < argc; d++){
    filename = argv[d];
    fd = open(filename, O_RDONLY);
    if(fd < 0){
      printf(1, "File %s does not exist. Please try again\n", filename);
      exit();// an error occurred
    }
    int a = get_length(fd); // a is the length of the file
    char file_array[a];
    close(fd);
    
    fd = open(filename, O_RDONLY);
    read_line(fd,file_array); // file_array has the content now
    
    char *p1,*p2, *p3; // p2 is the query
    int i, j;
    int line_number;
    
    p1 = file_array; // p1 points to the line content
    query = argv[1];
    p2 = query;	
    
    for (i = 0; i < a; i++){
      if (*p1 == *p2){
        p3 = p1;
        line_number = get_line_number(i);				
        for(j = 0; j < strlen(query); j++){
          if(*p3 == *p2){						
            p3++;
            p2++;
          }else{
            break;
          }
        }
        p2 = query;
        char *q1, str_sentence[a];
        int index = 0, k, check = 0;
        /* j is the length of the query*/
        if(j == strlen(query)){ 
          q1 = file_array; // q1 points to the line content
          if(line_number == 1){
            check = 1;
          }
          for(k = 1; k <= a; k++){
            if(line_number > 1 && k == lines[line_number - 2]){
              check = 1;
            }else if(check == 1){
              /*The matching line is stored here now*/
              str_sentence[index++] = *q1;
            }
            if(k == lines[line_number - 1]){
              break;
            }
            q1++;
          }
          str_sentence[index]='\0';// add a NULL to the end of file
          printf(1, "%s[%d]:%s \n",filename,line_number,str_sentence);
        }
      }
      p1++;	
    }
    close(fd);
  }
  exit();
}
