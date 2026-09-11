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

// Game phase thresholds
int opening_phase_score = 6192;
int endgame_phase_score = 518;

// Material scores
int material_score[2][15] = {
    // Opening piece values
    0, 12000, 82, 337, 365, 477, 1025, 0, 0, -12000, -82, -337, -365, -477, -1025,
    
    // Endgame piece values
    0, 12000, 94, 281, 297, 512,  936, 0, 0, -12000, -94, -281, -297, -512,  -936
};

// Positional scores
int positional_score[2][7][128] = {
    { // Opening square values

        {0},

        {  // King
            -65,  23,  16, -15, -56, -34,   2,  13,     NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
             29,  -1, -20,  -7,  -8,  -4, -38, -29,     NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
             -9,  24,   2, -16, -20,   6,  22, -22,     NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -17, -20, -12, -27, -30, -25, -14, -36,     NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -49,  -1, -27, -39, -46, -44, -33, -51,     NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -14, -14, -22, -46, -44, -30, -15, -27,     NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
              1,   7,  -8, -64, -43, -16,   9,   8,     NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -15,  36,  12, -54,   8, -28,  24,  14,     NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE
        },

        { // Pawn
              0,   0,   0,   0,   0,   0,   0,   0,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
             98, 134,  61,  95,  68, 126,  34, -11,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
             -6,   7,  26,  31,  65,  56,  25, -20,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -14,  13,   6,  21,  23,  12,  17, -23,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -27,  -2,  -5,  12,  17,   6,  10, -25,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -26,  -4,  -4, -10,   3,   3,  33, -12,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -35,  -1, -20, -23, -15,  24,  38, -22,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
              0,   0,   0,   0,   0,   0,   0,   0,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE
        },

        { // Knight
           -167, -89, -34, -49,  61, -97, -15,-107,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -73, -41,  72,  36,  23,  62,   7, -17,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -47,  60,  37,  65,  84, 129,  73,  44,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
             -9,  17,  19,  53,  37,  69,  18,  22,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -13,   4,  16,  13,  28,  19,  21,  -8,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -23,  -9,  12,  10,  19,  17,  25, -16,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -29, -53, -12,  -3,  -1,  18, -14, -19,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
           -105, -21, -58, -33, -17, -28, -19, -23,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE
        },

        { // Bishop
            -29,   4, -82, -37, -25, -42,   7,  -8,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -26,  16, -18, -13,  30,  59,  18, -47,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -16,  37,  43,  40,  35,  50,  37,  -2,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
             -4,   5,  19,  50,  37,  37,   7,  -2,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
             -6,  13,  13,  26,  34,  12,  10,   4,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
              0,  15,  15,  15,  14,  27,  18,  10,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
              4,  15,  16,   0,   7,  21,  33,   1,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -33,  -3, -14, -21, -13, -12, -39, -21,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE
        },

        { // Rook
             32,  42,  32,  51,  63,   9,  31,  43,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
             27,  32,  58,  62,  80,  67,  26,  44,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
             -5,  19,  26,  36,  17,  45,  61,  16,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -24, -11,   7,  26,  24,  35,  -8, -20,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -36, -26, -12,  -1,   9,  -7,   6, -23,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -45, -25, -16, -17,   3,   0,  -5, -33,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -44, -16, -20,  -9,  -1,  11,  -6, -71,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -19, -13,   1,  17,  16,   7, -37, -26,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE
        },

        { // Queen
            -28,   0,  29,  12,  59,  44,  43,  45,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -24, -39,  -5,   1, -16,  57,  28,  54,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -13, -17,   7,   8,  29,  56,  47,  57,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -27, -27, -16, -16,  -1,  17,  -2,   1,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
             -9, -26,  -9, -10,  -2,  -4,   3,  -3,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -14,   2, -11,  -2,  -5,   2,  14,   5,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -35,  -8,  11,   2,   8,  15,  -3,   1,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
             -1, -18,  -9,  10, -15, -25, -31, -50,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE
        }
    },

    { // Endgame square values

        {0},

        { // King
            -74, -35, -18, -18, -11,  15,   4, -17,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -12,  17,  14,  17,  17,  38,  23,  11,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
             10,  17,  23,  15,  20,  45,  44,  13,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
             -8,  22,  24,  27,  26,  33,  26,   3,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -18,  -4,  21,  24,  27,  23,   9, -11,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -19,  -3,  11,  21,  23,  16,   7,  -9,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -27, -11,   4,  13,  14,   4,  -5, -17,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -53, -34, -21, -11, -28, -14, -24, -43,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE
        },

        { // Pawn
              0,   0,   0,   0,   0,   0,   0,   0,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            178, 173, 158, 134, 147, 132, 165, 187,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
             94, 100,  85,  67,  56,  53,  82,  84,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
             32,  24,  13,   5,  -2,   4,  17,  17,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
             13,   9,  -3,  -7,  -7,  -8,   3,  -1,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
              4,   7,  -6,   1,   0,  -5,  -1,  -8,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
             13,   8,   8,  10,  13,   0,   2,  -7,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
              0,   0,   0,   0,   0,   0,   0,   0,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE
        },

        { // Knight
            -58, -38, -13, -28, -31, -27, -63, -99,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -25,  -8, -25,  -2,  -9, -25, -24, -52,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -24, -20,  10,   9,  -1,  -9, -19, -41,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -17,   3,  22,  22,  22,  11,   8, -18,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -18,  -6,  16,  25,  16,  17,   4, -18,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -23,  -3,  -1,  15,  10,  -3, -20, -22,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -42, -20, -10,  -5,  -2, -20, -23, -44,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -29, -51, -23, -15, -22, -18, -50, -64,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE
        },

        { // Bishop
            -14, -21, -11,  -8,  -7,  -9, -17, -24,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
             -8,  -4,   7, -12,  -3, -13,  -4, -14,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
              2,  -8,   0,  -1,  -2,   6,   0,   4,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
             -3,   9,  12,   9,  14,  10,   3,   2,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
             -6,   3,  13,  19,   7,  10,  -3,  -9,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -12,  -3,   8,  10,  13,   3,  -7, -15,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -14, -18,  -7,  -1,   4,  -9, -15, -27,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -23,  -9, -23,  -5,  -9, -16,  -5, -17,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
        },

        { // Rook
             13,  10,  18,  15,  12,  12,   8,   5,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
             11,  13,  13,  11,  -3,   3,   8,   3,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
              7,   7,   7,   5,   4,  -3,  -5,  -3,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
              4,   3,  13,   1,   2,   1,  -1,   2,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
              3,   5,   8,   4,  -5,  -6,  -8, -11,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
             -4,   0,  -5,  -1,  -7, -12,  -8, -16,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
             -6,  -6,   0,   2,  -9,  -9, -11,  -3,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
             -9,   2,   3,  -1,  -5, -13,   4, -20,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
        },

        { // Queen
             -9,  22,  22,  27,  27,  19,  10,  20,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -17,  20,  32,  41,  58,  25,  30,   0,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -20,   6,   9,  49,  47,  35,  19,   9,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
              3,  22,  24,  45,  57,  40,  57,  36,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -18,  28,  19,  47,  31,  34,  39,  23,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -16, -27,  15,   6,   9,  17,  10,   5,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -22, -23, -30, -16, -16, -23, -36, -32,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -33, -28, -22, -43,  -5, -32, -20, -41,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE
        }
    }
};

// Mirror score for black
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