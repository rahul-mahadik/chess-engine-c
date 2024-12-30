#include "defs.h"
#include "stdlib.h"

// Arrays for mapping between 120-square board (includes padding) and 64-square board (just chess squares)
int Sq120ToSq64[NUM_SQ];
int Sq64ToSq120[64];

// Bitboard masks for bit manipulation operations
U64 SetMask[64];
U64 ClearMask[64];

// Generates a 64-bit random number by combining multiple rand() calls
#define RAND_64 ((U64)rand() | \
                (((U64)rand() << 15) | \
                ((U64)rand() << 30) | \
                ((U64)rand() << 45) | \
                (((U64)rand() & 0xf) << 60)))
                
// Zobrist hash keys for efficient position comparison (XORing these keys creates unique position signatures)
U64 PieceKeys[13][120];  // Random key for each piece type on each square
U64 SideKey;             // Random key for side to move
U64 CastleKeys[16];      // Random keys for castling rights

// Track file (A-H) and rank (1-8) for each square
int FilesBoard[NUM_SQ];
int RanksBoard[NUM_SQ];

// Initialize file and rank lookup arrays
void InitFilesRanksBoard() {
	int index = 0;
	int file = FILE_A;
	int rank = RANK_1;
	int sq = A1;

	// Mark all squares as off board initially
	for(index = 0; index < NUM_SQ; ++index) {
		FilesBoard[index] = OFFBOARD;
		RanksBoard[index] = OFFBOARD;
	}

	// Set file and rank values for valid squares
	for(rank = RANK_1; rank <= RANK_8; ++rank) {
		for(file = FILE_A; file <= FILE_H; ++file) {
			sq = FrToSq(file,rank);
			FilesBoard[sq] = file;
			RanksBoard[sq] = rank;
		}
	}
}

// Initialize Zobrist hash keys for position hashing
void InitHashKeys() {
    int index_i = 0;
    int index_j = 0;
    
    for(index_i = 0; index_i < 13; index_i++) {
        for(index_j = 0; index_j < 120; index_j++) {
            PieceKeys[index_i][index_j] = RAND_64;
        }
    }
    SideKey = RAND_64;
    for(index_i = 0; index_i < 16; index_i++) {
        CastleKeys[index_i] = RAND_64;
    }
}

// Initialize bit masks for bitboard operations
void InitBitMasks() {
    int index = 0;
    
    // Initialize masks to zero
    for(index = 0; index < 64; index++) {
        SetMask[index] = 0ULL;
        ClearMask[index] = 0ULL;
    }

    // Create masks for each square:
    // SetMask: 1 at square position, 0 elsewhere
    // ClearMask: 0 at square position, 1 elsewhere
    for(index = 0; index < 64; index++) {
        SetMask[index] |= (1ULL << index);
        ClearMask[index] = ~SetMask[index];
    }
}

// Initialize lookup tables for square mapping
void InitSq120To64() {
    int index = 0;
    int file = FILE_A;
    int rank = RANK_1;
    int sq = A1;
    int sq64 = 0;
    
    // Set invalid default values
    for(index = 0; index < NUM_SQ; index++) {
        Sq120ToSq64[index] = 65;
    }
    for(index = 0; index < 64; index++) {
        Sq64ToSq120[index] = 120;
    }

    // Create bidirectional mapping between board representations
    for(rank = RANK_1; rank <= RANK_8; rank++) {
        for(file = FILE_A; file <= FILE_H; file++) {
            sq = FrToSq(file, rank);
            Sq64ToSq120[sq64] = sq;
            Sq120ToSq64[sq] = sq64;
            sq64++;
        }
    }
}

// Initialize all lookup tables and arrays
void AllInit() {
    InitSq120To64();
    InitBitMasks();
    InitHashKeys();
    InitFilesRanksBoard();
}