#include "evaluate_fitness.h"
#include "ttt_helpers.h"
#include <stdlib.h>
double evaluate_fitness(void * individual)
{
	uint32_t board=0x15555;
	uint32_t boards[9];
	int x;
	double total_fitness;
        total_fitness=evaluate_fitness_iterate(board,2,individual);
	for(x=0;x<9;x++)
	{
		boards[x]=add_a_move(board,2,x);
		total_fitness += evaluate_fitness_iterate(boards[x],3,individual);
	}




        //printf("fitness is %lf\n",total_fitness);
	return total_fitness;
}


double evaluate_fitness_iterate(uint32_t board, int offense,void * individual)
{
  double sum_fitness=0;
  int x;
  uint32_t boards[9];
  if(game_done(board)) return final_fitness(board,offense);
  //printf("Computer picks %d\n",get_computer_pick(board,offense,individual));
  board=add_a_move(board,offense,get_computer_pick(board,offense,individual));
 // print_board(board);
  if(game_done(board)) return final_fitness(board,offense);
  for(x=0;x<9;x++)
  { boards[x]=board;
    if(get_square(board,x)==1) 
    {
     boards[x]=add_a_move(board,offense^1,x);
     sum_fitness+=evaluate_fitness_iterate(boards[x],offense,individual);
    }
  }
  return sum_fitness;
}
	    
double final_fitness(uint32_t board, int offense)
{  int gd;
	static int count=0;
	//offense==2 means computer is x and plays first
	//offense==3 means computer plays second
	//there are three outcomes (win, lose, tie)
	//for computer == X and computer == O
	//each has a fitness value

//  printf("In final fitness, count = %d \n",++count);
//  print_board(board);
//printf("Computer is %d \n",offense);  
  if(!(gd=game_done(board))) return 0; //shouldn't happen
  if(gd==offense) //computer won
  { if(offense==2) return 10; //computer played first and won
    else return 20; //computer played second and won
  }
  else if(gd==1) //tied
  {
	  if(offense==2) return 5; //computer played first and tied
	  else return 10; //computer played second and tied
  }
  else if(offense==2) return 0; //computer played first and lost
       else return 0;          //computer played second and lost
}
 
