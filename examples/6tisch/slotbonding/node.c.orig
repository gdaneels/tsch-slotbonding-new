/*
 * Copyright (c) 2017, RISE SICS.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */

/**
 * \file
 *         NullNet unicast example
 * \author
*         Simon Duquennoy <simon.duquennoy@ri.se>
 *
 */

#include "contiki.h"
#include "net/netstack.h"
#include "net/nullnet/nullnet.h"

#include <string.h>
#include <stdio.h> /* For printf() */

/* Log configuration */
#include "sys/log.h"
#include "net/mac/tsch/tsch.h"
#include "net/mac/tsch/tsch-asn.h"
#include "net/mac/tsch/tsch-slot-operation.h"

#include "net/mac/tsch/sixtop/sixtop.h"
//#include "sf-simple.h"

#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

/* Configuration */
//#define SEND_INTERVAL (1 * CLOCK_SECOND)
//#define SEND_INTERVAL 128l


///// START CHANGE SENDINTERVAL /////


//#define STATS_INTERVAL (60 * CLOCK_SECOND)
#define START_EXPERIMENT_INTERVAL (5 * 60 * CLOCK_SECOND) // start after 5 minutes
#define STOP_EXPERIMENT_INTERVAL (9 * 60 * CLOCK_SECOND) // stop after 15 minutes so you havea  10 minutes experiment

//#define START_EXPERIMENT_INTERVAL (10 * 60 * CLOCK_SECOND) // start after 5 minutes
//#define STOP_EXPERIMENT_INTERVAL (15 * 60 * CLOCK_SECOND) // stop after 15 minutes so you havea  10 minutes experiment


#define PAYLOAD_SIZE 15 // 8 bytes for the linkaddr, plus 2 byte for the count
//#define PAYLOAD_SIZE 104 // 8 bytes for the linkaddr, plus 2 byte for the count

//static linkaddr_t orig_addr = {{0x00, 0x12, 0x4b, 0x00, 0x14, 0xd5, 0x2b, 0xab}};
//static linkaddr_t dest_addr = {{0x00, 0x12, 0x4b, 0x00, 0x19, 0x32, 0xe4, 0xb2}};


///// START CHANGE DESTADDR /////


//static linkaddr_t dest_addr = {{0x00, 0x12, 0x4b, 0x00, 0x19, 0x32, 0xe3, 0x20}};


//static linkaddr_t node_1 = {{0x00, 0x12, 0x4b, 0x00, 0x14, 0xd5, 0x2b, 0xab}};
//static linkaddr_t node_2 = {{0x00, 0x12, 0x4b, 0x00, 0x19, 0x32, 0xe4, 0xb2}};
#if MAC_CONF_WITH_TSCH
#include "net/mac/tsch/tsch.h"

///// START CHANGE COORDINATOR /////


//static linkaddr_t coordinator_addr =  {{ 0x00, 0x12, 0x4b, 0x00, 0x19, 0x32, 0xe3, 0x20 }};

static int start_experiment = 0;

#endif /* MAC_CONF_WITH_TSCH */

/*---------------------------------------------------------------------------*/
PROCESS(nullnet_example_process, "NullNet unicast example");
//PROCESS(stats_process, "Process to print out stats");
//PROCESS(start_process, "Process to print start experiment");
//PROCESS(stop_process, "Process to print stop experiment");

PROCESS(start_stop_process, "Process to print start and stop experiment");

//PROCESS(sixp_process, "6P process");
//PROCESS(allocate_process, "Statically allocate a cell");

//AUTOSTART_PROCESSES(&nullnet_example_process, &sixp_process);
//AUTOSTART_PROCESSES(&nullnet_example_process, &stats_process, &start_process, &stop_process);

// monitoring
//AUTOSTART_PROCESSES(&nullnet_example_process, &start_process, &stop_process);
AUTOSTART_PROCESSES(&nullnet_example_process, &start_stop_process);

/*---------------------------------------------------------------------------*/
void input_callback(const void *data, uint16_t len,
					const linkaddr_t *src, const linkaddr_t *dest) {
	unsigned char addr_and_count[PAYLOAD_SIZE];
	uint16_t count;
	struct tsch_asn_t received_asn;
//	LOG_INFO("%u", len);
//	LOG_INFO("\n");
//	LOG_INFO("%i", sizeof(addr_and_count));
//	LOG_INFO("\n");
	if (len == sizeof(addr_and_count)) {
//		unsigned count;
//		memcpy(&count, data, sizeof(count));
		memcpy(&addr_and_count, data, sizeof(addr_and_count));
		count = addr_and_count[8];
		count |= (addr_and_count[9] << 8);
		received_asn.ls4b = (uint32_t)addr_and_count[10];
		received_asn.ls4b |= (uint32_t)addr_and_count[11] << 8;
		received_asn.ls4b |= (uint32_t)addr_and_count[12] << 16;
		received_asn.ls4b |= (uint32_t)addr_and_count[13] << 24;
		received_asn.ms1b = (uint8_t)addr_and_count[14];
		int32_t diff = TSCH_ASN_DIFF(tsch_current_asn, received_asn);
		if (linkaddr_cmp(&coordinator_addr, &linkaddr_node_addr)) { // only print this if you are the coordinator
			LOG_INFO(">>> RX %u (asn %02x.%08lx - d %" PRIu32 ") from ", count, received_asn.ms1b, received_asn.ls4b, diff);
			linkaddr_t orig_src_addr =  {{ addr_and_count[0], addr_and_count[1], addr_and_count[2], addr_and_count[3], addr_and_count[4], addr_and_count[5], addr_and_count[6], addr_and_count[7] }};
			LOG_INFO_LLADDR(&orig_src_addr);
			LOG_INFO_("\n");
		}
		if (!linkaddr_cmp(&coordinator_addr, &linkaddr_node_addr)) { // if this is not the coordinator, forward to parent
//			LOG_INFO(">>> FW %u to ", count);
//			LOG_INFO_LLADDR(&dest_addr);
//			LOG_INFO_("\n");
			NETSTACK_NETWORK.output_extra(&dest_addr, src, addr_and_count);
		}

	}
}
/*---------------------------------------------------------------------------*/

void print_network_information() {
	struct tsch_slotframe *sf;
//	LOG_INFO("TSCH status:\n");

	LOG_INFO("-- Is coordinator: %u\n", tsch_is_coordinator);
	LOG_INFO("-- Is associated: %u\n", tsch_is_associated);
	if(tsch_is_associated) {
		struct tsch_neighbor *n = tsch_queue_get_time_source();
		LOG_INFO("-- PAN ID: 0x%x\n", frame802154_get_pan_id());
//		LOG_INFO("-- Is PAN secured: %u\n", tsch_is_pan_secured);
//		LOG_INFO("-- Join priority: %u\n", tsch_join_priority);
		LOG_INFO("-- Time source: ");
		if(n != NULL) {
			LOG_INFO_LLADDR(tsch_queue_get_nbr_address(n));
			LOG_INFO_("\n");
		} else {
			LOG_INFO_("none\n");
		}
//		LOG_INFO("-- Last synchronized: %lu seconds ago\n",
//				 (clock_time() - tsch_last_sync_time) / CLOCK_SECOND);
//		LOG_INFO("-- Drift w.r.t. coordinator: %ld ppm\n",
//				 tsch_adaptive_timesync_get_drift_ppm());
//		LOG_INFO("-- Network uptime: %lu seconds\n",
//				 (unsigned long)(tsch_get_network_uptime_ticks() / CLOCK_SECOND));
	}

	if(!tsch_is_locked()) {
		sf = tsch_schedule_slotframe_head();
		if (sf == NULL) {
			LOG_INFO("TSCH schedule: no slotframe\n");
		} else {
//			LOG_INFO("TSCH schedule:\n");
			while (sf != NULL) {
				struct tsch_link *l = list_head(sf->links_list);
				LOG_INFO("-- SF: handle %u, size %u, links:\n", sf->handle, sf->size.val);
				while (l != NULL) {
					LOG_INFO("---- Opt %02x, t %u, ts %u, ch %u, phy %u, addr ", l->link_options, l->link_type, l->timeslot, l->channel_offset
#if TSCH_SLOTBONDING
							, l->current_phy
#else
							, 0
#endif
					);
					LOG_INFO_LLADDR(&l->addr);
					LOG_INFO_("\n");
					l = list_item_next(l);
				}
				sf = tsch_schedule_slotframe_next(sf);
			}
		}
	}
}

void print_network_information_short() {
	LOG_INFO("-- Is associated: %u\n", tsch_is_associated);
}

void print_transmission_information() {
	if (slotbonding_num_ack > 10) {
		LOG_INFO_(" (ETX = %" PRIu16 ", NUM_TX = %" PRIu16 ", NUM_ACK = %" PRIu16 ")\n", (uint16_t)(slotbonding_num_tx / slotbonding_num_ack),
				slotbonding_num_tx, slotbonding_num_ack);
	} else {
		LOG_INFO_(" (ETX = UNDEFINED, NUM_TX = %"
		PRIu16
		", NUM_ACK = %"
		PRIu16
		")\n", slotbonding_num_tx, slotbonding_num_ack);
	}
//	if (!linkaddr_cmp(&coordinator_addr, &linkaddr_node_addr)) {
////		if (slotbonding_num_tx > 50 && slotbonding_num_ack < 2) {
//		if (slotbonding_num_tx > 20) {
//			LOG_INFO("Disassociating from network because for some reason, I can't connect.\n");
//			slotbonding_num_tx = 0;
//			slotbonding_num_ack = 0;
//			tsch_disassociate();
//		}
//	}
}

//PROCESS_THREAD(allocate_process, ev, data) {
//	static struct etimer start_timer;
//	static struct etimer allocate_periodical_timer;
//	PROCESS_BEGIN();
//
//	uint8_t link_options;
//
//	etimer_set(&start_timer, (20 * CLOCK_SECOND));
//	PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&start_timer));
//	LOG_INFO("Start timer expired...\n");
//	// the root
//	if (linkaddr_cmp(&dest_addr, &linkaddr_node_addr)) {
//		link_options = linkaddr_cmp(&dest_addr, &linkaddr_node_addr) ? LINK_OPTION_RX : LINK_OPTION_TX;
//		tsch_schedule_add_link(tsch_schedule_slotframe_head(),
//							   link_options,
//							   LINK_TYPE_NORMAL, &orig_addr,
//							   5, 5, 0);
//		tsch_schedule_add_link(tsch_schedule_slotframe_head(),
//							   link_options,
//							   LINK_TYPE_NORMAL, &orig_addr,
//							   6, 6, 1);
//		LOG_INFO("Allocated a slot at the root...\n");
//	}
//	// the leaf
//	if (!linkaddr_cmp(&dest_addr, &linkaddr_node_addr)) {
//		etimer_set(&allocate_periodical_timer, (5 * CLOCK_SECOND));
//		while (1) {
//			PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&allocate_periodical_timer));
//			LOG_INFO("Polling for association...\n");
//			if (tsch_is_associated) {
//				LOG_INFO("Leaf is associated...\n");
//				link_options = linkaddr_cmp(&dest_addr, &linkaddr_node_addr) ? LINK_OPTION_RX : LINK_OPTION_TX;
//				tsch_schedule_add_link(tsch_schedule_slotframe_head(),
//									   link_options,
//									   LINK_TYPE_NORMAL, &dest_addr,
//									   5, 5, 0);
//				tsch_schedule_add_link(tsch_schedule_slotframe_head(),
//									   link_options,
//									   LINK_TYPE_NORMAL, &dest_addr,
//									   6, 6, 1);
//				LOG_INFO("Allocated a slot at the leaf...\n");
//				break;
//			} else {
//				etimer_reset(&allocate_periodical_timer);
//			}
//		}
//	}
//	LOG_INFO("Ending allocation process...\n");
//	PROCESS_END();
//}

PROCESS_THREAD(nullnet_example_process, ev, data) {
	static struct etimer periodic_timer;
	static uint16_t count = 0;
	static unsigned char addr_and_count[PAYLOAD_SIZE];
	addr_and_count[0] = linkaddr_node_addr.u8[0];
	addr_and_count[1] = linkaddr_node_addr.u8[1];
	addr_and_count[2] = linkaddr_node_addr.u8[2];
	addr_and_count[3] = linkaddr_node_addr.u8[3];
	addr_and_count[4] = linkaddr_node_addr.u8[4];
	addr_and_count[5] = linkaddr_node_addr.u8[5];
	addr_and_count[6] = linkaddr_node_addr.u8[6];
	addr_and_count[7] = linkaddr_node_addr.u8[7];
	addr_and_count[8] = 0;
	addr_and_count[9] = 0;
	addr_and_count[10] = 0; // asn.ls4b
	addr_and_count[11] = 0; // asn.ls4b >> 8
	addr_and_count[12] = 0; // asn.ls4b >> 16
	addr_and_count[13] = 0; // asn.ls4b >> 24
	addr_and_count[14] = 0; // asn.ms1b

	PROCESS_BEGIN();

	#if MAC_CONF_WITH_TSCH
	tsch_set_coordinator(linkaddr_cmp(&coordinator_addr, &linkaddr_node_addr));
	#endif /* MAC_CONF_WITH_TSCH */

	nullnet_set_input_callback(input_callback);
	if (!linkaddr_cmp(&coordinator_addr, &linkaddr_node_addr)) {

		etimer_set(&periodic_timer, SEND_INTERVAL);
		while (1) {
			PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
			if (!linkaddr_cmp(&dest_addr, &linkaddr_node_addr)) {
				addr_and_count[8] = ((uint16_t) count >> 0) & 0xFF;
				addr_and_count[9] = ((uint16_t) count >> 8) & 0xFF;
				addr_and_count[10] = tsch_current_asn.ls4b; // asn.ls4b
				addr_and_count[11] = tsch_current_asn.ls4b >> 8; // asn.ls4b >> 8
				addr_and_count[12] = tsch_current_asn.ls4b >> 16; // asn.ls4b >> 16
				addr_and_count[13] = tsch_current_asn.ls4b >> 24; // asn.ls4b >> 24
				addr_and_count[14] = tsch_current_asn.ms1b; // asn.ms1b
				LOG_INFO(">>> TX %u (asn %02x.%08lx) to ", count, tsch_current_asn.ms1b, tsch_current_asn.ls4b);
				LOG_INFO_LLADDR(&dest_addr);
				print_transmission_information();
				NETSTACK_NETWORK.output_extra(&dest_addr, &linkaddr_node_addr, addr_and_count);
				count++;
				if (start_experiment == 0 && !linkaddr_cmp(&coordinator_addr, &linkaddr_node_addr)) {
					// only do so if the experiment has not started yet
					if (count > 45 && slotbonding_num_ack < 2) {
						LOG_INFO("Disassociating from network because for some reason, I can't connect.\n");
						count = 0;
						slotbonding_num_tx = 0;
						slotbonding_num_ack = 0;
						tsch_disassociate();
					}
				}
			}
				etimer_reset(&periodic_timer);
		}
	}

	PROCESS_END();
}
/*---------------------------------------------------------------------------*/


//PROCESS_THREAD(stats_process, ev, data) {
//static struct etimer periodic_timer;
//
//PROCESS_BEGIN();
//etimer_set(&periodic_timer, STATS_INTERVAL);
//while (1) {
//PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
//print_network_information();
//etimer_reset(&periodic_timer);
//}
//
//
//PROCESS_END();
//}
/*---------------------------------------------------------------------------*/

// process to print out start of experiment
PROCESS_THREAD(start_stop_process, ev, data) {
	static struct etimer two_shot_timer1;
	PROCESS_BEGIN();
	etimer_set(&two_shot_timer1, START_EXPERIMENT_INTERVAL);
	PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&two_shot_timer1));
	LOG_INFO("***** START EXPERIMENT *****\n");
	start_experiment = 1;
	if (!linkaddr_cmp(&coordinator_addr, &linkaddr_node_addr)) {
		print_transmission_information();
	}
		print_network_information_short();

	// reset the timer
	etimer_reset(&two_shot_timer1);
	PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&two_shot_timer1)); // wait again for it to expire

	print_network_information();
	LOG_INFO("***** STOP EXPERIMENT *****\n");
	if (!linkaddr_cmp(&coordinator_addr, &linkaddr_node_addr)) {
		print_transmission_information();
	}
	PROCESS_END();
}

//
//// process to print out start of experiment
//PROCESS_THREAD(start_process, ev, data) {
//	static struct etimer one_shot_timer1;
//	PROCESS_BEGIN();
//	etimer_set(&one_shot_timer1, START_EXPERIMENT_INTERVAL);
//	PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&one_shot_timer1));
//	LOG_INFO("***** START EXPERIMENT *****\n");
//	start_experiment = 1;
//	if (!linkaddr_cmp(&coordinator_addr, &linkaddr_node_addr)) {
//		print_transmission_information();
//	}
//	print_network_information_short();
//	PROCESS_END();
//}
///*---------------------------------------------------------------------------*/
//
//// process to print out end of experiment
//PROCESS_THREAD(stop_process, ev, data) {
//	static struct etimer one_shot_timer2;
//	PROCESS_BEGIN();
//	etimer_set(&one_shot_timer2, STOP_EXPERIMENT_INTERVAL);
//	PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&one_shot_timer2));
//	print_network_information();
//	LOG_INFO("***** STOP EXPERIMENT *****\n");
//	if (!linkaddr_cmp(&coordinator_addr, &linkaddr_node_addr)) {
//		print_transmission_information();
//	}
//	PROCESS_END();
//}
/*---------------------------------------------------------------------------*/


//PROCESS_THREAD(sixp_process, ev, data) {
////	static struct etimer periodic_timer;
////	static unsigned count = 0;
//
//	static int added_num_of_links = 0;
//	static struct etimer et;
//	struct tsch_neighbor *n;
//
//	PROCESS_BEGIN();
//
//	sixtop_add_sf(&sf_simple_driver);
//
//#if MAC_CONF_WITH_TSCH
//				tsch_set_coordinator(linkaddr_cmp(&coordinator_addr, &linkaddr_node_addr));
//#endif /* MAC_CONF_WITH_TSCH */
//
//				etimer_set(&et, CLOCK_SECOND * 15);
//				while(1) {
//					PROCESS_YIELD_UNTIL(etimer_expired(&et));
//					etimer_reset(&et);
//
//					/* Get time-source neighbor */
//					n = tsch_queue_get_time_source();
//
//					if(!linkaddr_cmp(&coordinator_addr, &linkaddr_node_addr)) {
////						if((added_num_of_links == 1) || (added_num_of_links == 3)) {
////							printf("--------------------------- App : Add a link\n");
////							sf_simple_add_links(&n->addr, 1);
////						} else if(added_num_of_links == 5) {
////							printf("--------------------------- App : Delete a link\n");
////							sf_simple_remove_links(&n->addr);
////						}
//						if((added_num_of_links == 0)) {
//							printf("--------------------------- App : Add a link\n");
//							if (linkaddr_cmp(&node_1, &linkaddr_node_addr)) {
////								sf_simple_add_links_ts_ch(&n->addr, 1, TSCH_CONF_SLOTBONDING_1000_KBPS_PHY);
//								sf_simple_add_links_ts_ch(&n->addr, 1, 4, 0, TSCH_CONF_SLOTBONDING_1000_KBPS_PHY);
//							} else if (linkaddr_cmp(&node_2, &linkaddr_node_addr)) {
////								sf_simple_add_links_ts_ch(&n->addr, 1, TSCH_CONF_SLOTBONDING_50_KBPS_PHY);
//								sf_simple_add_links_ts_ch(&n->addr, 1, 11, 0, TSCH_CONF_SLOTBONDING_50_KBPS_PHY);
//							}
//						}
//						added_num_of_links++;
//					}
//				}
//
//	PROCESS_END();
//}
/*---------------------------------------------------------------------------*/
