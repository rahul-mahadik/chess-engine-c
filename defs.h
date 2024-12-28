#ifndef DEFS_H // Header guard to prevent multiple inclusion
#define DEFS_H

#include <stdlib.h>

// Debug assertion macro - only active when DEBUG is defined
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
#define NUM_SQ 120 // Board representation uses 120 squares (12x10 board)
#define MAX_MOVES 2048 // Maximum number of moves in a game

// Piece representation - empty square and white/black pieces
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
// First digit is rank, second digit is file (e.g. A1 = 21)
enum {
    A1 = 21, B1, C1, D1, E1, F1, G1, H1,
    A2 = 31, B2, C2, D2, E2, F2, G2, H2,
    A3 = 41, B3, C3, D3, E3, F3, G3, H3,
    A4 = 51, B4, C4, D4, E4, F4, G4, H4,
    A5 = 61, B5, C5, D5, E5, F5, G5, H5,
    A6 = 71, B6, C6, D6, E6, F6, G6, H6,
    A7 = 81, B7, C7, D7, E7, F7, G7, H7,
    A8 = 91, B8, C8, D8, E8, F8, G8, H8, NO_SQ
};

// Castling permission bits
enum {
    WKCA = 1, WQCA = 2, BKCA = 4, BQCA = 8
};

// Boolean values
enum { 
    FALSE, TRUE 
};

// Structure to store moves for undo functionality
typedef struct {
    int move;
    int castlePerm;
    int enPas;
    int fiftyMove;
    U64 posKey;
} UNDO;

// Main board structure containing all game state
typedef struct {
    // Board representation
    int pieces[NUM_SQ];    // Piece placement on 120-square board
    U64 pawns[3];         // Bitboards for white, black, and both pawns
    int KingSq[2];        // King positions for both sides

    // Game state information
    int side;             // Side to move
    int enPas;            // En passant square
    int fiftyMove;        // Fifty move counter
    int ply;              // Half moves in current search
    int hisPly;           // Total half moves played
    int castlePerm;       // Castling permissions
    U64 posKey;           // Unique position key

    // Piece counting and tracking
    int pceNum[13];       // Number of each piece type
    int bigPce[3];        // Number of non-pawn pieces per side
    int majPce[3];        // Number of major pieces (rooks, queens) per side
    int minPce[3];        // Number of minor pieces (bishops, knights) per side
    int pList[13][10];    // Piece list: tracks locations of each piece type

    // Move history
    UNDO history[MAX_MOVES];
} BOARD;

// Utility macros
#define FrToSq(f,r) ( (21 + (f) + (r) * 10) )  // Convert file/rank to 120-square index
#define Sq64(sq120) Sq120ToSq64[sq120]         // Convert 120-square to 64-square index
#define POP(b) PopBit(b)                        // Pop least significant bit from bitboard
#define COUNT(b) CountBits(b)                   // Count bits in bitboard
#define SET_BIT(bb, sq) bb |= SetMask[sq]      // Set bit in bitboard
#define CLEAR_BIT(bb, sq) bb &= ClearMask[sq]  // Clear bit in bitboard

// Globals
extern int Sq120ToSq64[NUM_SQ];
extern int Sq64ToSq120[64];
extern U64 SetMask[64];
extern U64 ClearMask[64];


// Function declarations
    // init.c
extern void AllInit();

    // bitboards.c
extern void PrintBitBoard(U64 bitBoard);
extern int PopBit(U64 *bitBoard);
extern int CountBits(U64 bitBoard);
#endif