#include "abc.h"

#define tricky_position "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 "
#define killer_position "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"
#define cmk_position "r2q1rk1/ppp2ppp/2n1bn2/2b1p3/3pP3/3P1NPP/PPP1NPB1/R1BQ1RK1 b - - 0 9 "

// Run engine in UCI mode
int main() {
    uci();
    // 8/8/1p6/p2k2K1/P7/8/7p/6q1 w - - 2 51 
    // 8/8/1P6/P2K2k1/p7/8/7P/6Q1 w - - 2 51 
    return 0;
}