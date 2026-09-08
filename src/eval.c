#include "abc.h"

int evaluate_position() {
    int score = 0;
    for (int square = 0; square < 128; square++) {
        if (!(square & 0x88)) {
            int piece = board[square];
            score += material_score[piece];
			switch(piece) {
				case WP: 
				    score += pawn_score[square];
				    if (board[square - 16] == WP) score -= 100;
				    break;
				case WN: score += knight_score[square]; break;
				case WB: score += bishop_score[square]; break;
				case WR: score += rook_score[square]; break;
				case WK: score += king_score[square]; break;
				case BP:
				    score -= pawn_score[mirror_score[square]];
				    if (board[square + 16] == BP) score += 100;
				    break;
				case BN: score -= knight_score[mirror_score[square]]; break;
				case BB: score -= bishop_score[mirror_score[square]]; break;
				case BR: score -= rook_score[mirror_score[square]]; break;
				case BK: score -= king_score[mirror_score[square]]; break;
			}
            
        }
    } return !side ? score : -score;
}