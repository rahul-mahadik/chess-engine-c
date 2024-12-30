#include "defs.h"
#include "stdio.h"
// Generates a Zobrist hash key for the current board position
// Zobrist hashing creates a unique key by XORing random values for:
// - Each piece on each square
// - Side to move
// - En passant square
// - Castling rights
U64 GenerateHashKey(const BOARD *pos) {

    int sq = 0;
    U64 finalKey = 0;

    int pce = EMPTY;
    
    // XOR random values for each piece on the board
    for(sq = 0; sq < NUM_SQ; sq++) {
        pce = pos->pieces[sq];
        if(pce != NO_SQ && pce != EMPTY && pce != OFFBOARD) {
            ASSERT(pce >= wP && pce <= bK);
            finalKey ^= PieceKeys[pce][sq];
        }
    }

    // XOR if White to move (Black is default)
    if(pos->side == WHITE) {
        finalKey ^= SideKey;
    }

    // XOR for en passant square if one exists
    if(pos->enPas != NO_SQ) {
        ASSERT(pos->enPas >= 0 && pos->enPas < NUM_SQ);
        finalKey ^= PieceKeys[EMPTY][pos->enPas];
    }

    // XOR for current castling permissions
    ASSERT(pos->castlePerm >= 0 && pos->castlePerm <= 15);
    finalKey ^= CastleKeys[pos->castlePerm];
    return finalKey;
}