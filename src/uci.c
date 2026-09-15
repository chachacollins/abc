#include "abc.h"

// Reset time control variables
void reset_time_control() {
    quit = 0;
    movestogo = 30;
    movetime = -1;
    time = -1;
    inc = 0;
    starttime = 0;
    stoptime = 0;
    timeset = 0;
    stopped = 0;
}

// Search time settings
void parse_go(char *command) {
    reset_time_control();
    int depth = -1;
    char *argument = NULL;
    if ((argument = strstr(command,"infinite"))) {}
    if ((argument = strstr(command,"binc")) && side == BLACK) inc = atoi(argument + 5);
    if ((argument = strstr(command,"winc")) && side == WHITE) inc = atoi(argument + 5);
    if ((argument = strstr(command,"wtime")) && side == WHITE) time = atoi(argument + 6);
    if ((argument = strstr(command,"btime")) && side == BLACK) time = atoi(argument + 6);
    if ((argument = strstr(command,"movestogo"))) movestogo = atoi(argument + 10);
    if ((argument = strstr(command,"movetime"))) movetime = atoi(argument + 9);
    if ((argument = strstr(command,"depth"))) depth = atoi(argument + 6);
    if(movetime != -1) {
        time = movetime;
        movestogo = 1;
    } starttime = get_time_ms();
    depth = depth;
    if (time != -1) {
        timeset = 1;
        time /= movestogo;
        if (time > 1500) time -= 50;
        stoptime = starttime + time + inc;
        if (time < 1500 && inc && depth == 64) stoptime = starttime + inc - 50;
    } if (depth == -1) depth = 64;
    search_position(depth);
}

// Encode UCI move to integer
int parse_move(char *move_str) {
    // Generate moves
	Movelist moves[1];
	generate_moves(moves);
	
    // Extract move params
    int source = (move_str[0] - 'a') + (8 - (move_str[1] - '0')) * 16;
	int target = (move_str[2] - 'a') + (8 - (move_str[3] - '0')) * 16;
	int promoted_piece = 0;
	
    // Create move
    int move;
    
    // Encode move if available in move list
	for(int count = 0; count < moves->count; count++) {
		move = moves->moves[count];
		if(get_move_source(move) == source && get_move_target(move) == target) {
			promoted_piece = get_move_promoted(move);
			if(promoted_piece) {
				if((promoted_piece == WN || promoted_piece == BN) && move_str[4] == 'n') return move;
				else if((promoted_piece == WB || promoted_piece == BB) && move_str[4] == 'b') return move;
				else if((promoted_piece == WR || promoted_piece == BR) && move_str[4] == 'r') return move;
				else if((promoted_piece == WQ || promoted_piece == BQ) && move_str[4] == 'q') return move;
				continue;
			} return move;
		}
	}
    
    // Error
    return 0;
}

// UCI protocol
void uci() {
    // Keys for position hashing
    init_random_keys();
    
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
                    repetition_index++;
                    repetition_table[repetition_index] = generate_hash_key();
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
                        repetition_index++;
                        repetition_table[repetition_index] = generate_hash_key();
					} *moves++;
				}
			}
		}
        
        // Search commands
        else if (!strncmp(user_input, "go", 2)) parse_go(user_input);
        
        // Debug command to print board
        else if (!strncmp(user_input, "board", 5)) print_board();
		
        // Command "quit"
        else if (!strncmp(user_input, "quit", 4)) break;
	}
}