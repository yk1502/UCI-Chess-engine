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
#include "tt.h"

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


// pesto

void InitTables();



void InitAll() {
    InitMask();
    InitSliderAttacks();
    InitLeaperAttacks();
    InitPosKeys();
    InitTables();
}



int main() {
    
    InitAll();

    int debug = 0;

    if (debug) {
        ParseFen(startPos);
        
        RunPerft(5);
    } else {
        UciLoop();      
    }
    
}

