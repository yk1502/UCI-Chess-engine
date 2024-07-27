#pragma once


uint64_t GenRookMask(int square);

uint64_t GenBishopMask(int square);

uint64_t GetOccupancies(uint64_t fullMask, int index);

uint64_t GenPartialBishopAttacks(int square, uint64_t blocker);

uint64_t GenPartialRookAttacks(int square, uint64_t blocker);
