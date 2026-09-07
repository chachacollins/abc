#include "defs.h"

int evaluate_position() {
    int score = 0;
    for (int square = 0; square < 128; square++) {
        if (!(square & 0x88)) {
            int piece = board[square];
            score += material_score[piece];
			switch(piece) {
				case P: 
				    score += pawn_score[square];
				    if (board[square - 16] == P) score -= 100;
				    break;
				case N: score += knight_score[square]; break;
				case B: score += bishop_score[square]; break;
				case R: score += rook_score[square]; break;
				case K: score += king_score[square]; break;
				case p:
				    score -= pawn_score[mirror_score[square]];
				    if (board[square + 16] == p) score += 100;
				    break;
				case n: score -= knight_score[mirror_score[square]]; break;
				case b: score -= bishop_score[mirror_score[square]]; break;
				case r: score -= rook_score[mirror_score[square]]; break;
				case k: score -= king_score[mirror_score[square]]; break;
			}
            
        }
    } return !side ? score : -score;
}