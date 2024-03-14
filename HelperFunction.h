#pragma once
#include "Child.h"
#include <omp.h>


void _copy_board(int dest[], const int src[], const int size);
void _copy(Child_Node dest[], Child_Node src[]);