/*
 * link_layer.h
 *
 *  Created on: Mar 27, 2017
 *      Author: gs-ms
 */

#ifndef INC_LINK_LAYER_H_
#define INC_LINK_LAYER_H_

#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "of_reed-solomon_gf_2_m.h"

#ifndef MAC_UNCODED_PACKET_SIZE
#define MAC_UNCODED_PACKET_SIZE 223
#endif
#ifndef MAC_PAYLOAD_SIZE
#define MAC_PAYLOAD_SIZE 219
#endif

#ifndef RADIO_PACKET_STRUCT
#define RADIO_PACKET_STRUCT
typedef union __attribute__ ((__packed__)) radio_packet_s{
        uint8_t     raw[MAC_UNCODED_PACKET_SIZE + 2];
        /* + 2 which are rssi+lqi */
        struct __attribute__ ((packed)){
            /* protocol information is 2 bytes:
             * * 4 bits protocol ID
             * * 4 bits ESI
             * * 4 bits K value
             * * 4 bits R value
             */
            uint8_t     info_n_esi;
            uint8_t     k_n_r;
            uint8_t     chunk_sequence;
            uint8_t     source_address;
            uint8_t     data[MAC_PAYLOAD_SIZE]; /* --> this shit contains the length... */ /* Inside the chunk */
            /* those two are not appended in transmit mode ofc */
            uint8_t     rssi;
            uint8_t     lqi;
        }fields;
}radio_packet_t;
#endif

typedef struct __attribute__ ((__packed__)) llc_parms_s{
	uint8_t k;
	uint8_t r;
	uint8_t esi;
	uint8_t chunk_seq;
	uint8_t src_addr;
}llc_parms_t;

typedef struct __attribute__ ((__packed__)) chunk_handler_s{
	of_rs_2_m_cb_t 	of_handler;
	void *			symb_tabs[OF_MAX_ENCODING_SYMBOLS];
	uint8_t 		chunk_reserved_memory[OF_MAX_ENCODING_SYMBOLS * OF_MAX_SYMBOL_SIZE];
	uint8_t 		current_chunk_count;
	uint8_t 		current_sequence;
	uint8_t 		last_sequence;
	bool 			module_initialised;
	bool 			library_initialised;
	llc_parms_t		llc;
}chunk_handler_t;

int init_chunk_handler(chunk_handler_t * handler);
/* Definition of the packet */
int build_llc_packet(uint8_t * buffer, uint8_t size, llc_parms_t * parms, radio_packet_t * p);
int get_llc_packet(radio_packet_t * p, llc_parms_t * parms);

int set_new_packet_to_chunk(chunk_handler_t * handler, radio_packet_t * p, uint8_t * chunk);
int get_new_packet_from_chunk(chunk_handler_t * handler, uint8_t * chunk, uint16_t size, uint8_t redundancy, radio_packet_t * p);

#endif /* INC_LINK_LAYER_H_ */
