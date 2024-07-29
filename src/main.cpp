#include <cstdint>
#include <iostream>
#include <string>
#include <algorithm>


#include "bitboard.h"
#include "board.h"
#include "move.h"
#include "utils.h"
#include "positions.h"
#include "makemove.h"
#include "movegen.h"
#include "eval.h"

// init functions
void InitSliderAttacks();
void InitMask();
void InitLeaperAttacks();



// for perft/debugging
void ParseFen(std::string fen);
int RunPerft(int depth);
void PerftMoveCount(int depth, int ply = 0);
int ParseMove(const char* moveInStr);
void PerftPositions();


// UCI 
void UciLoop();



void InitAll() {
    InitMask();
    InitSliderAttacks();
    InitLeaperAttacks();
}



int main() {
    
    InitAll();

    int debug = 0;

    if (debug) {

        ParseFen("rnb1kbnQ/p1pp4/1p3q1p/8/8/8/PPP1PPPP/RNBQKBNR w KQq - 1 6");
        PrintBoard();
        int score = Evaluate();
        std::cout << score << std::endl;

    } else {
        UciLoop();      
    }
    
}

