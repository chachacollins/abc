#include "abc.h"

// Encode UCI move to integer
int parse_move(char *move_str) {
    // Generate moves
	Movelist moves[1];
	generate_moves(moves);
	
    // Extract move params
    int parse_from = (move_str[0] - 'a') + (8 - (move_str[1] - '0')) * 16;
	int parse_to = (move_str[2] - 'a') + (8 - (move_str[3] - '0')) * 16;
	int prom_piece = 0;
	
    // Create move
    int move;
    
    // Encode move if available in move list
	for(int count = 0; count < moves->count; count++) {
		move = moves->moves[count];
		if(get_move_source(move) == parse_from && get_move_target(move) == parse_to) {
			prom_piece = get_move_promoted(move);
			if(prom_piece) {
				if((prom_piece == WN || prom_piece == BN) && move_str[4] == 'n') return move;
				else if((prom_piece == WB || prom_piece == BB) && move_str[4] == 'b') return move;
				else if((prom_piece == WR || prom_piece == BR) && move_str[4] == 'r') return move;
				else if((prom_piece == WQ || prom_piece == BQ) && move_str[4] == 'q') return move;
				continue;
			} return move;
		}
	}
    
    // Error
    return 0;
}

// UCI protocol
void uci() {
    // User input
	char user_input[2400];
	
    // Engine info
    printf("id name ABC\n");
	printf("id author Code Monkey King\n");
	printf("uciok\n");
	
    // UCI loop
    while(1) {
        // Handle user input
		memset(&user_input[0], 0, sizeof(user_input));
		fflush(stdout);
		if(!fgets(user_input, sizeof(user_input), stdin)) continue;
		
        // No command
        if(user_input[0] == '\n') continue;
		
        // Command "ucinewgame"
        if (!strncmp(user_input, "ucinewgame", 10)) set_board(START_POSITION);
        
        // Command "uci"
        else if (!strncmp(user_input, "uci", 3)) {
			printf("id name ABC\n");
			printf("id author Code Monkey King\n");
			printf("uciok\n");
		}
        
        // Command "isready"
        else if (!strncmp(user_input, "isready", 7)) {
			printf("readyok\n");
			continue;
		}
		
        // Command "position startpos moves"
        else if (!strncmp(user_input, "position startpos moves", 23)) {
			set_board(START_POSITION);
			char *moves = user_input;
			moves += 23;
			int countChar = -1;
			while(*moves) {
				if(*moves == ' ') {
					*moves++;
					make_move(parse_move(moves), ALL_MOVES);
				} *moves++;
			}
		}
        
        // Command "position startpos"
        else if (!strncmp(user_input, "position startpos", 17)) set_board(START_POSITION);
        
        // Command "position fen"
        else if (!strncmp(user_input, "position fen", 12)) {
			char *fen = user_input;
			fen += 13;
			set_board(fen);
			char *moves = user_input;
			while(strncmp(moves, "moves", 5)) {
				*moves++;
				if(*moves == '\0') break;
			} moves += 4;
			if(*moves == 's') {
				int countChar = -1;
				while(*moves) {
					if(*moves == ' ') {
						*moves++;
						make_move(parse_move(moves), ALL_MOVES);
					} *moves++;
				}
			}
		}
        
        // Command "go depth"
        else if (!strncmp(user_input, "go depth", 8)) {
			char *go = user_input;
			go += 9;
			int depth = *go - '0';
			search_position(depth);
		}
        
        // Other "go" commands
        else if (!strncmp(user_input, "go", 2)) search_position(6);
        
        // Debug command to print board
        else if (!strncmp(user_input, "board", 5)) print_board();
		
        // Command "quit"
        else if (!strncmp(user_input, "quit", 4)) break;
	}
}