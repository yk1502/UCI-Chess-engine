#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>

#include "board.h"
#include "bitboard.h"
#include "move.h"



void MoveGen(MoveList* moveList);

bool MakeMove(int move);


static inline int pieceToNum(char piece) {
    switch (piece) {
        case 'P' : return 0;
        case 'N' : return 1;
        case 'B' : return 2;
        case 'R' : return 3;
        case 'Q' : return 4;
        case 'K' : return 5;

        case 'p' : return 6;
        case 'n' : return 7;
        case 'b' : return 8;
        case 'r' : return 9;
        case 'q' : return 10;
        case 'k' : return 11;
    }

    return -1;
}


void ParseFen(std::string fen) {

    std::fill(std::begin(bitboards), std::end(bitboards), 0ULL);
    std::fill(std::begin(occupancies), std::end(occupancies), 0ULL);
    std::fill(std::begin(mailbox), std::end(mailbox), -1);
    side = 0;
    castling = 0;
    enpassantSquare = Squares::noSquare;
    hashKey = 0ULL;    
    

    std::stringstream ss(fen);
    std::string fenSplit[6];

    for (int i = 0; i < 6; ++i) {
        ss >> fenSplit[i];
    }

    // set up position
    int square = -1;
    for (char p : fenSplit[0]) {
        int piece = pieceToNum(p);
        if (piece != -1) {
            ++square;
            mailbox[square] = piece;
            SetBit(&bitboards[piece], square);
        } else {
            if (p != '/') {
                int skip = p - '0';
                square += skip;
            } 
        }
    }

    // set up board state (side to move)
    if (fenSplit[1] == "w") {
        side = white;
    } else {
        side = black;
    }

    // castling rights
    for (char c : fenSplit[2]) {
        if (c == 'K') {
            castling = castling | (1);
        } else if (c == 'Q') {
            castling = castling | (1 << 1);
        } else if (c == 'k') {
            castling = castling | (1 << 2);
        } else if (c == 'q') {
            castling = castling | (1 << 3);
        }
    }

    // enpassant square
    if (fenSplit[3] != "-") {
        
        int file = fenSplit[3][0] - 'a';
        int rank = 8 - (fenSplit[3][1] - '0');
        
        int square = rank * 8 + file;
        enpassantSquare = square;
    }

    for (int i = 0; i < 6; ++i) {
        occupancies[white] |= bitboards[i];
        occupancies[black] |= bitboards[i + 6];
    }

    occupancies[both] = occupancies[white] | occupancies[black];
    
}


int ParseMove(const char* moveInStr) {
    int sourceSquare = (moveInStr[0] - 'a') + 8 * (8 - (moveInStr[1] - '0')); 
    int targetSquare = (moveInStr[2] - 'a') + 8 * (8 - (moveInStr[3] - '0')); 

    MoveList moveList[1];
    MoveGen(moveList);

    for (int moveCount = 0; moveCount < moveList->count; ++moveCount) {
        int move = moveList->moves[moveCount];

		if (sourceSquare == GetMoveSource(move) && targetSquare == GetMoveTarget(move)) {
			int promotedPiece = GetMovePromotion(move);

			if (promotedPiece) {
				if ((promotedPiece == Pieces::Q || promotedPiece == Pieces::q) && moveInStr[4] == 'q')
					return move;
				else if ((promotedPiece == Pieces::R || promotedPiece == Pieces::r) && moveInStr[4] == 'r')
					return move;
				else if ((promotedPiece == Pieces::B || promotedPiece == Pieces::b) && moveInStr[4] == 'b')
					return move;
				else if ((promotedPiece == Pieces::N || promotedPiece == Pieces::n) && moveInStr[4] == 'n')
					return move;

				continue;
			}

			return move;
		}
    }
    return 0;
}