#include "abc.h"

// Pack move params into a single integer
int encode_move(int source, int target, int promoted, int capture, int push, int enpassant, int castling) {
    return (
               (source) |
               (target << 7) |
               (promoted << 14) |
               (capture << 18) |
               (push << 19) |
               (enpassant << 20) |
               (castling << 21)
           );
}

// Extract move params
int get_move_source(int move) { return move & 0x7f; }
int get_move_target(int move) { return (move >> 7) & 0x7f; }
int get_move_promoted(int move) { return (move >> 14) & 0xf; }
int get_move_capture(int move) { return (move >> 18) & 0x1; }
int get_move_push(int move) { return (move >> 19) & 0x1; }
int get_move_enpassant(int move) { return (move >> 20) & 0x1; }
int get_move_castling(int move) { return (move >> 21) & 0x1; }

// Add move to the move list
void add_move(moves *move_list, int move) {
    move_list->moves[move_list->count] = move;
    move_list->count++;
}

// Find whether square is attacked
int is_square_attacked(int square, int color) {
    // Loop over piece types
    for (int piece_type = KING; piece_type <= QUEEN; piece_type++) {
        int piece = piece_type | (color << 3);
        
        // Attacked by pawns
        if (piece_type == PAWN) {
            int direction = 16 * (1 - 2 * color);
            for (int left_right = -1; left_right <= 1; left_right += 2) {
                int target = square + direction + left_right;
                if (!(target & 0x88) && board[target] == piece) return 1;
            }
        }
        
        // Attacked by pieces
        else {
            int slider = piece_type & 0x04;
            for (int d = 0; d < offset_length[piece_type]; d++) {
                int target = square;
                do {
                    target += move_offsets[piece_type][d];
                    if (target & 0x88) break;
                    int attacker = board[target];
                    if (attacker != EMPTY) {
                        if (attacker == piece) return 1;
                        break;
                    }
                } while (slider);
            }
        }
    }
    
    // Not attacked
    return 0;
}

// Generate pseudo legal moves
void generate_moves(moves *move_list) {
    // Reset move counter
    move_list->count = 0;
    
    // Loop over board squares
    for (int src = 0; src < 128; src++) {
        
        // Filter offboard squares
        if (!(src & 0x88)) {
            int piece = board[src];
            int piece_type = piece & 7;
            
            // Find piece of side to move
            if ((piece >> 3) == side) {
                
                // Generate pawn moves
                if (piece_type == PAWN) {
                    int direction = -16 * (1 - 2 * side);
                    int dst = src + direction;
                    if ((dst & 0x88) == 0 && board[dst] == EMPTY) { 
                        if ((dst & 0xF0) == pawn_promoting_rank[side]) {
                            for (int promoted_piece = QUEEN; promoted_piece >= KNIGHT; promoted_piece--)
                                add_move(move_list, encode_move(src, dst, (promoted_piece | (side << 3)), 1, 0, 0, 0));
                        } else {
                            add_move(move_list, encode_move(src, dst, 0, 0, 0, 0, 0));
                            int double_dst = src + direction * 2;
                            if ((src & 0xF0) == pawn_starting_rank[side] && board[double_dst] == EMPTY)
                                add_move(move_list, encode_move(src, double_dst, 0, 0, 1, 0, 0));
                        }
                    } for (int lr = -1; lr <= 1; lr += 2) {
                        dst = src + direction + lr;
                        if (dst & 0x88) continue;
                        int dst_piece = board[dst];
                        if (dst_piece != EMPTY && (dst_piece >> 3) != side) {
                            if ((dst & 0xF0) == pawn_promoting_rank[side]) {
                                for (int promoted_piece = QUEEN; promoted_piece >= KNIGHT; promoted_piece--)
                                    add_move(move_list, encode_move(src, dst, (promoted_piece | (side << 3)), 1, 0, 0, 0));
                            } else add_move(move_list, encode_move(src, dst, 0, 1, 0, 0, 0));
                        } if (dst == enpassant) add_move(move_list, encode_move(src, dst, 0, 1, 0, 1, 0));
                    }
                }
                
                // Generate castling moves
                else if (piece_type == KING) {
                    int ks = king_square[side];
                    if (castle & castling_side[side][0]) {
                      if (board[ks + 1] == EMPTY && board[ks + 2] == EMPTY) {
                        if (is_square_attacked(ks, 1 - side) == 0 && is_square_attacked(ks + 1, 1 - side) == 0)
                            add_move(move_list, encode_move(ks, ks + 2, 0, 0, 0, 0, 1));
                      }
                    } if (castle & castling_side[side][1]) {
                      if (board[ks - 1] == EMPTY && board[ks - 2] == EMPTY && board[ks - 3] == EMPTY) {
                        if (is_square_attacked(ks, 1 - side) == 0 &&
                            is_square_attacked(ks - 1, 1 - side) == 0)
                            add_move(move_list, encode_move(ks, ks - 2, 0, 0, 0, 0, 1));
                      }
                    }
                }
                
                // Generate piece moves
                if (piece_type != PAWN) {
                    int slider = piece_type & 0x04;
                    for (int d = 0; d < offset_length[piece_type]; d++) {
                      int dst = src;
                      do {
                        dst += move_offsets[piece_type][d];
                        if (dst & 0x88) break;
                        int dst_piece = board[dst];
                        if (dst_piece != EMPTY) {
                          if ((dst_piece >> 3) != side) add_move(move_list, encode_move(src, dst, 0, 1, 0, 0, 0));
                          break;
                        } add_move(move_list, encode_move(src, dst, 0, 0, 0, 0, 0));
                      } while (slider);
                    }
                }
            }
        }
    }
}

// Play move on board
int make_move(int move, int capture_flag) {
    // Increment ply
    ply++;

    // Make all moves
    if (capture_flag == ALL_MOVES) {
        // Preserve board position
        Position position;
        save_position(&position);
        
        // Decode move
        int from_square = get_move_source(move);
        int to_square = get_move_target(move);
        int promoted_piece = get_move_promoted(move);
        int ep = get_move_enpassant(move);
        int double_push = get_move_push(move);
        int castling = get_move_castling(move);
        
        // Move piece
        board[to_square] = board[from_square];
        board[from_square] = EMPTY;
        
        // Pawn promotion
        if (promoted_piece) board[to_square] = promoted_piece;
        
        // Enpassant capture
        if (ep) !side ? (board[to_square + 16] = EMPTY) : (board[to_square - 16] = EMPTY);
        enpassant = NONE;
        if (double_push) !side ? (enpassant = to_square + 16) : (enpassant = to_square - 16);
        
        // Castling move
        if (castling) {
            switch(to_square) {
                case G1: board[F1] = board[H1]; board[H1] = EMPTY; break;
                case C1: board[D1] = board[A1]; board[A1] = EMPTY; break;
                case G8: board[F8] = board[H8]; board[H8] = EMPTY; break;
                case C8: board[D8] = board[A8]; board[A8] = EMPTY; break;
            }
        }
        
        // Update castling rights
        castle &= castling_rights[from_square];
        castle &= castling_rights[to_square];
        
        // Update king square
        if (board[to_square] == WK || board[to_square] == BK)
            king_square[side] = to_square;

        // Switch side to move
        side ^= 1;
        
        // Filter illegal moves
        if (is_square_attacked(!side ? king_square[side ^ 1] : king_square[side ^ 1], side)) {
            restore_position(&position);
            return 0;
        } else return 1;
    }
    
    // Make only captures
    else {
        if (get_move_capture(move)) make_move(move, ALL_MOVES);
        else return 0;
    }
}

// Preserve current position state
void save_position(Position *position) {
    memcpy(position->board, board, sizeof(board));
    memcpy(position->king_square, king_square, sizeof(king_square));
    position->side = side;
    position->enpassant = enpassant;
    position->castle = castle;
}

// Restore preserved position state
void restore_position(Position *position) {
    ply--;
    memcpy(board, position->board, sizeof(board));
    memcpy(king_square, position->king_square, sizeof(king_square));
    side = position->side;
    enpassant = position->enpassant;
    castle = position->castle;
}