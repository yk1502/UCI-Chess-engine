#include <string>
#include <algorithm>
#include "board.h"
#include "move.h"
#include "bitboard.h"
#include "attacks.h"





void AddMove(MoveList* moveList, int encodedMove) {
    moveList->moves[moveList->count] = encodedMove;
    moveList->count++;
}



bool MakeMove(int move) {
    // the make move function is structed as :

    // quiet moves without promotion
    // 1. single push
    // 2. double push -> might generate an enpassant move
    // 3. knight jump, bishop jumps
    // 4. rook quiets
    // 5. queen quiets
    // 6. king quiets, king castles

    // capture moves without promotion
    // 1. pawn capture
    // 2. enpassant capture
    // 3. knight, bishop captures
    // 4. rook captues
    // 5. queen captues
    // 6. king captures

    // all move
    // 1. handle resetting enpassant squares
    // 2. flipping side
    // 3. renewing occupancies
    // 4. renewing castling rights
    // 5. renewing hashKey

    // extra
    // capture promotion

    CopyBoard();


    int piece = GetMovePiece(move);
    int sourceSquare = GetMoveSource(move);
    int targetSquare = GetMoveTarget(move);
    int isCapture = GetMoveCapture(move);
    int isEnpassant = GetMoveEnpassant(move);
    int isCastling = GetMoveCastling(move);
    int isDouble = GetMoveDouble(move);
    int isPromotion = GetMovePromotion(move);
    
    int pawnMarchOne = (side == white) ? Directions::up : Directions::down;
    
    int otherSide = side ^ 1;
    // btw the code is probbaly very inefficient because 
    // i could use popbit source, and setbit target once,
    // and handle the rest in their own if else statements lol, but idc

    if (!isEnpassant && enpassantSquare != Squares::noSquare) {
        enpassantSquare = Squares::noSquare;
    }

    if (!isCapture && !isPromotion) {
        
        // we'll start with the simple quiets first,
        // let's do non castling, and non double pushes first
        if (!isCastling && !isDouble) {
            PopBit(&bitboards[piece], sourceSquare);
            SetBit(&bitboards[piece], targetSquare);
            mailbox[sourceSquare] = -1;
            mailbox[targetSquare] = piece;

        } else if (isCastling) {
            // now let's handle castling moves
            // we have to move both the king and the rook,
            // so let's move the king first
            PopBit(&bitboards[piece], sourceSquare);
            SetBit(&bitboards[piece], targetSquare);
            mailbox[sourceSquare] = -1;
            mailbox[targetSquare] = piece;
            // to move the rook, we need to know what type of castling
            // we're dealing with, and its simple, we just have to know the
            // targetSquare to determine it, if targetSquare is :
            // 1. g1 : white king side castle
            // 2. c1 : white queen side castle
            // 3. g8 : black king side castle
            // 4. c8 : black queen side castle
            if (targetSquare == Squares::g1) {      // white king side
                PopBit(&bitboards[Pieces::R], Squares::h1);
                SetBit(&bitboards[Pieces::R], Squares::f1);
                mailbox[Squares::h1] = -1;
                mailbox[Squares::f1] = Pieces::R;
            } else if (targetSquare == Squares::c1) { // white queen side
                PopBit(&bitboards[Pieces::R], Squares::a1);
                SetBit(&bitboards[Pieces::R], Squares::d1);
                mailbox[Squares::a1] = -1;
                mailbox[Squares::d1] = Pieces::R;
            } else if (targetSquare == Squares::g8) { // black king side
                PopBit(&bitboards[Pieces::r], Squares::h8);
                SetBit(&bitboards[Pieces::r], Squares::f8);
                mailbox[Squares::h8] = -1;
                mailbox[Squares::f8] = Pieces::r;
            } else {                                  // black queen side
                PopBit(&bitboards[Pieces::r], Squares::a8);
                SetBit(&bitboards[Pieces::r], Squares::d8);
                mailbox[Squares::a8] = -1;
                mailbox[Squares::d8] = Pieces::r;
            }
        } else if (isDouble) {
            // now let's handle double moves
            PopBit(&bitboards[piece], sourceSquare);
            SetBit(&bitboards[piece], targetSquare);
            mailbox[sourceSquare] = -1;
            mailbox[targetSquare] = piece;

            // we need to check for enpassant square generation, so if 
            // there is a pawn on either side of the targetSquare pawn,
            // the enpassant square should be right below us for white,
            // and right above us for black 

            if (GetPawnAttacks(sourceSquare + pawnMarchOne, side) & bitboards[otherSide * 6]) {
                enpassantSquare = sourceSquare + pawnMarchOne;
            }
        }

    } else if (isCapture && !isPromotion){
        
        // capture move can be classified into enpassant and non enpassant,
        // so let's do non-enpassant first
        if (!isEnpassant) {
    
            PopBit(&bitboards[piece], sourceSquare);
            SetBit(&bitboards[piece], targetSquare);
            
            // after moving the piece to the targetSquare, we have to pop the 
            // captured piece out of the bitboard too !
            PopBit(&bitboards[mailbox[targetSquare]], targetSquare);
            mailbox[sourceSquare] = -1;
            mailbox[targetSquare] = piece;

        } else {
            // now let's handle enpassant
            PopBit(&bitboards[piece], sourceSquare);
            SetBit(&bitboards[piece], targetSquare);
            mailbox[sourceSquare] = -1;
            mailbox[targetSquare] = piece;

            // same thing, we need to pop out the pawn that is right above the enpassant square
            PopBit(&bitboards[otherSide * 6], enpassantSquare - pawnMarchOne);
            mailbox[enpassantSquare - pawnMarchOne] = -1;
        }

    } else {
        
        // now we'll handle promotions
        // there're 2 types of promotions, capture promotions, and normal promotion
        // but 2 of them require us to pop out the original pawn from sourceSquare,
        // and set the promotion piece type on the target square
        // so let's do that before anything else

        PopBit(&bitboards[piece], sourceSquare);
        SetBit(&bitboards[isPromotion], targetSquare);
        mailbox[sourceSquare] = -1;
        

        // but if its a capture promotion, we need to pop out the original piece
        // on that target square too
        if (isCapture) {
            PopBit(&bitboards[mailbox[targetSquare]], targetSquare);
        }

        mailbox[targetSquare] = isPromotion;
        
    }

    

    // finally, don't forget to update our game state
    // which includes castling rights, and enpassant square

    // enpassant square
    if (isEnpassant) {
        enpassantSquare = Squares::noSquare;
    }

    // renewing occupancies
    std::fill(std::begin(occupancies), std::end(occupancies), 0ULL);

    for (int i = 0; i < 6; ++i) {
        occupancies[white] |= bitboards[i];
        occupancies[black] |= bitboards[i + 6];
    }

    occupancies[both] = occupancies[white] | occupancies[black];

    // also, we need to detect if this is an illegal move after we set the occupancies
    int kingSquare = GetLsbIndex(bitboards[Pieces::K + (side * 6)]);
    if (IsSquareAttacked(kingSquare, otherSide)) {
        
        TakeBack();
        // return false if its an illegal move
        return false;
    }

    // renewing castling rights
    castling &= castlingRights[sourceSquare];
	castling &= castlingRights[targetSquare];

    
    

    // flipping side
    side ^= 1;

    

    // return true if its legal
    return true;
}