#include "Game.h"
#include "HelperFunction.h"
#include <Windows.h>
#include <chrono>
#include <omp.h>


void _copy_board(int dest[], const int src[], const int size);
void _copy(Child_Node dest[], Child_Node src[]);
int main() {
	using namespace std;
	using namespace std::chrono;

	const int Size = 9;
	int defaultBoard[Size * Size]{
		2,4,8,0,7,6,0,0,9,
		1,0,7,2,3,0,0,0,6,
		3,9,6,0,0,5,7,1,0,
		0,6,0,8,2,4,0,9,7,
		7,3,4,0,5,1,2,0,8,
		0,0,9,7,6,0,0,0,0,
		0,0,0,3,4,0,9,8,5,
		0,0,5,0,0,8,0,0,0,
		0,8,0,5,9,0,0,0,1
	};

	Child_Node generationArr[totalChildren];
	Child_Node nextGenerationArr[totalChildren];
	Child_Node O_GenArr[totalChildren];

	Game randObj(defaultBoard, Size);
	/* K = randChildren
	* T(n) = [2 . (Size * Size) + 1] . K
	* O(N) = K . (Size * Size)
	*/
	Child_Node bestChild; bestChild.FScore = 0;
	bestChild.Board = new int[Size * Size];
	float x = 0.0;
	float total = 0.0;
	auto start = high_resolution_clock::now();


	//Random generation
#pragma omp parallel for reduction(+:total) shared(generationArr)
	for (int i = 0; i < totalChildren; i++) {
		randObj.generate_random_board(generationArr[i].Board);
		x = randObj.fitness(generationArr[i].Board);
		if (x > bestChild.FScore) {
			bestChild.Board = generationArr[i].Board;
			bestChild.FScore = x;
		}
		generationArr[i].FScore = x;
		total += x;
	}
	_copy(O_GenArr, generationArr);

	cout << "Randomization done\n";

	omp_set_num_threads(numberThreads);

#pragma omp parallel for shared(nextGenerationArr, generationArr) firstprivate(total)
	for (int generations = 1; generations <= totalGenerations; generations += numberThreads)
	{
		randObj.select_best(nextGenerationArr, generationArr, total);

		//Copy from next gen to curr gen.
		_copy(nextGenerationArr, generationArr);

		//Crossover
		randObj.crossover(generationArr);

		//Mutation
		randObj.mutation(generationArr, 0.1);

		//Select best child.
#pragma omp critical(checkBest)
		{
			total = 0.0;
			for (int i = 0; i < totalChildren; i++) {
				if (generationArr[i].FScore > bestChild.FScore) {
					_copy_board(bestChild.Board, generationArr[i].Board, Size);
					bestChild.FScore = generationArr[i].FScore;
					total += generationArr[i].FScore;
				}
			}
			//cout << "Generation: " << generations << " to " << generations + numberThreads << " - Has score : " << bestChild.FScore << endl;
			//cout << "I AM THREAD " << omp_get_thread_num() << endl;
		}
	}

	auto stop = high_resolution_clock::now();

	auto diff = duration_cast<seconds>(stop - start);
	cout << "Generations evaluated: " << totalGenerations << endl;
	cout << "Time took = " << diff.count() << endl;
	cout << "Best Score achived: " << bestChild.FScore << endl;

	for (int i = 0; i < totalChildren; i++) {
		delete[] O_GenArr[i].Board;
		O_GenArr[i].Board = nullptr;
	}


	return 0;
}