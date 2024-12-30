#include "defs.h"

// Piece characters for board visualization (empty, Pawn, kNight, Bishop, Rook, Queen, King)
char PceChar[] = ".PNBRQKpnbrqk";
// Characters for ranks 1-8
char RankChar[] = "12345678"; 
// Characters for files a-h
char FileChar[] = "abcdefgh";
// Characters for side to move (White, Black, Both)
char SideChar[] = "wb-";

// Arrays indexed by piece type (13 pieces including empty):
int PceBig[13] = { FALSE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE }; // Is piece "big" (all except pawns)
int PceMin[13] = { FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, TRUE, TRUE, TRUE }; // Is minor piece (bishop/knight)
int PceMaj[13] = { FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, FALSE }; // Is major piece (rook/queen)
int PceVal[13]= { 0, 100, 325, 325, 550, 1000, 50000, 100, 325, 325, 550, 1000, 50000  }; // Standard piece values
int PceColor[13] = { BOTH, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK }; // Color of each piece