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

        int arrayTest[2][2] = {{1, 41}, {43, 12313213}};

        std::memset(arrayTest, 0, sizeof(arrayTest));

        std::cout << arrayTest[0][0] << arrayTest[1][0] << std::endl;
        std::cout << arrayTest[0][1] << arrayTest[1][1] << std::endl;
        //PerftPositions();

    } else {
        UciLoop();      
    }
    
}

