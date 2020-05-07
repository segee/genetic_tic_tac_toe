#include "ttt_helpers.h"
#include "evaluate_fitness.h"
#define POPSIZE 1000 
#define INDSIZE 10000


void crossover(void * mom, void * dad, int slot);
void *roulette(void);
void mutate(void *);


#include <stdio.h>
#include <string.h>
//global variables might just be lazy programming
char population[POPSIZE][INDSIZE];
char  kids[POPSIZE][INDSIZE];
double total_fitness, fitness[POPSIZE];

void * roulette();
int my_rand(void *, int);
int read_individual_from_file(char *, char *);
int write_individual_to_file(char *, char *);
void make_next_generation(void);
int generation;
main()
{ int x;
  int y;
  double max_fitness;
  max_fitness=0;
  my_rand(population,sizeof(population));
  printf("A\n");
  for(y=0;y<20000;y++) //generations
  {   generation=y;
      total_fitness=0;
      for(x=0;x<POPSIZE;x++)
      { //printf("x is %d\n",x);
        total_fitness+=fitness[x]=evaluate_fitness(population[x]);
//
//printf("Total fitness %lf\n",total_fitness);
        
	if(y>601) if(fitness[x]>max_fitness) write_individual_to_file("brucebest.txt",population[x]),max_fitness=fitness[x];
      }
      //printf("B\n");
     printf("generation %d average fitness %lf max fitness so far %lf\n",y,total_fitness/POPSIZE,max_fitness);
     make_next_generation();
     if((y>500) && ((y%20)==0))read_individual_from_file("brucebest.txt",population[0]);
//     max_fitness=0;
  }
  for(x=0;x<POPSIZE;x++) 
  {
//	  printf("Individual %d:\n",x);
//	  for(y=0;y<9;y++) printf("%d\n",population[x][y]);
  }
 }

void * roulette(void)
{  int x;
   double fitness_so_far;
   unsigned int random;
   my_rand(&random,sizeof(int));

   random %= (int)total_fitness;
   //printf("random is %d\n",random);
   fitness_so_far=0;
   for(x=0;x<POPSIZE;x++)
   {       //printf("total_fitness %lf fitness so far%d fitness[%d]%lf\n",total_fitness,fitness_so_far,x,fitness[x]);
	   fitness_so_far+=fitness[x];
	   if(fitness_so_far >random) break;
   }
   if(x==POPSIZE) x--,printf("Crisis averted \n");
   return population[x];
}
void make_next_generation()
{
	int x,y;
	void *mom,*dad;

	for(x=0;x<(POPSIZE/2);x++)
	{  //printf("in make next generation x is %d \n",x);
	   mom=roulette();
	   dad=roulette();
	   crossover(mom,dad,x<<1);
	}
	for(x=0;x<POPSIZE;x++) for(y=0;y<INDSIZE;y++) population[x][y]=kids[x][y];
}
void crossover(void * mom, void * dad, int slot)
{  unsigned int random;
   void * k1 = kids[slot];
   void * k2 = kids[slot+1];

   my_rand(&random,sizeof(unsigned int));
  //printf("random is %u\n",random);
   random %=INDSIZE;
  // printf("random is %u\n",random);

   //printf("random is %u mom %p dad %p \n\n",random,mom,dad);
   memcpy(k1,mom,random);
   memcpy(k2,dad,random);
   memcpy(k1+random,dad+random,INDSIZE-random);
   memcpy(k2+random,mom+random,INDSIZE-random);
   mutate(kids[slot]);
   mutate(kids[slot+1]);

   //printf("random is %u mom %p dad %p \n\n",random,mom,dad);
//for(int x=0;x<9;x++) printf(" %x %x %x %x \n",((int *)mom)[x],((int *)dad)[x],((int *)k1)[x],((int *) k2)[x]);
// if(random==0)  printf("Slot is %d \n",slot);
}
void mutate(void * gene)
{
    char * bruce;
    unsigned int random_byte;
    unsigned int random_bit;
    unsigned int zap;
    bruce=gene;
    my_rand(&random_byte,sizeof(unsigned int));
    my_rand(&random_bit,sizeof(unsigned int));
    my_rand(&zap,sizeof(unsigned int));
    random_byte %= INDSIZE;
    random_bit %= 8;
    zap %= 1000;
    //printf("zap %x random_byte %u random_bit %u\n",zap,random_byte,random_bit);
    if(zap<1500) 
    { bruce[random_byte] ^= 1<<random_bit;
//	    printf("MUTATE\n\n\n");
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

int read_individual_from_file(char * fname,char * individual)
{
	FILE *fp;
	int x;
	int ylow,yhigh;

	fp=fopen(fname,"r");
	if(fp==NULL) return 0;
	for(x=0;x<INDSIZE;x++)
	{
	        fscanf(fp,"%i\n%i",&ylow,&yhigh);
		individual[x]=(yhigh<<4) + ylow;
		//printf("x %d individual %x \n",x,individual[x]);
	}
	fclose(fp);
	return 1;
}
int write_individual_to_file(char * fname, char * individual)
{
	FILE *fp;
	int x;

	fp=fopen(fname,"w");
	if(fp==NULL) return 0;
	for(x=0;x<INDSIZE;x++) fprintf(fp," %d\n%d\n",individual[x]&0xf,(individual[x]>>4)&0xf);
	fclose(fp);
	return 1;
}


int get_computer_pick(uint32_t board, int player,void * individual)
{
        int state,x;
        int count;
        char *hash;
	int temp=0;
        hash=individual;
	

	state=0;
	count=how_many_moves(board);
	//print_board(board);
	for(x=8;x>=0;x--)
	{       state *=3;
	        state += get_square(board,x)-1; 	
		//printf("state is %d\n",state);
	}
	//printf("State is %d count is %d\n",state,count);
	temp=hash[state/2];
	if(state&1) temp = (temp>>4) & 0xf;
	else temp &=0xf;
	//printf("temp is %d \n",temp);
	count=temp%how_many_moves(board);
	if(count<0)count+=how_many_moves(board);
	//printf("Count is %d\n",count);
        for(x=0;x<9;x++)
	{
		if(get_square(board,x)==1)
			if(count==0) break;
			else count--;
	}
	//printf ("computer picks square %d\n",x);
	return x;

}



