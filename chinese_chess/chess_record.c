#include "chess_record.h"

void cchess_stream_init(struct cchess_stream *cs)
{
  cs->stream = NULL;
  cs->size = cs->cap = 0;
}
void cchess_stream_push_init0(struct cchess_stream *cs)
{

}

void cchess_stream_push_init1(struct cchess_stream *cs, const uint8_t *stream, int num_pieces)
{

}

void cchess_stream_push_move(struct cchess_stream *cs, uint8_t from, uint8_t to)
{

}

void cchess_stream_push_eat(struct cchess_stream *cs, uint8_t from, uint8_t to, uint8_t piece)
{

}

int cchess_stream_check(struct cchess_stream *cs)
{
  return 0;
}
