#include "abc.h"

// Chess board representation
int board[128];
int king_square[2] = {E1, E8};
int side = WHITE;
int enpassant = NONE;
int castle = 15;

// Algebraic square names
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

// Encode FEN pieces
int char_pieces[] = {
    ['P'] = WP, ['N'] = WN, ['B'] = WB, ['R'] = WR, ['Q'] = WQ, ['K'] = WK,
    ['p'] = BP, ['n'] = BN, ['b'] = BB, ['r'] = BR, ['q'] = BQ, ['k'] = BK
};

// Extract promoted piece char
int promoted_pieces[] = {
    [WQ] = 'q', [WR] = 'r', [WB] = 'b', [WN] = 'n',
    [BQ] = 'q', [BR] = 'r', [BB] = 'b', [BN] = 'n'
};

// Visual pieces representation
char ascii_pieces[] = ".KPNBRQ--kpnbrq";

// Piece move rules
int move_offsets[7][8] = {
    { 0 }, { -16, -1, 16, 1, -17, -15, 17, 15 },
    { 0 }, { -33, -31, 33, 31, -18, -14, 18, 14 },
    { -17, -15, 17, 15}, {-16, -1, 16, 1 },
    { -16, -1, 16, 1, -17, -15, 17, 15 }
};

// Castling rules
int castling_rights[128] = {
     7, 15, 15, 15,  3, 15, 15, 11,  NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
    15, 15, 15, 15, 15, 15, 15, 15,  NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
    15, 15, 15, 15, 15, 15, 15, 15,  NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
    15, 15, 15, 15, 15, 15, 15, 15,  NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
    15, 15, 15, 15, 15, 15, 15, 15,  NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
    15, 15, 15, 15, 15, 15, 15, 15,  NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
    15, 15, 15, 15, 15, 15, 15, 15,  NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
    13, 15, 15, 15, 12, 15, 15, 14,  NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE
};

// Move generator helpers
int offset_length[7] = {0, 8, 0, 8, 4, 4, 8};
int castling_side[2][2] = {{1, 2}, {4, 8}};
int pawn_starting_rank[] = {0x60, 0x10};
int pawn_promoting_rank[] = {0x00, 0x70};

// Nodes searched
long nodes = 0;

// Most valuable victim, least valuable attacker
int mvv_lva[15][15] = {
	0,   0,   0,   0,   0,   0,   0,  0,  0,  0,   0,   0,   0,   0,   0,
	0, 600, 100, 200, 300, 400, 500,  0,  0,  600, 100, 200, 300, 400, 500,
	0, 605, 105, 205, 305, 405, 505,  0,  0,  605, 105, 205, 305, 405, 505,
	0, 604, 104, 204, 304, 404, 504,  0,  0,  604, 104, 204, 304, 404, 504,
	0, 603, 103, 203, 303, 403, 503,  0,  0,  603, 103, 203, 303, 403, 503,
	0, 602, 102, 202, 302, 402, 502,  0,  0,  602, 102, 202, 302, 402, 502,
	0, 601, 101, 201, 301, 401, 501,  0,  0,  601, 101, 201, 301, 401, 501,
    0,   0,   0,   0,   0,   0,   0,  0,  0,    0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,  0,  0,    0,   0,   0,   0,   0,   0,
	0, 600, 100, 200, 300, 400, 500,  0,  0,  600, 100, 200, 300, 400, 500,
	0, 605, 105, 205, 305, 405, 505,  0,  0,  605, 105, 205, 305, 405, 505,
	0, 604, 104, 204, 304, 404, 504,  0,  0,  604, 104, 204, 304, 404, 504,
	0, 603, 103, 203, 303, 403, 503,  0,  0,  603, 103, 203, 303, 403, 503,
	0, 602, 102, 202, 302, 402, 502,  0,  0,  602, 102, 202, 302, 402, 502,
	0, 601, 101, 201, 301, 401, 501,  0,  0,  601, 101, 201, 301, 401, 501
};

// Beta cutoff moves
int killer_moves[2][64];

// History best moves
int history_moves[15][128];

// PV moves
int pv_table[64][64];
int pv_length[64];

// Position repetitions
unsigned long long repetition_table[1000];

// Repetition table index
int repetition_index;

// Half move counter
int ply = 0;

int quit = 0;

// UCI "movestogo" command moves counter
int movestogo = 30;

// UCI "movetime" command time counter
int movetime = -1;

// UCI "time" command holder (ms)
int time = -1;

// UCI "inc" command's time increment holder
int inc = 0;

// UCI "starttime" command time holder
int starttime = 0;

// UCI "stoptime" command time holder
int stoptime = 0;

// variable to flag time control availability
int timeset = 0;

// variable to flag when the time is up
int stopped = 0;

// Piece weights
/*int material_score[15] = {
    0, 10000, 100, 300, 350, 500, 1000, 0, 0,
    -10000, -100, -300, -350, -500, -1000
};*/

// Positional scores
int pawn_score[128] =  {
    90,  90,  90,  90,  90,  90,  90,  90,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
    30,  30,  30,  40,  40,  30,  30,  30,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
    20,  20,  20,  30,  30,  30,  20,  20,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
    10,  10,  10,  20,  20,  10,  10,  10,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
     5,   5,  10,  20,  20,   5,   5,   5,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
     0,   0,   0,   5,   5,   0,   0,   0,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
     0,   0,   0, -10, -10,   0,   0,   0,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
     0,   0,   0,   0,   0,   0,   0,   0,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE
};

int knight_score[128] =  {
    -5,   0,   0,   0,   0,   0,   0,  -5,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
    -5,   0,   0,  10,  10,   0,   0,  -5,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
    -5,   5,  20,  20,  20,  20,   5,  -5,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
    -5,  10,  20,  30,  30,  20,  10,  -5,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
    -5,  10,  20,  30,  30,  20,  10,  -5,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
    -5,   5,  20,  10,  10,  20,   5,  -5,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
    -5,   0,   0,   0,   0,   0,   0,  -5,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
    -5, -10,   0,   0,   0,   0, -10,  -5,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE
};

int bishop_score[128] =  {
     0,   0,   0,   0,   0,   0,   0,   0,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
     0,   0,   0,   0,   0,   0,   0,   0,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
     0,   0,   0,  10,  10,   0,   0,   0,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
     0,   0,  10,  20,  20,  10,   0,   0,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
     0,   0,  10,  20,  20,  10,   0,   0,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
     0,  10,   0,   0,   0,   0,  10,   0,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
     0,  30,   0,   0,   0,   0,  30,   0,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
     0,   0, -10,   0,   0, -10,   0,   0,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE

};

int rook_score[128] = {
    50,  50,  50,  50,  50,  50,  50,  50,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
    50,  50,  50,  50,  50,  50,  50,  50,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
     0,   0,  10,  20,  20,  10,   0,   0,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
     0,   0,  10,  20,  20,  10,   0,   0,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
     0,   0,  10,  20,  20,  10,   0,   0,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
     0,   0,  10,  20,  20,  10,   0,   0,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
     0,   0,  10,  20,  20,  10,   0,   0,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
     0,   0,   0,  20,  20,   0,   0,   0,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE

};

int king_score[128] =  {
     0,   0,   0,   0,   0,   0,   0,   0,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
     0,   0,   5,   5,   5,   5,   0,   0,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
     0,   5,   5,  10,  10,   5,   5,   0,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
     0,   5,  10,  20,  20,  10,   5,   0,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
     0,   5,  10,  20,  20,  10,   5,   0,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
     0,   0,   5,  10,  10,   5,   0,   0,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
     0,   5,   5,  -5,  -5,   0,   5,   0,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
     0,   0,   5,   0, -15,   0,  10,   0,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE

};

int mirror_score[128] = {
	A1, B1, C1, D1, E1, F1, G1, H1,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
	A2, B2, C2, D2, E2, F2, G2, H2,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
	A3, B3, C3, D3, E3, F3, G3, H3,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
	A4, B4, C4, D4, E4, F4, G4, H4,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
	A5, B5, C5, D5, E5, F5, G5, H5,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
	A6, B6, C6, D6, E6, F6, G6, H6,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
	A7, B7, C7, D7, E7, F7, G7, H7,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
	A8, B8, C8, D8, E8, F8, G8, H8,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE
};