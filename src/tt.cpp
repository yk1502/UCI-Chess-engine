#include <cstdint>
#include <string>
#include "board.h"
#include "bitboard.h"



uint64_t pieceKeys[12][64];
uint64_t castleKeys[16];
uint64_t enpassantKeys[8];
uint64_t sideKeys;



uint64_t GenRandU64Num();




void InitPosKeys() {
	for (int i = 0; i < 12; i++) {
		for (int sq = 0; sq < 64; sq++) {
			pieceKeys[i][sq] = GenRandU64Num();
		}
	}

	for (int i = 0; i < 16; i++) {
		castleKeys[i] = GenRandU64Num();
	}

	sideKeys = GenRandU64Num();

	for (int f = 0; f < 8; f++) {
		enpassantKeys[f] = GenRandU64Num();
	}
}




uint64_t GeneratePosKey() {
	uint64_t finalkey = 0ULL;
	uint64_t bitboard;

	// for every piece
	for (int i = 0; i < 12; i++) {
		bitboard = bitboards[i];

		while (bitboard) {
			int index = GetLsbIndex(bitboard);
			finalkey ^= pieceKeys[i][index];
			PopBit(&bitboard, index);
		}
	}

	// include side in the key
	if (side == black) {
		finalkey ^= sideKeys;
	}
	// include the ep square in the key
	if (enpassantSquare != Squares::noSquare) {
		finalkey ^= enpassantKeys[enpassantSquare % 8];
	}
	// add to the key the status of the castling permissions
	finalkey ^= castleKeys[castling];
	return finalkey;
}