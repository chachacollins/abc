#include "defs.h"

char *square_to_coords[] = {
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8", "i8", "j8", "k8", "l8", "m8", "n8", "o8", "p8",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7", "i7", "j7", "k7", "l7", "m7", "n7", "o7", "p7",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6", "i6", "j6", "k6", "l6", "m6", "n6", "o6", "p6",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5", "i5", "j5", "k5", "l5", "m5", "n5", "o5", "p5",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4", "i4", "j4", "k4", "l4", "m4", "n4", "o4", "p4",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3", "i3", "j3", "k3", "l3", "m3", "n3", "o3", "p3",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2", "i2", "j2", "k2", "l2", "m2", "n2", "o2", "p2",
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1", "i1", "j1", "k1", "l1", "m1", "n1", "o1", "p1"
};

int char_pieces[] = {
    ['P'] = P, ['N'] = N, ['B'] = B, ['R'] = R, ['Q'] = Q, ['K'] = K,
    ['p'] = p, ['n'] = n, ['b'] = b, ['r'] = r, ['q'] = q, ['k'] = k
};

int promoted_pieces[] = {
    [Q] = 'q', [R] = 'r', [B] = 'b', [N] = 'n',
    [q] = 'q', [r] = 'r', [b] = 'b', [n] = 'n'
};

int move_offsets[7][8] = {
    { 0 }, { -16, -1, 16, 1, -17, -15, 17, 15 },
    { 0 }, { -33, -31, 33, 31, -18, -14, 18, 14 },
    {-17, -15, 17, 15}, {-16, -1, 16, 1},
    {-16, -1, 16, 1, -17, -15, 17, 15}
};

int castling_rights[128] = {
     7, 15, 15, 15,  3, 15, 15, 11,  o, o, o, o, o, o, o, o,
    15, 15, 15, 15, 15, 15, 15, 15,  o, o, o, o, o, o, o, o,
    15, 15, 15, 15, 15, 15, 15, 15,  o, o, o, o, o, o, o, o,
    15, 15, 15, 15, 15, 15, 15, 15,  o, o, o, o, o, o, o, o,
    15, 15, 15, 15, 15, 15, 15, 15,  o, o, o, o, o, o, o, o,
    15, 15, 15, 15, 15, 15, 15, 15,  o, o, o, o, o, o, o, o,
    15, 15, 15, 15, 15, 15, 15, 15,  o, o, o, o, o, o, o, o,
    13, 15, 15, 15, 12, 15, 15, 14,  o, o, o, o, o, o, o, o
};

const int material_score[13] = { // TODO: fix piece order!!!
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

int offset_length[7] = {0, 8, 0, 8, 4, 4, 8};
int castling_side[2][2] = {{1, 2}, {4, 8}};
int pawn_starting_rank[] = {0x60, 0x10};
int pawn_promoting_rank[] = {0x00, 0x70};
int board[128];
int side = WHITE;
int enpassant = no_sq;
int castle = 15;
int king_square[2] = {e1, e8};