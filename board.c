#include "defs.h"
#include "stdio.h"

// Validate the board state
int CheckBoard(const BOARD *pos) {
    int temp_PceNum[13] = {0};
    int temp_BigPce[2] = {0};
    int temp_MajPce[2] = {0};
    int temp_MinPce[2] = {0};
    int temp_Material[2] = {0};

    int sq64, temp_pce, temp_pce_num, sq120, color, pawn_count;
    U64 temp_pawns[3] = {0ULL, 0ULL, 0ULL};

    temp_pawns[WHITE] = pos->pawns[WHITE];
    temp_pawns[BLACK] = pos->pawns[BLACK];
    temp_pawns[BOTH] = pos->pawns[BOTH];

    // Validate piece positions
    for(temp_pce = wP; temp_pce < bK; temp_pce++) {
        for(temp_pce_num = 0; temp_pce_num < pos->pceNum[temp_pce]; temp_pce_num++) {
            sq120 = pos->pList[temp_pce][temp_pce_num];
            ASSERT(pos->pieces[sq120] == temp_pce);
        }
    }

    // Count pieces and validate material
    for (sq64 = 0; sq64 < 64; sq64++) {
        sq120 = Sq120(sq64);
        temp_pce = pos->pieces[sq120];
        temp_PceNum[temp_pce]++;
        color = PceColor[temp_pce];
        if (PceBig[temp_pce]) temp_BigPce[color]++;
        if (PceMaj[temp_pce]) temp_MajPce[color]++;
        if (PceMin[temp_pce]) temp_MinPce[color]++;
        temp_Material[color] += PceVal[temp_pce];
    }

    // Validate piece counts
    for(temp_pce = wP; temp_pce <= bK; ++temp_pce) {
        ASSERT(temp_PceNum[temp_pce] == pos->pceNum[temp_pce]);
    }

    // Validate pawn counts
    pawn_count = COUNT(temp_pawns[WHITE]);
    ASSERT(pawn_count == pos->pceNum[wP]);
    pawn_count = COUNT(temp_pawns[BLACK]);
    ASSERT(pawn_count == pos->pceNum[bP]);
    pawn_count = COUNT(temp_pawns[BOTH]);
    ASSERT(pawn_count == (pos->pceNum[bP] + pos->pceNum[wP]));

    // Validate pawn positions
    while(temp_pawns[WHITE]) {
        sq64 = POP(&temp_pawns[WHITE]);
        ASSERT(pos->pieces[Sq120(sq64)] == wP);
    }
    while(temp_pawns[BLACK]) {
        sq64 = POP(&temp_pawns[BLACK]);
        ASSERT(pos->pieces[Sq120(sq64)] == bP);
    }
    while(temp_pawns[BOTH]) {
        sq64 = POP(&temp_pawns[BOTH]);
        ASSERT((pos->pieces[Sq120(sq64)] == bP) || (pos->pieces[Sq120(sq64)] == wP));
    }

    // Validate material, piece counts, and other board state
    ASSERT(temp_Material[WHITE] == pos->material[WHITE] && temp_Material[BLACK] == pos->material[BLACK]);
    ASSERT(temp_MinPce[WHITE] == pos->minPce[WHITE] && temp_MinPce[BLACK] == pos->minPce[BLACK]);
    ASSERT(temp_MajPce[WHITE] == pos->majPce[WHITE] && temp_MajPce[BLACK] == pos->majPce[BLACK]);
    ASSERT(temp_BigPce[WHITE] == pos->bigPce[WHITE] && temp_BigPce[BLACK] == pos->bigPce[BLACK]);

    ASSERT(pos->side == WHITE || pos->side == BLACK);
    ASSERT(GenerateHashKey(pos) == pos->posKey);
    ASSERT(pos->enPas == NO_SQ || (RanksBoard[pos->enPas] == RANK_6 && pos->side == WHITE) || (RanksBoard[pos->enPas] == RANK_3 && pos->side == BLACK));
    ASSERT(pos->pieces[pos->KingSq[WHITE]] == wK);
    ASSERT(pos->pieces[pos->KingSq[BLACK]] == bK);
    ASSERT(pos->castlePerm >= 0 && pos->castlePerm <= 15);

    return TRUE;
}

// Update material counts and piece lists for the given board position
void UpdateMaterial(BOARD *pos) {
    int pce, sq, idx, color;

    // Loop through all squares on the board
    for(idx = 0; idx < NUM_SQ; idx++) {
        sq = idx;
        pce = pos->pieces[idx];
        if(pce != EMPTY && pce != OFFBOARD) {
            color = PceColor[pce];
            pos->material[color] += PceVal[pce]; // Add piece value to material count
            if(PceBig[pce]) pos->bigPce[color]++; // Increment big piece count
            if(PceMaj[pce]) pos->majPce[color]++; // Increment major piece count
            if(PceMin[pce]) pos->minPce[color]++; // Increment minor piece count

            // Add piece to piece list and increment piece count
            pos->pList[pce][pos->pceNum[pce]] = sq;
            pos->pceNum[pce]++;

            // Update king square positions
            if(pce == wK) pos->KingSq[WHITE] = sq;
            if(pce == bK) pos->KingSq[BLACK] = sq;

            // Update pawn bitboards
            if(pce == wP) {
                SET_BIT(pos->pawns[WHITE], Sq64(sq));
                SET_BIT(pos->pawns[BOTH], Sq64(sq));
            } else if(pce == bP) {
                SET_BIT(pos->pawns[BLACK], Sq64(sq));
                SET_BIT(pos->pawns[BOTH], Sq64(sq));
            }
        }
    }
}

// Parse FEN string and set up board position
int ParseFen(char *fen, BOARD *pos) {
    ASSERT(fen);
    ASSERT(pos);

    int sq = 0, piece = 0, count = 0, rank = RANK_8, file = FILE_A;
    int sq64 = 0, sq120 = 0;

    ResetBoard(pos); // Reset board to initial empty state

    // Loop through FEN string to set up pieces
    while ((rank >= RANK_1) && (*fen)) {
        count = 1;
        // Convert FEN characters to piece codes
        switch (*fen) {
            case 'p': piece = bP; break;
            case 'r': piece = bR; break;
            case 'n': piece = bN; break;
            case 'b': piece = bB; break;
            case 'q': piece = bQ; break;
            case 'k': piece = bK; break;
            case 'P': piece = wP; break;
            case 'R': piece = wR; break;
            case 'N': piece = wN; break;
            case 'B': piece = wB; break;
            case 'Q': piece = wQ; break;
            case 'K': piece = wK; break;
            // Handle empty squares
            case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8':
                piece = EMPTY; count = *fen - '0'; break;
            // Handle rank separators and spaces
            case '/': case ' ':
                rank--; file = FILE_A; fen++; continue;
            default: printf("Parsing FEN error\n"); return -1;
        }
        // Place pieces on the board
        for (sq = 0; sq < count; sq++) {
            sq64 = rank * 8 + file;
            sq120 = Sq120(sq64);
            if (piece != EMPTY) {
                pos->pieces[sq120] = piece;
            }
            file++;
        }
        fen++;
    }
    ASSERT(*fen == 'w' || *fen == 'b');
    pos->side = (*fen == 'w') ? WHITE : BLACK; // Set side to move
    fen += 2;
    
    // Parse castling permissions
    for (sq = 0; sq < 4; sq++) {
        if (*fen == ' ') break;
        switch (*fen) {
            case 'K': pos->castlePerm |= WKCA; break;
            case 'Q': pos->castlePerm |= WQCA; break;
            case 'k': pos->castlePerm |= BKCA; break;
            case 'q': pos->castlePerm |= BQCA; break;
            default: break;
        }
        fen++;
    }
    fen++;
    ASSERT(pos->castlePerm >= 0 && pos->castlePerm <= 15);
    
    // Parse en passant square
    if (*fen != '-') {
        file = fen[0] - 'a';
        rank = fen[1] - '1';
        ASSERT(file >= FILE_A && file <= FILE_H);
        ASSERT(rank >= RANK_1 && rank <= RANK_8);
        pos->enPas = FrToSq(file, rank);
    }
    pos->posKey = GenerateHashKey(pos); // Generate position hash key
    UpdateMaterial(pos);
    return 0;
}

// Reset board to initial empty state
void ResetBoard(BOARD *pos) {
    int sq = 0;

    // Mark all squares as off board initially
    for(sq = 0; sq < NUM_SQ; sq++) {
        pos->pieces[sq] = OFFBOARD;
    }

    // Set valid squares to empty
    for(sq = 0; sq < 64; sq++) {
        pos->pieces[Sq120(sq)] = EMPTY;
    }

    // Clear piece counts and bitboards
    for(sq = 0; sq < 3; sq++) {
        pos->bigPce[sq] = 0;
        pos->majPce[sq] = 0;
        pos->minPce[sq] = 0;
        pos->pawns[sq] = 0ULL;
    }

    // Clear piece counts array
    for(sq = 0; sq < 13; sq++) {
        pos->pceNum[sq] = 0;
    }

    // Reset king positions
    pos->KingSq[WHITE] = NO_SQ;
    pos->KingSq[BLACK] = NO_SQ;

    // Reset game state variables
    pos->side = BOTH;
    pos->enPas = NO_SQ;
    pos->fiftyMove = 0;
    pos->ply = 0;
    pos->hisPly = 0;
    pos->castlePerm = 0;
    pos->posKey = 0ULL;
}

// Print the current board position
void PrintBoard(const BOARD *pos) {
    int sq, file, rank, piece;

    printf("\nGame Board:\n\n");

    // Print board ranks from top (8) to bottom (1)
    for(rank = RANK_8; rank >= RANK_1; rank--) {
        printf("%d ", rank + 1);
        for(file = FILE_A; file <= FILE_H; file++) {
            sq = FrToSq(file, rank);
            piece = pos->pieces[sq];
            printf("%3c ", PceChar[piece]);
        }
        printf("\n");
    }
    printf("\n  ");
    // Print file labels (a-h)
    for(file = FILE_A; file <= FILE_H; file++) {
        printf("%3c ", FileChar[file]);
    }
    printf("\n");
    // Print side to move, en passant square, castling permissions, and position key
    printf("Side: %c\n", SideChar[pos->side]);
    printf("En Passant: %d\n", pos->enPas);
    printf("Castle Perm:%c%c%c%c\n", 
        (pos->castlePerm & WKCA) ? 'K' : '-',
        (pos->castlePerm & WQCA) ? 'Q' : '-',
        (pos->castlePerm & BKCA) ? 'k' : '-',
        (pos->castlePerm & BQCA) ? 'q' : '-');
    printf("Pos Key: %llX\n", pos->posKey);
}