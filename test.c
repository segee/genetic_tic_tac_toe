#include "ttt_helpers.h"
#include <stdio.h>

main()
{
	uint32_t board;


	board=0x3f;
	print_board(board);
	board=0xfc0;
	print_board(board);
	board=0x3f000;
	print_board(board);
	board=0x30c3;
	print_board(board);
	board=0xc30c;
	print_board(board);
	board=0x30c30;
	print_board(board);
	
	board=0x30303;
	print_board(board);
	board=0x3330;
	print_board(board);
	board=0x55555;
	print_board(board);
	printf("There are %d moves \n",how_many_moves(board));
	board=add_a_move(board,2,8);
        print_board(board);	
	printf("There are %d moves \n",how_many_moves(board));
	board=add_a_move(board,3,0);
	print_board(board);

	printf("There are %d moves \n",how_many_moves(board));
	board=add_a_move(board,2,2);
	print_board(board);
	
	printf("There are %d moves \n",how_many_moves(board));
	printf("check_for_win returned %d\n",check_for_win(board));
	board=add_a_move(board,2,5);
	print_board(board);

	printf("There are %d moves \n",how_many_moves(board));
	printf("check_for_win returned %d\n",check_for_win(board));
}
