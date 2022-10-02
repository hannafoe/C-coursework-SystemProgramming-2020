#include<stdlib.h>
#include<stdio.h>

struct board_structure {
  int row_size;
  int col_size;
  char **matrix;
};

int main(){
  struct board_structure board = {8,4};
  board.matrix = malloc(8*sizeof(char*));
  for(int i=0;i<8;i++){
    board.matrix[i] = malloc(4*sizeof(char));
    for(int j=0;j<4;j++){
      board.matrix[i][j]='.';
    }
  }
  for(int i=0;i<8;i++){
    printf("%s\n",board.matrix[i]);
  }
  return 0;
}
