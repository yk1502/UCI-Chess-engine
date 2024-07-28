#pragma once


// leaper pieces
extern uint64_t pawnAttacks[2][64];
extern uint64_t knightAttacks[64];
extern uint64_t kingAttacks[64];

// sliding pieces
extern uint64_t bishopAttacks[64][1024];
extern uint64_t rookAttacks[64][16384];

// full mask
extern uint64_t bishopMask[64];
extern uint64_t rookMask[64];


uint64_t GenRookMask(int square);

uint64_t GenBishopMask(int square);

uint64_t GetOccupancies(uint64_t fullMask, int index);

uint64_t GenPartialBishopAttacks(int square, uint64_t blocker);

uint64_t GenPartialRookAttacks(int square, uint64_t blocker);



uint64_t GetPawnAttacks(int square, int colour);

uint64_t GetKnightAttacks(int square);

uint64_t GetKingAttacks(int square);

uint64_t GetBishopAttacks(int square, uint64_t occupancy); 

uint64_t GetRookAttacks(int square, uint64_t occupancy);

uint64_t GetQueenAttacks(int square, uint64_t occupancy); 