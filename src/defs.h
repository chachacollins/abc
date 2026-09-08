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

    #define INPUT_BUFFER (400 * 6)
    #define START_POSITION "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 "

    enum sides { WHITE, BLACK };
    
    enum squares {
        A8 = 0,   B8, C8, D8, E8, F8, G8, H8,
        A7 = 16,  B7, C7, D7, E7, F7, G7, H7,
        A6 = 32,  B6, C6, D6, E6, F6, G6, H6,
        A5 = 48,  B5, C5, D5, E5, F5, G5, H5,
        A4 = 64,  B4, C4, D4, E4, F4, G4, H4,
        A3 = 80,  B3, C3, D3, E3, F3, G3, H3,
        A2 = 96,  B2, C2, D2, E2, F2, G2, H2,
        A1 = 112, B1, C1, D1, E1, F1, G1, H1
    };

    enum types { NONE = -1, EMPTY, KING, PAWN, KNIGHT, BISHOP, ROOK, QUEEN };
    enum pieces { WK = 1, WP, WN, WB, WR, WQ, BK = 9, BP, BN, BB, BR, BQ };
    enum castling { WKC = 1, WQC = 2, BKC = 4, BQC = 8 };
    enum capture_flags {ALL_MOVES, ONLY_CAPTURES};
    
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

    extern int board[128];
    extern int side;
    extern int enpassant;
    extern int castle;
    extern int king_square[2];

    extern char *square_to_coords[];
    extern int char_pieces[];
    extern int promoted_pieces[];
    extern char ascii_pieces[];
    
    extern int move_offsets[7][8];
    extern int castling_rights[128];
    extern int offset_length[7];
    extern int castling_side[2][2];
    extern int pawn_starting_rank[];
    extern int pawn_promoting_rank[];
    
    extern long nodes;

    extern const int material_score[13]; // TODO: fix piece order!!!
    extern const int pawn_score[128];
    extern const int knight_score[128];
    extern const int bishop_score[128];
    extern const int rook_score[128];
    extern const int queen_score[128];
    extern const int king_score[128];
    extern const int mirror_score[128];

    extern void set_board(char *fen);
    extern void print_board();

    extern void perft_driver(int depth);
    extern void perft_test(int depth);



    extern int encode_move(int source, int target, int promoted, int capture, int push, int enpassant, int castling);
    extern int get_move_source(int move);
    extern int get_move_target(int move);
    extern int get_move_promoted(int move);
    extern int get_move_capture(int move);
    extern int get_move_push(int move);
    extern int get_move_enpassant(int move);
    extern int get_move_castling(int move);
    
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

    extern int get_time_ms();
#endif