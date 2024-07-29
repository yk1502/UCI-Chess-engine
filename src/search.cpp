#include <cstdint>
#include <string>
#include <unistd.h>
#include "move.h"
#include "board.h"
#include "utils.h"
#include "makemove.h"
#include "search.h"
#include "eval.h"



int bestMove = 0;


int Negamax(int depth, int alpha, int beta, int ply) {

    if (depth == 0) {
        return Evaluate();
    }

    MoveList moveList[1];
    MoveGen(moveList);
    
    int bestScore = -MAX_SCORE;

    for (int moveCount = 0; moveCount < moveList->count; ++moveCount) {
        CopyBoard()

        if (!MakeMove(moveList->moves[moveCount])) {
            continue;
        }

        int score = -Negamax(depth - 1, -beta, -alpha, ply + 1);

        if (score > bestScore) {
            bestScore = score;

            if (ply == 0) {
                bestMove = moveList->moves[moveCount];
            }
        }

        TakeBack();

    }

    return bestScore;
}




void SearchPosition(int maxDepth, int timeLeft, int timeInc) {

    int alpha = -MAX_SCORE;
    int beta = MAX_SCORE;

    int ply = 0;

    int score = Negamax(3, alpha, beta, ply);

    std::cout << "bestmove ";
    PrintMove(bestMove);
    std::cout << std::endl;
    
}