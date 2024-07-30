#pragma once


extern uint64_t bitboards[12];

extern uint64_t occupancies[3];

extern int mailbox[64];

extern int side;

extern int castling;

extern int enpassantSquare;

extern uint64_t hashKey;

extern uint64_t repHistory[1500];

extern int repIndex;

bool IsSquareAttacked(int square, int side);


enum {
    white,
    black,
    both
};



static constexpr int castlingRights[64] = {
	 7, 15, 15, 15,  3, 15, 15, 11,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	13, 15, 15, 15, 12, 15, 15, 14
};


#define CopyBoard()                                                                      \
    uint64_t c_bitboards[12], c_occupancies[3], c_mailbox[64], c_hashKey;                \
    int c_enpassantSquare, c_side, c_castling;                                           \
    c_enpassantSquare = enpassantSquare, c_side = side, c_castling = castling;           \
    std::copy(std::begin(bitboards), std::end(bitboards), std::begin(c_bitboards));      \
    std::copy(std::begin(occupancies), std::end(occupancies), std::begin(c_occupancies));\
    std::copy(std::begin(mailbox), std::end(mailbox), std::begin(c_mailbox));            \
    c_hashKey = hashKey;



#define TakeBack()                                                                                 \
    enpassantSquare = c_enpassantSquare, side = c_side, castling = c_castling, hashKey = c_hashKey;\
    std::copy(std::begin(c_bitboards), std::end(c_bitboards), std::begin(bitboards));              \
    std::copy(std::begin(c_occupancies), std::end(c_occupancies), std::begin(occupancies));        \
    std::copy(std::begin(c_mailbox), std::end(c_mailbox), std::begin(mailbox));                    \




namespace Files {
    static constexpr uint64_t aFile = 72340172838076673ULL;
    static constexpr uint64_t hFile = 9259542123273814144ULL;

    static constexpr uint64_t abFile = 13889313184910721216ULL;
    static constexpr uint64_t ghFile = 217020518514230019ULL;
}


namespace Squares {
    enum {
        a8, b8, c8, d8, e8, f8, g8, h8,
        a7, b7, c7, d7, e7, f7, g7, h7,
        a6, b6, c6, d6, e6, f6, g6, h6,
        a5, b5, c5, d5, e5, f5, g5, h5,
        a4, b4, c4, d4, e4, f4, g4, h4,
        a3, b3, c3, d3, e3, f3, g3, h3,
        a2, b2, c2, d2, e2, f2, g2, h2,
        a1, b1, c1, d1, e1, f1, g1, h1, noSquare
    };

    static const std::string numToSquare[65] = {
        "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
        "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
        "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
        "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
        "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
        "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
        "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
        "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1", "noSquare"
    };
}


namespace Directions {
    static constexpr int up = -8;
    static constexpr int down = +8;
    static constexpr int left = -1;
    static constexpr int right = +1;

    static constexpr int upRight = up + right;
    static constexpr int upLeft = up + left;
    static constexpr int downRight = down + right;
    static constexpr int downLeft = down + left;

    static constexpr int doubleRight = right + right;
    static constexpr int doubleLeft = left + left;
    static constexpr int doubleUp = up + up;
    static constexpr int doubleDown = down + down;
}


namespace Pieces {
    enum {
        P, N, B, R, Q, K,
        p, n, b, r, q, k
    };

    static constexpr char numToPiece[12] = {
        'P', 'N', 'B', 'R', 'Q', 'K',
        'p', 'n', 'b', 'r', 'q', 'k'
    };

}

