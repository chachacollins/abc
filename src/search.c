#include "abc.h"

// Static evaluation
int evaluate_position() {
    int score = 0;
    for (int square = 0; square < 128; square++) {
        if (!(square & 0x88)) {
            int piece = board[square];
            score += material_score[piece];
			switch(piece) {
				case WP: 
				    score += pawn_score[square];
				    if (board[square - 16] == WP) score -= 100;
				    break;
				case WN: score += knight_score[square]; break;
				case WB: score += bishop_score[square]; break;
				case WR: score += rook_score[square]; break;
				case WK: score += king_score[square]; break;
				case BP:
				    score -= pawn_score[mirror_score[square]];
				    if (board[square + 16] == BP) score += 100;
				    break;
				case BN: score -= knight_score[mirror_score[square]]; break;
				case BB: score -= bishop_score[mirror_score[square]]; break;
				case BR: score -= rook_score[mirror_score[square]]; break;
				case BK: score -= king_score[mirror_score[square]]; break;
			}
            
        }
    } return !side ? score : -score;
}

// Score urgency for move ordering
static inline int score_move(int move) {
    if (pv_table[0][ply] == move) return 20000;
    int score = mvv_lva[board[get_move_source(move)]][board[get_move_target(move)]];         
    if (get_move_capture(move)) score += 10000;
    else {
        if (killer_moves[0][ply] == move) score = 9000;
        else if (killer_moves[1][ply] == move) score = 8000;
        else score = history_moves[board[get_move_source(move)]][get_move_target(move)] + 7000;
    } return score;
}

// Sort moves based on urgency
static inline void sort_moves(Movelist *moves) {
    int move_scores[moves->count];
    for (int count = 0; count < moves->count; count++)
        move_scores[count] = score_move(moves->moves[count]);
    for (int current = 0; current < moves->count; current++) {
        for (int next = current + 1; next < moves->count; next++) {
            if (move_scores[current] < move_scores[next]) {
                int temp_score = move_scores[current];
                move_scores[current] = move_scores[next];
                move_scores[next] = temp_score;
                int temp_move = moves->moves[current];
                moves->moves[current] = moves->moves[next];
                moves->moves[next] = temp_move;
            }
        }
    }    
}

// quiescence search
static inline int quiescence_search(int alpha, int beta) {
    if((nodes & 2047 ) == 0) communicate();
    nodes++;
    int eval = evaluate_position();
    if (eval >= beta) return beta;
    if (eval > alpha) alpha = eval;
    Movelist moves[1];
    generate_moves(moves);
    sort_moves(moves);
    for (int count = 0; count < moves->count; count++) {      
        Position position;
        save_position(&position); ply++;
        if (!make_move(moves->moves[count], ONLY_CAPTURES)) { ply--; continue; }
        int score = -quiescence_search(-beta, -alpha);
        restore_position(&position); ply--;
        if (stopped == 1) break;
        if (score >= beta) return beta;
        if (score > alpha) alpha = score;
    } return alpha;
}

// negamax search
int negamax_search(int alpha, int beta, int depth) {
    int legal_moves = 0;
    int old_alpha = alpha;
    pv_length[ply] = ply;
    if((nodes & 2047 ) == 0) communicate();
    if  (!depth) return quiescence_search(alpha, beta);
    nodes++;
    int in_check = is_square_attacked(king_square[side], side ^ 1);
    if (in_check) depth++;
    Movelist moves[1];
    generate_moves(moves);
    sort_moves(moves);
    int moves_searched = 0;
    for (int count = 0; count < moves->count; count++) {
        int move = moves->moves[count];
        Position position;
        save_position(&position); ply++;
        if (!make_move(move, ALL_MOVES)) { ply--; continue; }
        legal_moves++; int score = 0;
        if (moves_searched == 0) score = -negamax_search(-beta, -alpha, depth - 1);
        else {
            if ( moves_searched >= 4 && depth >= 3 && in_check == 0 && 
                 get_move_capture(move) == 0 &&
                 get_move_promoted(move) == 0
               ) score = -negamax_search(-alpha - 1, -alpha, depth - 2);
            else score = alpha + 1;
            if(score > alpha) {
                score = -negamax_search(-alpha - 1, -alpha, depth-1);
                if((score > alpha) && (score < beta))
                    score = -negamax_search(-beta, -alpha, depth-1);
            }
        } restore_position(&position); ply--;
        if (stopped == 1) break;
        moves_searched++;
        if (score > alpha) {
            history_moves[board[get_move_source(move)]][get_move_target(move)] += depth;
            alpha = score;
			pv_table[ply][ply] = move;
			for (int i = ply + 1; i < pv_length[ply + 1]; i++) pv_table[ply][i] = pv_table[ply + 1][i];
			pv_length[ply] = pv_length[ply + 1];
            if (score >= beta) {
                killer_moves[1][ply] = killer_moves[0][ply];
                killer_moves[0][ply] = move;
                return beta;
            }
        }      
    } if (!legal_moves) {
        if (in_check) return -49000 + ply;
        else return 0;
    } return alpha;
}

// search position
int search_position(int depth)
{
    int start = get_time_ms();
    // Clear search
    nodes = 0;
    stopped = 0;
    ply = 0;
    memset(pv_table, 0, sizeof(pv_table));
    memset(pv_length, 0, sizeof(pv_length));
    memset(killer_moves, 0, sizeof(killer_moves));
    memset(history_moves, 0, sizeof(history_moves));
    
    // Iterative deepening
    for (int current_depth = 1; current_depth <= depth; current_depth++) {    
        // Search position with current depth
        if (stopped == 1) break;
	    int score = negamax_search(-50000, 50000, current_depth);
        
        // Output UCI info
        if (score > -49000 && score < -48000)
            printf("info score mate %d depth %d nodes %lld time %d pv ", -(score + 49000) / 2 - 1, current_depth, nodes, get_time_ms() - start);
        else if (score > 48000 && score < 49000)
            printf("info score mate %d depth %d nodes %lld time %d pv ", (49000 - score) / 2 + 1, current_depth, nodes, get_time_ms() - start);   
        else printf("info score cp %d depth %d nodes %lld time %d pv ", score, current_depth, nodes, get_time_ms() - start);
        
        //printf("info score cp %d depth %d nodes %ld pv ", score, current_depth, nodes);
        for (int i = 0; i < pv_length[0]; i++) {
            int move = pv_table[0][i];
            print_move(get_move_source(move), get_move_target(move), get_move_promoted(move));
        } printf("\n"); fflush(stdout);
    }

	// print best move
    int move = pv_table[0][0];
    printf("\nbestmove ");
    print_move(get_move_source(move), get_move_target(move), get_move_promoted(move));
    printf("\n");
}