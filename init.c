#include "defs.h"

// Initialize the arrays
int Sq120ToSq64[NUM_SQ];
int Sq64ToSq120[64];

U64 SetMask[64]; // Set mask for each square
U64 ClearMask[64]; // Clear mask for each square

void InitBitMasks() {
    int index = 0;
    
    // First initialize all 64 positions in both arrays to have no bits set (all zeros)
    // Example: 0000 0000 for each position
    for(index = 0; index < 64; index++) {
        SetMask[index] = 0ULL;
        ClearMask[index] = 0ULL;
    }

    // For each square (0-63):
    // SetMask: Change just one bit from 0 to 1 at the square's position
    // Example for index 3: 0000 1000 (fourth bit is 1, rest are 0)
    // ClearMask: Flip all bits in SetMask (so index position is 0, rest are 1)
    // Example for index 3: 1111 0111 (fourth bit is 0, rest are 1)
    for(index = 0; index < 64; index++) {
        SetMask[index] |= (1ULL << index);
        ClearMask[index] = ~SetMask[index];
    }
}

void InitSq120To64() {
    int index = 0;
    int file = FILE_A;
    int rank = RANK_1;
    int sq = A1;
    int sq64 = 0;
    
    for(index = 0; index < NUM_SQ; index++) {
        Sq120ToSq64[index] = 65;
    }

    for(index = 0; index < 64; index++) {
        Sq64ToSq120[index] = 120;
    }

    for(rank = RANK_1; rank <= RANK_8; rank++) {
        for(file = FILE_A; file <= FILE_H; file++) {
            sq = FrToSq(file, rank);
            Sq64ToSq120[sq64] = sq;
            Sq120ToSq64[sq] = sq64;
            sq64++;
        }
    }
}

void AllInit() {
    InitSq120To64();
    InitBitMasks();
}