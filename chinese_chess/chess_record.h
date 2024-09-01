#ifndef _CHAOS_CHESS_RECORD_H_
#define _CHAOS_CHESS_RECORD_H_
#include <stdint.h>

// OP, no oprand
#define CQ_OP_INIT0 0x01
// OP, num (PIECE POS)+
#define CQ_OP_INIT1 0x02
// OP, start_pos, end_pos
#define CQ_OP_MOV 0x03
// OP_EAT start_pos, end_pos, piece
#define CQ_OP_EAT 0x04

struct cchess_stream
{
  uint8_t *stream;
  int size;
  int cap;
};

typedef union 
{
    uint32_t i32;
    struct {
      uint8_t op;
      uint8_t from;
      uint8_t to;
      uint8_t piece; // contain color flag
    };
} chess_record_t;

static_assert(sizeof(chess_record_t) == 4, "flat record must be 4 bytes");
// flat record


void cchess_stream_init(struct cchess_stream *cs);
void cchess_stream_destory(struct cchess_stream *cs);



#endif  /* _CHAOS_CHESS_RECORD_H_ */