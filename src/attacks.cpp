#include <cstdint>
#include <algorithm>
#include <string>
#include "bitboard.h"
#include "board.h"
#include "magics.h"


// leaper pieces
uint64_t pawnAttacks[2][64];
uint64_t knightAttacks[64];
uint64_t kingAttacks[64];

// sliding pieces
uint64_t bishopAttacks[64][1024];
uint64_t rookAttacks[64][16384];

// full mask
uint64_t bishopMask[64];
uint64_t rookMask[64];




uint64_t GenPawnAttacks(int square, int colour) {

    uint64_t pawnBitboard = 0ULL;
    SetBit(&pawnBitboard, square);

    bool isOnAFile = pawnBitboard & Files::aFile;
    bool isOnHFile = pawnBitboard & Files::hFile; 

    if ((square > 7) && (square < 56)) {

        uint64_t attackBitboard = 0ULL;

        if (colour == white) {    

            if (!isOnAFile) {
                SetBit(&attackBitboard, square + Directions::upLeft);
            }

            if (!isOnHFile) {
                SetBit(&attackBitboard, square + Directions::upRight);
            }

        }  else {

            if (!isOnAFile) {
                SetBit(&attackBitboard, square + Directions::downLeft);
            }

            if (!isOnHFile) {
                SetBit(&attackBitboard, square + Directions::downRight);
            }

        }
        
        return attackBitboard;
    } 

    return 0ULL;
}


uint64_t GenKnightAttacks(int square) {

    uint64_t knightBitboard = 0ULL;
    SetBit(&knightBitboard, square);
    
    int leapSquares[8] = {
        Directions::doubleUp + Directions::left,     // 2 up squares
        Directions::doubleUp + Directions::right,
        Directions::doubleRight + Directions::up,    // 2 right squares
        Directions::doubleRight + Directions::down,
        Directions::doubleDown + Directions::right,  // 2 down squares
        Directions::doubleDown + Directions::left,
        Directions::doubleLeft + Directions::up,     // 2 left squares
        Directions::doubleLeft + Directions::down
    };

    uint64_t attackBitboard = 0ULL;
    
    for (int index = 0; index < 8; ++index) {
        int attackSquare = leapSquares[index] + square;
        if ((attackSquare >= 0) && (attackSquare <= 63)) {
            SetBit(&attackBitboard, attackSquare);
        }
    }

    bool isOnABFile = knightBitboard & Files::abFile;
    bool isOnGHFile = knightBitboard & Files::ghFile;

    if (isOnABFile) {
        attackBitboard &= ~(Files::ghFile);
    } else if (isOnGHFile) {
        attackBitboard &= ~(Files::abFile);
    }

    return attackBitboard;
}


uint64_t GenKingAttacks(int square) {

    uint64_t kingBitboard = 0ULL;
    SetBit(&kingBitboard, square);

    int leapSquares[8] = {
        Directions::up,     
        Directions::down,
        Directions::left,    
        Directions::right,
        Directions::upLeft,  
        Directions::upRight,
        Directions::downLeft,    
        Directions::downRight
    };

    uint64_t attackBitboard = 0ULL;

    for (int index = 0; index < 8; ++index) {
        int attackSquare = leapSquares[index] + square;
        if ((attackSquare >= 0) && (attackSquare <= 63)) {
            SetBit(&attackBitboard, attackSquare);
        }
    }

    bool isOnAFile = kingBitboard & Files::aFile;
    bool isOnHFile = kingBitboard & Files::hFile;

    if (isOnAFile) {
        attackBitboard &= ~(Files::hFile);
    } else if (isOnHFile) {
        attackBitboard &= ~(Files::aFile);
    }

    return attackBitboard;
}


uint64_t GenBishopMask(int square) {

    uint64_t bishopBitboard = 0ULL;
    SetBit(&bishopBitboard, square);

    uint64_t bishopMask = 0ULL;

    int dstFromLeftFile = square % 8;
    int dstFromUpRank = square / 8; 
    int dstFromRightFile = 7 - dstFromLeftFile; 
    int dstFromDownRank = 7 - dstFromUpRank;

    int attackSquare = square;

    // right up diagonal
    int minRightUpDiag = std::min(dstFromRightFile, dstFromUpRank) - 1;
    for (int index = 0; index < minRightUpDiag; ++index) {
        attackSquare += Directions::upRight;
        SetBit(&bishopMask, attackSquare);
    }

    // left up diagonal
    attackSquare = square;
    int minLeftUpDiag = std::min(dstFromLeftFile, dstFromUpRank) - 1;
     for (int index = 0; index < minLeftUpDiag; ++index) {
        attackSquare += Directions::upLeft;
        SetBit(&bishopMask, attackSquare);
    }

    // right down diagonal
    attackSquare = square;
    int minRightDownDiag = std::min(dstFromRightFile, dstFromDownRank) - 1;
     for (int index = 0; index < minRightDownDiag; ++index) {
        attackSquare += Directions::downRight;
        SetBit(&bishopMask, attackSquare);
    }

    // left down diagonal
    attackSquare = square;
    int minLeftDownDiag = std::min(dstFromLeftFile, dstFromDownRank) - 1;
     for (int index = 0; index < minLeftDownDiag; ++index) {
        attackSquare += Directions::downLeft;
        SetBit(&bishopMask, attackSquare);
    }

    return bishopMask;
}


uint64_t GenRookMask(int square) {

    uint64_t rookBitboard = 0ULL;
    SetBit(&rookBitboard, square);

    uint64_t rookMask = 0ULL;

    int dstFromLeftFile = square % 8;
    int dstFromUpRank = square / 8; 
    int dstFromRightFile = 7 - dstFromLeftFile; 
    int dstFromDownRank = 7 - dstFromUpRank;

    int attackSquare = square;

    // up
    for (int index = 0; index < (dstFromUpRank - 1); ++index) {
        attackSquare += Directions::up;
        SetBit(&rookMask, attackSquare);
    }

    // down
    attackSquare = square;
     for (int index = 0; index < (dstFromDownRank - 1); ++index) {
        attackSquare += Directions::down;
        SetBit(&rookMask, attackSquare);
    }

    // right
    attackSquare = square;
     for (int index = 0; index < (dstFromRightFile - 1); ++index) {
        attackSquare += Directions::right;
        SetBit(&rookMask, attackSquare);
    }

    // left
    attackSquare = square;
     for (int index = 0; index < (dstFromLeftFile - 1); ++index) {
        attackSquare += Directions::left;
        SetBit(&rookMask, attackSquare);
    }

    return rookMask;
}


uint64_t GetOccupancies(uint64_t fullMask, int index) { 
    uint64_t occupancy = 0ULL;
    int bitCount = GetBitCount(fullMask);
    
    // 512 is the max index for bishop   
    // 4096 is the max index for rook
    for (int i = 0; i < bitCount; ++i) {
        int lsbIndex = GetLsbIndex(fullMask);

        PopBit(&fullMask, lsbIndex);

        if ((1ULL << i) & index) {
            SetBit(&occupancy, lsbIndex);
        }

    }

    return occupancy; // its not a full mask now
}


uint64_t GenFullBishopAttacks(int square) {
    // yeah its probably identical to the mask, only difference is the bits now hit
    // the edge of the board
    uint64_t bishopBitboard = 0ULL;
    SetBit(&bishopBitboard, square);

    uint64_t fullBishopAttack = 0ULL;

    int dstFromLeftFile = square % 8;
    int dstFromUpRank = square / 8; 
    int dstFromRightFile = 7 - dstFromLeftFile; 
    int dstFromDownRank = 7 - dstFromUpRank;

    int attackSquare = square;

    // right up diagonal
    int minRightUpDiag = std::min(dstFromRightFile, dstFromUpRank);
    for (int index = 0; index < minRightUpDiag; ++index) {
        attackSquare += Directions::upRight;
        SetBit(&fullBishopAttack, attackSquare);
    }

    // left up diagonal
    attackSquare = square;
    int minLeftUpDiag = std::min(dstFromLeftFile, dstFromUpRank);
     for (int index = 0; index < minLeftUpDiag; ++index) {
        attackSquare += Directions::upLeft;
        SetBit(&fullBishopAttack, attackSquare);
    }

    // right down diagonal
    attackSquare = square;
    int minRightDownDiag = std::min(dstFromRightFile, dstFromDownRank);
     for (int index = 0; index < minRightDownDiag; ++index) {
        attackSquare += Directions::downRight;
        SetBit(&fullBishopAttack, attackSquare);
    }

    // left down diagonal
    attackSquare = square;
    int minLeftDownDiag = std::min(dstFromLeftFile, dstFromDownRank);
     for (int index = 0; index < minLeftDownDiag; ++index) {
        attackSquare += Directions::downLeft;
        SetBit(&fullBishopAttack, attackSquare);
    }

    return fullBishopAttack;
}


uint64_t GenFullRookAttacks(int square) {

    uint64_t rookBitboard = 0ULL;
    SetBit(&rookBitboard, square);

    uint64_t fullRookAttack = 0ULL;

    int dstFromLeftFile = square % 8;
    int dstFromUpRank = square / 8; 
    int dstFromRightFile = 7 - dstFromLeftFile; 
    int dstFromDownRank = 7 - dstFromUpRank;

    int attackSquare = square;

    // up
    for (int index = 0; index < (dstFromUpRank); ++index) {
        attackSquare += Directions::up;
        SetBit(&fullRookAttack, attackSquare);
    }

    // down
    attackSquare = square;
     for (int index = 0; index < (dstFromDownRank); ++index) {
        attackSquare += Directions::down;
        SetBit(&fullRookAttack, attackSquare);
    }

    // right
    attackSquare = square;
     for (int index = 0; index < (dstFromRightFile); ++index) {
        attackSquare += Directions::right;
        SetBit(&fullRookAttack, attackSquare);
    }

    // left
    attackSquare = square;
     for (int index = 0; index < (dstFromLeftFile); ++index) {
        attackSquare += Directions::left;
        SetBit(&fullRookAttack, attackSquare);
    }

    return fullRookAttack;
}


uint64_t GenPartialBishopAttacks(int square, uint64_t blocker) {
    uint64_t fullBishopAttack = GenFullBishopAttacks(square);

    int dstFromLeftFile = square % 8;
    int dstFromUpRank = square / 8; 
    int dstFromRightFile = 7 - dstFromLeftFile; 
    int dstFromDownRank = 7 - dstFromUpRank;

    int blockerSquare = square;

    bool lock = false;

    // right up diagonal
    int minRightUpDiag = std::min(dstFromRightFile, dstFromUpRank);
    for (int index = 0; index < minRightUpDiag; ++index) {
        blockerSquare += Directions::upRight;
        if (GetBit(blocker, blockerSquare) && !lock) {
            lock = true;
            continue;
        }

        if (lock) PopBit(&fullBishopAttack, blockerSquare);
    }

    // left up diagonal
    lock = false;
    blockerSquare = square;
    int minLeftUpDiag = std::min(dstFromLeftFile, dstFromUpRank);
     for (int index = 0; index < minLeftUpDiag; ++index) {
        blockerSquare += Directions::upLeft;
        if (GetBit(blocker, blockerSquare) && !lock) {
            lock = true;
            continue;
        }

        if (lock) PopBit(&fullBishopAttack, blockerSquare);
    }

    // right down diagonal
    lock = false;
    blockerSquare = square;
    int minRightDownDiag = std::min(dstFromRightFile, dstFromDownRank);
     for (int index = 0; index < minRightDownDiag; ++index) {
        blockerSquare += Directions::downRight;
        if (GetBit(blocker, blockerSquare) && !lock) {
            lock = true;
            continue;
        }

        if (lock) PopBit(&fullBishopAttack, blockerSquare);
    }

    // left down diagonal
    lock = false;
    blockerSquare = square;
    int minLeftDownDiag = std::min(dstFromLeftFile, dstFromDownRank);
     for (int index = 0; index < minLeftDownDiag; ++index) {
        blockerSquare += Directions::downLeft;
        if (GetBit(blocker, blockerSquare) && !lock) {
            lock = true;
            continue;
        }

        if (lock) PopBit(&fullBishopAttack, blockerSquare);
    }

    return fullBishopAttack; // its not even a full bishop anymore
}


uint64_t GenPartialRookAttacks(int square, uint64_t blocker) {
    uint64_t fullRookAttack = GenFullRookAttacks(square);

    int dstFromLeftFile = square % 8;
    int dstFromUpRank = square / 8; 
    int dstFromRightFile = 7 - dstFromLeftFile; 
    int dstFromDownRank = 7 - dstFromUpRank;

    int blockerSquare = square;

    bool lock = false;

    // up
    for (int index = 0; index < dstFromUpRank; ++index) {
        blockerSquare += Directions::up;
        if (GetBit(blocker, blockerSquare) && !lock) {
            lock = true;
            continue;
        }

        if (lock) PopBit(&fullRookAttack, blockerSquare);
    }

    // down
    lock = false;
    blockerSquare = square;
     for (int index = 0; index < dstFromDownRank; ++index) {
        blockerSquare += Directions::down;
        if (GetBit(blocker, blockerSquare) && !lock) {
            lock = true;
            continue;
        }

        if (lock) PopBit(&fullRookAttack, blockerSquare);
    }

    // right
    lock = false;
    blockerSquare = square;
     for (int index = 0; index < dstFromRightFile; ++index) {
        blockerSquare += Directions::right;
        if (GetBit(blocker, blockerSquare) && !lock) {
            lock = true;
            continue;
        }

        if (lock) PopBit(&fullRookAttack, blockerSquare);
    }

    // left
    lock = false;
    blockerSquare = square;
     for (int index = 0; index < dstFromLeftFile; ++index) {
        blockerSquare += Directions::left;
        if (GetBit(blocker, blockerSquare) && !lock) {
            lock = true;
            continue;
        }

        if (lock) PopBit(&fullRookAttack, blockerSquare);
    }

    return fullRookAttack; // its not even a full bishop anymore
}


void InitLeaperAttacks() {
    // white pawn
    for (int square = 0; square < 64; ++square) {
        pawnAttacks[white][square] = GenPawnAttacks(square, white);
    }

    // black pawn
    for (int square = 0; square < 64; ++square) {
        pawnAttacks[black][square] = GenPawnAttacks(square, black);
    }

    // knight and king
    for (int square = 0; square < 64; ++square) {
        knightAttacks[square] = GenKnightAttacks(square);
        kingAttacks[square] = GenKingAttacks(square);
    }
}


void InitSliderAttacks() {
    for (int square = 0; square < 64; ++square) {

        uint64_t fullBishopMask = GenBishopMask(square);
        uint64_t fullRookMask = GenRookMask(square);

        int maxBishopOccupancies = 1 << GetBitCount(fullBishopMask);
        int maxRookOccupancies = 1 << GetBitCount(fullRookMask);

        for (int index = 0; index < maxBishopOccupancies; ++index) {
            uint64_t bishopOccupancy = GetOccupancies(fullBishopMask, index);
            uint64_t bishopAttack = GenPartialBishopAttacks(square, bishopOccupancy);
            int magicIndex = (bishopMagics[square] * bishopOccupancy) >> 54;
            bishopAttacks[square][magicIndex] = bishopAttack;
        }

        for (int index = 0; index < maxRookOccupancies; ++index) {
            uint64_t rookOccupancy = GetOccupancies(fullRookMask, index);
            uint64_t rookAttack = GenPartialRookAttacks(square, rookOccupancy);
            int magicIndex = (rookMagics[square] * rookOccupancy) >> 50;
            rookAttacks[square][magicIndex] = rookAttack;
        }
    }
}


void InitMask() {
    for (int square = 0; square < 64; ++square) {
        bishopMask[square] = GenBishopMask(square);
        rookMask[square] = GenRookMask(square);
    }
}


uint64_t GetPawnAttacks(int square, int colour) {
    return pawnAttacks[colour][square];
}


uint64_t GetKnightAttacks(int square) {
    return knightAttacks[square];
}


uint64_t GetKingAttacks(int square)  {
    return kingAttacks[square];
}


uint64_t GetBishopAttacks(int square, uint64_t occupancy) {
    occupancy &= bishopMask[square];
    int magicIndex = (occupancy * bishopMagics[square]) >> 54;
    return bishopAttacks[square][magicIndex];
}


uint64_t GetRookAttacks(int square, uint64_t occupancy) {
    occupancy &= rookMask[square];
    int magicIndex = (occupancy * rookMagics[square]) >> 50;
    return rookAttacks[square][magicIndex];
}

