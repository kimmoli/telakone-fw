#ifndef TK_TCP_STREAM_H
#define TK_TCP_STREAM_H

#include "hal_streams.h"

#define _tcp_stream_data \
  _base_sequential_stream_data

struct TcpStreamVMT
{
  _base_sequential_stream_methods
};

typedef struct
{
  const struct TcpStreamVMT *vmt;
  _tcp_stream_data
} TcpStream;

extern TcpStream TCPD1;

extern void tcpStreamInit(TcpStream *tsp);
extern void tcpStreamStop(TcpStream *tsp);

#endif
