#include "evaluate_fitness.h"
#include "ttt_helpers.h"
#include <stdlib.h>
int how_many_games;
int wins;
int losses;
int ties;
extern int generation;

double evaluate_fitness(void * individual)
{
	uint32_t board=0x15555;
	uint32_t boards[9];
	int x;
	double total_fitness;
	//printf("Beginning of evaluate_fitness\n");
	how_many_games=0;
	wins=losses=ties=0;
	//printf("About to call iterate board: %x individual %p\n",board,individual);
        total_fitness=evaluate_fitness_iterate(board,2,individual);
	//printf("After first call to iterate\n");
	for(x=0;x<9;x++)
	{
		boards[x]=add_a_move(board,2,x);
		total_fitness += evaluate_fitness_iterate(boards[x],3,individual);
	}

//printf("Did I get this far?\n");
total_fitness /= how_many_games*losses;
total_fitness *= (wins+ties);
if(losses<5) printf("losses %d wins %d ties %d how_many_games %d\n",losses, wins, ties, how_many_games);
//printf("games %d losses %d \n",how_many_games,losses);
//total_fitness *= 1000; //bring into integer range
if(total_fitness<0) total_fitness=1;

        //printf("XXXfitness is %lf\n",total_fitness);
	return total_fitness;
}


double evaluate_fitness_iterate(uint32_t board, int offense,void * individual)
{
  double sum_fitness=0;
  int x;
  uint32_t boards[9];
  //printf("1Game done returns %d\n",game_done(board));
  if(game_done(board)) return final_fitness(board,offense);
  //printf("Computer picks %d\n",get_computer_pick(board,offense,individual));
  board=add_a_move(board,offense,get_computer_pick(board,offense,individual));
  //print_board(board);
  //printf("2Game done returns %d\n",game_done(board));
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
how_many_games++;  
  if(!(gd=game_done(board))) return 0; //shouldn't happen
  if(gd==offense) //computer won
  { if(offense==2){ wins++;return 150;} //computer played first and won
    else {wins++;return 10;} //computer played second and won
  }
  else if(gd==1) //tied
  {
	  if(offense==2){ ties++;return 50;} //computer played first and tied
	  else{ ties++;return 100;} //computer played second and tied
  }
  else if(offense==2) {losses++;return 0; }//computer played first and lost
       else {losses++;return 0;}          //computer played second and lost
}
 
