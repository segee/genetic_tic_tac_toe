#include "ttt_helpers.h"
#include "evaluate_fitness.h"


#include <stdio.h>

int my_individual[9];

int my_rand(void *, int);
int read_individual_from_file(char *, int *);
int write_individual_to_file(char *, int *);

main()
{ int x;
  int y;
  double max_fitness,min_fitness,fitness;

  if(read_individual_from_file("brucebest.txt",my_individual)) max_fitness=evaluate_fitness(my_individual);
  else max_fitness=1;
  if(read_individual_from_file("bruceworst.txt",my_individual)) min_fitness=evaluate_fitness(my_individual);
  else min_fitness=1e6;
  for(y=0;y<10000;y++)
  {
	my_rand(my_individual,sizeof(my_individual));
//	for(x=0;x<9;x++) printf("my_individual[%d]=%d\n",x,my_individual[x]);
	fitness=evaluate_fitness(my_individual);
	if(fitness>max_fitness) write_individual_to_file("brucebest.txt",my_individual),max_fitness=fitness;
	if(fitness<min_fitness) write_individual_to_file("bruceworst.txt",my_individual),min_fitness=fitness;
	printf("fitness is %lf max_fitness is %lf min_fitness is %lf\n",fitness, max_fitness, min_fitness);
  }
}

int my_rand(void * wheretoputit, int size)
{
        FILE *fp;
        fp=fopen("/dev/urandom","r");
        fread(wheretoputit,size,1,fp);
        fclose(fp);
	return 1;
}

int read_individual_from_file(char * fname,int * individual)
{
	FILE *fp;
	int x;

	fp=fopen(fname,"r");
	if(fp==NULL) return 0;
	for(x=0;x<9;x++) fscanf(fp,"%i",&individual[x]);
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
        int *hash;
	long int temp=0;
        hash=individual;
	for(x=0;x<9;x++)
	{  //printf("hash[%d] is %d, get_square is %d\n",x,hash[x],get_square(board,x));
		temp +=hash[x]*(long)get_square(board,x);
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



