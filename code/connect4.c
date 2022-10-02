//You can include any of headers defined in the C11 standard here. They are:
//assert.h, complex.h, ctype.h, errno.h, fenv.h, float.h, inttypes.h, iso646.h, limits.h, locale.h, math.h, setjmp.h, signal.h, stdalign.h, stdarg.h, stdatomic.h, stdbool.h, stddef.h, stdint.h, stdio.h, stdlib.h, stdnoreturn.h, string.h, tgmath.h, threads.h, time.h, uchar.h, wchar.h or wctype.h
//You may not include any other headers.
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<ctype.h>
#include"connect4.h"
//Extra functions declarations
int** four_in_a_row(char player,board u);
int** check_diagonal(char player,board u);
int** check_vertical(char player,board u);
int** check_horizontal(char player, board u);

struct board_structure {
  int row_size;
  int col_size;
  char player;
  char **matrix;
};


board setup_board(){
  board u;
  u = malloc(sizeof(struct board_structure));
  if(u==NULL){
    fprintf(stderr,"out of memory\n");
    exit(1);
  }
  return u;
}

void cleanup_board(board u){
  for(int i=0;i<((*u).row_size);i++){
    free(u->matrix[i]);
    u->matrix[i]=NULL;
  }
  free(u->matrix);
  u->matrix=NULL;
  free(u);
  u=NULL;
}

void read_in_file(FILE *infile, board u){
  if(infile == NULL){
    fprintf(stderr,"Can't open file.\n");
    free(u);
    u=NULL;
    fclose(infile);
    exit(1);
  }
  //First get the size of column and row
  int n=0;
  char *a;
  a=malloc(513*sizeof(char));
  if(a==NULL){
    fprintf(stderr,"out of memory\n");
    free(u);
    u=NULL;
    fclose(infile);
    exit(1);
  }
  char c=fgetc(infile);
  while(c!='\n'&&!isspace(c)){
    c=fgetc(infile);
    n++;
  }
  int m=1;
  while((fscanf(infile,"%s\n",a))!=EOF){
    m++;
  }
  fseek(infile, 0, SEEK_SET);
  free(a);
  a=NULL;
  if(n<4){
    fprintf(stderr,"Input is not valid, not enough columns.\n");
    free(u);
    u=NULL;
    fclose(infile);
    exit(1);
  }
  //Now we've got m=row_size and n=col_size
  //Check if the input has enough rows and columns
  if(n>512){
    fprintf(stderr,"Input is not valid, too many columns.\n");
    free(u);
    u=NULL;
    fclose(infile);
    exit(1);
  }
  if(m<4){
    fprintf(stderr,"Input is not valid, not enough rows.\n");
    free(u);
    u=NULL;
    fclose(infile);
    exit(1);
  }
  //Now construct the matrix to save the grid from the input file
  u->matrix = malloc(m*sizeof(char*));
  if(u->matrix ==NULL){
    fprintf(stderr,"out of memory\n");
    free(u);
    u=NULL;
    fclose(infile);
    exit(1);
  } 
  for(int i=0;i<m;i++){
    u->matrix[i] = malloc(n*sizeof(char));
    if(u->matrix[i]==NULL){
      fprintf(stderr,"out of memory\n");
      for(int j=0;j<i;j++){
        free(u->matrix[j]);
        u->matrix[j]=NULL;
      }
      free(u->matrix);
      u->matrix=NULL;
      free(u);
      u=NULL;
      fclose(infile);
      exit(1);
    }
  }
  //While checking if each character is valid, save the characters into the matrix
  for(int i=0;i<m;i++){
    for(int j=0;j<n;j++){
      char a;
      int x = fscanf(infile,"%c",&a);
      if(x==0||(!(isalpha(a))&&!(ispunct(a)))){
        fprintf(stderr,"Not valid input file.\n");
        for(int x=0;x<m;x++){
          free(u->matrix[x]);
          u->matrix[x]=NULL;
        }
        free(u->matrix);
        u->matrix=NULL;
        free(u);
        u=NULL;
        fclose(infile);
        exit(1);
      }
      if(((ispunct(a))==0)&&a!='x'&&a!='o'&&a!='O'&&a!='X'){
        fprintf(stderr,"Not valid input file.\n");
        for(int x=0;x<m;x++){
          free(u->matrix[x]);
          u->matrix[x]=NULL;
        }
        free(u->matrix);
        u->matrix=NULL;
        free(u);
        u=NULL;
        fclose(infile);
        exit(1);
      }
      u->matrix[i][j]=a;
    }
    char c = fgetc(infile);
    //Check for trailing spaces
    if(isspace(c)){
      while(c!='\n'){
        c=fgetc(infile);
        if(!isspace(c)){
          break;
        }
      }
    }
    if(c!='\n'&&c!=EOF){
      fprintf(stderr,"Error, column size differs from column size in row 1.\n");
      for(int x=0;x<m;x++){
        free(u->matrix[x]);
        u->matrix[x]=NULL;
      }
      free(u->matrix);
      u->matrix=NULL;
      free(u);
      u=NULL;
      fclose(infile);
      exit(1);
    }
  }
  u->col_size=n;
  u->row_size=m;
  //CHECK WHETHER GRAVITY HAS WORKED IN INPUT FILE
  for(int i=0;i<(u->col_size);i++){
    for(int j=(u->row_size)-1;j>0;j--){
      if(u->matrix[j][i]=='.'){
        int onlydots = j;
        int ptr = 0;
        for(int a=j-1;a>=0;a--){
          if(u->matrix[a][i]!='.'){
            u->matrix[j][i]=u->matrix[a][i];
            u->matrix[a][i]='.';
            break;
          }
          ptr++;
        }
        if(ptr==onlydots){
          break;
        }
      }
    }
  }
  u->player='-';
  u->player=next_player(u);
}

void write_out_file(FILE *outfile, board u){
  int **arr1;
  int **arr2;
  int row_size =(u->row_size);
  int col_size =(u->col_size);
  arr1=four_in_a_row('x',u);
  if(arr1[0][0]!=-1){//if arr1[0][0]==-1 then four in a row were not found
    //TURN THE 4 INTO CAPITALS
    for(int a=0;a<4;a++){
      u->matrix[(arr1[a][0])][(arr1[a][1])]='X';
    }
  }
  //free the memory of arr1
  for(int i=0;i<4;i++){
    free(arr1[i]);
    arr1[i]=NULL;
  }
  free(arr1);
  arr1=NULL;
  
  arr2=four_in_a_row('o',u);
  if(arr2[0][0]!=-1){// if arr2[0][0]==-1 then four in a row were not found
    //TURN THE 4 INTO CAPITALS
    for(int a=0;a<4;a++){
      u->matrix[(arr2[a][0])][(arr2[a][1])]='O';
    }
  }
  //free the memory of arr2
  for(int i=0;i<4;i++){
    free(arr2[i]);
    arr2[i]=NULL;
  }
  free(arr2);
  arr2=NULL;
  
  //Write out the current matrix
  
  for(int i=0;i<row_size;i++){
    char line[col_size + 1];
    for(int j=0;j<col_size;j++){
      line[j]=(u->matrix[i][j]);
    }
    line[col_size] = '\0';
    fprintf(outfile, "%s\n",line);
  }
  
}

char next_player(board u){
  //The standard case
  if(u->player!='-'){
    return u->player;
  }
  //The initialisation case
  int count_x=0;
  int count_o=0;
  for(int i=0;i<((*u).row_size);i++){
    for(int j=0;j<((*u).col_size);j++){
      if(u->matrix[i][j]=='o'){
        count_o++;
      }
      if(u->matrix[i][j]=='x'){
        count_x++;
      }
    }
  }
  if(count_x>count_o){
    return 'o';
  }else if(count_o>count_x){
    return 'x';
  }else{
    return 'x';
  }
}

char current_winner(board u){
//You may put code here
  int **arr1;
  int **arr2;
  int win[2];
  win[0]=0;//Notes if x is winning
  win[1]=0;//Notes if o is winning
  arr1=four_in_a_row('x',u);
  if(arr1[0][0]!=-1){
    win[0]=1;
  }
  //free the memory of arr1
  for(int i=0;i<4;i++){
    free(arr1[i]);
    arr1[i]=NULL;
  }
  free(arr1);
  arr1=NULL;
  
  arr2=four_in_a_row('o',u);
  if(arr2[0][0]!=-1){
    //TURN THE 4 INTO CAPITALS
    win[1]=1;
  }
  //free the memory of arr2
  for(int i=0;i<4;i++){
    free(arr2[i]);
    arr2[i]=NULL;
  }
  free(arr2);
  arr2=NULL;
  if(win[0]==1&&win[1]==1){//TIE, BOTH WIN
    return 'd';
  }else if(win[0]==0&&win[1]==1){//O WINS
    return 'o';
  }else if(win[0]==1&&win[1]==0){//X WINS
    return 'x';
  }else{
    int flag = 0;
    for(int i=0;i<(u->col_size);i++){
      if(u->matrix[0][i]=='.'){
        flag =1;
        break;
      }
    }
    if(flag==0){//TIE, NO SPACE LEFT IN MATRIX
      return 'd';
    }
    return '.';
  }
}

struct move read_in_move(board u){
  printf("Player %c enter column to place your token: ",next_player(u)); //Do not edit this line
  int col;
  int i=scanf("%d",&col);
  if(i==0){
    fprintf(stderr,"The input given was not an integer.\n");
    cleanup_board(u);
    exit(1);
  }
  printf("Player %c enter row to rotate: ",next_player(u)); // Do not edit this line
  int row;
  i = scanf("%d",&row);
  if(i==0){
    fprintf(stderr,"The input given was not an integer.\n");
    cleanup_board(u);
    exit(1);
  }
  struct move m;
  m.column=col;//INDEX 1!!
  m.row=row;//HAVE TO CONSIDER THE DIFFERENCE(FLIPPED AND INDEX 1)
  return m;
}

int is_valid_move(struct move m, board u){
  if(m.column>(u->col_size)){
    return 0;
  }
  if(abs(m.row)>(u->row_size)){
    return 0;
  }
  if(u->matrix[0][m.column-1]!='.'){
    return 0;
  }
  //printf("Is valid move.\n");
  return 1;
}

char is_winning_move(struct move m, board u){
  board temp = setup_board();
  temp->player=u->player;
  temp->col_size=u->col_size;
  temp->row_size=u->row_size;
  temp->matrix = malloc((temp->row_size)*sizeof(char*));
  if(temp->matrix ==NULL){
    fprintf(stderr,"out of memory\n");
    free(temp);
    temp=NULL;
    cleanup_board(u);
    exit(1);
  }
  for(int i=0;i<(temp->row_size);i++){
    temp->matrix[i] = malloc((temp->col_size)*sizeof(char));
    if(temp->matrix[i]==NULL){
      fprintf(stderr,"out of memory\n");
      for(int j=0;j<i;j++){
        free(temp->matrix[j]);
        temp->matrix[j]=NULL;
      }
      free(temp->matrix);
      temp->matrix=NULL;
      free(temp);
      temp=NULL;
      cleanup_board(u);
      exit(1);
    }
  }
  for(int i=0;i<(temp->row_size);i++){
    for(int j=0;j<(temp->col_size);j++){
      temp->matrix[i][j]=(u->matrix[i][j]);
    }
  }
  play_move(m,temp);
  char winner;
  winner = current_winner(temp);
  cleanup_board(temp);
  return winner;
}

void play_move(struct move m, board u){
  int column=m.column-1;//Normalise the column index to suit matrix, by subtracting one and making it index 0 based
  u->matrix[0][column]=u->player;//Put token in the highest position of the column
  for(int i=1;i<(u->row_size);i++){//Let gravity work to push the token down the column
    if(u->matrix[i][column]!='.'){
      break;
    }
    if(u->matrix[i][column]=='.'){
      u->matrix[i][column]=u->player;
      u->matrix[i-1][column]='.';
    }else{
      break;
    }
  }
  //Rotate the row that was specified in move
  if(m.row>0){//Case of rotating to the right
    int row=(u->row_size)-m.row;//Normalise the row index to suit matrix, by flipping and making it index 0 based
    char temp=u->matrix[row][(u->col_size)-1];
    for(int i=(u->col_size)-1;i>0;i--){
      u->matrix[row][i]=u->matrix[row][i-1];
      for(int j=(row)+1;j<(u->row_size);j++){
        if(u->matrix[j][i]=='.'){
          u->matrix[j][i]=u->matrix[j-1][i];
          u->matrix[j-1][i]='.';
        }else{
          break;
        }
      }
    }
    u->matrix[row][0]=temp;
    for(int j=row+1;j<(u->row_size);j++){
      if(u->matrix[j][0]=='.'){
        u->matrix[j][0]=u->matrix[j-1][0];
        u->matrix[j-1][0]='.';
      }else{
        break;
      }
    }
    //Now look for gaps in the columns and let gravity work
    for(int i=0;i<(u->col_size);i++){
      //printf("%c %d %d\n",u->matrix[row][i],row,i);
      if(u->matrix[row][i]=='.'){
        int j=row;
        //printf("col %d row %d---\n",i,j);
        if(j<=0){
          break;
        }
        while(u->matrix[j-1][i]!='.'&&j>0){
          //printf("col %d row %d\n",i,j);
          u->matrix[j][i]=u->matrix[j-1][i];
          u->matrix[j-1][i]='.';
          j--;
          if((j-1)<0){
            break;
          }
        }
      }
    }
  }
  else if(m.row<0){//Case of rotating to the left
    int row=(u->row_size)-abs(m.row);
    char temp=u->matrix[row][0];
    for(int i=0;i<(u->col_size)-1;i++){
      u->matrix[row][i]=u->matrix[row][i+1];
      for(int j=row+1;j<(u->row_size);j++){
        if(u->matrix[j][i]=='.'){
          u->matrix[j][i]=u->matrix[j-1][i];
          u->matrix[j-1][i]='.';
        }else{
          break;
        }
      }
    }
    u->matrix[row][(u->col_size)-1]=temp;
    for(int j=row+1;j<(u->row_size);j++){
      if(u->matrix[j][(u->col_size)-1]=='.'){
        u->matrix[j][(u->col_size)-1]=u->matrix[j-1][(u->col_size)-1];
        u->matrix[j-1][(u->col_size)-1]='.';
      }else{
        break;
      }
    }
    //Now look for gaps in the columns and let gravity work
    for(int i=0;i<(u->col_size);i++){
      if(u->matrix[row][i]=='.'){
        int j=row;
        if(j<=0){
          break;
        }
        while(u->matrix[j-1][i]!='.'&&j>0){
          u->matrix[j][i]=u->matrix[j-1][i];
          u->matrix[j-1][i]='.';
          j--;
          if((j-1)<0){
            break;
          }
        }
      }
    }
  }
  if(u->player=='x'){
    u->player='o';
  }else{
    u->player='x';
  }
}
//You may put additional functions here if you wish.

//This function checks whether there are any tokens
//that are four in a row and returns the coords
//of those tokens if they exist

int** four_in_a_row(char player,board u){
  int **arr1;
  int **arr2;
  int **arr3;
  arr1=check_horizontal(player,u);
  if(arr1[0][0]!=-1){
    return arr1;
  }
  arr2=check_vertical(player,u);
  if(arr2[0][0]!=-1){
    for(int i=0;i<4;i++){
      free(arr1[i]);
      arr1[i]=NULL;
    }
    free(arr1);
    arr1=NULL;
    return arr2;
  }
  //free the memory of arr2 if not used
  for(int i=0;i<4;i++){
    free(arr2[i]);
    arr2[i]=NULL;
  }
  free(arr2);
  arr2=NULL;

  arr3=check_diagonal(player,u);
  if(arr3[0][0]!=-1){
    for(int i=0;i<4;i++){
      free(arr1[i]);
      arr1[i]=NULL;
    }
    free(arr1);
    arr1=NULL;
    return arr3;
  }
  //free the memory of arr3 if not used
  for(int i=0;i<4;i++){
    free(arr3[i]);
    arr3[i]=NULL;
  }
  free(arr3);
  arr3=NULL;
  arr1[0][0]=-1;
  return arr1;
}

int** check_diagonal(char player,board u){
  int **arr=malloc(4*sizeof(int*));;
  if(arr==NULL){
    fprintf(stderr,"out of memory\n");
    cleanup_board(u);
    exit(1);
  }
  for(int i=0;i<4;i++){
    arr[i]=malloc(2*sizeof(int));
    if(arr[i]==NULL){
      fprintf(stderr,"out of memory\n");
      cleanup_board(u);
      for(int j=0;j<i;j++){
        free(arr[j]);
        arr[j]=NULL;
      }
      free(arr);
      arr=NULL;
      exit(1);
    }
  }
  int ptr=0;
  //Check down right
  for(int i=0;i<((*u).col_size);i++){
    int over =0;
    for(int j=0;j<((*u).row_size);j++){
      if((i+j)>(((*u).col_size)-1)){
        if(u->matrix[j][over]==player||u->matrix[j][over]==toupper(player)){
          arr[ptr][0]=j;
          arr[ptr][1]=over;
          ptr++;
        }else{
          ptr=0;
        }
        over++;
      }
      else if(u->matrix[j][i+j]==player||u->matrix[j][i+j]==toupper(player)){
        arr[ptr][0]=j;
        arr[ptr][1]=i+j;
        ptr++;
      }else{
        ptr=0;
      }
      if(ptr==4){
        return arr;
      }
    }
    ptr=0;
  }
  //Check down left
  for(int i=0;i<((*u).col_size);i++){
    int over =((*u).col_size)-1;
    for(int j=0;j<((*u).row_size);j++){
      if((i-j)<0){
        if(u->matrix[j][over]==player||u->matrix[j][over]==toupper(player)){
          arr[ptr][0]=j;
          arr[ptr][1]=over;
          ptr++;
        }else{
          ptr=0;
        }
        over--;
      }
      else if(u->matrix[j][i-j]==player||u->matrix[j][i-j]==toupper(player)){
        arr[ptr][0]=j;
        arr[ptr][1]=i-j;
        ptr++;
      }else{
        ptr=0;
      }
      if(ptr==4){
        return arr;
      }
    }
    ptr=0;
  }
  arr[0][0]=-1;
  return arr;
}

int** check_vertical(char player,board u){
  int **arr=malloc(4*sizeof(int*));
  if(arr==NULL){
    fprintf(stderr,"out of memory\n");
    cleanup_board(u);
    exit(1);
  }
  for(int i=0;i<4;i++){
    arr[i]=malloc(2*sizeof(int));
    if(arr[i]==NULL){
      fprintf(stderr,"out of memory\n");
      cleanup_board(u);
      for(int j=0;j<i;j++){
        free(arr[j]);
        arr[j]=NULL;
      }
      free(arr);
      arr=NULL;
      exit(1);
    }
  }
  char c;
  int ptr=0;
  for(int i=0;i<((*u).col_size);i++){
    for(int j=0;j<((*u).row_size);j++){
      c=(u->matrix[j][i]);
      if(c==player||c==toupper(player)){
        arr[ptr][0]=j;
        arr[ptr][1]=i;
        ptr++;
      }else{
        ptr=0;
      }
      if(ptr==4){
        return arr;
      }
    }
    ptr=0;
  }
  arr[0][0]=-1;
  return arr;
}

int** check_horizontal(char player,board u){
  int **arr=malloc(4*sizeof(int*));
  if(arr==NULL){
    fprintf(stderr,"out of memory\n");
    cleanup_board(u);
    exit(1);
  }
  for(int i=0;i<4;i++){
    arr[i]=malloc(2*sizeof(int));
    if(arr[i]==NULL){
      fprintf(stderr,"out of memory\n");
      cleanup_board(u);
      for(int j=0;j<i;j++){
        free(arr[j]);
        arr[j]=NULL;
      }
      free(arr);
      arr=NULL;
      exit(1);
    }
  }

  //The following covers the normal case
  char c;
  int ptr=0;
  for(int i=0;i<((*u).row_size);i++){
    for(int j=0;j<((*u).col_size);j++){
      c=(u->matrix[i][j]);
      if(c==player||c==toupper(player)){
        arr[ptr][0]=i;
        arr[ptr][1]=j;
        ptr++;
      }else{
        ptr=0;
      }
      if(ptr==4){
        return arr;
      }
    }
    //The following covers the special cases with wrap-around
    ptr=0;
    int flag1=0;
    int flag2=0;
    int flag3=0;
    if(u->matrix[i][((*u).col_size)-3]==player||(u->matrix[i][((*u).col_size)-3]==toupper(player))){//[...._..]
      arr[ptr][0]=i;
      arr[ptr][1]=((*u).col_size)-3;
      ptr++;
      flag1=1;
    }
    if(u->matrix[i][((*u).col_size)-2]==player||u->matrix[i][((*u).col_size)-2]==toupper(player)){//[....._.]
      arr[ptr][0]=i;
      arr[ptr][1]=((*u).col_size)-2;
      ptr++;
      flag2=1;
    }
    if(u->matrix[i][((*u).col_size)-1]==player||u->matrix[i][((*u).col_size)-1]==toupper(player)){//[......_]
      arr[ptr][0]=i;
      arr[ptr][1]=((*u).col_size)-1;
      ptr++;
      flag3=1;
    }
    if(u->matrix[i][0]==player||u->matrix[i][0]==toupper(player)){//[_......]
        arr[ptr][0]=i;
        arr[ptr][1]=0;
        if(flag1==1&&flag2==1&&flag3==1&&ptr==3){
          return arr;//[X...XXX]
        }
    }
    
    if(u->matrix[i][0]==player||u->matrix[i][0]==toupper(player)){
        arr[ptr][0]=i;
        arr[ptr][1]=0;
        ptr++;
        if(u->matrix[i][1]==player||u->matrix[i][1]==toupper(player)){//[._.....]
          arr[ptr][0]=i;
          arr[ptr][1]=1;
          if(flag2==1&&flag3==1&&ptr==3){
            return arr;//[XX...XX]
          }
        }
        ptr--;
    }
    if(u->matrix[i][0]==player||u->matrix[i][0]==toupper(player)){
        arr[ptr][0]=i;
        arr[ptr][1]=0;
        ptr++;
        if(u->matrix[i][1]==player||u->matrix[i][1]==toupper(player)){
          arr[ptr][0]=i;
          arr[ptr][1]=1;
          ptr++;
          if(u->matrix[i][2]==player||u->matrix[i][2]==toupper(player)){//[.._....]
            arr[ptr][0]=i;
            arr[ptr][1]=2;
            if(flag3==1){
              return arr;//[XXX...X]
            }
          }
        }
    }
    ptr=0;
  }
  arr[0][0]=-1;
  return arr;
}
