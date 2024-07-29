#include <algorithm>
#include <string>
#include <iostream>
#include <chrono>

#include "board.h"
#include "move.h"
#include "utils.h"
#include "movegen.h"
#include "makemove.h"


std::string fenList[100] = {
	{"bn2kb2/3p3r/r2q3p/p1PB1pp1/pQ4P1/PR2pPnP/1BPPP2R/4K1N1 w - - 0 23 "},
	{"r2qkb1r/p2p1ppp/npp1pn2/8/P6P/2NP4/1PP1PPb1/R1BQKBNR w KQkq - 0 8 "},
	{"r1bk1b1r/2n1q2p/p2pBp2/1pp1P2P/1P1N2p1/P1N3P1/2PP1P2/1RBQKR2 b - - 1 17 "},
	{"1rbqkbnr/pp2pp2/3p3p/2p1n1pP/P1P2PPR/1P1P4/4P3/RNBQKBN1 b Qk - 0 12 "},
	{"r1b1kbnr/pp1ppppp/nqp5/8/8/4P2N/PPPPBPPP/RNBQK2R w KQkq - 4 4 "},
	{"rn1qkb2/p3n3/1p1p1p1r/1N2p1pp/P1p1PP2/2P2QPN/1P1P3P/R1BK3R w - - 1 15 "},
	{"1nbqk2r/3pbppp/rpp1pn2/p5P1/P2P1P2/2N5/1PP1P2P/R1BQKBNR w KQk - 1 8 "},
	{"r2q1b1r/pp1bpkpp/2np1n2/2pP1p2/2P2P2/4B2P/PP2P1P1/RN1QKBNR b KQ - 0 8 "},
	{"r3kb1r/1p2pp1p/p1p1bn2/P2n2p1/3Pp3/4BPPB/2P1N2P/RqQ2K1R b k - 9 17 "},
	{"rnb1kb2/ppppqpQ1/4pn1r/2P2P1p/1P6/8/P2PP1PP/RNB1KBNR b KQq - 0 8 "},
	{"rnb1kb2/1pq1p2r/2pP1npp/p4p1P/P4PP1/1P2P3/3P4/RNBQKBNR b KQq - 0 11 "},
	{"rnb1k1nr/pp1p1ppp/2p1p3/1Nb5/1P4P1/3P1P1P/PBP1P3/R2QKBNR w KQkq - 1 9 "},
	{"1rbqkbnr/pppp1ppp/4p3/n7/P1PP1PP1/5N2/1P2PK1P/RNBQ1B1R b k - 0 7 "},
	{"1rb1kr2/p1p4p/1p1q1bpn/3p1pP1/Pn1PpPN1/2P4P/R2QP1B1/1NB1K1R1 w - - 6 19 "},
	{"rnbk2qr/5pp1/3bpn1p/p2p4/p1P1N1PP/4PP2/RP1PR3/2B1K1N1 b - - 4 15 "},
	{"rq2n3/p2kpNb1/P3b2r/4n2p/2pp4/2P2N2/1P1PPP1P/RBBQR1K1 b - - 2 18 "},
	{"1n4nr/r3kq2/p2Npp1b/2pb2Pp/p2PP3/7P/PPP3PB/2RQK2R b K - 5 22 "},
	{"1r1qk2r/p1p1n3/n3pp2/1p1pb1pp/PP1P4/3KNN1P/R1PBPP2/3Q1B1R w k - 1 19 "},
	{"1rbqk1nr/ppppppbp/n5p1/8/8/PPP5/R2PPPPP/1NBQKBNR w Kk - 3 5 "},
	{"rnb1k1nr/pppp1pbp/4p3/8/P2q1PpR/1P1PP1P1/2PQB3/RNB2KN1 b kq - 4 11 "},
	{"rnbqkbnr/pppppp1p/8/6p1/8/6P1/PPPPPP1P/RNBQKBNR w KQkq - 0 2 "},
	{"rnbqkbnr/1pp3p1/p3pP2/3p3p/P2P4/R4N2/1PP1PP1P/1NBQKB1R b Kkq - 1 7 "},
	{"rn1qkb1r/p1ppp2p/1p6/1b3pp1/5Pn1/1P2PQ2/P1PP3P/RNB1KBNR w KQkq - 3 8 "},
	{"r1b2bnr/p3pqp1/p1nk4/RPpp2N1/5p2/2PP2pP/4PP1R/2BQKBN1 b - - 1 18 "},
	{"rnb2bnr/1p3ppQ/1q1pk3/p1p1p3/P1P5/NP2P2P/3P1PP1/1RB1KBNR w K - 1 10 "},
	{"r2qkbnr/2p2p1p/pp2p3/3p2p1/PP1N4/3PPQPb/1P3PBP/RNB1K2R b KQkq - 1 11 "},
	{"r2qkb1r/p1pppp2/1p3npp/6P1/1n5N/NPP2b2/P2PP2P/1RBQKBR1 w q - 0 11 "},
	{"2b2bnr/q2npk2/1pp4p/6r1/2pPP3/N1P4P/PB1KN2R/3R2r1 w - - 2 24 "},
	{"rn1qk1nr/p1pp1pp1/b6p/1p2p3/2B5/4PP1N/PPPbK1PP/RNBQ3R w kq - 0 8 "},
	{"r3kb1r/pp3pp1/2n1pn2/q1pB1b1P/P1P2P2/7N/1P1PPK1P/RNBQ3R b kq - 2 9 "},
	{"rnb1kb1r/p4pp1/5n1p/1Pp1p3/3p1NP1/N1P2P2/1P1P1P1P/R1BQKB1R w KQkq - 0 13 "},
	{"rn1qkb1r/pp3ppp/2p1pn2/3p4/5P1P/P1PP3b/1P2P1P1/RNBQKBNR w KQkq - 0 6 "},
	{"rnbqkb2/2pp2pr/pp2Q3/1n2p1pp/6P1/P1PPP3/R2N1P1P/1N2KBR1 b q - 1 16 "},
	{"rnbqkbnr/1ppp1ppp/8/p3p3/1P6/8/P1PPPPPP/RNBQKBNR w KQkq - 0 3 "},
	{"1n2k1r1/1p1np3/r3b1p1/1Np2p1p/P1N2b1P/4PP2/1qQPK1P1/2B2BR1 b - - 1 23 "},
	{"rn1qkbnr/p1p1pp2/3p3p/1p1N2p1/5PbP/8/PPPPP1P1/1RBQKBNR b Kkq - 1 6 "},
	{"1rbqk2r/2p1p1b1/1N1p2p1/p5pp/1PP3P1/n4P1R/3PP1P1/1RBQKB2 w - - 0 18 "},
	{"rnb1kbnr/ppp2p1p/3p2p1/4p1q1/4P3/7P/PPPPBPP1/RNBQK1NR w KQkq - 2 5 "},
	{"rn1qkbnr/pbppp1p1/5p1p/1p6/PP6/R3P3/2PPQPPP/1NB1KBNR b Kkq - 3 5 "},
	{"rnbqkb1r/p2p1ppp/1pp1p2n/8/2P2P2/N5P1/PP1PP2P/R1BQKBNR w KQkq - 0 5 "},
	{"rnbq1k1r/1p1ppp1p/p1p4b/6p1/BP2P2P/N2PB1n1/P1P2PP1/R2QK1NR w KQ - 3 9 "},
	{"1nb3nB/rp1p1k2/2pb4/5p1p/pPPQ3q/N2Pp1P1/P3PP1P/R3KBNR b KQ - 1 14 "},
	{"r1b1kbn1/p1pp1ppr/np2p3/P6p/3Pq2P/RNP5/1P1QPPP1/2B1KBNR b Kq - 6 11 "},
	{"rnbqkbnr/1ppppppp/p7/8/8/4P3/PPPP1PPP/RNBQKBNR w KQkq - 0 2 "},
	{"2n4r/p2p4/r1N2kpb/1p3p1Q/1P2P3/2N3P1/1PPP1P1P/2B1KB1R w K - 1 20 "},
	{"rnbqkb1r/2pppppp/pp1n4/8/PP4PP/8/R1PPPP2/1NBQKBNR b Kkq - 0 6 "},
	{"2b1q2r/r2kb2p/nBp1p3/3P4/3Pnp2/P4PPP/RP2N3/1N3RK1 b - - 0 20 "},
	{"r1bq1bnr/pppppk1p/2n3p1/5p2/Q4PP1/2P2N2/PP1PP2P/RNB1KB1R b KQ - 3 5 "},
	{"1n3r2/4knp1/4b3/pp1pP2p/1Pp2BP1/2PB2qP/P1K1P3/RN4NR b - - 1 24 "},
	{"1n1q3r/rb1pk1bp/B3pp1n/1Qp4P/4P1p1/1P6/P1PPNPPR/RNB1K3 w Q - 1 15 "},
	{"rnbqkbnr/pppppppp/8/8/1P6/8/P1PPPPPP/RNBQKBNR b KQkq - 0 1 "},
	{"rnb1kb1r/p4ppp/1pp4n/3pp3/2P4q/1P4PN/P2PPP2/RNBQKB1R w KQkq - 0 7 "},
	{"Q1b1kbnr/1p6/2p1pp2/4N1pp/2P4P/3Pq1PB/PP1P1P2/RNB2KR1 b k - 0 15 "},
	{"1rb1k1nr/p4p1p/n1ppp2b/1pP3p1/P2P2P1/1QP1B2P/2N1PP1N/R3KB1R b KQk - 0 14 "},
	{"r1bqkbnr/ppppppp1/4n2p/8/2P4P/6P1/PP1PPP1R/RNBQKBN1 w Qkq - 1 5 "},
	{"r1q1kb1r/pppbpppp/n7/3p4/2N1nP2/2PP2P1/PP2P2P/1RBQKBNR b Kkq - 2 7 "},
	{"5bnr/r2qk1p1/2n2p2/2p1N2p/p2N2bP/BPP2PP1/PR1PP2R/3QKB2 w - - 3 19 "},
	{"1rb1kb1r/2pp3p/8/3Q1np1/1n1P2pq/P6P/RKP1PP1R/1NB2BN1 w k - 1 22 "},
	{"r1b2bnr/pppp1pp1/5k2/4p2p/Pq2P2P/N1P2P2/1P1P2PR/2n1KBN1 w - - 3 12 "},
	{"3r1b2/ppp1kp1r/3p1p1p/N1nPp3/PP4P1/2P3n1/3KP1P1/R1B2BNR b - - 0 19 "},
	{"rn1qkb1r/ppp2pp1/7n/3ppb1p/2P1N3/P2P2PN/RP1QPP1P/2B1KB1R w Kkq - 1 10 "},
	{"rnbqkbr1/p2npp1p/1pp4B/3p3p/2P3P1/P2P4/1P2PP2/RN1QKBNR w KQq - 0 10 "},
	{"r1b1qNn1/2pp1pp1/2nk3p/p7/QPP1pP2/2N4P/P2PP1P1/R1B1KB1R b KQ - 0 16 "},
	{"rnb2N1k/1pp5/p2p1p1n/8/3Pp1P1/2P4p/Pb1QP2P/1RB3KR w - - 0 21 "},
	{"r4k1r/2p2npp/b3p3/pp1P1p2/1N3PP1/8/qP2PN1P/R1B1KB1R w KQ - 1 20 "},
	{"rnbqk2r/p1pp1npp/1p6/2b1pp2/P2N4/2P4P/RP1PPPP1/1NBQKB1R w Kkq - 3 7 "},
	{"N3k3/1p2bppr/2n4p/2ppp3/QPP2nbP/2KP4/4PPPN/1RB2B1R w - - 1 21 "},
	{"rnbqkbnr/1ppp1p1p/4p3/p5P1/8/N3P3/PPPP2PP/R1BQKBNR w KQkq - 0 5 "},
	{"rn2kbnr/p1p2q2/b5pp/P2ppp2/3P4/1PQ3PB/2P1PP1P/R1B2KNR b kq - 1 13 "},
	{"r1b1kb2/p3p2r/n4p2/1qPp2N1/P5np/2P1PKR1/3P1P1P/RNB1Q3 w q - 0 16 "},
	{"rnbk1bnr/pp1ppppp/2p5/3P2B1/1P4P1/N7/P1P1PK1P/R2Q1BNR b - - 0 8 "},
	{"rn2kb2/p2q1p1r/b2p2pp/2p1p2n/P5P1/Np5B/RPPPPPKN/2BQ3R b q - 1 18 "},
	{"1nbk3r/1p2ppb1/1q4pp/1r1p1P1Q/5P2/P1p1P1NP/2PP4/RNB2K1R w - - 0 17 "},
	{"1q2kb2/5np1/1p3pr1/2pppbrp/1p2PNPP/N1BP1Q1B/P1P2P2/n2RK2R w K - 4 25 "},
	{"r1bk3r/p2p1p1p/n1p2n2/1pP3p1/Q3qp1P/2P2N2/PB1PPKP1/RN3B1R w - - 2 13 "},
	{"r3k1nr/p1pqppbp/n2pb3/1p2B3/1P4PP/2PP4/1R2PP2/1N1QKBNR b K - 8 16 "},
	{"rnbqkbnr/p1pp4/1p2p3/5ppp/4P3/N2B1PP1/PPPP3P/R1BQK1NR w KQkq - 0 6 "},
	{"r2qkb2/3bpp1r/1pp4p/p1np2pn/P2P3P/5NR1/1PPBPPP1/Q3KBNR b q - 3 14 "},
	{"4rb1r/1pNkpppq/4nn1p/p1p4B/3PpR2/1PP3P1/P4K1P/1RBb3N w - - 4 26 "},
	{"1rbqkb1r/pppppp2/n5p1/7p/1P2PP1P/P7/2PP1KPn/RNBQ1BNR w - - 5 10 "},
	{"1nb2b1r/1r1pk1p1/pp5P/2pn1p2/3P1P2/1q1pP3/P1PNQ1KP/R1B2BR1 b - - 4 23 "},
	{"r1bk1b1r/pp1p1pp1/n1p5/4p1qp/6n1/P1NP2PP/1PPBPPN1/R2QKB1R b KQ - 2 9 "},
	{"rnb2b1r/1ppkn1p1/p5p1/4pp2/2NpqP2/1P1BQ2P/P1PP2P1/R1B1K2R b KQ - 3 13 "},
	{"r2k1b2/p2bn1p1/n2ppq2/1Pp1P1Br/2PP1p1P/1P4PN/R2Q1P2/1N3K1R w - - 1 21 "},
	{"rnbqkb1r/ppp1pppp/3p4/5n2/4P3/N4QP1/PPPP1P1P/R1B1KBNR b KQkq - 2 4 "},
	{"rnb1kbr1/2pp1p1p/pp5n/4p3/1PP3Qq/4PpPN/P2P3P/RNB1KB1R w KQq - 0 11 "},
	{"rnbqkb1r/3p1ppp/1p5n/p1p1p3/P2PP1P1/8/1PP2P1P/RNBQKBNR b Kkq - 0 6 "},
	{"r1b1k1nr/2ppnpp1/3bp2p/1p4q1/Q1PP1NPP/8/PP2PP2/RNB1KB1R w KQkq - 0 10 "},
	{"rn1qk2r/p1p1ppbp/1P1p3n/5b2/R4QpP/2P1PP1R/1P1P2P1/1NB1KBN1 b - - 1 13 "},
};


int actualNodeCount[100] = {
	83609796,
	36210911,
	28519413,
	18980078,
	23266392,
	24574786,
	17683273,
	31826556,
	50831828,
	19390997,
	35947213,
	19427343,
	38083986,
	59754860,
	35429796,
	58451992,
	50867838,
	32490320,
	7063763	,
	67106452,
	5971356	,
	32163445,
	40218657,
	57503226,
	23339617,
	43175973,
	31086948,
	27126945,
	33891137,
	110085141,
	35718797,
	25157131,
	2170658	,
	12880276,
	72926342,
	18468491,
	34493279,
	26984558,
	14238887,
	13479951,
	45970534,
	41699210,
	54871761,
	11766661,
	48846438,
	7966214	,
	45018590,
	22959648,
	43080509,
	51463588,
	5293555	,
	30097641,
	30539088,
	16009362,
	8337529	,
	38024908,
	45358679,
	64107475,
	17295863,
	13969508,
	50581299,
	33077355,
	19395872,
	9386434	,
	24550986,
	24078053,
	53426391,
	26033110,
	37401255,
	41821056,
	8307979	,
	27115510,
	41541497,
	58811246,
	44119022,
	34653466,
	17670040,
	33107333,
	54485112,
	15728641,
	52121244,
	50737853,
	25386992,
	76200106,
	28095725,
	41362276,
	34938392,
	92542149,
	87979928,
};







void ParseFen(std::string fen);




static int nodes = 0;

static int cumNodes = 0;





void Perft(int depth) {

    if (depth == 0) {
        nodes++;
        return;
    }

    MoveList moveList[1];
    MoveGen(moveList);

    for (int moveCount = 0; moveCount < moveList->count; ++moveCount) {
        CopyBoard() 

        if (!MakeMove(moveList->moves[moveCount])) {
            continue;
        }

        Perft(depth - 1);

        TakeBack();
    }

    return;
}


void RunPerft(int maxDepth) {
    
    for (int currDepth = 1; currDepth <= maxDepth; ++currDepth) {
        nodes = 0;
        uint64_t startTime = GetTimeMs();
        Perft(currDepth);
        uint64_t duration = GetTimeMs() - startTime;

        std::cout << "depth : " << currDepth << " nodes : " << nodes << " time : " << duration << " nps : " << (static_cast<float>(nodes) / (static_cast<float>(duration) / 1000.0f)) / 1000000.0f << "Mnps" << std::endl;
    }

}



void PerftMoveCount(int depth, int ply = 0) {

    if (depth == 0) {
        nodes++;
        return;
    }

    MoveList moveList[1];
    MoveGen(moveList);

    for (int moveCount = 0; moveCount < moveList->count; ++moveCount) {
        
        CopyBoard() 

        if (!MakeMove(moveList->moves[moveCount])) {
            continue;
        }

        PerftMoveCount(depth - 1, ply + 1);

        if (ply == 0) {

            PrintMove(moveList->moves[moveCount]);
            std::cout << " - " << nodes - cumNodes << std::endl;

            cumNodes = nodes;

        }

        TakeBack();
    }

    return;
}



void PerftPositions() {
    for (int i = 0; i < 100; ++i) {
        ParseFen(fenList[i]);
        nodes = 0;
        Perft(5);
        if (nodes == actualNodeCount[i]) {
            std::cout << "test : " << i << " ...passed" << std::endl;
        } else {
            std::cout << "test : " << i << " ...failed" << " node : " << nodes << " should be : " << actualNodeCount << std::endl;
        }
    }
}