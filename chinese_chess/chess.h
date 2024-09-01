#ifndef _CHAOS_CHESS_H_
#define _CHAOS_CHESS_H_
#include <assert.h>
#include <stdint.h>

#define CQ_GENERAL  0x01
#define CQ_CHE 0x02
#define CQ_MA 0x03
#define CQ_XIANG 0x04
#define CQ_SHI 0x05
#define CQ_PAO 0x06
#define CQ_SOLDIER 0x07

#define CQ_PIECE_MASK 0x07

#define CQ_JIANG  CQ_GENERAL
#define CQ_ZU CQ_SOLDIER

#define CQ_RED_SHUAI  CQ_GENERAL
#define CQ_RED_CHE CQ_CHE
#define CQ_RED_MA CQ_MA
#define CQ_RED_XIANG CQ_XIANG
#define CQ_RED_SHI CQ_SHI
#define CQ_RED_PAO CQ_PAO
#define CQ_RED_BING CQ_SOLDIER

#define CQ_BLACK_JIANG  (0x08 | CQ_JIANG)
#define CQ_BLACK_CHE (0x08 | CQ_CHE)
#define CQ_BLACK_MA (0x08 | CQ_MA)
#define CQ_BLACK_XIANG (0x08 | CQ_XIANG)
#define CQ_BLACK_SHI (0x08 | CQ_SHI)
#define CQ_BLACK_PAO (0x08 | CQ_PAO)
#define CQ_BLACK_ZU (0x08 | CQ_ZU)

struct cchess_state {
  uint16_t red_pieces[16];
  uint16_t black_pieces[16];
  uint8_t red_len;
  uint8_t black_len;
};

void cchess_init(struct cchess_state *cs);
uint8_t chhess_game_normal_rule_run(struct cchess_state *cs, uint8_t from,
                                    uint8_t to, bool turn_black, bool update);

// position
static inline bool cchess_valid_pos(uint8_t pos)
{
  return ((pos >> 4) < 9) && ((pos & 0x0F) < 10);
}

static inline uint8_t cchess_make_pos(uint8_t col, uint8_t row)
{
  assert(col < 9 && row < 10);
  return (col << 4) | (row & 0x0F);
}

static inline uint8_t cchess_pos_get_col(uint8_t pos)
{
  return pos >> 4;
}

static inline uint8_t cchess_pos_get_row(uint8_t pos)
{
  return pos & 0x0F;
}

static inline uint8_t swap_pos(uint8_t pos)
{
  assert(cchess_valid_pos(pos));
  uint8_t high = 8 - (pos >> 4);
  uint8_t low = 9 - (pos & 0x0F);
  return cchess_make_pos(high, low);
}

// piece and posstion
static bool cchess_is_valid_piece(uint8_t piece)
{
  uint8_t x = piece & 0x0F;
  return (x & CQ_PIECE_MASK) && (x == piece);
}

static inline uint16_t cchess_make_piece_pos(uint8_t piece, uint8_t pos)
{
  assert(cchess_is_valid_piece(piece));
  uint16_t piece_ = piece;
  uint16_t pos_ = pos;
  return (piece_ << 8) | pos_;
}

static inline uint8_t cchess_get_piece(uint16_t piece_pos)
{
  return (uint8_t)(piece_pos >> 8);
}

static inline uint8_t cchess_get_pos(uint16_t piece_pos)
{
  return (uint8_t)piece_pos;
}

static inline bool cchess_is_red_piece(uint8_t piece)
{
  assert(cchess_is_valid_piece(piece));
  return (piece & 0x08) == 0;
}

static inline bool cchess_is_black_piece(uint8_t piece)
{
  assert(cchess_is_valid_piece(piece));
  return (piece & 0x08) != 0;
}


#endif /* _CHAOS_CHESS_H_ */