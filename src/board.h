#pragma once


extern uint64_t bitboards[12];
extern uint64_t occupancies[3];

extern int side;

extern int castling;

extern int enpassantSquare;

extern uint64_t hashKey;


enum {
    white,
    black,
    both
};




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

