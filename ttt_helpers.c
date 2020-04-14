#include "ttt_helpers.h"
#include <stdio.h>

/*counts empty spaces, 1==empty, 2==X, 3==O, zero is not valid */
/* board is an 18 bit value encoding 9 board positions with pairs of bits */
int how_many_moves(uint32_t board)
{
	int x;
	int mask=3;
	int count=0;

	for(x=0;x<9;x++)
	{
		 if((get_square(board,x)==1)) count++;
	}
	return count;
}
int get_square(uint32_t board, int square)
{
	return (board & (3<<(square<<1)))>>(square<<1);
}



/* return 2 if x wins, return 3 if O wins, return 0 if no winner */
int check_for_win(uint32_t board)
{
	uint32_t mask[8]={0x3f,0xfc0,0x3f000,0x30c3,0xc30c,0x30c30,0x30303,0x3330};
	uint32_t all3s=0xfffff;
	uint32_t all2s=0xaaaaa;
	uint32_t all1s=0x55555;
	int x;

	for(x=0;x<8;x++)
	{
		if((board&mask[x])==(all3s&mask[x])) return 3;
		if((board&mask[x])==(all2s&mask[x])) return 2;
	}
	return 0;
}

uint32_t add_a_move(uint32_t board, int player, int where)
{
	/* player should be 2 or 3 for x and y (x goes first) */
	/* where should be 0-8 */
	/* 0|1|2 */
	/* ______*/
	/* 3|4|5 */
	/* ______*/
	/* 6|7|8 */

	/* returns new board if everything is ok, 0 for error */
	uint32_t mask;

	if((player>3)||(player<2)) return 0;
	if((where>8)||(where<0)) return 0;
	if(get_square(board,where)!=1) return 0;
	mask = ~(3<<(where<<1));
	board=(board & mask)|(player<<(where<<1));
	return board;
}
	
void print_board(uint32_t board)
{
	char vals[9]="         ";
	int x;

	for(x=0;x<9;x++)
	{
		if(get_square(board,x)== 1) vals[x]=' ';
		if(get_square(board,x)== 2) vals[x]='X';
		if(get_square(board,x)== 3) vals[x]='O';
	}
	printf("%c|%c|%c\n",vals[0],vals[1],vals[2]);
	printf("______\n");
	printf("%c|%c|%c\n",vals[3],vals[4],vals[5]);
	printf("______\n");
	printf("%c|%c|%c\n\n\n",vals[6],vals[7],vals[8]);
}

int game_done(uint32_t board)
{
   if(how_many_moves(board)!=0 && check_for_win(board)==0)return 0; //not done
   if(check_for_win(board)==2) return 2;
   if(check_for_win(board)==3) return 3;
   return 1;
}



