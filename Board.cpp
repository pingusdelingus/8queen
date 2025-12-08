#include <iostream>


class Board
{

public:
 
  bool operator>(const Board& other){
    return (fitness > other.fitness);
  }
  bool operator<(const Board& other){
    return (fitness < other.fitness);
  }

  bool operator+(const Board& other){
    return (fitness + other.fitness);
  }
  
  bool operator!=(const Board& other){
    return (fitness != other.fitness);
  }

  bool operator-(const Board & other){
    return (fitness - other.fitness);
  }

  bool operator==(const  Board& other){
    return (fitness == other.fitness );
  }

  int rep[BOARD_SIZE];
  int fitness;
  Board(int* list, int len)
  {
    for (int i = 0; i < BOARD_SIZE && i < len; i++){
      this->rep[i] = list[i];
    }
    //this->fitness = fitnessFunction( this->rep, len);
  }// end of constructor
  //
  //
  
  int getFitness(){
    return this->fitness;
  }

  int* getRep(){
    return this->rep;
  }





private:
}; // end of Board Class
