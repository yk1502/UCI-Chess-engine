#include <string>
#include <sstream>
#include <iostream>


#include "board.h"
#include "bitboard.h"





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
            castling = castling | (1 << 3);
        } else if (c == 'Q') {
            castling = castling | (1 << 2);
        } else if (c == 'k') {
            castling = castling | (1 << 1);
        } else if (c == 'q') {
            castling = castling | (1);
        }
    }

    // enpassant square
    if (fenSplit[3] != "-") {
        
        int file = fenSplit[3][0] - 'a';
        int rank = 8 - (fenSplit[3][1] - '0');
        
        int square = rank * 8 + file;
        enpassantSquare = square;
    }
    
}