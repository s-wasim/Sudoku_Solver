#include "HelperFunction.h"
#include <iostream>

void _copy_board(int dest[], const int src[], const int size) {
	int k = size * size;
	for (int i = 0; i < k; i++) {
		dest[i] = src[i];
	}
}

void _copy(Child_Node dest[], Child_Node src[]) {
	for (int i = 0; i < totalChildren; i++) {
		dest[i].Board = src[i].Board;
		dest[i].FScore = src[i].FScore;
	}
}