#pragma once


extern uint64_t pieceKeys[12][64];
extern uint64_t castleKeys[16];
extern uint64_t enpassantKeys[8];
extern uint64_t sideKeys;


void InitPosKeys();

uint64_t GeneratePosKey();