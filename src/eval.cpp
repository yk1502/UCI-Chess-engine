#include <string>

#include "board.h"
#include "bitboard.h"






int Evaluate() {

    int score = 0;
    
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

    return (side == white) ? score : -score;

}