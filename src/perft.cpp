#include <algorithm>
#include <string>
#include <iostream>
#include <chrono>

#include "board.h"
#include "move.h"
#include "utils.h"


void MoveGen(MoveList* moveList);

bool MakeMove(int move);

int nodes = 0;

int cumNodes = 0;

void Perft(int depth) {

    if (depth == 0) {
        nodes++;
        return;
    }

    MoveList moveList[1];
    MoveGen(moveList);

    for (int moveCount = 0; moveCount < moveList -> count; ++moveCount) {
        CopyBoard() 

        if (!MakeMove(moveList->moves[moveCount])) {
            continue;
        }

        Perft(depth - 1);

        TakeBack();
    }
}


void RunPerft(int maxDepth) {
    
    for (int currDepth = 1; currDepth <= maxDepth; ++currDepth) {
        nodes = 0;
        uint64_t startTime = GetTimeMs();
        Perft(currDepth);
        uint64_t duration = GetTimeMs() - startTime;

        std::cout << "depth : " << currDepth << " nodes : " << nodes << " time : " << duration << " nps : " << (static_cast<float>(nodes) / (static_cast<float>(duration) / 1000.0f)) / 1000000.0f << "Mnps" << std::endl;
    }


}


void PerftMoveCount(int depth, int ply = 0) {

    if (depth == 0) {
        nodes++;
        return;
    }

    MoveList moveList[1];
    MoveGen(moveList);

    for (int moveCount = 0; moveCount < moveList -> count; ++moveCount) {
        CopyBoard() 

        if (!MakeMove(moveList->moves[moveCount])) {
            continue;
        }

        PerftMoveCount(depth - 1, ply + 1);

        TakeBack();

        if (ply == 0) {

            std::cout << "move : ";
            PrintMove(moveList->moves[moveCount]);
            std::cout << "     nodes : " << nodes - cumNodes << std::endl;

            cumNodes = nodes;
        }
    }

}