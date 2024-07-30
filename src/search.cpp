#include <cstdint>
#include <string>
#include <unistd.h>
#include <iostream>
#include "move.h"
#include "board.h"
#include "bitboard.h"
#include "utils.h"
#include "makemove.h"
#include "search.h"
#include "eval.h"
#include "move.h"



static int nodes = 0;

bool stopEarly = false;

static uint64_t startTime = 0;
static uint64_t moveTime = 0;

int pvTable[64][64];
int pvLength[64];

int killerMoves[64][2];


static int MVVLVA[12][12] = {
 	{105, 205, 305, 405, 505, 605,  105, 205, 305, 405, 505, 605},
	{104, 204, 304, 404, 504, 604,  104, 204, 304, 404, 504, 604},
	{103, 203, 303, 403, 503, 603,  103, 203, 303, 403, 503, 603},
	{102, 202, 302, 402, 502, 602,  102, 202, 302, 402, 502, 602},
	{101, 201, 301, 401, 501, 601,  101, 201, 301, 401, 501, 601},
	{100, 200, 300, 400, 500, 600,  100, 200, 300, 400, 500, 600},

	{105, 205, 305, 405, 505, 605,  105, 205, 305, 405, 505, 605},
	{104, 204, 304, 404, 504, 604,  104, 204, 304, 404, 504, 604},
	{103, 203, 303, 403, 503, 603,  103, 203, 303, 403, 503, 603},
	{102, 202, 302, 402, 502, 602,  102, 202, 302, 402, 502, 602},
	{101, 201, 301, 401, 501, 601,  101, 201, 301, 401, 501, 601},
	{100, 200, 300, 400, 500, 600,  100, 200, 300, 400, 500, 600}
};




static inline bool IsRepetition() {
    for (int index = 0; index < repIndex; ++index) {
        if (hashKey == repHistory[index]) {
            return true;
        }
    }
    return false;
}



static inline int ScoreMoves(int move, int ply) {

    int startPiece, endPiece;

    if (GetMoveCapture(move)) {

        if (side == black) {
            startPiece = 0;
            endPiece = 5;
        } else {
            startPiece = 6;
            endPiece = 11;
        }

        int sourcePiece = GetMovePiece(move);
        int targetSq = GetMoveTarget(move);
        int targetPiece = Pieces::P;
        
        if (!GetMoveEnpassant(move)) {
            for (int count = startPiece; count <= endPiece; count++) {
                if (GetBit(bitboards[count], targetSq)) {
                    targetPiece = count;
                    break;
                }
            }
        }

        return MVVLVA[sourcePiece][targetPiece] + 10000;
    } else {
        if (killerMoves[ply][0] == move) {
            //return 0;
            return 9000;
        } else if (killerMoves[ply][1] == move) {
            //return 0;
            return 8000;
        } 
        return 0;   
    }
}



static inline void SortMoves(MoveList* moveList, int ply) {
    int scores[moveList->count];

    for (int count = 0; count < moveList->count; count++) {
        scores[count] = ScoreMoves(moveList->moves[count], ply);
    }

    for (int currMove = 0; currMove < moveList->count; ++currMove) {
        for (int nextMove = currMove + 1; nextMove < moveList->count; ++nextMove) {
            if (scores[currMove] < scores[nextMove]) {
                int tempMove = moveList->moves[currMove];
                moveList->moves[currMove] = moveList->moves[nextMove];
                moveList->moves[nextMove] = tempMove;

                int tempScore = scores[currMove];
                scores[currMove] = scores[nextMove];
                scores[nextMove] = tempScore;
            }
        }
    }
}



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
    SortMoves(moveList, -1);

    for (int moveCount = 0; moveCount < moveList->count; ++moveCount) {
        CopyBoard()

        if (!MakeMove(moveList->moves[moveCount])) {
            continue;
        }

        int score = -QSearch(-beta, -alpha);

        TakeBack();
        repIndex--;

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

    pvLength[ply] = ply;

    if (depth == 0) {
        return QSearch(alpha, beta);
    }

    if (IsRepetition() && ply) {
        return 0;
    }

    nodes++;

    bool isInCheck = IsSquareAttacked(GetLsbIndex(bitboards[(side == white) ? Pieces::K : Pieces::k]), side ^ 1);

    if (!isInCheck && ply) {
        int eval = Evaluate();
        if ((eval - 90 * depth >= beta) && (depth <= 7)) {
            return eval;
        }
    }

    if (isInCheck) {
        depth++;
    }

    MoveList moveList[1];
    MoveGen(moveList);
    SortMoves(moveList, ply);
    
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
        repIndex--;

        if (GetTimeMs() - startTime >= moveTime) {
            stopEarly = true;
            return 0;
        }

        if (score > bestScore) {
            bestScore = score;

            if (score > alpha) {

                alpha = score;

                pvTable[ply][ply] = moveList->moves[moveCount];
                for (int nextPly = ply + 1; nextPly < pvLength[ply + 1]; nextPly++) {
                    pvTable[ply][nextPly] = pvTable[ply + 1][nextPly];
                }
                pvLength[ply] = pvLength[ply + 1];

                if (score >= beta) {
                    if (!GetMoveCapture(moveList->moves[moveCount])) {
                        killerMoves[ply][1] = killerMoves[ply][0];
                        killerMoves[ply][0] = moveList->moves[moveCount];
                    }
                    return score;
                } 
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

    std::memset(pvLength, 0, sizeof(pvLength));
    std::memset(pvTable, 0, sizeof(pvTable));
    std::memset(killerMoves, 0, sizeof(killerMoves));

    int alpha = -MAX_SCORE;
    int beta = MAX_SCORE;

    startTime = GetTimeMs();
    moveTime = (timeLeft / 20) + (timeInc / 2);

    int ply = 0;
    int bestMoveCurrIter = 0;
    int score = 0;
    int delta = 40;
    bool doAspiration = true;

    stopEarly = false;
    nodes = 0;

    for (int currDepth = 1; currDepth <= maxDepth; ++currDepth) {

        if (currDepth >= 3 && doAspiration) {
            alpha = score - delta;
            beta = score + delta;
        }

        score = Negamax(currDepth, alpha, beta, ply);

        if ((score <= alpha) || (score >= beta)) {
            alpha = -MAX_SCORE;
            beta = MAX_SCORE;
            currDepth--;
            doAspiration = false;
            continue;
        }

        doAspiration = true;

        if (!stopEarly) {
            bestMoveCurrIter = pvTable[0][0];
        }

        // info score cp 2 depth 6 nodes 52805 time 93 pv
        std::cout << "info score cp " << score << " depth " << currDepth << " nodes " << nodes << " time " << GetTimeMs() - startTime << " pv ";

        for (int count = 0; count < pvLength[0]; ++count) {
            PrintMove(pvTable[0][count]);
             std::cout << " ";
        }

        std::cout << std::endl;
        
        if (GetTimeMs() - startTime >= moveTime) {
            break;
        }
        
    }
    

    std::cout << "bestmove ";
    PrintMove(bestMoveCurrIter);
    std::cout << std::endl;
    stopEarly = false;
    
}