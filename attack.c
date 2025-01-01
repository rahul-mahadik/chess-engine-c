#include "stdio.h"
#include "defs.h"

// Move offsets for each piece type
const int KnightMoves[8] = { -8, -19, -21, -12, 8, 19, 21, 12 };
const int RookMoves[4] = { -1, -10, 1, 10 };
const int BishopMoves[4] = { -9, -11, 11, 9 };
const int KingMoves[8] = { -1, -10, 1, 10, -9, -11, 11, 9 };

// Check if a square is attacked by a given side
int SqAttacked(const int sq, const int side, const BOARD *pos) {
	int pce, idx, temp_sq, dir;

	ASSERT(CheckBoard(pos));

	// Check pawn attacks
	if (side == WHITE) {
		if (pos->pieces[sq-11] == wP || pos->pieces[sq-9] == wP) {
			return TRUE;
		}
	} else {
		if (pos->pieces[sq+11] == bP || pos->pieces[sq+9] == bP) {
			return TRUE;
		}
	}

	// Check knight attacks
	for (idx = 0; idx < 8; ++idx) {
		pce = pos->pieces[sq + KnightMoves[idx]];
		if (pce != OFFBOARD && IsKn(pce) && PceColor[pce] == side) {
			return TRUE;
		}
	}

	// Check rook and queen attacks
	for (idx = 0; idx < 4; ++idx) {
		dir = RookMoves[idx];
		temp_sq = sq + dir;
		pce = pos->pieces[temp_sq];
		while (pce != OFFBOARD) {
			if (pce != EMPTY) {
				if (IsRQ(pce) && PceColor[pce] == side) {
					return TRUE;
				}
				break;
			}
			temp_sq += dir;
			pce = pos->pieces[temp_sq];
		}
	}

	// Check bishop and queen attacks
	for (idx = 0; idx < 4; ++idx) {
		dir = BishopMoves[idx];
		temp_sq = sq + dir;
		pce = pos->pieces[temp_sq];
		while (pce != OFFBOARD) {
			if (pce != EMPTY) {
				if (IsBQ(pce) && PceColor[pce] == side) {
					return TRUE;
				}
				break;
			}
			temp_sq += dir;
			pce = pos->pieces[temp_sq];
		}
	}

	// Check king attacks
	for (idx = 0; idx < 8; ++idx) {
		pce = pos->pieces[sq + KingMoves[idx]];
		if (pce != OFFBOARD && IsKi(pce) && PceColor[pce] == side) {
			return TRUE;
		}
	}

	return FALSE;
}