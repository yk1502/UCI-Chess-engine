#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>

#include "board.h"
#include "bitboard.h"
#include "move.h"
#include "utils.h"
#include "positions.h"
#include "makemove.h"
#include "movegen.h"
#include "search.h"
#include "tt.h"





void ParseFen(std::string fen) {

    std::memset(bitboards, 0ULL, sizeof(bitboards));
    std::memset(occupancies, 0ULL, sizeof(occupancies));
    std::memset(repHistory, 0ULL, sizeof(repHistory));
    std::fill(std::begin(mailbox), std::end(mailbox), -1);
    repIndex = 0;
    
    side = 0;
    castling = 0;
    enpassantSquare = Squares::noSquare;
    hashKey = 0ULL;    
    

    std::stringstream ss(fen);
    std::string fenSplit[6];

    for (int i = 0; i < 6; ++i) {
        ss >> fenSplit[i];
    }

    // set up position
    int square = -1;
    for (char p : fenSplit[0]) {
        int piece = pieceToNum(p);
        if (piece != -1) {
            ++square;
            mailbox[square] = piece;
            SetBit(&bitboards[piece], square);
        } else {
            if (p != '/') {
                int skip = p - '0';
                square += skip;
            } 
        }
    }

    // set up board state (side to move)
    if (fenSplit[1] == "w") {
        side = white;
    } else {
        side = black;
    }

    // castling rights
    for (char c : fenSplit[2]) {
        if (c == 'K') {
            castling = castling | (1);
        } else if (c == 'Q') {
            castling = castling | (1 << 1);
        } else if (c == 'k') {
            castling = castling | (1 << 2);
        } else if (c == 'q') {
            castling = castling | (1 << 3);
        }
    }

    // enpassant square
    if (fenSplit[3] != "-") {
        
        int file = fenSplit[3][0] - 'a';
        int rank = 8 - (fenSplit[3][1] - '0');
        
        int square = rank * 8 + file;
        enpassantSquare = square;
    }

    for (int i = 0; i < 6; ++i) {
        occupancies[white] |= bitboards[i];
        occupancies[black] |= bitboards[i + 6];
    }

    occupancies[both] = occupancies[white] | occupancies[black];
    hashKey = GeneratePosKey();
    
}


int ParseMove(std::string moveInStr) {
    int sourceSquare = (moveInStr[0] - 'a') + (8 - (moveInStr[1] - '0')) * 8; 
    int targetSquare = (moveInStr[2] - 'a') + (8 - (moveInStr[3] - '0')) * 8; 

    MoveList moveList[1];
    MoveGen(moveList);

    for (int moveCount = 0; moveCount < moveList->count; ++moveCount) {
        int move = moveList->moves[moveCount];



		if (sourceSquare == GetMoveSource(move) && targetSquare == GetMoveTarget(move)) {
            
			int promotedPiece = GetMovePromotion(move);

			if (promotedPiece) {
				if ((promotedPiece == Pieces::Q || promotedPiece == Pieces::q) && moveInStr[4] == 'q')
					return move;
				else if ((promotedPiece == Pieces::R || promotedPiece == Pieces::r) && moveInStr[4] == 'r')
					return move;
				else if ((promotedPiece == Pieces::B || promotedPiece == Pieces::b) && moveInStr[4] == 'b')
					return move;
				else if ((promotedPiece == Pieces::N || promotedPiece == Pieces::n) && moveInStr[4] == 'n')
					return move;

				continue;
			}

            

			return move;
		}
    }
    return 0;
}


void ParseMoves(std::string& moveString) {
    
    std::vector<std::string> moveTokens = SplitString(moveString);

    for (size_t i = 0; i < moveTokens.size(); ++i) {
        int move = ParseMove(moveTokens[i]);
        MakeMove(move);
    }

}


void HandlePosition(std::string& input) {

    if (input.find("startpos") != std::string::npos) {
        ParseFen(startPos);
    } else if (input.find("fen") != std::string::npos) {
        ParseFen(input.substr(input.find("fen") + 4, std::string::npos));
    } 
    

    if (input.find("moves") != std::string::npos) {
        
        if (input[input.find("moves") + 6] >= 'a' && input[input.find("moves") + 6] <= 'h') {
            std::string moveString = input.substr(input.find("moves") + 6, std::string::npos);
            ParseMoves(moveString);
        }
        
    }

    // PrintBoard();
}


void HandleGo(std::string& input) {

    int maxDepth = 64;
    int wtime = 800000000;
    int btime = 800000000;
    int winc = 80;
    int binc = 80;

    std::vector<std::string> splitGo = SplitString(input);

    for (size_t i = 0; i < splitGo.size(); ++i) {
        if (splitGo[i] == "wtime") {
            try {
                wtime = std::stoi(splitGo[i + 1]); 
            } catch (...) { 
                std::cout << "Invalid time" << std::endl;
            }
        }

        if (splitGo[i] == "btime") {
            try {
                btime = std::stoi(splitGo[i + 1]); 
            } catch (...) { 
                std::cout << "Invalid time" << std::endl;
            }
        }

        if (splitGo[i] == "winc") {
            try {
                winc = std::stoi(splitGo[i + 1]); 
            } catch (...) { 
                std::cout << "Invalid time" << std::endl;
            }
        }

        if (splitGo[i] == "binc") {
            try {
                binc = std::stoi(splitGo[i + 1]); 
            } catch (...) { 
                std::cout << "Invalid increment" << std::endl;
            }
        }

        if (splitGo[i] == "depth") {
            try {
                maxDepth = std::stoi(splitGo[i + 1]); 
            } catch (...) { 
                std::cout << "Invalid depth" << std::endl;
            }
        }
    }

    

    SearchPosition(maxDepth, (side == white) ? wtime : btime, (side == white) ? winc : binc);

}



void UciLoop() {
    
    while (true) {
        std::string input;
        std::getline(std::cin, input);

        if (input.empty()) {
            continue;
        }

        std::vector<std::string> commands = SplitString(input);
        
        if (commands[0] == "uci") {
            std::cout << "id name Shumi" << std::endl;
            std::cout << "id author ykkk" << std::endl; 
            std::cout << "uciok" << std::endl; 
        } else if (commands[0] == "isready") {
            std::cout << "readyok" << std::endl; 
        } else if (commands[0] == "quit") {
            break;
        } else if (commands[0] == "position") {
            HandlePosition(input);
        } else if (commands[0] == "go") {
            HandleGo(input);
        } else if (commands[0] == "ucinewgame") {
            ClearTT();
            ParseFen(startPos);
        } else {
            std::cout << "Unknown command" << std::endl; 
        }

    }

}