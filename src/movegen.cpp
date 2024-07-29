#include <string>
#include <cstdint>
#include <algorithm>

#include "bitboard.h"
#include "board.h"
#include "move.h"
#include "attacks.h"
#include "utils.h"
#include "makemove.h"





void MoveGen(MoveList* moveList) {

    int otherSide = side ^ 1;

    int pawnMarchOne = (side == white) ? Directions::up : Directions::down;
    int pawnMarchDouble = pawnMarchOne + pawnMarchOne;
    
    int pawnOriginLeft = (side == white) ? Squares::a2 : Squares::a7;
    int pawnOriginRight = (side == white) ? Squares::h2 : Squares::h7;

    int pawnPromotionLeft = (side == white) ? Squares::a7 : Squares::a2;
    int pawnPromotionRight = (side == white) ? Squares::h7 : Squares::h2;

    int pawnEnpassantLeft = (side == white) ? Squares::a5 : Squares::a4;
    int pawnEnpassantRight = (side == white) ? Squares::h5 : Squares::h4;
    
    int startPiece = 0, endPiece = 5;
    int offsetPiece = side * 6;

    uint64_t bitboard, attack;
    int sourceSquare, attackSquare;

    /*
    
    int movePieceType, 
    int sourceSquare, 
    int targetSquare, 
    int isCapture,           
    int isEnpassant, 
    int isCastling, 
    int isDouble,
    int promotionPieceType
    */
    

    for (int piece = (startPiece + offsetPiece); piece <= (endPiece + offsetPiece); ++piece) {

        bitboard = bitboards[piece];

        while (bitboard) {
            // loop through all the pawns in the current board, and 
            // generate moves for all of them
            sourceSquare = GetLsbIndex(bitboard);

            // generate pawn moves
            if (piece == (Pieces::P + offsetPiece)) {
                // this one is single push, well if it can't make a single push, 
                // it definately can't make a double push too
                if (!GetBit(occupancies[both], sourceSquare + pawnMarchOne)) {

                    if ((sourceSquare >= pawnPromotionLeft) && (sourceSquare <= pawnPromotionRight)) {
                        // if it can make a single push, it has
                        // the potential to be a promotion, so check that,
                        // btw, this is just a single push promotion, not included capture promotion
                        // the +offsetPiece is used to take care of other side's promotion piece type (black queen, white queen, etc.)
                        AddMove(moveList, EncodeMove(piece, sourceSquare, sourceSquare + pawnMarchOne, 0, 0, 0, 0, Pieces::Q + offsetPiece));
                        AddMove(moveList, EncodeMove(piece, sourceSquare, sourceSquare + pawnMarchOne, 0, 0, 0, 0, Pieces::R + offsetPiece));
                        AddMove(moveList, EncodeMove(piece, sourceSquare, sourceSquare + pawnMarchOne, 0, 0, 0, 0, Pieces::B + offsetPiece));
                        AddMove(moveList, EncodeMove(piece, sourceSquare, sourceSquare + pawnMarchOne, 0, 0, 0, 0, Pieces::N + offsetPiece));
                    } else {
                        // if it can make a single push, we need to check if its on their
                        // origin square(rank) to make a double move
                        AddMove(moveList, EncodeMove(piece, sourceSquare, sourceSquare + pawnMarchOne, 0, 0, 0, 0, 0));
                        if (   (sourceSquare >= pawnOriginLeft)
                            && (sourceSquare <= pawnOriginRight)
                            && !GetBit(occupancies[both], sourceSquare + pawnMarchDouble)) {
                            AddMove(moveList, EncodeMove(piece, sourceSquare, sourceSquare + pawnMarchDouble, 0, 0, 0, 1, 0));
                        }
                    }  
                }

                // if it can make an attack on *opponent pieces* (that's why occupancies is
                // using otherSide) loop through the possible attack, we need a loop since
                // the pawn can attack 2 squares diagonally
                attack = pawnAttacks[side][sourceSquare] & occupancies[otherSide];
                while (attack) {
                    attackSquare = GetLsbIndex(attack);

                    // we need to think about promotion captures too, the thing is, 
                    // it can't be a promotion capture, and a non-promotion capture
                    // at the same time, if that make sense, so we'll use "if else", instead
                    // of just "if"
                    if ((sourceSquare >= pawnPromotionLeft) && (sourceSquare <= pawnPromotionRight)) {
                        // now if its a promotion capture, generate the move !
                        AddMove(moveList, EncodeMove(piece, sourceSquare, attackSquare, 1, 0, 0, 0, Pieces::Q + offsetPiece));
                        AddMove(moveList, EncodeMove(piece, sourceSquare, attackSquare, 1, 0, 0, 0, Pieces::R + offsetPiece));
                        AddMove(moveList, EncodeMove(piece, sourceSquare, attackSquare, 1, 0, 0, 0, Pieces::B + offsetPiece));
                        AddMove(moveList, EncodeMove(piece, sourceSquare, attackSquare, 1, 0, 0, 0, Pieces::N + offsetPiece));
                    } else {
                        // if its not a promotion capture, simply generate a capture move
                        AddMove(moveList, EncodeMove(piece, sourceSquare, attackSquare, 1, 0, 0, 0, 0));
                    }

                    PopBit(&attack, attackSquare);
                }

                // now its time for enpassant move, first check if enpassant square is available 
                if ((enpassantSquare != Squares::noSquare)
                    && (sourceSquare >= pawnEnpassantLeft)
                    && (sourceSquare <= pawnEnpassantRight)) {
                    // if enpassant square is available, check if we are the right pawn for it
                    // to check that, its quite simple, all we have to do, is to use the pawn attack map
                    // from the current square, and if that intersects with the enpassant square, we know
                    // we are the correct pawn to make that move, so generate it !
                    if (GetBit(pawnAttacks[side][sourceSquare], enpassantSquare)) {
                        AddMove(moveList, EncodeMove(piece, sourceSquare, enpassantSquare, 1, 1, 0, 0, 0));
                    }
                    
                }

                
            } else {
                // if its not a pawn move, we'll generate the attack map for other pieces,
                // since they are all very similar, we can put them in one chunk of 
                // if else like this
                if (piece == (Pieces::N + offsetPiece)) {
                    attack = GetKnightAttacks(sourceSquare);
                } else if (piece == (Pieces::B + offsetPiece)) {
                    attack = GetBishopAttacks(sourceSquare, occupancies[both]);
                } else if (piece == (Pieces::R + offsetPiece)) {
                    attack = GetRookAttacks(sourceSquare, occupancies[both]);
                } else if (piece == (Pieces::Q + offsetPiece)) {
                    attack = GetQueenAttacks(sourceSquare, occupancies[both]);
                } else if (piece == (Pieces::K + offsetPiece)) {
                    attack = GetKingAttacks(sourceSquare);

                    // since we can only make a castling move if we have the rights
                    // to do so, so check that first !
                    if (side == white) {
                        // if we have the rights to do castling, there're a few things to consider before
                        // we can generate it, 
                        // 1. king must not be in checked
                        // 2. rook must be in the original spot
                        // 3. king must be in the original spot
                        // 4. no opponent pieces is attacking the range of castling
                        // though we can skip step 2 and 3, because if it moves the castling rights will be lost anyway
                        if (castling & 0b0001) {  // white king side castling
                            if (!(  IsSquareAttacked(Squares::e1, otherSide)
                                ||  IsSquareAttacked(Squares::f1, otherSide)
                                ||  IsSquareAttacked(Squares::g1, otherSide)
                                ||  GetBit(occupancies[both], Squares::f1)
                                ||  GetBit(occupancies[both], Squares::g1)
                            )) {
                                AddMove(moveList, EncodeMove(piece, Squares::e1, Squares::g1, 0, 0, 1, 0, 0));
                            }
                        }

                        if (castling & 0b0010) {  // white queen side castling 
                            if (!(  IsSquareAttacked(Squares::c1, otherSide)
                                ||  IsSquareAttacked(Squares::d1, otherSide)
                                ||  IsSquareAttacked(Squares::e1, otherSide)
                                ||  GetBit(occupancies[both], Squares::d1)
                                ||  GetBit(occupancies[both], Squares::c1)
                                ||  GetBit(occupancies[both], Squares::b1)
                            )) {
                                AddMove(moveList, EncodeMove(piece, Squares::e1, Squares::c1, 0, 0, 1, 0, 0));
                            }
                        }
                    } else {
                        if (castling & 0b0100) {  // black king side castling
                            if (!(  IsSquareAttacked(Squares::e8, otherSide)
                                ||  IsSquareAttacked(Squares::f8, otherSide)
                                ||  IsSquareAttacked(Squares::g8, otherSide)
                                ||  GetBit(occupancies[both], Squares::f8)
                                ||  GetBit(occupancies[both], Squares::g8)
                            )) {
                                AddMove(moveList, EncodeMove(piece, Squares::e8, Squares::g8, 0, 0, 1, 0, 0));
                            }
                        }

                        if (castling & 0b1000) {  // black queen side castling 
                            if (!(  IsSquareAttacked(Squares::c8, otherSide)
                                ||  IsSquareAttacked(Squares::d8, otherSide)
                                ||  IsSquareAttacked(Squares::e8, otherSide)
                                ||  GetBit(occupancies[both], Squares::d8)
                                ||  GetBit(occupancies[both], Squares::c8)
                                ||  GetBit(occupancies[both], Squares::b8)
                            )) {
                                AddMove(moveList, EncodeMove(piece, Squares::e8, Squares::c8, 0, 0, 1, 0, 0));
                            }
                        }
                    }
                }

                // now we'll loop through all the attacks, and generate what's possible
                
                while (attack) {
                    attackSquare = GetLsbIndex(attack); 
                    // if the current square is not occupied by one of our own pieces,
                    // then generate the attack to that square
                    if (!GetBit(occupancies[side], attackSquare)) {
                        
                        // if the current square is occupied bt opponent pieces, generate the move
                        // and mark it as a capture
                        if (GetBit(occupancies[otherSide], attackSquare)) {
                            AddMove(moveList, EncodeMove(piece, sourceSquare, attackSquare, 1, 0, 0, 0, 0));
                        } else {
                            // if not, then simply generate a quiet move to that square
                            AddMove(moveList, EncodeMove(piece, sourceSquare, attackSquare, 0, 0, 0, 0, 0));
                        }
                    }

                    PopBit(&attack, attackSquare);
                }
            }

            // remember to pop the piece out of the board after generating all of its
            // possible moves
            PopBit(&bitboard, sourceSquare);
        }
    }
}




