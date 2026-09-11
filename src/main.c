#include "abc.h"

#define tricky_position "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 "
#define killer_position "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"
#define cmk_position "r2q1rk1/ppp2ppp/2n1bn2/2b1p3/3pP3/3P1NPP/PPP1NPB1/R1BQ1RK1 b - - 0 9 "
#define repetitions "2r3k1/R7/8/1R6/8/8/P4KPP/8 w - - 0 40 "

// Run engine in UCI mode
int main() {
    
    uci();
    //2r3k1/R7/8/1R6/8/8/P4KPP/8 w - - 0 40 
    
    //init_random_keys();
    /*set_board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 "); printf("hash %d\n", generate_hash_key()); getchar();
    Movelist moves[1];
    generate_moves(moves);
    for (int i = 0; i < moves->count; i++) {
        int move = moves->moves[i];
        Position position;
        save_position(&position);
        make_move(move, ALL_MOVES);
        print_board();
        printf("hash %d\n", generate_hash_key()); evaluate_position(); getchar();
        
        restore_position(&position);
        print_board();
        printf("hash %d\n", generate_hash_key()); evaluate_position(); getchar();
    }*/
    return 0;
}