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

static uint64_t startTime = 0;
static uint64_t moveTime = 0;



int QSearch(int alpha, int beta) {
    
    int bestScore = Evaluate();

    nodes++;

    if (bestScore > alpha) {
        alpha = bestScore;
    }

    if (bestScore >= beta) {
        return bestScore;
    } 

    MoveList moveList[1];
    MoveGen(moveList, true);

    for (int moveCount = 0; moveCount < moveList->count; ++moveCount) {
        CopyBoard()

        if (!MakeMove(moveList->moves[moveCount])) {
            continue;
        }

        int score = -QSearch(-beta, -alpha);

        TakeBack();

        if (GetTimeMs() - startTime >= moveTime) {
            return 0;
        }

        if (score > bestScore) {
            bestScore = score;

            if (score > alpha) {

                if (score >= beta) {
                    return score;
                } 

                alpha = score;
            }

        }

    }

    return bestScore;
}



int Negamax(int depth, int alpha, int beta, int ply) {

    if (depth == 0) {
        return QSearch(alpha, beta);
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

        TakeBack();

        if (GetTimeMs() - startTime >= moveTime) {
            return 0;
        }

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

    startTime = GetTimeMs();
    moveTime = (timeLeft / 20) + (timeInc / 2);

    int ply = 0;
    int bestMoveCurrIter = 0;
    nodes = 0;

    for (int currDepth = 1; currDepth <= maxDepth; ++currDepth) {
        int score = Negamax(currDepth, alpha, beta, ply);

        // info score cp 2 depth 6 nodes 52805 time 93 pv
        std::cout << "info score cp " << score << " depth " << currDepth << " nodes " << nodes << " time " << GetTimeMs() - startTime << std::endl;

        if (GetTimeMs() - startTime >= moveTime) {
            break;
        }

        bestMoveCurrIter = bestMove;
    }
    

    std::cout << "bestmove ";
    PrintMove(bestMoveCurrIter);
    std::cout << std::endl;
    
}