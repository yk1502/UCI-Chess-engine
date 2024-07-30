#include <cstdint>
#include <string>
#include <algorithm>
#include "board.h"
#include "attacks.h"



// board state
uint64_t bitboards[12];

uint64_t occupancies[3];

int mailbox[64];

int side;

int castling = 0;

int enpassantSquare = Squares::noSquare;

uint64_t hashKey = 0ULL;

uint64_t repHistory[1500];

int repIndex = 0;






bool IsSquareAttacked(int square, int side) {

    // pawn
	if (bitboards[Pieces::P + (side * 6)] & GetPawnAttacks(square, side ^ 1)) return true;

	// knight
	if (bitboards[Pieces::N + (side * 6)] & GetKnightAttacks(square)) return true;

	// bishops
	if (bitboards[Pieces::B + (side * 6)] & GetBishopAttacks(square, occupancies[both])) return true;

	// rooks
	if (bitboards[Pieces::R + (side * 6)] & GetRookAttacks(square, occupancies[both])) return true;

	// queen
	if (bitboards[Pieces::Q + (side * 6)] & GetQueenAttacks(square, occupancies[both])) return true;

	// king
	if (bitboards[Pieces::K + (side * 6)] & GetKingAttacks(square)) return true;


	return false;

}