#include <cstdint>
#include <string>
#include "board.h"



// board state
uint64_t bitboards[12];
uint64_t occupancies[3];

int side;

int castling = 0;

int enpassantSquare = Squares::noSquare;

uint64_t hashKey = 0ULL;