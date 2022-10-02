#include<stdio.h>
#include<stdlib.h>

int main(){
  FILE *infile;
  infile = fopen("initial_board.txt","r");
  int n=0;
  char a[513];
  while((fgetc(infile))!='\n'){
    n++;
  }
  int m=1;
  while((fscanf(infile,"%s\n",a))!=EOF){
    m++;
  }
  printf("Number of columns %d, Number of rows %d\n",n,m);
  return 0;
}
