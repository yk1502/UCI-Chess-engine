#pragma once



static inline bool GetBit(uint64_t bitboard, int square) {
    return bitboard & (1ULL << square);
}

static inline void SetBit(uint64_t* bitboard, int square) {
    (*bitboard) |= (1ULL << square);
}

static inline void PopBit(uint64_t* bitboard, int square) {
    (*bitboard) &= ~(1ULL << square);
}

static inline int GetLsbIndex(uint64_t bitboard) {
    return std::countr_zero(bitboard);
}

static inline int GetBitCount(uint64_t bitboard) {
    return std::popcount(bitboard);
}


