#include "defs.h"

void set_board(char *fen) {
    clear_board();
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 16; file++) {
            int square = rank * 16 + file;
            if (!(square & 0x88)) {
                if ((*fen >= 'a' && *fen <= 'z') || (*fen >= 'A' && *fen <= 'Z')) {
                    if (*fen == 'K') king_square[WHITE] = square;
                    else if (*fen == 'k') king_square[BLACK] = square;
                    board[square] = char_pieces[*fen];
                    *fen++;
                } if (*fen >= '0' && *fen <= '9') {
                    int offset = *fen - '0';
                    if (!(board[square])) file--;
                    file += offset;
                    *fen++;
                } if (*fen == '/') *fen++;
            }
        }
    } *fen++;
    side = (*fen == 'w') ? WHITE : BLACK;
    fen += 2;
    while (*fen != ' ') {
        switch(*fen) {
            case 'K': castle |= KC; break;
            case 'Q': castle |= QC; break;
            case 'k': castle |= kc; break;
            case 'q': castle |= qc; break;
            case '-': break;
        } *fen++;
    } *fen++;
    if (*fen != '-') {
        int file = fen[0] - 'a';
        int rank = 8 - (fen[1] - '0');
        enpassant = rank * 16 + file;
    } else enpassant = no_sq;   
}

void print_board() {
    char ascii_pieces[] = ".KPNBRQ--kpnbrq";
    printf("\n");
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 16; file++) {
            int square = rank * 16 + file;
            if (file == 0) printf(" %d  ", 8 - rank);
            if (!(square & 0x88)) printf("%c ", ascii_pieces[board[square]]);
        } printf("\n");
    } printf("\n    a b c d e f g h\n\n");
    printf("    Side:     %s\n", (side == WHITE) ? "white": "black");
    printf("    Castling:  %c%c%c%c\n", (castle & KC) ? 'K' : '-', 
                                        (castle & QC) ? 'Q' : '-',
                                        (castle & kc) ? 'k' : '-',
                                        (castle & qc) ? 'q' : '-');
    printf("    Enpassant:   %s\n", (enpassant == no_sq)? "no" : square_to_coords[enpassant]);
    printf("    King square: %s\n\n", square_to_coords[king_square[side]]);
}

void print_move_list(moves *move_list) {
    printf("\n    Move     Capture  Double   Enpass   Castling\n\n");
    for (int index = 0; index < move_list->count; index++) {
        int move = move_list->moves[index];
        printf("    %s%s", square_to_coords[get_move_source(move)], square_to_coords[get_move_target(move)]);
        printf("%c    ", get_move_piece(move) ? promoted_pieces[get_move_piece(move)] : ' ');
        printf("%d        %d        %d        %d\n", get_move_capture(move), get_move_PAWN(move), get_move_enpassant(move), get_move_castling(move));
    } printf("\n    Total moves: %d\n\n", move_list->count);
}