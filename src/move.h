#pragma once



struct MoveList {
    int moves[256];
    int count = 0;
};


//0000 0000     0000 0 0 0 0 000000 000000 0000

static inline int EncodeMove(   int movePieceType, 
                                int sourceSquare, 
                                int targetSquare, 
                                int isCapture,           
                                int isEnpassant, 
                                int isCastling, 
                                int isDouble,
                                int promotionPieceType) {
    
    int encodedMove = 0;
    encodedMove |= movePieceType;
    encodedMove |= (sourceSquare << 4);
    encodedMove |= (targetSquare << 10);
    encodedMove |= (isCapture << 16);
    encodedMove |= (isEnpassant << 17);
    encodedMove |= (isCastling << 18);
    encodedMove |= (isDouble << 19);
    encodedMove |= (promotionPieceType << 20);
    return encodedMove;
}


static inline int GetMovePiece(int encodedMove) {
    return (encodedMove & 15);
}

static inline int GetMoveSource(int encodedMove) {
    return ((encodedMove & 1008) >> 4);
}

static inline int GetMoveTarget(int encodedMove) {
    return ((encodedMove & 64512) >> 10);
}

static inline int GetMoveCapture(int encodedMove) {
    return (encodedMove & 65536);
}

static inline int GetMoveEnpassant(int encodedMove) {
    return (encodedMove & 131072);
}

static inline int GetMoveCastling(int encodedMove) {
    return (encodedMove & 262144);
}

static inline int GetMoveDouble(int encodedMove) {
    return (encodedMove & 524288);
}

static inline int GetMovePromotion(int encodedMove) {
    return ((encodedMove & 15728640) >> 20);
}


static inline void AddMove(MoveList* moveList, int encodedMove) {
    moveList->moves[moveList->count] = encodedMove;
    moveList->count++;
}
