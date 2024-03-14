#pragma once
#include <iostream>
#include <time.h>
#include <omp.h>
#include <chrono>
#include "Child.h"

class Game
{
private:
	int* Board;
	int Size;

	int check_similar_in_row_col(int rowCol, int* myBoard);
	int check_similar_in_grd(int rowCol, int* myBoard);

public:
	Game(int* defaultBoard, const int Size);
	
	void generate_random_board(int*& myBoard);
	bool insert_value(int x, int y, int Val);
	int count_collisions(int* myBoard);
	int fitness(int* myBoard);
	void print_board();
	int* get_board();

	void select_best(Child_Node boardDest[], Child_Node const boardSrc[], int total);

	void crossover(Child_Node board[]);

	void mutation(Child_Node boardDest[], float mutationRate);

	~Game();
};