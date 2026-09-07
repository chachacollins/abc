#include "defs.h"

void perft_driver(int depth) {
    if  (!depth) { nodes++; return; }
    moves move_list[1];
    generate_moves(move_list);
    for (int move_count = 0; move_count < move_list->count; move_count++) {
        board_state state;
        save_state(&state);
        if (!make_move(move_list->moves[move_count], ALL_MOVES)) continue;
        perft_driver(depth - 1);
        restore_state(&state);
    }
}

void perft_test(int depth) {
    printf("\n    Performance test:\n\n");
    int start_time = get_time_ms();
    moves move_list[1];
    generate_moves(move_list);
    for (int move_count = 0; move_count < move_list->count; move_count++) {
        board_state state;
        save_state(&state);
        if (!make_move(move_list->moves[move_count], ALL_MOVES)) continue;
        long cum_nodes = nodes;
        perft_driver(depth - 1);
        long old_nodes = nodes - cum_nodes;
        restore_state(&state);
        printf("    move %d: %s%s%c    %ld\n",
            move_count + 1,
            square_to_coords[get_move_source(move_list->moves[move_count])],
            square_to_coords[get_move_target(move_list->moves[move_count])],
            promoted_pieces[get_move_piece(move_list->moves[move_count])],
            old_nodes
        );
    } printf("\n    Depth: %d", depth);
    printf("\n    Nodes: %ld", nodes);
    printf("\n     Time: %d ms\n\n", get_time_ms() - start_time);
}