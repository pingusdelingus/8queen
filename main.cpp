#include <iostream>
#include <cmath>
#include <cstdlib>
#include <random>
#include <chrono>
#include <algorithm>
#include <vector>
#include <functional>
#include <numeric>

const int BOARD_SIZE = 8;
const int POP_SIZE = 10000;
//static int board[BOARD_SIZE];
const int SMART_MUTATE_ONLY = 1;



#include "./Board.cpp"

void smart_mutate(Board* output, Board* mother, Board* father);

void mutate(Board* out, Board* mom, Board* dad, std::mt19937* gen, std::uniform_int_distribution<int>* dis );

void printBoard(int b[], int len)
{
  int i = 0;
  while (i < len - 1){ 
    std::cout << b[i] << ", "; 
    i++;
  }// end of while

  std::cout<< b[i] << std::endl;
}// end of printBoard


// returns a relative fitness
// here, we return how many queens are attacking each other
// if it retusns 0, it is a valid solution
//
// board representation works as follows:
// ex: 3,0,1 means a queen in the first col, 3 rows down, NO queen in second col, and a queen in the 3rd col, first row.



void fillBoardRandomly(int* board, int len, std::mt19937* gen, std::uniform_int_distribution<int>* dis)
{
  auto distribution = *dis;
  std::cout << "setup dist" << std::endl;
  for (int index = 0; index < len; index++){
    board[index] = distribution(*gen);
  }
}// end of fillBoardRandomly 


int fitnessFunction(int* board, int len)
{
  int conflictCount = 0;

  for (int index = 0; index < len; index++){
      int curr = board[index];
    if (curr == 0) {continue;}

      for (int jdex = index + 1; jdex < len; jdex++){

        if (board[jdex] == 0) {continue;}

      int rowDiff = std::abs(board[index] - board[jdex]);
      int colDiff = std::abs(index - jdex);

      if (board[index] == board[jdex]){
        conflictCount++;
      }

      if (rowDiff == colDiff){
        conflictCount++;
      }

    }// end of for (inner )


  }// end of for

  return conflictCount;

}// end of fitness fucnction

typedef int32_t i32;


extern "C" int comparator(const void* a, const void * b){
  const Board* b_a = static_cast<const Board*> (a) ;
  const Board* b_b = static_cast<const Board*> (b );


  if (b_a->fitness < b_b->fitness) return -1;
    if (b_a->fitness > b_b->fitness) return 1;
    return 0;
}

int getValidSolutionIndex(Board* pop, int size, bool startFromFront)
{
  if (startFromFront){
    for (int i = 0; i < size; i++){
      Board curr = pop[i];
      if (curr.fitness == 0){
        return i;
      }
    }// end of for
    return -1;
  }// end of startFromFront
  
  for (int i = size; i > 0; i--){
      Board curr = pop[i];
      if (curr.fitness == 0){
        return i;
      }
  }// end of for 
  return -1;

}


int main(void )
{
  int min_val = 1;
  int max_val = BOARD_SIZE;
  
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  
  std::mt19937 generator(seed);
  std::uniform_int_distribution<int> distribution(min_val, max_val);

  std::cout << "initializing population " << std::endl;
  Board* population = (Board* ) calloc(sizeof(Board ), POP_SIZE);
  

  for (int index = 0; index < POP_SIZE; index++){
    Board& curr = population[index];
    std::cout << "grabbed curr " << std::endl;

    // this function call is seg faulting 
    fillBoardRandomly( curr.rep , BOARD_SIZE, &generator, &distribution);
  //  printBoard(curr.rep, BOARD_SIZE);
    curr.fitness = fitnessFunction(curr.rep, BOARD_SIZE);
  }// end of for 
  
  std::cout << "finished initializing" << std::endl;

  // GL (2, R ); 
  // SL (2, R );
  // SL <= GL 

  // we sort our population from smallest to largest
  qsort(population ,POP_SIZE,  sizeof(population[0]),  comparator ) ; // this isn't working as intended



  std::cout << "finished sorting  \n";  

  for (int index = 0; index < POP_SIZE; index++){
      if (population[index].fitness == 0){
      std::cout << "fintess is zero ?? skipping.. \n";
      continue;
    }
      printBoard(population[index].rep , BOARD_SIZE);
      std::cout <<  ", fitness: " << population[index].getFitness() << "\n";
  }// end of for 


  int validIndex = getValidSolutionIndex(population, POP_SIZE, false);

  if (validIndex == -1 ){
    std::cout << "No Solution found \n";
  }

  std::cout << "Smallest (best) fitness is : " << std::endl;

    printBoard(population[POP_SIZE - 1].rep, BOARD_SIZE); 
      std::cout <<  ", fitness: " << population[POP_SIZE -1].getFitness() << "\n";
  std::cout << std::endl;

Board* next_gen = (Board*) calloc(sizeof(Board), POP_SIZE);
    int generations = 100; // Define how long you want to evolve

    for (int g = 0; g < generations; g++) {
        std::qsort(population, POP_SIZE, sizeof(Board), comparator);

        if (population[0].fitness == 0) {
            std::cout << "Solution found in generation " << g << "!" << std::endl;
            printBoard(population[0].rep, BOARD_SIZE);
            break;
        }

        next_gen[0] = population[0];

        for (int i = 1; i < POP_SIZE; i++) {
            int parentA_idx = i % (POP_SIZE / 10); 
            int parentB_idx = (i + 1) % (POP_SIZE / 10);

            if (SMART_MUTATE_ONLY ) {
                smart_mutate(&next_gen[i], &population[parentA_idx], &population[parentB_idx]);
            } else {
                mutate(&next_gen[i], &population[parentA_idx], &population[parentB_idx], &generator, &distribution);
            }
        }

        for (int i = 0; i < POP_SIZE; i++) {
            population[i] = next_gen[i];
        }

        if (g % 10 == 0) std::cout << "Gen " << g << " Best Fitness: " << population[0].fitness << std::endl;
    }

    free(next_gen);


  free(population);
  return 0;
} // end of main


void mutate(Board* out, Board* mom, Board* dad, std::mt19937* gen, std::uniform_int_distribution<int> *dis )
{
    auto dist = *dis;

    int splitIndex =  dist(*gen);

    for(int i = 0; i < BOARD_SIZE; i++){
    if (i <= splitIndex){
      out->switchRepAt(i, mom->rep[i]);
    }else{
      out->switchRepAt(i, dad->rep[i]);
    }
  }
   out->fitness = fitnessFunction(out->rep, BOARD_SIZE); 
}// end of mutate


// try to "mix" mother and father to make a better child
// but using fitness as heuristic for adding, not ramdomness
void smart_mutate(Board* output, Board* mother, Board* father)
{
  // assume output is zero'd output 
  // start with "better" (lower) fitness value 
  if (mother->fitness < father->fitness){

    *output = *mother; // copy (hopefully)
   
   for (int i = 0; i < BOARD_SIZE; i++){
      // try output new val 
    int prevRepEntry = mother->rep[i];
    int prevFitness = output->fitness; // original mom's fitness 
      // try 0th from dad into moms copy inside of output
      
      output->switchRepAt(i, father->rep[i]);
      output->fitness = fitnessFunction(output->rep, BOARD_SIZE);
    
      if (output->fitness > prevFitness){
        //revert and go next
        output->switchRepAt(i, prevRepEntry);
        output->fitness = prevFitness;
      }

    }// end of for loop 
  }else if (father->fitness >= mother->fitness){

      *output = *father;

   for (int i = 0; i < BOARD_SIZE; i++){
      // try output new val 
    int prevRepEntry = father->rep[i];
    int prevFitness = output->fitness; // original dads's fitness 
      // try 0th from dad into moms copy inside of output
      
      output->switchRepAt(i, mother->rep[i]);
      output->fitness = fitnessFunction(output->rep, BOARD_SIZE);
    
      if (output->fitness > prevFitness){
        //revert and go next
        output->switchRepAt(i, prevRepEntry);
        output->fitness = prevFitness;
      }

    }// end of for loop 


  }

}// end of smart_mutate

