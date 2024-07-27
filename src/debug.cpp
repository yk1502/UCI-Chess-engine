#include <iostream>
#include <string>
#include <bitset>
#include "bitboard.h"
#include "board.h"


void PrintBitboard(uint64_t bitboard) {
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


void PrintBoard() {
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


