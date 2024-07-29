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
static int nodes = 0;


int Negamax(int depth, int alpha, int beta, int ply) {

    if (depth == 0) {
        return Evaluate();
    }

    nodes++;

    bool isInCheck = IsSquareAttacked(GetLsbIndex(bitboards[(side == white) ? Pieces::K : Pieces::k]), side ^ 1);

    MoveList moveList[1];
    MoveGen(moveList);
    
    int bestScore = -MAX_SCORE;
    int totalMoves = 0;

    for (int moveCount = 0; moveCount < moveList->count; ++moveCount) {
        CopyBoard()

        if (!MakeMove(moveList->moves[moveCount])) {
            continue;
        }

        totalMoves++;

        int score = -Negamax(depth - 1, -beta, -alpha, ply + 1);

        if (score > bestScore) {
            bestScore = score;

            if (ply == 0) {
                bestMove = moveList->moves[moveCount];
            }

            if (score > alpha) {

                if (score >= beta) {
                    return score;
                } 

                alpha = score;

            }

        }

        TakeBack();

    }

    if (totalMoves == 0) {
        if (isInCheck) {return -MATE_SCORE + ply;}
        if (!isInCheck) {return 0;}
    }

    return bestScore;
}




void SearchPosition(int maxDepth, int timeLeft, int timeInc) {

    int alpha = -MAX_SCORE;
    int beta = MAX_SCORE;

    int ply = 0;
    nodes = 0;

    int score = Negamax(3, alpha, beta, ply);

    // info score cp 2 depth 6 nodes 52805 time 93 pv
    std::cout << "info score cp " << score << "depth 3 nodes " << nodes << std::endl;

    std::cout << "bestmove ";
    PrintMove(bestMove);
    std::cout << std::endl;
    
}