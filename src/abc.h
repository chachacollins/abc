// Manage header includes
#ifndef ARRAY_BASED_CHESS
#define ARRAY_BASED_CHESS
    // Standard libraries
    #include <stdio.h>
    #include <unistd.h>
    #include <string.h>
    
    // OS specific libraries
    #ifdef WIN64
        #include "windows.h"
    #else
        #include "sys/time.h"
        #include "sys/select.h"
        #include "string.h"
    #endif

    // Initial position FEN string
    #define START_POSITION "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 "

    // Board position snapshot
    typedef struct {
        int board[128];
        int king_square[2];
        int side;
        int enpassant;
        int castle;
    } Position;
    
    // Move list
    typedef struct {
        int moves[256];
        int count;
    } Movelist;

    // Board square constants
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

    // Constants
    enum sides { WHITE, BLACK };
    enum types { NONE = -1, EMPTY, KING, PAWN, KNIGHT, BISHOP, ROOK, QUEEN };
    enum pieces { WK = 1, WP, WN, WB, WR, WQ, BK = 9, BP, BN, BB, BR, BQ };
    enum castling { WKC = 1, WQC = 2, BKC = 4, BQC = 8 };
    enum capture_flags {ALL_MOVES, ONLY_CAPTURES};
    enum game_phase { OPENING, ENDGAME, MIDDLEGAME };

    // Definitions in "defs.c"
    extern int board[128];
    extern int king_square[2];
    extern int side;
    extern int enpassant;
    extern int castle;
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
    extern int random_state;
    extern int piece_keys[7][128];
    extern long nodes;
    extern int mvv_lva[15][15];
    extern int killer_moves[2][64];
    extern int history_moves[15][128];
    extern int pv_table[64][64];
    extern int pv_length[64];
    extern int repetition_table[1000];
    extern int repetition_index;
    extern int ply;
    extern int opening_phase_score;
    extern int endgame_phase_score;
    extern int material_score[2][15];
    extern int positional_score[2][7][128];
    extern int mirror_score[128];
    extern int quit;
    extern int movestogo;
    extern int movetime;
    extern int time;
    extern int inc;
    extern int starttime;
    extern int stoptime;
    extern int timeset;
    extern int stopped;

    // Functions in "io.c"
    extern void set_board(char *fen);
    extern void print_board();
    extern void print_move(int source, int target, int promoted);

    // Functions in "movegen.c"
    extern int encode_move(int source, int target, int promoted, int capture, int push, int enpassant, int castling);
    extern int get_move_source(int move);
    extern int get_move_target(int move);
    extern int get_move_promoted(int move);
    extern int get_move_capture(int move);
    extern int get_move_push(int move);
    extern int get_move_enpassant(int move);
    extern int get_move_castling(int move);
    extern int is_square_attacked(int square, int color);
    extern void add_move(Movelist *moves, int move);
    extern void generate_moves(Movelist *moves);
    extern int make_move(int move, int capture_flag);
    extern void save_position(Position *position);
    extern void restore_position(Position *position);

    // Functions in "perft.c"
    extern void perft_driver(int depth);
    extern void perft_test(int depth);

    // Functions in "hash.c"
    extern int get_random_32_number();
    extern void init_random_keys();
    extern int generate_hash_key();

    // Functions in "search.c"
    extern int evaluate_position();
    extern int score_move(int move);
    extern void sort_moves(Movelist *moves);
    extern int is_repetition();
    extern int quiescence_search(int alpha, int beta);
    extern int negamax_search(int alpha, int beta, int depth);
    extern int search_position(int depth);

    // Functions in "uci.c"
    extern void reset_time_control();
    extern void parse_go(char *command);
    extern int parse_move(char *move_str);
    extern void uci();

    // Functions in "misc.c"
    extern int get_time_ms();
    extern void communicate();
    extern void read_input();
    extern int input_waiting();
#endif