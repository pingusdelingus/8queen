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
static int board[BOARD_SIZE];



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

void fillBoardRandomly(int* board, int len, std::mt19937* gen, std::uniform_int_distribution<int>* dis)
{
  auto distribution = *dis;

  for (int index = 0; index < len; index++){
    board[index] = distribution(*gen);
  }
}// end of fillBoardRandomly 


std::function<bool(const int*, const int*)> compare_fitness=
         [](const int* a, const int* b){
    return a[9] > b[9];
  };


int main(void )
{
  int min_val = 0;
  int max_val = BOARD_SIZE;
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::mt19937 generator(seed);
  std::uniform_int_distribution<int> distribution(min_val, max_val);
  
  std::uniform_real_distribution<float> u_0_1(0.0f, 1.0f);


 //testing 

//  printBoard(board);
  //std::cout << fitnessFunction(board) << std:: endl;


 std::cout << "filling in board randomly" << std::endl;


//  board = printBoard(board);

//  std::cout << fitnessFunction(board) << std:: endl;

int population[POP_SIZE][BOARD_SIZE + 1];

  std::cout << "initializing population " << std::endl;

  for (int index = 0; index < POP_SIZE; index++){
      
    int* curr = &population[index][0];
    fillBoardRandomly(curr, BOARD_SIZE, &generator, &distribution);
    curr[BOARD_SIZE + 1] = fitnessFunction(curr, BOARD_SIZE);
  }

  for (int i = 0; i < POP_SIZE; i++){
    std::cout << "\n";
    printBoard(&population[i][0], BOARD_SIZE);
    std::cout << "board fitness is : " << population[i][9] << std::endl;
  }


  // we sort our population from smallest to largest
  std::sort(population[0][0], population[POP_SIZE][0], &compare_fitness);
  

  for (int i = 0; i < POP_SIZE; i++){
    std::cout << "\n";
    printBoard(&population[i][0], BOARD_SIZE);
    std::cout << "board fitness is : " << population[i][9] << std::endl;
  }


//  return 0;
}// end of main
