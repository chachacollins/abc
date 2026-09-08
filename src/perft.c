#include "abc.h"

// Traverse all nodes for a given move
void perft_driver(int depth) {
    // Count nodes
    if  (!depth) { nodes++; return; }
    
    // Generate moves
    Movelist moves[1];
    generate_moves(moves);
    
    // Play moves
    for (int move_count = 0; move_count < moves->count; move_count++) {
        Position position;
        save_position(&position);
        if (!make_move(moves->moves[move_count], ALL_MOVES)) continue;
        perft_driver(depth - 1);
        restore_position(&position);
    }
}

// Traverse all nodes for a given position
void perft_test(int depth) {
    // Generate moves
    printf("\n      Performance test:\n\n");
    int start_time = get_time_ms();
    Movelist moves[1];
    generate_moves(moves);
    
    // Play moves
    for (int move_count = 0; move_count < moves->count; move_count++) {
        Position position;
        save_position(&position);
        int move = moves->moves[move_count];
        if (!make_move(move, ALL_MOVES)) continue;
        long cum_nodes = nodes;
        perft_driver(depth - 1);
        long old_nodes = nodes - cum_nodes;
        restore_position(&position);
        printf("    move %d:\t", move_count + 1);
        print_move(get_move_source(move), get_move_target(move), get_move_promoted(move));
        printf("\t%ld\n", old_nodes);
    }
    
    // Print resutls
    printf("\n      Depth: %d", depth);
    printf("\n      Nodes: %ld", nodes);
    printf("\n       Time: %d ms\n\n", get_time_ms() - start_time);
}