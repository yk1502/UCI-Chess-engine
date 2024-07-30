#pragma once


extern uint64_t pieceKeys[12][64];
extern uint64_t castleKeys[16];
extern uint64_t enpassantKeys[8];
extern uint64_t sideKeys;


const int NO_TT = 60000;


enum {
    UPPERBOUND, LOWERBOUND, EXACT, NO_FLAG
};

struct ttEntry {
	uint64_t hashKey;
	short depth;
	short flag;
	int eval;
	int move;
};


extern ttEntry tt[2000000];


void InitPosKeys();

uint64_t GeneratePosKey();


uint64_t Index();

void ClearTT();

void StoreTTEntry(short depth, short flag, int eval, int move, short ply);

int ProbeTT(short depth, short ply, int alpha, int beta);

