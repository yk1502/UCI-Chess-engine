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
        ParseFen("3rr1k1/ppp1qppp/2nbbn2/3pp3/P7/8/RPPPPPPP/1NBQKBNR b K - 17 11");
        PrintBoard();

        std::cout << Evaluate() << std::endl;

    } else {
        UciLoop();      
    }
    
}

