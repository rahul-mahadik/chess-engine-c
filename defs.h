#ifndef DEFS_H
#define DEFS_H

#include <stdlib.h>

// Debug assertion macro
#define DEBUG

#ifndef DEBUG
#define ASSERT(x)
#else
#define ASSERT(x) \
if (!(x)) { \
printf("Assertion failed: %s, file %s, line %d\n", #x, __FILE__, __LINE__); \
exit(1); \
}
#endif

// 64-bit unsigned integer type for bitboards
typedef unsigned long long U64;

// Basic constants and enums
#define NAME "Chess Engine V1"
#define NUM_SQ 120 // 120 squares for board representation
#define MAX_MOVES 2048 // Maximum number of moves in a game

#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" // Starting position FEN

// Piece representation
enum {
    EMPTY, wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ, bK
};

// Chess board coordinates
enum { 
    FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_NONE 
};

enum { 
    RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8, RANK_NONE 
};

// Side to move
enum {
    WHITE, BLACK, BOTH
};

// Square indices for 120-square board representation
enum {
    A1 = 21, B1, C1, D1, E1, F1, G1, H1,
    A2 = 31, B2, C2, D2, E2, F2, G2, H2,
    A3 = 41, B3, C3, D3, E3, F3, G3, H3,
    A4 = 51, B4, C4, D4, E4, F4, G4, H4,
    A5 = 61, B5, C5, D5, E5, F5, G5, H5,
    A6 = 71, B6, C6, D6, E6, F6, G6, H6,
    A7 = 81, B7, C7, D7, E7, F7, G7, H7,
    A8 = 91, B8, C8, D8, E8, F8, G8, H8, NO_SQ, OFFBOARD
};

// Castling permission bits
enum {
    WKCA = 1, WQCA = 2, BKCA = 4, BQCA = 8
};

// Boolean values
enum { 
    FALSE, TRUE 
};

typedef struct {
    int move;
    int score;
} MOVE;

// Structure to store moves for undo functionality
typedef struct {
    int move;
    int castlePerm;
    int enPas;
    int fiftyMove;
    U64 posKey; // Unique position key
} UNDO;

// Main board structure containing all game state
typedef struct {
    int pieces[NUM_SQ];    // Piece placement on 120-square board
    U64 pawns[3];         // Bitboards for white, black, and both pawns
    int KingSq[2];        // King positions for both sides

    int side;             // Side to move
    int enPas;            // En passant square
    int fiftyMove;        // Fifty move counter
    int ply;              // Half moves in current search
    int hisPly;           // Total half moves played
    int castlePerm;       // Castling permissions
    U64 posKey;           // Unique position key

    int pceNum[13];       // Number of each piece type
    int bigPce[2];        // Number of non-pawn pieces per side
    int majPce[2];        // Number of major pieces per side
    int minPce[2];        // Number of minor pieces per side
    int material[2];      // Material count per side
    int pList[13][10];    // Piece list: tracks locations of each piece type

    UNDO history[MAX_MOVES]; // Move history
} BOARD;

// Utility macros
#define FrToSq(f,r) ( (21 + (f) + (r) * 10) )  // Convert file/rank to 120-square index
#define Sq64(sq120) Sq120ToSq64[sq120]        // Convert 120-square to 64-square index
#define Sq120(sq64) Sq64ToSq120[sq64]        // Convert 64-square to 120-square index
#define POP(b) PopBit(b)                        // Pop least significant bit from bitboard
#define COUNT(b) CountBits(b)                   // Count bits in bitboard
#define SET_BIT(bb, sq) bb |= SetMask[sq]      // Set bit in bitboard
#define CLEAR_BIT(bb, sq) bb &= ClearMask[sq]  // Clear bit in bitboard
#define IsBQ(pce) (PceBishopQueen[pce])
#define IsRQ(pce) (PceRookQueen[pce])
#define IsKi(pce) (PceKing[pce])
#define IsKn(pce) (PceKnight[pce])

// Move bit operations - move is stored in a single 32-bit integer
// bits 0-6: From square (7 bits)
// bits 7-13: To square (7 bits)  
// bits 14-17: Captured piece (4 bits)
// bits 18-19: Reserved
// bits 20-23: Promoted piece (4 bits)
// bits 24-27: Move flags
#define FROMSQ(m) ((m) & 0x7F)
#define TOSQ(m) ((m) >> 7 & 0x7F)
#define CAPTURED(m) ((m) >> 14 & 0xF)
#define PROMOTED(m) ((m) >> 20 & 0xF)

// Move flag bits
#define FLAG_EP 0x40000        // En passant capture
#define FLAG_CASTLE 0x80000    // Castle move
#define FLAG_PAWNSTART 0x1000000  // Pawn first move
#define FLAG_CAPTURE 0x7C000   // Capture
#define FLAG_PROMOTE 0xF00000  // Promotion

// Globals
extern int Sq120ToSq64[NUM_SQ];
extern int Sq64ToSq120[64];
extern U64 SetMask[64];
extern U64 ClearMask[64];
extern U64 PieceKeys[13][120];  
extern U64 SideKey;            
extern U64 CastleKeys[16];      
extern char PceChar[];
extern char RankChar[];
extern char FileChar[];
extern char SideChar[];

extern int PceBig[13];
extern int PceMin[13];
extern int PceMaj[13];
extern int PceVal[13];
extern int PceColor[13];

extern int FilesBoard[NUM_SQ];
extern int RanksBoard[NUM_SQ];

extern int PceKnight[13];
extern int PceBishopQueen[13];
extern int PceRookQueen[13];
extern int PceKing[13];

// Function declarations
    // init.c
extern void AllInit();

    // bitboards.c
extern void PrintBitBoard(U64 bitBoard);
extern int PopBit(U64 *bitBoard);
extern int CountBits(U64 bitBoard);

    // hashkeys.c
extern U64 GenerateHashKey(const BOARD *pos);

    // board.c
extern void ResetBoard(BOARD *pos);
extern int ParseFen(char *fen, BOARD *pos);
extern void PrintBoard(const BOARD *pos);
extern void UpdateMaterial(BOARD *pos);
extern int CheckBoard(const BOARD *pos);

    // attack.c
extern int SqAttacked(const int sq, const int side, const BOARD *pos);

    // io.c
extern char *PrMove(const int move);
extern char *PrSq(const int sq);

#endif
