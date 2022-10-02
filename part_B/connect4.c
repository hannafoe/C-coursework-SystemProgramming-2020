//You can include any of headers defined in the C11 standard here. They are:
//assert.h, complex.h, ctype.h, errno.h, fenv.h, float.h, inttypes.h, iso646.h, limits.h, locale.h, math.h, setjmp.h, signal.h, stdalign.h, stdarg.h, stdatomic.h, stdbool.h, stddef.h, stdint.h, stdio.h, stdlib.h, stdnoreturn.h, string.h, tgmath.h, threads.h, time.h, uchar.h, wchar.h or wctype.h
//You may not include any other headers.
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include"connect4.h"
//Extra functions declarations
int** four_in_a_row(char player,board u);
int** check_diagonal(char player,board u);
int** check_vertical(char player,board u);
int** check_horizontal(char player, board u);

struct board_structure {
  int row_size;
  int col_size;
  char **matrix;
};

board setup_board(){
  board u;
  u = malloc(sizeof(struct board_structure));
  u->row_size=4;
  u->col_size=4;
  u->matrix = malloc(4*4*sizeof(char));
  if(u->matrix ==NULL) exit(1);
/*  for(int i=0;i<4;i++){
    u->matrix[i] = malloc(4*sizeof(char));
    if(u->matrix[i]==NULL) exit(1);
  }*/
  printf("%d\n",u->row_size);
  return u;
}

void cleanup_board(board u){
  for(int i=0;i<((*u).row_size);i++){
    free(u->matrix[i]);
    u->matrix[i]=NULL;
  }
  free(u->matrix);
  u->matrix=NULL;
}

void read_in_file(FILE *infile, board u){
  if(infile == NULL){
    printf("Can't open file");
    exit(1);
  }
  int n=0;
  char *a;
  a=malloc(513*sizeof(char));
  if(a==NULL){
    exit(1);
  }
  while((fgetc(infile))!='\n'){
    n++;
  }
  int m=1;
  while((fscanf(infile,"%s\n",a))!=EOF){
    m++;
  }
  free(a);
  a=NULL;
  printf("Number of columns %d, Number of rows %d\n",n,m);
  printf("%d\n",u->row_size);
  u->col_size=n;
  u->row_size=m;
  printf("Hello-");
  for(int i=0;i<4;i++){
    char* newm;
    printf("%d",i);
    newm =realloc((u->matrix[i]),n*sizeof(char));
    if(newm==NULL){
      exit(1);
    }
    u->matrix[i]=newm;
    newm==NULL;
  }
  printf("Hello");
  char **newmatrix = realloc(u->matrix,m*sizeof(char*));
  if(newmatrix==NULL){
    exit(1);
  }
  u->matrix = newmatrix;
  free(newmatrix);
  newmatrix=NULL;
  for(int i=4;i<m;i++){
    printf("%d",i);
    u->matrix[i]=malloc(n*sizeof(char));
  }

  for(int i=0;i<m;i++){
    for(int j=0;j<n;j++){
      u->matrix[i][j]=fgetc(infile);
    }
    char c = fgetc(infile);
    if(c!='\n'){
      printf("Error, not end of line!");
      exit(1);
    }
  }
}

void write_out_file(FILE *outfile, board u){
  int **arr;
  int row_size =(u->row_size);
  int col_size =(u->col_size);
  arr=four_in_a_row('x',u);
  if(arr[0][0]!='\0'){
    //TURN THE 4 INTO CAPITALS
    for(int a=0;a<4;a++){
      u->matrix[(arr[a][0])][(arr[a][1])]='X';
    }
  }
  arr=four_in_a_row('o',u);
  if(arr[0][0]!='\0'){
    //TURN THE 4 INTO CAPITALS
    for(int a=0;a<4;a++){
      u->matrix[(arr[a][0])][(arr[a][1])]='O';
    }
  }
  for(int i=0;i<row_size;i++){
    char line[col_size];
    for(int j=0;j<col_size;j++){
      line[j]=(u->matrix[i][j]);
    }
    printf("%s\n",line);
  }
}

char next_player(board u){
//You may put code here
}

char current_winner(board u){
//You may put code here
}

struct move read_in_move(board u){
//You may put code here
  printf("Player %c enter column to place your token: ",next_player(u)); //Do not edit this line
//You may put code here
  printf("Player %c enter row to rotate: ",next_player(u)); // Do not edit this line
//You may put code here
}

int is_valid_move(struct move m, board u){
//You may put code here
}

char is_winning_move(struct move m, board u){
//You may put code here
}

void play_move(struct move m, board u){
//You may put code here
}
//You may put additional functions here if you wish.
//This function checks whether there are any tokens
//that are four in a row and returns the coords
//of those tokens if they exist
int** four_in_a_row(char player,board u){
  int **arr;
  arr=check_horizontal(player,u);
  if(arr[0][0]!='\0'){
    return arr;
  }
/*
  arr=check_vertical(player,u);
  if(arr[0][0]!='\0'){
    return arr;
  }
  arr=check_diagonal(player,u);
  if(arr[0][0]!='\0'){
    return arr;
  }
*/
  arr[0][0]='\0';
  return arr;
}
/*
int** check_diagonal(char player,board u){
  int **arr;
  char c;
  int ptr=0;
  for(int i=0;i<((*u).row_size)-4;i++){
    for(int j=0;j<((*u).col_size);j++){
      //Special case right end
      if(u->matrix[((*u).col_size)-3][i]==player){
        arr[ptr][0]=((*u).col_size)-3;
        arr[ptr][1]=i;
        if(u->matrix[((*u).col_size)-2][i-1]==player){
          arr[ptr][0]==((*u).col_size)-2;
          arr[ptr][1]==i-1;
          if(u->matrix[((*u).col_size)-1][i-2]==player){
            arr[ptr][0]==((*u).col_size)-1;
            arr[ptr][1]==i-2;
            if(u->matrix[0][i-3]==player){
              arr[ptr][0]==0;
              arr[ptr][1]==i-3;
              return arr;
            }
          }
        }
      }
      else if(u->matrix[((*u).col_size)-2][i]==player){
        arr[ptr][0]=((*u).col_size)-2;
        arr[ptr][1]=i;
        if(u->matrix[((*u).col_size)-1][i-1]==player){
          arr[ptr][0]==((*u).col_size)-1;
          arr[ptr][1]==i-1;
          if(u->matrix[0][i-2]==player){
            arr[ptr][0]==0;
            arr[ptr][1]==i-2;
            if(u->matrix[1][i-3]==player){
              arr[ptr][0]==1;
              arr[ptr][1]==i-3;
              return arr;
            }
          }
        }
      }
      else if(u->matrix[((*u).col_size)-1][i]==player){
        arr[ptr][0]=((*u).col_size)-1;
        arr[ptr][1]=i;
        if(u->matrix[0][i-1]==player){
          arr[ptr][0]==0;
          arr[ptr][1]==i-1;
          if(u->matrix[1][i-2]==player){
            arr[ptr][0]==1;
            arr[ptr][1]==i-2;
            if(u->matrix[2][i-3]==player){
              arr[ptr][0]==2;
              arr[ptr][1]==i-3;
              return arr;
            }
          }
        }
      }
      //Check down right
      else{
        for(int a=0;a<4;a++){
          if(u->matrix[i+a][j+a]==player){
            arr[ptr][0]=i+a;
            arr[ptr][1]=j+a;
            ptr++;
          }
        }
        if(ptr==4){
          return arr;
        }
        //Check down left
        for(int a=0;a<4;a++){
          if(u->matrix[i-a][j-a]==player){
            arr[ptr][0]=i-a;
            arr[ptr][1]=j-a;
            ptr++;
          }
        }
        if(ptr==4){
          return arr;
        }
      }
    ptr=0;
    }
  }
  arr[0][0]='\0';
  return arr;
}

int** check_vertical(char player,board u){
  int **arr;
  char c;
  int flag=0;
  int ptr=0;
  for(int i=0;i<((*u).col_size);i++){
    for(int j=0;j<((*u).row_size);j++){
      c=(u->matrix[j][i]);
      if(c==player){
        arr[ptr][0]=j;
        arr[ptr][1]=i;
        ptr++;
        flag=1;
      }else{
        flag=0;
        ptr=0;
      }
      if(ptr==4){
        return arr;
      }
    }
    ptr=0;
    int flag1=0;
    int flag2=0;
    int flag3=0;
    if(u->matrix[((*u).col_size)-3][i]==player){
      arr[ptr][0]=((*u).col_size)-3;
      arr[ptr][1]=i;
      ptr++;
      flag1=1;
    if(u->matrix[((*u).col_size)-2][i]==player){
      arr[ptr][0]=((*u).col_size)-2;
      arr[ptr][1]=i;
      ptr++;
      flag2=1;
    if(u->matrix[((*u).col_size)-1][i]==player){
      arr[ptr][0]=((*u).col_size)-1;
      arr[ptr][1]=i;
      ptr++;
      flag3=1;
    if(flag1==1||flag2==1||flag3==1){
      if(u->matrix[0][i]==player){
        arr[ptr][0]=0;
        arr[ptr][1]=i;
        ptr++;
      }
      if(ptr==4){
        return arr;
      }
      if(flag1==0){
        if(u->matrix[1][i]==player){
          arr[ptr][0]=1;
          arr[ptr][1]=i;
          ptr++;
        }
        if(ptr==4){
          return arr;
        }
      }
      if(flag2==0){
        if(flag1==1){
          arr[0][0]=arr[1][0];
          arr[0][1]=arr[1][1];
          arr[1][0]=arr[2][0];
          arr[1][1]=arr[2][1];
          arr[2][0]=arr[3][0];
          arr[2][1]=arr[3][1];
          ptr--;
        }
        if(u->matrix[2][i]==player){
          arr[ptr][0]=2;
          arr[ptr][1]=i;
          ptr++;
        }
        if(ptr==4){
          return arr;
        }
      }
    }
    ptr=0;
  }
  arr[0][0]='\0';
  return arr;
}
*/
int** check_horizontal(char player,board u){
  int **arr=malloc(4*sizeof(int*));
  for(int i=0;i<4;i++){
    arr[i]=malloc(2*sizeof(int));
  }
  char c;
  int flag=0;
  int ptr=0;
  for(int i=0;i<((*u).row_size);i++){
    for(int j=0;j<((*u).col_size);j++){
      c=(u->matrix[i][j]);
      if(c==player){
        arr[ptr][0]=i;
        arr[ptr][1]=j;
        ptr++;
        flag=1;
      }else{
        flag=0;
        ptr=0;
      }
      if(ptr==4){
        return arr;
      }
    }
    ptr=0;
    int flag1=0;
    int flag2=0;
    int flag3=0;
    if(u->matrix[i][((*u).row_size)-3]==player){
      arr[ptr][0]=i;
      arr[ptr][1]=((*u).row_size)-3;
      ptr++;
      flag1=1;
    }
    if(u->matrix[i][((*u).row_size)-2]==player){
      arr[ptr][0]=i;
      arr[ptr][1]=((*u).row_size)-2;
      ptr++;
      flag2=1;
    }
    if(u->matrix[i][((*u).row_size)-1]==player){
      arr[ptr][0]=i;
      arr[ptr][1]=((*u).row_size)-1;
      ptr++;
      flag3=1;
    }
    if(flag1==1||flag2==1||flag3==1){
      if(u->matrix[i][0]==player){
        arr[ptr][0]=i;
        arr[ptr][1]=0;
        ptr++;
      }
      if(ptr==4){
        return arr;
      }
      if(flag1==0){
        if(u->matrix[i][1]==player){
          arr[ptr][0]=i;
          arr[ptr][1]=1;
          ptr++;
        }
        if(ptr==4){
          return arr;
        }
      }
      if(flag2==0){
        if(flag1==1){
          arr[0][0]=arr[1][0];
          arr[0][1]=arr[1][1];
          arr[1][0]=arr[2][0];
          arr[1][1]=arr[2][1];
          arr[2][0]=arr[3][0];
          arr[2][1]=arr[3][1];
          ptr--;
        }
        if(u->matrix[i][2]==player){
          arr[ptr][0]=i;
          arr[ptr][1]=2;
          ptr++;
        }
        if(ptr==4){
          return arr;
        }
      }
    }
    ptr=0;
  }
  arr[0][0]='\0';
  return arr;
}
