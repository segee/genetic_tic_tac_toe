#include "ttt_helpers.h"
#include <stdio.h>

int ask_player(void);
int ask_move(uint32_t board);
int get_computer_pick(uint32_t,int);

main()
{
  uint32_t board=0x55555;
  int move;
  int player;
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
    else board=add_a_move(board,whose_turn,get_computer_pick(board,whose_turn));
    whose_turn ^= 1; //change 2 to 3 or  3 to 2;
    print_board(board);
  }
  if(game_done(board)==1) printf("Tie\n");
  else if(game_done(board)==2) printf("X wins\n");
  else printf("O wins\n");
}
int get_computer_pick(uint32_t board, int player)
{
	int x;
	for(x=0;x<9;x++) if(get_square(board,x)==1) return x;
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
