#include <cstdint>
#include <iostream>
#include <random>
#include "attacks.h"
#include "bitboard.h"



uint64_t randomState = 12297829382473034410ULL;

uint64_t GenRandU64Num() {
    // I'm generating random num using this XOR shift cause i don't care lmao
    
    uint64_t x = randomState;
    x ^= x << 13;
    x ^= x >> 7;
    x ^= x << 17;
    randomState = x;
    return x;

}


uint64_t GenBishopMagic(int square) {
    // the maximum bishop attacks is 512, but generating magic num to fit exactly 512 is going to take
    // a long time, so i generate magic number for 1024, well the attacks is still 512, its just that
    // the array is going to have alot of empty attacks, and only 512 is occupied with actual attacks
    // the magic index for bishop is right shift by 54 because that produces max index of 1024, while
    // rook attacks right shift by 50 to get the maximum of 16384 attacks
    uint64_t fullBishopMask = GenBishopMask(square);
    int maxOccupancies = 1 << GetBitCount(fullBishopMask);

    uint64_t attacks[512];
    uint64_t occupancies[512];
    uint64_t magicAttacks[1024];
    bool isFail;

    for (int index = 0; index < maxOccupancies; ++index) {
        occupancies[index] = GetOccupancies(fullBishopMask, index);
        attacks[index] = GenPartialBishopAttacks(square, occupancies[index]);
    }

    while (true) {
        isFail = false;
        std::fill(std::begin(magicAttacks), std::end(magicAttacks), 0ULL);
        uint64_t magicNumber = GenRandU64Num();

        for (int index = 0; index < maxOccupancies; ++index) {
            int magicIndex = (occupancies[index] * magicNumber) >> 54;

            if (magicAttacks[magicIndex] == 0ULL) {
                magicAttacks[magicIndex] = attacks[index];
            } else {
                isFail = true;
                break;
            }
        }

        if (isFail) {
            continue;
        } 

        return magicNumber;
    }
}


uint64_t GenRookMagic(int square) {
    uint64_t fullRookMask = GenRookMask(square);
    int maxOccupancies = 1 << GetBitCount(fullRookMask);

    uint64_t attacks[4096];
    uint64_t occupancies[4069];
    uint64_t magicAttacks[16384];
    bool isFail;

    for (int index = 0; index < maxOccupancies; ++index) {
        occupancies[index] = GetOccupancies(fullRookMask, index);
        attacks[index] = GenPartialRookAttacks(square, occupancies[index]);
    }

    while (true) {
        isFail = false;
        std::fill(std::begin(magicAttacks), std::end(magicAttacks), 0ULL);
        uint64_t magicNumber = GenRandU64Num();

        for (int index = 0; index < maxOccupancies; ++index) {
            int magicIndex = (occupancies[index] * magicNumber) >> 50;

            if (magicAttacks[magicIndex] == 0ULL) {
                magicAttacks[magicIndex] = attacks[index];
            } else {
                isFail = true;
                break;
            }
        }

        if (isFail) {
            continue;
        }

        return magicNumber;
    }
}
