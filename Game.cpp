#include "Game.h"

//O(Size)
int Game::check_similar_in_row_col(int rowCol, int* myBoard) {
	int rowItr = ((int)(rowCol / Size)) * Size;
	int colItr = rowCol % Size;

	bool* hashMapR = new bool[Size];
	bool* hashMapC = new bool[Size];
	for (int i = 0; i < Size; i++)
	{
		hashMapR[i] = false;
		hashMapC[i] = false;
	}

	int count = 0;
	for (int i = 0; i < Size; i++) {
		if (hashMapR[myBoard[rowItr] - 1])
			count++;
		else
			hashMapR[myBoard[rowItr] - 1] = true;
		rowItr++;

		if (hashMapC[myBoard[colItr] - 1])
			count++;
		else
			hashMapC[myBoard[colItr] - 1] = true;
		colItr += Size;
	}
	return count;
}
//O(Size)
int Game::check_similar_in_grd(int rowCol, int* myBoard) {
	/*
	* 0  1  2	3  4  5		6  7  8
	* 9  10 11	12 13 14	15 16 17
	* 18 19 20	21 22 23	24 25 26
	* 
	* 27 28 29	30 31 32	33 34 35
	* 36 37 38	39 40 41	42 43 44
	* 45 46 47	48 49 50	51 52 53
	* 
	* 54 55 56	57 58 59	60 61 62
	* 63 64 65	66 67 68	69 70 71
	* 72 73 74	75 76 77	78 79 80
	*/
	
	switch (rowCol)
	{
	case 0:
		rowCol = 0; break;
	case 10:
		rowCol = 3; break;
	case 20:
		rowCol = 6; break;
	case 30:
		rowCol = 27; break;
	case 40:
		rowCol = 30; break;
	case 50:
		rowCol = 33; break;
	case 60:
		rowCol = 54; break;
	case 70:
		rowCol = 57; break;
	case 80:
		rowCol = 60; break;
	default:
		break;
	}

	int eV = rowCol + (Size * 2) + 2;

	bool* hashMap = new bool[Size + 1];
	for (int i = 0; i < Size; i++) hashMap[i] = false;
	int count = 0;
	for (int i = rowCol; i < eV; i+= Size) {
		for (int j = 0; j < 3; j++) {
			if (hashMap[myBoard[i + j]] == true)
				count++;
			else
				hashMap[myBoard[i + j]] = true;
		}
	}
	return count;
}

Game::Game(int* defaultBoard, const int Size)
{
	srand((unsigned int)time(NULL));
	this->Board = new int[Size * Size];
	for (int i = 0; i < Size * Size; i++) {
		this->Board[i] = defaultBoard[i];
	}
	this->Size = Size;
}

//O(Size*Size)
void Game::generate_random_board(int*& myBoard) {
	myBoard = new int[Size * Size];
	for (int i = 0; i < Size * Size; i++) {
		if (Board[i] == 0) {
			myBoard[i] = rand() % 9 + 1;
		}
		else {
			myBoard[i] = Board[i];
		}
	}
}
//O(1)
bool Game::insert_value(int x, int y, int Val) {
	int i = Size * y + x;
	if (Board[i] == 0) {
		Board[i] = Val;
		return true;
	}
	return false;
}
//O(Size*Size)
int Game::count_collisions(int* myBoard) {
	int x = 0;
	/*
	* Loop = O(Size)
	* In each Loop:
	*	O(2.Size)
	* T(n) = Size . (2.Size)
	* O(n) = Size ^ 2
	*/
	for (int i = 0; i < Size * Size; i += Size + 1) {
		x += check_similar_in_row_col(i, myBoard);
		x += check_similar_in_grd(i, myBoard);
	}
	return x;
}
int Game::fitness(int* myBoard)
{
	const int worseScore = 216.0;
	return worseScore - this->count_collisions(myBoard);
}
void Game::print_board() {
	using namespace std;

	for (int i = 0; i < Size; i++) {
		if (i % 9 == 0)
			cout << endl;
		cout << Board[i] << ' ';
	}
}
int* Game::get_board() {
	return Board;
}

void Game::select_best(Child_Node boardDest[], Child_Node const boardSrc[], int total) {
	using namespace std::chrono;
	float c_prob[totalChildren] = {};
	c_prob[0] = boardSrc[0].FScore / total;

	for (int i = 1; i < totalChildren; i++)
	{
		c_prob[i] = (boardSrc[i].FScore / total) + c_prob[i - 1];
	}
	int x = 0;

	for (int i = 0; i < totalChildren; i++) {
		x = 0;
		float randomVal = ((float)rand()) / RAND_MAX;
		//Search
		for (int j = 0; j < totalChildren; j++) {
			if (c_prob[j] > randomVal) {
				boardDest[x].Board = boardSrc[j].Board;
				boardDest[x++].FScore = boardSrc[j].FScore;
				break;
			}
		}
	}
}

void Game::crossover(Child_Node boardSrc[]){
	//Performs uniform crossover for simplicity.
	int k = Size * Size;
	int* temp_board = new int[k];
	for (int i = 0; i < totalChildren - 1; i += 2) {
		float randomVal = ((float)rand()) / RAND_MAX;
		if (randomVal >= 0.5) {
			//Perform crossover from start to mid.
			for (int j = 0; j < k / 2; j++) {
				temp_board[j] = boardSrc[i].Board[j];
				boardSrc[i].Board[j] = boardSrc[i + 1].Board[j];
				boardSrc[i + 1].Board[j] = temp_board[j];
			}
		}
		else {
			//Perform crossover from mid to end.
			for (int j = k / 2; j < k; j++) {
				temp_board[j] = boardSrc[i].Board[j];
				boardSrc[i].Board[j] = boardSrc[i + 1].Board[j];
				boardSrc[i + 1].Board[j] = temp_board[j];
			}
		}
		boardSrc[i].FScore = this->fitness(boardSrc[i].Board);
		boardSrc[i + 1].FScore = this->fitness(boardSrc[i + 1].Board);
	}
	delete[] temp_board;
	temp_board = nullptr;
}

void Game::mutation(Child_Node boardDest[], float mutationRate) {
	for (int i = 0; i < totalChildren; i++) {
		float x = ((float)rand()) / RAND_MAX;
		if (x < mutationRate) {
			int randVal = rand() % (Size * Size);
			while (this->Board[randVal] != 0) {
				randVal = rand() % (Size * Size);
			}
			boardDest[i].Board[randVal] = (rand() % Size) + 1;
		}
		boardDest[i].FScore = this->fitness(boardDest[i].Board);
	}
}

Game::~Game() {
	delete[] Board;
	Board = nullptr;
	Size = 0;
}

/*
* Total Time for each random generation of Board and Check:
*	O(2 . Size^2) := O(Size^2)
*/