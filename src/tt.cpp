#include <cstdint>
#include <string>
#include "board.h"
#include "bitboard.h"
#include "tt.h"
#include "search.h"

uint64_t pieceKeys[12][64];
uint64_t castleKeys[16];
uint64_t enpassantKeys[8];
uint64_t sideKeys;




const int ttEntries = 2000000;
ttEntry tt[2000000];




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



uint64_t Index() {
	return (hashKey % ttEntries);
}


void ClearTT() {
	for (int i = 0; i < ttEntries; ++i) {
		tt[i].hashKey = 0ULL;
		tt[i].move = 0;
		tt[i].depth = 0;
		tt[i].flag = NO_FLAG;
		tt[i].eval = 0;
	}
}


void StoreTTEntry(short depth, short flag, int eval, int move, short ply) {

	ttEntry* entry = &tt[Index()];

	if (eval > MATE_FOUND) {
		eval = eval + ply;
	} else if (eval < -MATE_FOUND) {
		eval = eval - ply;
	}

	entry->hashKey = hashKey;
	entry->move = move;
	entry->depth = depth;
	entry->flag = flag;
	entry->eval = eval;

}


int ProbeTT(short depth, short ply, int alpha, int beta) {

	ttEntry* entry = &tt[Index()];

	int eval = entry->eval;

	if (eval > MATE_FOUND) {
		eval = eval - ply;
	} else if (eval < -MATE_FOUND) {
		eval = eval + ply;
	}

	if (entry->hashKey == hashKey && entry->depth >= depth) {
	
		if (entry->flag == UPPERBOUND && eval <= alpha) {
			return eval;
		} else if (entry->flag == LOWERBOUND && eval >= beta) {
			return eval;
		} else if (entry->flag == EXACT) {
			return eval;
		}

	}

	return NO_TT;
}








// ply 0    48999
// ply 1   -49000 + ply = -48999 
// ply 2
// ply 3


// ply 0    48999
// ply 1   -48999 
// ply 2    48997 > mate found ? + ply = 48999
// ply 3   -49000 + ply = -48997 


// ply 0    48997
// ply 1    -48997
// ply 2    48999 > mate found ? - ply = 48997
// ply 3   -49000 + ply = -48997 