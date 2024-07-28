#pragma once


#include <string>
#include "bitboard.h"
#include "attacks.h"
#include "board.h"


static inline void PrintBitboard(uint64_t bitboard) {
    std::cout << std::endl;
    std::cout << std::endl;

    for (int rank = 0; rank < 8; ++rank) {
        printf("    %d     ", 8 - rank);
        for (int file = 0; file < 8; ++file) {
            int square = rank * 8 + file;
            printf("%d  ", GetBit(bitboard, square));
            
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << "          a  b  c  d  e  f  g  h" << std::endl;
    std::cout << std::endl;
}


static inline void PrintBoard() {
    std::cout << std::endl;
    std::cout << std::endl;

    // print board position
    for (int rank = 0; rank < 8; ++rank) {
        printf("    %d     ", 8 - rank);
        for (int file = 0; file < 8; ++file) {
            int square = rank * 8 + file;
            char piece = '.';
            for (int i = 0; i < 12; ++i) {
                if (GetBit(bitboards[i], square)) {
                    piece = Pieces::numToPiece[i];
                    break;
                } 
            }
            std::cout << piece << "  ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << "          a  b  c  d  e  f  g  h" << std::endl;
    std::cout << std::endl;

    // print board state
    std::string sideStr = (side == white) ? "white" : "black";
    std::cout << "          side : " << sideStr << std::endl;
    std::cout << "          enpassantSquare : " << Squares::numToSquare[enpassantSquare] << std::endl;
    std::cout << "          castling : " << std::bitset<4>(castling) << std::endl;
    std::cout << "          hashKey : " << hashKey << std::endl;
    
}


static inline uint64_t GetTimeMs() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch()
    ).count();
}


static inline void PrintMove(int move) {
    int sourceSquare = GetMoveSource(move);
    int targetSquare = GetMoveTarget(move);
    int isPromotion = GetMovePromotion(move);

    std::string promotion = "";

    if (isPromotion == 1 || isPromotion == 7) {promotion = "n";}
    else if (isPromotion == 2 || isPromotion == 8) {promotion = "b";}
    else if (isPromotion == 3 || isPromotion == 9) {promotion = "r";}
    else if (isPromotion == 4 || isPromotion == 4) {promotion = "q";}
    

    std::cout << Squares::numToSquare[sourceSquare] << Squares::numToSquare[targetSquare] << promotion;
}