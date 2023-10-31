#include <stdio.h>
#include <ap_fixed.h>
#include <ap_int.h>
#include <iostream>
#include <hls_stream.h>

using namespace std;

typedef ap_int<16> uint16;
typedef ap_uint<1> uint1;

typedef struct
  {uint16  tdata;
  uint1 tlast ;
  } data_t;
void packet(hls:: stream< data_t > &In, hls:: stream< data_t > &out);
