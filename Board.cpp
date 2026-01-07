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

  void switchRepAt(int where, int newVal)
  {
    if ( where >= 0 && where < BOARD_SIZE){
    rep[where] = newVal; 
    }else{
      std::cout << "problem in switchRepAt \n";
    }

  }// end of switchRepAt

  int getFitness(){
    return this->fitness;
  }

  int* getRep(){
    return this->rep;
  }





private:
}; // end of Board Class
