#include <cstdint>
#include <iostream>
#include <string>
#include <algorithm>


#include "bitboard.h"
#include "board.h"
#include "move.h"
#include "utils.h"


// init functions
void InitSliderAttacks();
void InitMask();
void InitLeaperAttacks();


// move gen functions
void MoveGen(MoveList* moveList);
bool MakeMove(int move);


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
        
    UciLoop();      

}

