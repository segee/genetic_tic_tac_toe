#include "ttt_helpers.h"
#include <stdio.h>
#include <stdlib.h>

int ask_player(void);
int ask_move(uint32_t board);
int get_computer_pick(uint32_t,int,void *);

int read_individual_from_file(char * fname,long int * individual);
long int individual[9];

main()
{
  uint32_t board=0x55555;
  int move;
  int player;
  if(!read_individual_from_file("brucebest.txt",individual))printf("can't open file\n"),exit(0);

  int whose_turn=2;
  player=ask_player();
  if(player== 3)
  {
	  board=add_a_move(board,2,0);//if player is O computer goes first
	  print_board(board);
	  whose_turn=3;
  }
  while(!game_done(board))
  { if( whose_turn==player) board=add_a_move(board, player,ask_move(board));
    else board=add_a_move(board,whose_turn,get_computer_pick(board,whose_turn,individual));
    whose_turn ^= 1; //change 2 to 3 or  3 to 2;
    print_board(board);
  }
  if(game_done(board)==1) printf("Tie\n");
  else if(game_done(board)==2) printf("X wins\n");
  else printf("O wins\n");
}
int ask_move(uint32_t board)
{ int move;
  move=42;
  while(move>8||move<0||get_square(board,move)!=1)
  {
     printf("Please enter a move from 0 to 8\n");
     scanf("%d",&move);
  }
  return move;
}

int ask_player(void)
{  char c;
  printf("please choose X or O (X goes first)\n");
  while((c=getchar()) != 'X' && c!='O')printf("X or O\n");
  if(c=='O') return 3;
  else return 2;
}

int read_individual_from_file(char * fname,long int * individual)
{
	FILE *fp;
	int x;

	fp=fopen(fname,"r");
	if(fp==NULL) return 0;
	for(x=0;x<9;x++) fscanf(fp,"%li",&individual[x]);
	fclose(fp);
	return 1;
}
int write_individual_to_file(char * fname, int * individual)
{
	FILE *fp;
	int x;

	fp=fopen(fname,"w");
	if(fp==NULL) return 0;
	for(x=0;x<9;x++) fprintf(fp,"%d\n",individual[x]);
	fclose(fp);
	return 1;
}
int get_computer_pick(uint32_t board, int player,void * individual)
{
        int x;
        int count;
        long int *hash;
	char *hash2;
	long int temp=0;
        hash=individual;
	
	count=how_many_moves(board);
	for(x=0;x<9;x++)
	{  //printf("hash[%d] is %d, get_square is %d\n",x,hash[x],get_square(board,x));
		hash2=(void *) &hash[x];
		temp +=(hash2[9-count])*(long)get_square(board,x);
	}
	//printf("temp is %ld\n",temp);
	//printf("how_many_moves returned %d\n",how_many_moves(board));
	count=temp%how_many_moves(board);
	if(count<0)count+=how_many_moves(board);
//	printf("Count is %d\n",count);
        for(x=0;x<9;x++)
	{
		if(get_square(board,x)==1)
			if(count==0) break;
			else count--;
	}
	//printf ("computer picks square %d\n",x);
	return x;

}


