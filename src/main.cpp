#include <cstdint>
#include <iostream>
#include <string>
#include <algorithm>


#include "bitboard.h"
#include "board.h"
#include "move.h"
#include "utils.h"

uint64_t GenBishopMask(int square);
uint64_t GenRookMask(int square);
uint64_t GetOccupancies(uint64_t fullMask, int index);
uint64_t GenFullBishopAttacks(int square);
uint64_t GenFullRookAttacks(int square);
uint64_t GenPartialBishopAttacks(int square, uint64_t blocker);
uint64_t GenPartialRookAttacks(int square, uint64_t blocker);
uint64_t GenBishopMagic(int square);
uint64_t GenRookMagic(int square);
uint64_t GetBishopAttacks(int square, uint64_t occupancy);
uint64_t GetRookAttacks(int square, uint64_t occupancy);
uint64_t GetPawnAttacks(int square, int colour);
uint64_t GetKnightAttacks(int square);
uint64_t GetKingAttacks(int square);
uint64_t GetQueenAttacks(int square, uint64_t occupancy);
void InitSliderAttacks();
void InitMask();
void InitLeaperAttacks();
void ParseFen(std::string fen);
void MoveGen(MoveList* moveList);
int RunPerft(int depth);
void PerftMoveCount(int depth, int ply = 0);
bool MakeMove(int move)


std::string startPos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
std::string kiwipete = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ";
std::string position3 = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ";
std::string position4 = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1";
std::string position5 = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8";  
std::string position6 = "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 ";



int main() {

    InitMask();
    InitSliderAttacks();
    InitLeaperAttacks();

    ParseFen(startPos);
    //RunPerft(5);
    
    
    //PerftMoveCount(2);
    // PrintBoard();
    
    

    

    /*
    int move1 = EncodeMove(Pieces::Q, 63, 62, 0, 0, 0, 0, Pieces::k);
    int move2 = EncodeMove(Pieces::p, 63, 62, 0, 0, 0, 0, Pieces::k);
    
    MoveList moveList[1];
    AddMove(moveList, move1);
    AddMove(moveList, move2);

    std::cout << GetMovePiece(moveList->moves[0]) << std::endl;
    
    std::cout << GetMoveSource(moveList->moves[0]) << std::endl;
    std::cout << GetMoveTarget(moveList->moves[0]) << std::endl;
    std::cout << GetMoveCapture(moveList->moves[0]) << std::endl;
    std::cout << GetMoveEnpassant(moveList->moves[0]) << std::endl;
    std::cout << GetMoveCastling(moveList->moves[0]) << std::endl;
    std::cout << GetMoveDouble(moveList->moves[0]) << std::endl;
    std::cout << GetMovePromotion(moveList->moves[0]) << std::endl;

    std::cout << GetMovePiece(moveList->moves[1]) << std::endl;
    std::cout << GetMoveSource(moveList->moves[1]) << std::endl;
    std::cout << GetMoveTarget(moveList->moves[1]) << std::endl;
    std::cout << GetMoveCapture(moveList->moves[1]) << std::endl;
    std::cout << GetMoveEnpassant(moveList->moves[1]) << std::endl;
    std::cout << GetMoveCastling(moveList->moves[1]) << std::endl;
    std::cout << GetMoveDouble(moveList->moves[1]) << std::endl;
    std::cout << GetMovePromotion(moveList->moves[1]) << std::endl;

    std::cout << moveList->count << std::endl;
--------------------------------------------------------------------------

    int move = EncodeMove(Pieces::Q, 63, 62, 0, 0, 0, 0, Pieces::k);

    std::cout << GetMovePiece(move) << std::endl;
    std::cout << GetMoveSource(move) << std::endl;
    std::cout << GetMoveTarget(move) << std::endl;
    std::cout << GetMoveCapture(move) << std::endl;
    std::cout << GetMoveEnpassant(move) << std::endl;
    std::cout << GetMoveCastling(move) << std::endl;
    std::cout << GetMoveDouble(move) << std::endl;
    std::cout << GetMovePromotion(move) << std::endl;

   
------------------------------------------------------------------------------

    uint64_t blockertest = 0ULL;
    SetBit(&blockertest, Squares::g6);
    SetBit(&blockertest, Squares::h7);

    SetBit(&blockertest, Squares::d5);
    SetBit(&blockertest, Squares::b7);

    SetBit(&blockertest, Squares::c2);
    SetBit(&blockertest, Squares::b1);

    SetBit(&blockertest, Squares::f3);
    SetBit(&blockertest, Squares::g2);

    PrintBitboard(blockertest);
    PrintBitboard(GetBishopAttacks(Squares::e4, blockertest));
*/


/*
------------------------------------------------------------
    uint64_t blockertest = 0ULL;
    
    SetBit(&blockertest, Squares::e6);
    SetBit(&blockertest, Squares::e8);

    SetBit(&blockertest, Squares::g4);
    SetBit(&blockertest, Squares::h4);

    SetBit(&blockertest, Squares::d4);
    SetBit(&blockertest, Squares::a4);

    SetBit(&blockertest, Squares::e2);
    SetBit(&blockertest, Squares::e1);

    PrintBitboard(blockertest);
    PrintBitboard(GetRookAttacks(Squares::e4, blockertest));
    */

    
    



    
}

