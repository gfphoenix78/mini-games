#include <stdbool.h>
#include "chess.h"
#include "chess_record.h"


#ifndef NULL
#define NULL (void *)0
#endif


#define cchess_foreach(CS, tp, code, post)  do { \
  static_assert((tp) == 'r' || (tp) == 'b', \
                "tp must be one of 'r' or 'b'"); \
  if ((tp) == 'r') { \
    for (int i = 0; i < (CS)->red_len; i++) { \
      uint16_t pp = (CS)->red_pieces[i]; \
      code; \
    } \
  } else if ((tp) == 'b') { \
    for (int i = 0; i < (CS)->black_len; i++) { \
      uint16_t pp = (CS)->black_pieces[i]; \
      code; \
    } \
  } else { \
    assert(0); \
  } \
  post; \
} while(0)

void cchess_init(struct cchess_state *cs)
{
  cs->red_pieces[0] = cchess_make_piece_pos(CQ_RED_CHE, cchess_make_pos( 0, 0));
  cs->red_pieces[1] = cchess_make_piece_pos(CQ_RED_MA, cchess_make_pos( 1, 0));
  cs->red_pieces[2] = cchess_make_piece_pos(CQ_RED_XIANG, cchess_make_pos( 2, 0));
  cs->red_pieces[3] = cchess_make_piece_pos(CQ_RED_SHI, cchess_make_pos( 3, 0));
  cs->red_pieces[4] = cchess_make_piece_pos(CQ_RED_SHUAI, cchess_make_pos( 4, 0));
  cs->red_pieces[5] = cchess_make_piece_pos(CQ_RED_SHI, cchess_make_pos( 5, 0));
  cs->red_pieces[6] = cchess_make_piece_pos(CQ_RED_XIANG, cchess_make_pos( 6, 0));
  cs->red_pieces[7] = cchess_make_piece_pos(CQ_RED_MA, cchess_make_pos( 7, 0));
  cs->red_pieces[8] = cchess_make_piece_pos(CQ_RED_CHE, cchess_make_pos( 8, 0));
  
  cs->red_pieces[ 9] = cchess_make_piece_pos(CQ_RED_PAO, cchess_make_pos( 1, 2));
  cs->red_pieces[10] = cchess_make_piece_pos(CQ_RED_PAO, cchess_make_pos( 7, 2));

  cs->red_pieces[11] = cchess_make_piece_pos(CQ_RED_BING, cchess_make_pos( 0, 3));
  cs->red_pieces[12] = cchess_make_piece_pos(CQ_RED_BING, cchess_make_pos( 2, 3));
  cs->red_pieces[13] = cchess_make_piece_pos(CQ_RED_BING, cchess_make_pos( 4, 3));
  cs->red_pieces[14] = cchess_make_piece_pos(CQ_RED_BING, cchess_make_pos( 6, 3));
  cs->red_pieces[15] = cchess_make_piece_pos(CQ_RED_BING, cchess_make_pos( 8, 3));

  cs->black_pieces[0] = cchess_make_piece_pos(CQ_BLACK_CHE, cchess_make_pos( 0, 9));
  cs->black_pieces[1] = cchess_make_piece_pos(CQ_BLACK_MA, cchess_make_pos( 1, 9));
  cs->black_pieces[2] = cchess_make_piece_pos(CQ_BLACK_XIANG, cchess_make_pos( 2, 9));
  cs->black_pieces[3] = cchess_make_piece_pos(CQ_BLACK_SHI, cchess_make_pos( 3, 9));
  cs->black_pieces[4] = cchess_make_piece_pos(CQ_BLACK_JIANG, cchess_make_pos( 4, 9));
  cs->black_pieces[5] = cchess_make_piece_pos(CQ_BLACK_SHI, cchess_make_pos( 5, 9));
  cs->black_pieces[6] = cchess_make_piece_pos(CQ_BLACK_XIANG, cchess_make_pos( 6, 9));
  cs->black_pieces[7] = cchess_make_piece_pos(CQ_BLACK_MA, cchess_make_pos( 7, 9));
  cs->black_pieces[8] = cchess_make_piece_pos(CQ_BLACK_CHE, cchess_make_pos( 8, 9));
  
  cs->black_pieces[ 9] = cchess_make_piece_pos(CQ_BLACK_PAO, cchess_make_pos( 1, 7));
  cs->black_pieces[10] = cchess_make_piece_pos(CQ_BLACK_PAO, cchess_make_pos( 7, 7));

  cs->black_pieces[11] = cchess_make_piece_pos(CQ_BLACK_ZU, cchess_make_pos( 0, 6));
  cs->black_pieces[12] = cchess_make_piece_pos(CQ_BLACK_ZU, cchess_make_pos( 2, 6));
  cs->black_pieces[13] = cchess_make_piece_pos(CQ_BLACK_ZU, cchess_make_pos( 4, 6));
  cs->black_pieces[14] = cchess_make_piece_pos(CQ_BLACK_ZU, cchess_make_pos( 6, 6));
  cs->black_pieces[15] = cchess_make_piece_pos(CQ_BLACK_ZU, cchess_make_pos( 8, 6));

  cs->red_len = cs->black_len = 16;
}

// make sure piece is valid
static void cchess_piece_eaten_internal_(struct cchess_state *cs, uint16_t piece) {
  if (cchess_is_red_piece(piece)) {
    cchess_foreach(cs, 'r', {
      if (cs->red_pieces[i] == piece) {
        // swap and dec len
        if (i + 1 == cs->red_len) --cs->red_len;
        else {
          cs->red_pieces[i] = cs->red_pieces[--cs->red_len];
          cs->red_pieces[cs->red_len] = piece;
        }
        break;
      }
    }, );
  } else {
    assert(cchess_is_black_piece(piece));
    cchess_foreach(cs, 'b', {
      if (cs->black_pieces[i] == piece) {
        // swap and dec len
        if (i + 1 == cs->black_len) --cs->black_len;
        else {
          cs->black_pieces[i] = cs->black_pieces[--cs->black_len];
          cs->black_pieces[cs->black_len] = piece;
        }
        break;
      }
    }, );
  }
}

static void cchess_piece_move_internal_(struct cchess_state *cs, uint16_t piece, uint8_t pos) {
  uint8_t val = cchess_get_piece(piece);
  if (cchess_is_black_piece(val)) {
    cchess_foreach(cs, 'b', {
      if (cs->black_pieces[i] == piece) {
        cs->black_pieces[i] = cchess_make_piece_pos(val, pos);
        break;
      }
    }, );
  } else {
    cchess_foreach(cs, 'r', {
      if (cs->red_pieces[i] == piece) {
        cs->red_pieces[i] = cchess_make_piece_pos(val, pos);
        break;
      }
    }, );
  }
}

static int cchess_sort_by_col(const void *a, const void *b)
{
  uint16_t va = *(const uint16_t *)a;
  uint16_t vb = *(const uint16_t *)b;
  int cola = (int) cchess_pos_get_col(cchess_get_pos(va));
  int colb = (int) cchess_pos_get_col(cchess_get_pos(vb));
  return cola - colb;
}

static int cchess_sort_by_row(const void *a, const void *b)
{
  uint16_t va = *(const uint16_t *)a;
  uint16_t vb = *(const uint16_t *)b;
  int rowa = (int) cchess_pos_get_row(cchess_get_pos(va));
  int rowb = (int) cchess_pos_get_row(cchess_get_pos(vb));
  return rowa - rowb;
}

#define cchess_find_line(CS, test_expr, cmp_func, result) ({ \
  int n = 0; \
  for (int i = 0; i < (CS)->black_len; i++) { \
    uint16_t v = (CS)->black_pieces[i]; \
    uint8_t pos = cchess_get_pos(v); \
    uint8_t col = cchess_pos_get_col(pos); \
    uint8_t row = cchess_pos_get_row(pos); \
    if (test_expr) \
      result[n++] = v; \
  } \
  for (int i = 0; i < (CS)->red_len; i++) { \
    uint16_t v = (CS)->red_pieces[i]; \
    uint8_t pos = cchess_get_pos(v); \
    uint8_t col = cchess_pos_get_col(pos); \
    uint8_t row = cchess_pos_get_row(pos); \
    if (test_expr) \
      result[n++] = v; \
  } \
  if (n > 1) \
    qsort(result, n, sizeof(uint16_t), cmp_func); \
  \
  n; \
})

uint16_t cchess_find_by_pos(const struct cchess_state *cs, uint8_t pos)
{
  for (int i = 0; i < cs->black_len; i++) {
    uint16_t v = cs->black_pieces[i];
    if (cchess_get_pos(v) == pos)
      return v;
  }
  for (int i = 0; i < cs->red_len; i++) {
    uint16_t v = cs->red_pieces[i];
    if (cchess_get_pos(v) == pos)
      return v;
  }
  return 0;
}

uint8_t chhess_game_normal_rule_run(struct cchess_state *cs, uint8_t from,
                                    uint8_t to, bool turn_black, bool update)
{
  uint16_t p0;
  uint16_t p1;
  uint8_t op;

  if (from == to || !cchess_valid_pos(from) || !cchess_valid_pos(to)) return 0;

  p0 = cchess_find_by_pos(cs, from);
  // from处没有棋子或棋子与当前turn不一致，返回false
  if (p0 == 0 || turn_black != cchess_is_black_piece(cchess_get_piece(p0)))
    return 0;

  p1 = cchess_find_by_pos(cs, to);
  // can't overlap our own piece.
  if (p1 != 0 && turn_black == cchess_is_black_piece(cchess_get_piece(p1)))
    return 0;

  int8_t cf = (int8_t) cchess_pos_get_col(from);
  int8_t rf = (int8_t) cchess_pos_get_row(from);
  int8_t ct = (int8_t) cchess_pos_get_col(to);
  int8_t rt = (int8_t) cchess_pos_get_row(to);

  // default valid op
  op = p1 == 0 ? CQ_OP_MOV : CQ_OP_EAT;
  switch (cchess_get_piece(p0) & CQ_PIECE_MASK)
  {
  case CQ_SOLDIER: {
    if (cf == ct) {
      // 同一列，前进
      // 黑：rf - 1 = rt
      // 红：rf + 1 = rt
      if (turn_black ? rf != rt + 1 : rf + 1 != rt) return 0;
    } else if (rf == rt) {
      // 同一行，必须过河
      // 黑：rf <= 4
      // 红：rf >= 5
      if (turn_black ? rf > 4 : rf < 5) return 0;
    } else {
      // invalid rule
      return 0;
    }
    break;
  }
  case CQ_CHE: {
    uint16_t pps[12];
    int n;
    if (cf == ct) {
      // 同一列
      if (rf < rt) {
        n = cchess_find_line(cs, (col == cf && rf < row && row <= rt), cchess_sort_by_row, pps);
      } else {
        assert(rf > rt);
        n = cchess_find_line(cs, (col == cf && rf > row && row >= rt), cchess_sort_by_row, pps);
      }
    } else if (rf == rt) {
      // 同一行
      if (cf < ct) {
        n = cchess_find_line(cs, (row == rf && cf < col && col <= ct), cchess_sort_by_col, pps);
      } else {
        assert(cf > ct);
        n = cchess_find_line(cs, (row == rf && cf > col && col >= ct), cchess_sort_by_col, pps);
      }
    } else return 0;

    if (n == 0) {
      assert(p1 == 0);
      op = CQ_OP_MOV;
    } else if (n == 1 && p1 != 0) {
      assert(pps[0] == p1);
      op = CQ_OP_EAT;
    } else {
      return 0; // more than 1 piece in path, invalid
    } 
    break;
  }
  case CQ_MA: {
    int8_t dc = ct - cf;
    int8_t dr = rt - rf;
    if (dc * dc + dr * dr != 5) return 0;
    assert(((dc == -1 || dc == 1) && (dr == -2 || dr == 2)) ||
            ((dc == -2 || dc == 2) && (dr == -1 || dr == 1)));

    uint8_t pc = cf + (dc / 2);
    uint8_t pr = rf + (dr / 2);
    uint16_t px = cchess_find_by_pos(cs, cchess_make_pos_(pc, pr));
    if (px != 0) return 0; // 蹩腿马
    break;
  }
  case CQ_XIANG: {
    int8_t dc = ct - cf;
    int8_t dr = rt - rf;
    if (dc * dc != 4 || dr * dr != 4) return 0;
    // 不能过河
    if (turn_black ? rt < 5 : rt > 4) return 0;

    uint8_t pc = cf + (dc / 2);
    uint8_t pr = rf + (dr / 2);
    uint16_t px = cchess_find_by_pos(cs, cchess_make_pos(pc, pr));
    if (px != 0) return 0; // 塞相/象眼
    break;
  }
  case CQ_SHI: {
    int8_t dc = ct - cf;
    int8_t dr = rt - rf;
    if (dc * dc != 1 || dr * dr != 1) return 0;
    // 不能出宫
    if (3 <= cf && cf <= 5 &&
        (turn_black ? (rt >= 7) : rf <= 2))
      return 0;
    break;
  }
  case CQ_GENERAL: {
    int8_t dc = ct - cf;
    int8_t dr = rt - rf;
    if (dc * dc + dr * dr != 1) return 0;
    // 不能出宫
    if (3 <= cf && cf <= 5 &&
        (turn_black ? (rt >= 7) : rf <= 2))
      return 0;
    break;
  }
  case CQ_PAO: {
    uint16_t pps[12];
    int n;
    if (cf == ct) {
      // 同一列
      if (rf < rt) {
        n = cchess_find_line(cs, (col == cf && rf < row && row <= rt), cchess_sort_by_row, pps);
      } else {
        assert(rf > rt);
        n = cchess_find_line(cs, (col == cf && rf > row && row >= rt), cchess_sort_by_row, pps);
      }
    } else if (rf == rt) {
      // 同一行
      if (cf < ct) {
        n = cchess_find_line(cs, (row == rf && cf < col && col <= ct), cchess_sort_by_col, pps);
      } else {
        assert(cf > ct);
        n = cchess_find_line(cs, (row == rf && cf > col && col >= ct), cchess_sort_by_col, pps);
      }
    } else return false; // 不在同一行或同一列

    if (n == 0) {
      assert(p1 == 0);
      op = CQ_OP_MOV;
    } else if (n == 2 && p1 != 0) {
      assert(pps[rf < rt || cf < ct] == p1);
      op = CQ_OP_EAT;
    } else {
      return 0; // more than 1 piece in path, invalid
    } 
    break;
  }
  default:
    assert(0);
  }
  
  if (update) {
    switch (op)
    {
    case CQ_OP_MOV:
      cchess_piece_move_internal_(cs, p0, to);
      break;
    case CQ_OP_EAT:
      assert(p1 != 0);
      cchess_piece_eaten_internal_(cs, p1);
      cchess_piece_move_internal_(cs, p0, to);
      break;
    default:
      assert(0);
      break;
    }
  }
  return op;
}
