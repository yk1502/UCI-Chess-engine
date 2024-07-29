#include <cstdint>
#include <string>
#include <unistd.h>
#include "move.h"
#include "board.h"
#include "utils.h"


void MoveGen(MoveList* moveList);

bool MakeMove(int move);






void SearchPosition(int maxDepth, int wtime, int btime, int winc, int binc) {

    MoveList moveList[1];
    MoveGen(moveList);

    for (int i = 0; i < moveList->count; ++i) {
        CopyBoard();

        if (!MakeMove(moveList->moves[i])) {
            continue;
        }

        TakeBack();

        std::cout << "bestmove ";
        PrintMove(moveList->moves[i]);
        std::cout << std::endl;

        break;
    }
}