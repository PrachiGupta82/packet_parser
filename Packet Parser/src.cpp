#include "pkt_header.h"

void packet(hls:: stream< data_t > &in, hls:: stream< data_t > &out)
{
#pragma HLS inTERFACE axis register both port=in
#pragma HLS inTERFACE axis register both port=out
#pragma HLS PIPELinE II=1

    data_t in_rd;
    data_t out_wr;

	static ap_uint<18> head_count = 0;
#pragma HLS RESET variable=head_count

	static ap_uint<18> msg_count = 0;
#pragma HLS RESET variable=msg_count


	enum state{tag_H,tag_M, header, msg, footer} ;
	static state present_state;
#pragma HLS RESET variable=present_state

	//cout << " state " << present_state << endl;

	in>>in_rd;
	cout<<"in="<<in_rd.tdata<<endl;
	switch(present_state)
	{
	case(tag_H):{
					head_count= in_rd.tdata;
					present_state=tag_M;
				}
				break;

	case(tag_M):{
					msg_count= in_rd.tdata;
					if(head_count>0)
						present_state=header;
					else
						present_state=msg;
				}
				break;
	case(header):if(head_count>1)
				{
					head_count=head_count-1;
					present_state=header;
				}
				else{
						present_state=msg;
					}
				break;
	case(msg):if(msg_count>1)
				{
					msg_count=msg_count-1;
					out_wr.tlast=0;
					out_wr.tdata=in_rd.tdata;
					out<<out_wr;
					present_state=msg;
				}
			  else{
				  out_wr.tdata=in_rd.tdata;
				  out_wr.tlast=1;
				  out<<out_wr;
				  present_state=footer;
			  	  }
				  break;
	case(footer):if(!in_rd.tlast)
				{
					present_state=footer;
				}
				else{
					present_state=tag_H;
					}
				break;

	}

	}
