#include "defs.h"

int material_score[13] = { // TODO: fix piece order!!!
      0,      // empty square score
    100,      // WHITE PAWN score
    300,      // WHITE KNIGHT scrore
    350,      // WHITE BISHOP score
    500,      // WHITE ROOK score
   1000,      // WHITE QUEEN score
  10000,      // WHITE KING score
   -100,      // BLACK PAWN score
   -300,      // BLACK KNIGHT scrore
   -350,      // BLACK BISHOP score
   -500,      // BLACK ROOK score
  -1000,      // BLACK QUEEN score
 -10000,      // BLACK KING score
    
};

const int pawn_score[128] =  {
    90,  90,  90,  90,  90,  90,  90,  90,    o, o, o, o, o, o, o, o,
    30,  30,  30,  40,  40,  30,  30,  30,    o, o, o, o, o, o, o, o,
    20,  20,  20,  30,  30,  30,  20,  20,    o, o, o, o, o, o, o, o,
    10,  10,  10,  20,  20,  10,  10,  10,    o, o, o, o, o, o, o, o,
     5,   5,  10,  20,  20,   5,   5,   5,    o, o, o, o, o, o, o, o,
     0,   0,   0,   5,   5,   0,   0,   0,    o, o, o, o, o, o, o, o,
     0,   0,   0, -10, -10,   0,   0,   0,    o, o, o, o, o, o, o, o,
     0,   0,   0,   0,   0,   0,   0,   0,    o, o, o, o, o, o, o, o
};

const int knight_score[128] =  {
    -5,   0,   0,   0,   0,   0,   0,  -5,    o, o, o, o, o, o, o, o,
    -5,   0,   0,  10,  10,   0,   0,  -5,    o, o, o, o, o, o, o, o,
    -5,   5,  20,  20,  20,  20,   5,  -5,    o, o, o, o, o, o, o, o,
    -5,  10,  20,  30,  30,  20,  10,  -5,    o, o, o, o, o, o, o, o,
    -5,  10,  20,  30,  30,  20,  10,  -5,    o, o, o, o, o, o, o, o,
    -5,   5,  20,  10,  10,  20,   5,  -5,    o, o, o, o, o, o, o, o,
    -5,   0,   0,   0,   0,   0,   0,  -5,    o, o, o, o, o, o, o, o,
    -5, -10,   0,   0,   0,   0, -10,  -5,    o, o, o, o, o, o, o, o
};

const int bishop_score[128] =  {
     0,   0,   0,   0,   0,   0,   0,   0,    o, o, o, o, o, o, o, o,
     0,   0,   0,   0,   0,   0,   0,   0,    o, o, o, o, o, o, o, o,
     0,   0,   0,  10,  10,   0,   0,   0,    o, o, o, o, o, o, o, o,
     0,   0,  10,  20,  20,  10,   0,   0,    o, o, o, o, o, o, o, o,
     0,   0,  10,  20,  20,  10,   0,   0,    o, o, o, o, o, o, o, o,
     0,  10,   0,   0,   0,   0,  10,   0,    o, o, o, o, o, o, o, o,
     0,  30,   0,   0,   0,   0,  30,   0,    o, o, o, o, o, o, o, o,
     0,   0, -10,   0,   0, -10,   0,   0,    o, o, o, o, o, o, o, o

};

const int rook_score[128] = {
    50,  50,  50,  50,  50,  50,  50,  50,    o, o, o, o, o, o, o, o,
    50,  50,  50,  50,  50,  50,  50,  50,    o, o, o, o, o, o, o, o,
     0,   0,  10,  20,  20,  10,   0,   0,    o, o, o, o, o, o, o, o,
     0,   0,  10,  20,  20,  10,   0,   0,    o, o, o, o, o, o, o, o,
     0,   0,  10,  20,  20,  10,   0,   0,    o, o, o, o, o, o, o, o,
     0,   0,  10,  20,  20,  10,   0,   0,    o, o, o, o, o, o, o, o,
     0,   0,  10,  20,  20,  10,   0,   0,    o, o, o, o, o, o, o, o,
     0,   0,   0,  20,  20,   0,   0,   0,    o, o, o, o, o, o, o, o

};

const int king_score[128] =  {
     0,   0,   0,   0,   0,   0,   0,   0,    o, o, o, o, o, o, o, o,
     0,   0,   5,   5,   5,   5,   0,   0,    o, o, o, o, o, o, o, o,
     0,   5,   5,  10,  10,   5,   5,   0,    o, o, o, o, o, o, o, o,
     0,   5,  10,  20,  20,  10,   5,   0,    o, o, o, o, o, o, o, o,
     0,   5,  10,  20,  20,  10,   5,   0,    o, o, o, o, o, o, o, o,
     0,   0,   5,  10,  10,   5,   0,   0,    o, o, o, o, o, o, o, o,
     0,   5,   5,  -5,  -5,   0,   5,   0,    o, o, o, o, o, o, o, o,
     0,   0,   5,   0, -15,   0,  10,   0,    o, o, o, o, o, o, o, o

};

const int mirror_score[128] = {
	a1, b1, c1, d1, e1, f1, g1, h1,    o, o, o, o, o, o, o, o,
	a2, b2, c2, d2, e2, f2, g2, h2,    o, o, o, o, o, o, o, o,
	a3, b3, c3, d3, e3, f3, g3, h3,    o, o, o, o, o, o, o, o,
	a4, b4, c4, d4, e4, f4, g4, h4,    o, o, o, o, o, o, o, o,
	a5, b5, c5, d5, e5, f5, g5, h5,    o, o, o, o, o, o, o, o,
	a6, b6, c6, d6, e6, f6, g6, h6,    o, o, o, o, o, o, o, o,
	a7, b7, c7, d7, e7, f7, g7, h7,    o, o, o, o, o, o, o, o,
	a8, b8, c8, d8, e8, f8, g8, h8,    o, o, o, o, o, o, o, o
};

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