#include "abc.h"

// most valuable victim & less valuable attacker

/*
                          
    (Victims) Pawn Knight Bishop   Rook  Queen   King
  (Attackers)
        Pawn   105    205    305    405    505    605
      Knight   104    204    304    404    504    604
      Bishop   103    203    303    403    503    603
        Rook   102    202    302    402    502    602
       Queen   101    201    301    401    501    601
        King   100    200    300    400    500    600

*/

static int mvv_lva[13][13] = {
	0,   0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,
	0, 105, 205, 305, 405, 505, 605,  105, 205, 305, 405, 505, 605,
	0, 104, 204, 304, 404, 504, 604,  104, 204, 304, 404, 504, 604,
	0, 103, 203, 303, 403, 503, 603,  103, 203, 303, 403, 503, 603,
	0, 102, 202, 302, 402, 502, 602,  102, 202, 302, 402, 502, 602,
	0, 101, 201, 301, 401, 501, 601,  101, 201, 301, 401, 501, 601,
	0, 100, 200, 300, 400, 500, 600,  100, 200, 300, 400, 500, 600,

	0, 105, 205, 305, 405, 505, 605,  105, 205, 305, 405, 505, 605,
	0, 104, 204, 304, 404, 504, 604,  104, 204, 304, 404, 504, 604,
	0, 103, 203, 303, 403, 503, 603,  103, 203, 303, 403, 503, 603,
	0, 102, 202, 302, 402, 502, 602,  102, 202, 302, 402, 502, 602,
	0, 101, 201, 301, 401, 501, 601,  101, 201, 301, 401, 501, 601,
	0, 100, 200, 300, 400, 500, 600,  100, 200, 300, 400, 500, 600
};

// killer moves [id][ply]
int killer_moves[2][64];

// history moves [piece][square]
int history_moves[13][128];

// PV moves
int pv_table[64][64];
int pv_length[64];

// half move
int ply = 0;

// score move for move ordering
static inline int score_move(int move)
{
    // PV move
    if (pv_table[0][ply] == move)
        // score 20000 ( search it first )
        return 20000;
 
    // init current move score
    int score;
    
    // score MVV LVA (scores 0 for quiete moves)
    score = mvv_lva[board[get_move_source(move)]][board[get_move_target(move)]];         

    // on capture
    if (get_move_capture(move))
    {
        // add 10000 to current score
        score += 10000;
    }
    
    // on quiete move
    else {
        // on 1st killer move
        if (killer_moves[0][ply] == move)
            // score 9000
            score = 9000;
        
        // on 2nd killer move
        else if (killer_moves[1][ply] == move)
            // score 8000
            score = 8000;
                      
        // on history move (previous alpha's best score)
        else
            // score with history depth
            score = history_moves[board[get_move_source(move)]][get_move_target(move)] + 7000;
    }
    
    // return move score
    return score;
}

static inline void sort_moves(moves *move_list)
{
    // define move scores array
    int move_scores[move_list->count];
    
    // init move scores array
    for (int count = 0; count < move_list->count; count++)
        // score move
        move_scores[count] = score_move(move_list->moves[count]);
    
    // loop over current move score
    for (int current = 0; current < move_list->count; current++)
    {
        // loop over next move score
        for (int next = current + 1; next < move_list->count; next++)
        {
            // order moves descending
            if (move_scores[current] < move_scores[next])
            {
                // swap scores
                int temp_score = move_scores[current];
                move_scores[current] = move_scores[next];
                move_scores[next] = temp_score;
                
                // swap corresponding moves
                int temp_move = move_list->moves[current];
                move_list->moves[current] = move_list->moves[next];
                move_list->moves[next] = temp_move;
            }
        }
    }    
}

// quiescence search
static inline int quiescence_search(int alpha, int beta, int depth)
{
    // update nodes count
    nodes++;
    
    // evaluate position
    int eval = evaluate_position();
    
    //  fail hard beta-cutoff
    if (eval >= beta)
        return beta;

    // alpha acts like max in MiniMax
    if (eval > alpha)
        alpha = eval;

    // create move list variable
    moves move_list[1];
    
    // generate moves
    generate_moves(move_list);
    
    // move ordering
    sort_moves(move_list);
    
    // loop over the generated moves
    for (int count = 0; count < move_list->count; count++)
    {      
        // copy board position
        //copy_board();
        Position position;
        save_position(&position);
        
        // increment ply
        ply++;
        
        // make only legal moves
        if (!make_move(move_list->moves[count], ONLY_CAPTURES))
        {
            // decrement ply
            ply--;
            
            // skip illegal move
            continue;
        }
        
        // recursive call
        int score = -quiescence_search(-beta, -alpha, depth);
        
        // restore board position
        //take_back();
        //take_back(move_list->moves[count]);
        restore_position(&position);
        
        // decrement ply
        ply--;
        
        //  fail hard beta-cutoff
        if (score >= beta)
             return beta;
        
        // alpha acts like max in MiniMax
        if (score > alpha)
            alpha = score;
    }
        
    // return alpha score
    return alpha;
}

// negamax search
static inline int negamax_search(int alpha, int beta, int depth)
{       
    // legal moves
    int legal_moves = 0;
    
    // best move
    int best_so_far = 0;
    
    // old alpha
    int old_alpha = alpha;
    
    // PV length
    pv_length[ply] = ply;

    // escape condition
    if  (!depth)
        // search for calm position before evaluation
        return quiescence_search(alpha, beta, depth);

    // update nodes count
    nodes++;
    
    // is KING in check?
    int in_check = is_square_attacked(king_square[side], side ^ 1);
    
    // increase depth if KING is in check
    if (in_check)
        depth++;
    
    // create move list variable
    moves move_list[1];
    
    // generate moves
    generate_moves(move_list);
    
    // move ordering
    sort_moves(move_list);
    
    // loop over the generated moves
    for (int count = 0; count < move_list->count; count++)
    {
        // copy board position
        //copy_board();
        Position position;
        save_position(&position);
        
        // increment ply
        ply++;
        
        // make only legal moves
        if (!make_move(move_list->moves[count], ALL_MOVES))
        {
            // decrement ply
            ply--;
            
            // skip illegal move
            continue;
        }
         
        // increment legal moves
        legal_moves++;
        
        // recursive call
        int score = -negamax_search(-beta, -alpha, depth - 1);
        
        // restore board position
        //take_back();
        //take_back(move_list->moves[count]);
        restore_position(&position);
        
        // decrement ply
        ply--;

        //  fail hard beta-cutoff
        if (score >= beta)
        {
            // update killer moves
            killer_moves[1][ply] = killer_moves[0][ply];
            killer_moves[0][ply] = move_list->moves[count];
            
            return beta;
        }
        
        // alpha acts like max in MiniMax
        if (score > alpha)
        {
            // update history score
            history_moves[board[get_move_source(move_list->moves[count])]][get_move_target(move_list->moves[count])] += depth;

            // set alpha score
            alpha = score;
            
            // store PV move
			pv_table[ply][ply] = move_list->moves[count];
			
			for (int i = ply + 1; i < pv_length[ply + 1]; i++)
				pv_table[ply][i] = pv_table[ply + 1][i];
	
			pv_length[ply] = pv_length[ply + 1];
            
            // store current best move
            if(!ply)
                best_so_far = move_list->moves[count];
        }      
    }
    
    // if no legal moves
    if (!legal_moves)
    {
        // check mate detection
        if (in_check)
            return -49000 + ply;
        
        // stalemate detection
        else
            return 0;
    }
    
    // return alpha score
    return alpha;
}

// search position
int search_position(int depth)
{
    // init nodes count
    nodes = 0;
    
    // clear PV, killer and history moves
    memset(pv_table, 0, 16384);  // sizeof(pv_table)
    memset(killer_moves, 0, 512);  // sizeof(killer_moves)
    memset(history_moves, 0, 6656);  // sizeof(history_moves)
    
    // best score
    int score;
    
    // iterative deepening
    for (int current_depth = 1; current_depth <= depth; current_depth++)
    {    
        // search position with current depth 3
	    score = negamax_search(-50000, 50000, current_depth);
        
        // output best move
        printf("info score cp %d depth %d nodes %ld pv ", score, current_depth, nodes);
        
        // print PV line
        for (int i = 0; i < pv_length[0]; i++)
        {
            printf("%s%s%c ", square_to_coords[get_move_source(pv_table[0][i])],
                              square_to_coords[get_move_target(pv_table[0][i])],
                              promoted_pieces[get_move_promoted(pv_table[0][i])]);
        }
        
        printf("\n");
    }
	
	// print best move
    printf("\nbestmove %s%s%c\n", square_to_coords[get_move_source(pv_table[0][0])],
                                  square_to_coords[get_move_target(pv_table[0][0])],
                                  promoted_pieces[get_move_promoted(pv_table[0][0])]);
}