#include "stdio.h"
#include "defs.h"

int main() {
    // Initialize lookup tables and bit masks
    AllInit();
    
    // Create empty bitboard
    U64 pbitBoard = 0ULL;

    // Set bit at D2 square and print
    SET_BIT(pbitBoard, Sq64(D2));
    PrintBitBoard(pbitBoard);

    // Clear bit at D2 square and print
    CLEAR_BIT(pbitBoard, Sq64(D2));
    PrintBitBoard(pbitBoard);

    return 0;
}
