#include <string>

#include "board.h"
#include "bitboard.h"



const int pawnScore[64] = 
{
    90,  90,  90,  90,  90,  90,  90,  90,
    30,  30,  30,  40,  40,  30,  30,  30,
    20,  20,  20,  30,  30,  30,  20,  20,
    10,  10,  10,  20,  20,  10,  10,  10,
     5,   5,  10,  20,  20,   5,   5,   5,
     0,   0,   0,   5,   5,   0,   0,   0,
     0,   0,   0, -10, -10,   25,   25,   20,
     0,   0,   0,   0,   0,   0,   0,   0
};

// knight positional score
const int knightScore[64] = 
{
    -5,   0,   0,   0,   0,   0,   0,  -5,
    -5,   0,   0,  10,  10,   0,   0,  -5,
    -5,   5,  20,  20,  20,  20,   5,  -5,
    -5,  10,  20,  30,  30,  20,  10,  -5,
    -5,  10,  20,  30,  30,  20,  10,  -5,
    -5,   5,  20,  10,  10,  20,   5,  -5,
    -5,   0,   0,   0,   0,   0,   0,  -5,
    -5, -10,   0,   0,   0,   0, -10,  -5
};

// bishop positional score
const int bishopScore[64] = 
{
     0,   0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,  10,  10,   0,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,  10,   0,   0,   0,   0,  10,   0,
     0,  30,   0,   0,   0,   0,  30,   0,
     0,   0, -10,   0,   0, -10,   0,   0

};

// rook positional score
const int rookScore[64] =
{
    50,  50,  50,  50,  50,  50,  50,  50,
    50,  50,  50,  50,  50,  50,  50,  50,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,   0,  20,  20,   0,   0,   0

};

// king positional score
const int kingScore[64] = 
{
     0,   0,   0,   0,   0,   0,   0,   0,
     0,   0,   5,   5,   5,   5,   0,   0,
     0,   5,   5,  10,  10,   5,   5,   0,
     0,   5,  10,  20,  20,  10,   5,   0,
     0,   5,  10,  20,  20,  10,   5,   0,
     0,   0,   5,  10,  10,   5,   0,   0,
     0,   5,   5,  -5,  -5,   0,   5,   0,
     0,   0,   5,   0, -15,   0,  10,   0
};


int Evaluate() {

    int score = 0, square = 0;
    uint64_t bitboard;
    
    score += GetBitCount(bitboards[Pieces::P]) * 100;
    score += GetBitCount(bitboards[Pieces::N]) * 300;
    score += GetBitCount(bitboards[Pieces::B]) * 350;
    score += GetBitCount(bitboards[Pieces::R]) * 500;
    score += GetBitCount(bitboards[Pieces::Q]) * 900;

    score -= GetBitCount(bitboards[Pieces::p]) * 100;
    score -= GetBitCount(bitboards[Pieces::n]) * 300;
    score -= GetBitCount(bitboards[Pieces::b]) * 350;
    score -= GetBitCount(bitboards[Pieces::r]) * 500;
    score -= GetBitCount(bitboards[Pieces::q]) * 900;

    
    for (int i = 0;i < 12; i++) {
		bitboard = bitboards[i];

		while (bitboard) {
			square = GetLsbIndex(bitboard);

			switch (i) {
				case Pieces::P: score += pawnScore[square]; break;
				case Pieces::N: score += knightScore[square]; break;
				case Pieces::B: score += bishopScore[square]; break;
				case Pieces::R: score += rookScore[square]; break;
				case Pieces::K: score += kingScore[square]; break;

				case Pieces::p: score -= pawnScore[square ^ 56]; break;
				case Pieces::n: score -= knightScore[square ^ 56]; break;
				case Pieces::b: score -= bishopScore[square ^ 56]; break;
				case Pieces::r: score -= rookScore[square ^ 56]; break;
				case Pieces::k: score -= kingScore[square ^ 56]; break;
			}
			PopBit(&bitboard, square);
		}
	}
    
    return (side == white) ? score : -score;

}