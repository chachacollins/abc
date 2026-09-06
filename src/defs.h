#ifndef DEFS
#define DEFS

#include <stdio.h>
#include <string.h>
#ifdef WIN64
    #include "windows.h"
#else
    #include "sys/time.h"
    #include "sys/select.h"
    #include "string.h"
#endif

#define start_position "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 "
#define tricky_position "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 "
#define killer_position "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"
#define cmk_position "r2q1rk1/ppp2ppp/2n1bn2/2b1p3/3pP3/3P1NPP/PPP1NPB1/R1BQ1RK1 b - - 0 9 "

#define encode_move(source, target, piece, capture, PAWN, enpassant, castling) \
(                          \
    (source) |             \
    (target << 7) |        \
    (piece << 14) |        \
    (capture << 18) |      \
    (PAWN << 19) |         \
    (enpassant << 20) |    \
    (castling << 21)       \
)

#define get_move_source(move) (move & 0x7f)
#define get_move_target(move) ((move >> 7) & 0x7f)
#define get_move_piece(move) ((move >> 14) & 0xf)
#define get_move_capture(move) ((move >> 18) & 0x1)
#define get_move_PAWN(move) ((move >> 19) & 0x1)
#define get_move_enpassant(move) ((move >> 20) & 0x1)
#define get_move_castling(move) ((move >> 21) & 0x1)

enum sides { WHITE, BLACK };

enum types {
    KING = 1,
    PAWN = 2,
    KNIGHT = 3,
    BISHOP = 4,
    ROOK = 5,
    QUEEN = 6,
};

enum pieces {
    e = 0,
    K = 1,
    P = 2,
    N = 3,
    B = 4,
    R = 5,
    Q = 6,
    k = 9,
    p = 10,
    n = 11,
    b = 12,
    r = 13,
    q = 14,
    o = 15
};

enum squares {
    a8 = 0,   b8, c8, d8, e8, f8, g8, h8,
    a7 = 16,  b7, c7, d7, e7, f7, g7, h7,
    a6 = 32,  b6, c6, d6, e6, f6, g6, h6,
    a5 = 48,  b5, c5, d5, e5, f5, g5, h5,
    a4 = 64,  b4, c4, d4, e4, f4, g4, h4,
    a3 = 80,  b3, c3, d3, e3, f3, g3, h3,
    a2 = 96,  b2, c2, d2, e2, f2, g2, h2,
    a1 = 112, b1, c1, d1, e1, f1, g1, h1, no_sq
};

enum directions {
    UP = -16,
    RIGHT = +1,
    DOWN = +16,
    LEFT = -1,
};

enum castling { KC = 1, QC = 2, kc = 4, qc = 8 };

enum capture_flags {ALL_MOVES, ONLY_CAPTURES};


extern int side;
extern int enpassant;
extern int castle;
extern int board[128];
extern int king_square[2];
extern char *square_to_coords[];

extern long nodes;

extern int char_pieces[];
extern int promoted_pieces[];

extern int castling_rights[128];
extern int move_offsets[7][8];
extern int offset_length[7];

extern int pawn_promoting_rank[];
extern int pawn_starting_rank[];
extern int castling_side[2][2];

typedef struct {
    int moves[256];
    int count;
} moves;

typedef struct {
    int board[128];
    int king_square[2];
    int side;
    int enpassant;
    int castle;
} board_state;

extern void clear_board();

extern void set_board(char *fen);
extern void print_board();
extern void print_move_list(moves *move_list);

extern void perft_driver(int depth);
extern void perft_test(int depth);

extern int is_square_attacked(int square, int color);
extern void add_move(moves *move_list, int move);
extern void generate_moves(moves *move_list);
extern void save_state(board_state *state);
extern void restore_state(board_state *state);
extern int make_move(int move, int capture_flag);

extern int evaluate_position();

extern int search_position(int depth);

extern int parse_move(char *move_str);
extern void uci();
#endif