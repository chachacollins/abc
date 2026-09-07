#include "defs.h"

int is_square_attacked(int square, int color) {
    for (int piece_type = KING; piece_type <= QUEEN; piece_type++) {
        int piece = piece_type | (color << 3);
        if (piece_type == PAWN) {
            int direction = 16 * (1 - 2 * color);
            for (int lr = -1; lr <= 1; lr += 2) {
                int dst = square + direction + lr;
                if (!(dst & 0x88) && board[dst] == piece) return 1;
            }
        } else {
            int slider = piece_type & 0x04;
            for (int d = 0; d < offset_length[piece_type]; d++) {
                int dst = square;
                do {
                    dst += move_offsets[piece_type][d];
                    if (dst & 0x88) break;
                    int attacker = board[dst];
                    if (attacker != EMPTY) {
                        if (attacker == piece) return 1;
                        break;
                    }
                } while (slider);
            }
        }
    } return 0;
}

void add_move(moves *move_list, int move) {
    move_list->moves[move_list->count] = move;
    move_list->count++;
}

void generate_moves(moves *move_list) {
    move_list->count = 0;
    for (int src = 0; src < 128; src++) {
        if (!(src & 0x88)) {
            int piece = board[src];
            int piece_type = piece & 7;
            if ((piece >> 3) == side) {
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
                } else if (piece_type == KING) {
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
                } if (piece_type != PAWN) {
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

void save_state(board_state *state) {
    memcpy(state->board, board, sizeof(board));
    memcpy(state->king_square, king_square, sizeof(king_square));
    state->side = side;
    state->enpassant = enpassant;
    state->castle = castle;
}

void restore_state(board_state *state) {
    memcpy(board, state->board, sizeof(board));
    memcpy(king_square, state->king_square, sizeof(king_square));
    side = state->side;
    enpassant = state->enpassant;
    castle = state->castle;
}

int make_move(int move, int capture_flag) {
    if (capture_flag == ALL_MOVES) {
        board_state state;
        save_state(&state);
        int from_square = get_move_source(move);
        int to_square = get_move_target(move);
        int promoted_piece = get_move_piece(move);
        int enpass = get_move_enpassant(move);
        int double_push = get_move_PAWN(move);
        int castling = get_move_castling(move);
        board[to_square] = board[from_square];
        board[from_square] = EMPTY;
        if (promoted_piece) board[to_square] = promoted_piece;
        if (enpass) !side ? (board[to_square + 16] = EMPTY) : (board[to_square - 16] = EMPTY);
        enpassant = NONE;
        if (double_push) !side ? (enpassant = to_square + 16) : (enpassant = to_square - 16);
        if (castling) {
            switch(to_square) {
                case G1:
                    board[F1] = board[H1];
                    board[H1] = EMPTY;
                    break;
                case C1:
                    board[D1] = board[A1];
                    board[A1] = EMPTY;
                    break;
                case G8:
                    board[F8] = board[H8];
                    board[H8] = EMPTY;
                    break;
                case C8:
                    board[D8] = board[A8];
                    board[A8] = EMPTY;
                    break;
            }
        } if (board[to_square] == WK || board[to_square] == BK)
            king_square[side] = to_square;
        castle &= castling_rights[from_square];
        castle &= castling_rights[to_square];
        side ^= 1;
        if (is_square_attacked(!side ? king_square[side ^ 1] : king_square[side ^ 1], side)) {
            restore_state(&state);
            return 0;
        } else return 1;
    } else {
        if (get_move_capture(move)) make_move(move, ALL_MOVES);
        else return 0;
    }
}