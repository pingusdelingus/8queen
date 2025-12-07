#include <iostream>
#include <cmath>
#include <cstdlib>
#include <random>
#include <chrono>
#include <algorithm>
#include <vector>
#include <functional>


const int BOARD_SIZE = 8;
const int POP_SIZE = 10000;
//static int board[BOARD_SIZE];

#include "./Board.cpp"


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

int main(void )
{
  int min_val = 1;
  int max_val = BOARD_SIZE;
  
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  
  std::mt19937 generator(seed);
  std::uniform_int_distribution<int> distribution(min_val, max_val);
  




  std::cout << "initializing population " << std::endl;
  Board* population = (Board* ) malloc(sizeof(Board ) * POP_SIZE);


  for (int index = 0; index < POP_SIZE; index++){
    Board curr = population[index];
    std::cout << "grabbed curr " << std::endl;

    // this function call is seg faulting 
    fillBoardRandomly( (int*) curr.rep , BOARD_SIZE, &generator, &distribution);
    printBoard(curr.rep, BOARD_SIZE);
    curr.fitness = fitnessFunction(curr.rep, BOARD_SIZE);
  }// end of for 
  
  std::cout << "finished initializing" << std::endl;

  //test to see it work
 
 
  // SL <= GL 
  // GL (2, R ); 
  // SL (2, R );

  // we sort our population from smallest to largest
    std::sort(population[0], population[POP_SIZE], [](const Board& a, const Board& b){return a.getFitness() < b.getFitness();});
 
    
  for (int index = 0; index < POP_SIZE; index++){
        std::cout << population[index] << ", fitness: " << population[index].getFitness() << "\n";
  }// end of for 
  

  free(population);
  return 0;

} // end of main
