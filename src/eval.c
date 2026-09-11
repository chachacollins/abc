#include "abc.h"

// game phase scores
const int opening_phase_score = 6192;
const int endgame_phase_score = 518;

int material_score[2][15] =
{
    // opening material score
    0, 12000, 82, 337, 365, 477, 1025, 0, 0, -12000, -82, -337, -365, -477, -1025,
    
    // endgame material score
    0, 12000, 94, 281, 297, 512,  936, 0, 0, -12000, -94, -281, -297, -512,  -936
};


enum { OPENING, ENDGAME, MIDDLEGAME };

// positional piece scores [game phase][piece][square]
const int positional_score[2][7][128] =

// OPENING positional piece scores //
{
    {
        {0},
        // king
        {
            -65,  23,  16, -15, -56, -34,   2,  13,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            29,  -1, -20,  -7,  -8,  -4, -38, -29,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -9,  24,   2, -16, -20,   6,  22, -22,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -17, -20, -12, -27, -30, -25, -14, -36,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -49,  -1, -27, -39, -46, -44, -33, -51,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -14, -14, -22, -46, -44, -30, -15, -27,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            1,   7,  -8, -64, -43, -16,   9,   8,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -15,  36,  12, -54,   8, -28,  24,  14,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE
        },

        //pawn
        {
            0,   0,   0,   0,   0,   0,  0,   0,   NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            98, 134,  61,  95,  68, 126, 34, -11,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -6,   7,  26,  31,  65,  56, 25, -20,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -14,  13,   6,  21,  23,  12, 17, -23,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -27,  -2,  -5,  12,  17,   6, 10, -25,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -26,  -4,  -4, -10,   3,   3, 33, -12,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -35,  -1, -20, -23, -15,  24, 38, -22,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            0,   0,   0,   0,   0,   0,  0,   0,    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE
        },
        
        // knight
        {
            -167, -89, -34, -49,  61, -97, -15, -107,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -73, -41,  72,  36,  23,  62,   7,  -17,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -47,  60,  37,  65,  84, 129,  73,   44,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -9,  17,  19,  53,  37,  69,  18,   22,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -13,   4,  16,  13,  28,  19,  21,   -8,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -23,  -9,  12,  10,  19,  17,  25,  -16,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -29, -53, -12,  -3,  -1,  18, -14,  -19,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -105, -21, -58, -33, -17, -28, -19,  -23,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE
        },
        
        // bishop
        {
            -29,   4, -82, -37, -25, -42,   7,  -8,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -26,  16, -18, -13,  30,  59,  18, -47,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -16,  37,  43,  40,  35,  50,  37,  -2,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -4,   5,  19,  50,  37,  37,   7,  -2,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -6,  13,  13,  26,  34,  12,  10,   4,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            0,  15,  15,  15,  14,  27,  18,  10,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            4,  15,  16,   0,   7,  21,  33,   1,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -33,  -3, -14, -21, -13, -12, -39, -21,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE
        },
        
        // rook
        {
            32,  42,  32,  51, 63,  9,  31,  43,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            27,  32,  58,  62, 80, 67,  26,  44,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -5,  19,  26,  36, 17, 45,  61,  16,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -24, -11,   7,  26, 24, 35,  -8, -20,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -36, -26, -12,  -1,  9, -7,   6, -23,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -45, -25, -16, -17,  3,  0,  -5, -33,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -44, -16, -20,  -9, -1, 11,  -6, -71,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -19, -13,   1,  17, 16,  7, -37, -26,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE
        },
        
        // queen
        {
            -28,   0,  29,  12,  59,  44,  43,  45,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -24, -39,  -5,   1, -16,  57,  28,  54,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -13, -17,   7,   8,  29,  56,  47,  57,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -27, -27, -16, -16,  -1,  17,  -2,   1,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -9, -26,  -9, -10,  -2,  -4,   3,  -3,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -14,   2, -11,  -2,  -5,   2,  14,   5,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -35,  -8,  11,   2,   8,  15,  -3,   1,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -1, -18,  -9,  10, -15, -25, -31, -50,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE
        }
    },
    
    {


        // Endgame positional piece scores //

        
        {0},
        // king
        {
            -74, -35, -18, -18, -11,  15,   4, -17,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -12,  17,  14,  17,  17,  38,  23,  11,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            10,  17,  23,  15,  20,  45,  44,  13,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -8,  22,  24,  27,  26,  33,  26,   3,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -18,  -4,  21,  24,  27,  23,   9, -11,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -19,  -3,  11,  21,  23,  16,   7,  -9,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -27, -11,   4,  13,  14,   4,  -5, -17,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -53, -34, -21, -11, -28, -14, -24, -43,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE
        },

        //pawn
        {
            0,   0,   0,   0,   0,   0,   0,   0,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            178, 173, 158, 134, 147, 132, 165, 187,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            94, 100,  85,  67,  56,  53,  82,  84,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            32,  24,  13,   5,  -2,   4,  17,  17,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            13,   9,  -3,  -7,  -7,  -8,   3,  -1,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            4,   7,  -6,   1,   0,  -5,  -1,  -8,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            13,   8,   8,  10,  13,   0,   2,  -7,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            0,   0,   0,   0,   0,   0,   0,   0,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE
        },
        
        // knight
        {
            -58, -38, -13, -28, -31, -27, -63, -99,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -25,  -8, -25,  -2,  -9, -25, -24, -52,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -24, -20,  10,   9,  -1,  -9, -19, -41,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -17,   3,  22,  22,  22,  11,   8, -18,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -18,  -6,  16,  25,  16,  17,   4, -18,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -23,  -3,  -1,  15,  10,  -3, -20, -22,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -42, -20, -10,  -5,  -2, -20, -23, -44,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -29, -51, -23, -15, -22, -18, -50, -64,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE
        },
        
        // bishop
        {
            -14, -21, -11,  -8, -7,  -9, -17, -24,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -8,  -4,   7, -12, -3, -13,  -4, -14,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            2,  -8,   0,  -1, -2,   6,   0,   4,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -3,   9,  12,   9, 14,  10,   3,   2,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -6,   3,  13,  19,  7,  10,  -3,  -9,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -12,  -3,   8,  10, 13,   3,  -7, -15,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -14, -18,  -7,  -1,  4,  -9, -15, -27,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -23,  -9, -23,  -5, -9, -16,  -5, -17,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
        },
        
        // rook
        {
            13, 10, 18, 15, 12,  12,   8,   5,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            11, 13, 13, 11, -3,   3,   8,   3,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            7,  7,  7,  5,  4,  -3,  -5,  -3,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            4,  3, 13,  1,  2,   1,  -1,   2,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            3,  5,  8,  4, -5,  -6,  -8, -11,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -4,  0, -5, -1, -7, -12,  -8, -16,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -6, -6,  0,  2, -9,  -9, -11,  -3,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -9,  2,  3, -1, -5, -13,   4, -20,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
        },
        
        // queen
        {
            -9,  22,  22,  27,  27,  19,  10,  20,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -17,  20,  32,  41,  58,  25,  30,   0,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -20,   6,   9,  49,  47,  35,  19,   9,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            3,  22,  24,  45,  57,  40,  57,  36,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -18,  28,  19,  47,  31,  34,  39,  23,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -16, -27,  15,   6,   9,  17,  10,   5,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -22, -23, -30, -16, -16, -23, -36, -32,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
            -33, -28, -22, -43,  -5, -32, -20, -41,NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE
        }
    }
};

// position evaluation
int evaluate_position()
{   
    // get game phase score
    //int game_phase_score = get_game_phase_score();
    
    int piece_scores = 0;
    int game_phase_score = 0;
    int knights = 0;
    int bishops = 0;
    int rooks = 0;
    int queens = 0;
    
    // static evaluation score
    int score = 0, score_opening = 0, score_endgame = 0;
    
    for (int square = 0; square < 128; square++) {
        if (!(square & 0x88)) {
            int piece = board[square];
            score_opening += material_score[OPENING][piece];
            score_endgame += material_score[ENDGAME][piece];
			switch(piece) {
				case WK:
                    score_opening += positional_score[OPENING][KING][square];
                    score_endgame += positional_score[ENDGAME][KING][square];
                    break;
                case WP:
                    score_opening += positional_score[OPENING][PAWN][square];
                    score_endgame += positional_score[ENDGAME][PAWN][square];
                    break;
				case WN:
                    score_opening += positional_score[OPENING][KNIGHT][square];
                    score_endgame += positional_score[ENDGAME][KNIGHT][square];
                    knights++;
                    break;
				case WB:
                    score_opening += positional_score[OPENING][BISHOP][square];
                    score_endgame += positional_score[ENDGAME][BISHOP][square];
                    bishops++;
                    break;
				case WR:
                    score_opening += positional_score[OPENING][ROOK][square];
                    score_endgame += positional_score[ENDGAME][ROOK][square];
                    rooks++;
                    break;
                case WQ:
                    score_opening += positional_score[OPENING][QUEEN][square];
                    score_endgame += positional_score[ENDGAME][QUEEN][square];
                    queens++;
                    break;
                case BK:
                    score_opening -= positional_score[OPENING][KING][mirror_score[square]];
                    score_endgame -= positional_score[ENDGAME][KING][mirror_score[square]];
                    break;
                case BP:
                    score_opening -= positional_score[OPENING][PAWN][mirror_score[square]];
                    score_endgame -= positional_score[ENDGAME][PAWN][mirror_score[square]];
                    break;
				case BN:
                    score_opening -= positional_score[OPENING][KNIGHT][mirror_score[square]];
                    score_endgame -= positional_score[ENDGAME][KNIGHT][mirror_score[square]];
                    knights++;
                    break;
				case BB:
                    score_opening -= positional_score[OPENING][BISHOP][mirror_score[square]];
                    score_endgame -= positional_score[ENDGAME][BISHOP][mirror_score[square]];
                    bishops++;
                    break;
				case BR:
                    score_opening -= positional_score[OPENING][ROOK][mirror_score[square]];
                    score_endgame -= positional_score[ENDGAME][ROOK][mirror_score[square]];
                    rooks++;
                    
                    break;
                case BQ:
                    score_opening -= positional_score[OPENING][QUEEN][mirror_score[square]];
                    score_endgame -= positional_score[ENDGAME][QUEEN][mirror_score[square]];
                    queens++;
                    break;
				
			}
        }
    }
    
    game_phase_score = knights * material_score[OPENING][KNIGHT];
    game_phase_score += bishops * material_score[OPENING][BISHOP];
    game_phase_score += rooks * material_score[OPENING][ROOK];
    game_phase_score += queens * material_score[OPENING][QUEEN];
    
    //printf("game_phase_score %d  N %d  B %d  R %d  Q %d\n", game_phase_score, knights, bishops, rooks, queens);
    
    
    // game phase (OPENING, middle game, ENDGAME)
    int game_phase = -1;
    
    // pick up game phase based on game phase score
    if (game_phase_score > opening_phase_score) game_phase = OPENING;
    else if (game_phase_score < endgame_phase_score) game_phase = ENDGAME;
    else game_phase = MIDDLEGAME;
    
    //printf("game_phase %d\n", game_phase);
    
    //printf("score opening %d\n", score_opening);
    //printf("score endgame %d\n", score_endgame);
    
    // interpolate score in the MIDDLEGAME
    if (game_phase == MIDDLEGAME)
        score = (
            score_opening * game_phase_score +
            score_endgame * (opening_phase_score - game_phase_score)
        ) / opening_phase_score;

    // return pure OPENING score in OPENING
    else if (game_phase == OPENING) score = score_opening;
    
    // return pure ENDGAME score in ENDGAME
    else if (game_phase == ENDGAME) score = score_endgame;
    
    //printf("score %d\n", score);
    
    // return final evaluation based on side
    return (side == WHITE) ? score : -score;
}