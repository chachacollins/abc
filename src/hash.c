#include "abc.h"

// random seeds
int random_state = 12345;

// generate 32-bit pseudo random numbers (signed)
int get_random_32_number() {
  int number = random_state;
  number ^= number << 13;
  number ^= number >> 17;
  number ^= number << 5;
  random_state = number;
  return number;
}

// random piece keys
int piece_keys[7][128];

// init random hash keys
void init_random_keys() {
  for (int piece = 0; piece < 7; piece++)
    for (int square = 0; square < 128; square++)
      piece_keys[piece][square] = get_random_32_number();
}

// generate "almost" unique position ID aka hash key from scratch
int generate_hash_key() {
  int final_key = 0;
  for (int square = 0; square < 128; square++) {
    if (square&0x88) continue;
    char piece = board[square] & 7;
    if (piece) {
      final_key ^= piece_keys[piece][square];
      final_key ^= board[square] >> 3;
    }
  } return final_key;
}