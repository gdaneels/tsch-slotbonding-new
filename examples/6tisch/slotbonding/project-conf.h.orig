/*
 * Copyright (c) 2015, SICS Swedish ICT.
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
 */

/**
 * \author Dries Van Leemput <dries.vanleemput@ugent.be>
 * \author Glenn Daneels <glenn.daneels@uantwerpen.be>
 */

#ifndef PROJECT_CONF_H_
#define PROJECT_CONF_H_


/*******************************************************/
/******************* Configure Radio *******************/
/*******************************************************/
//
/* Use CC1200 radio */
#define ZOUL_CONF_USE_CC1200_RADIO 1

/* Correctly read timing in received packets */
//#define CC1200_CONF_USE_GPIO2 1

/* Use multi-PHY default PHY */
//#define CC1200_CONF_RF_CFG cc1200_50kbps_tsch_timing

/* More accurate timing */
#define SYS_CTRL_CONF_OSC32K_USE_XTAL 1

//#undef  NETSTACK_CONF_RADIO
//#define NETSTACK_CONF_RADIO         cc1200_driver
//#define CC1200_CONF_USE_GPIO2       0
//#define CC1200_CONF_USE_RX_WATCHDOG 0
//#define ANTENNA_SW_SELECT_DEF_CONF  ANTENNA_SW_SELECT_SUBGHZ

/*******************************************************/
/******************* Configure TSCH ********************/
/*******************************************************/

#define QUEUEBUF_CONF_NUM 8
#define TSCH_CONF_MAX_INCOMING_PACKETS 8

#define TSCH_CONF_EB_PERIOD (7 * CLOCK_SECOND)

#define TSCH_CONF_SLOTBONDING 1

//#define TSCH_CONF_SLOTBONDING_DEFAULT_TIMINGS_US cc1200_802154g_863_870_fsk_50kbps.tsch_timing
#define TSCH_CONF_SLOTBONDING_DEFAULT_TIMINGS_US cc1200_868_4gfsk_1000kbps.tsch_timing

#define TSCH_CONF_SLOTBONDING_1000_KBPS_PHY 0x0 // should be max 4 bits, is used in 6P cell options
#define	TSCH_CONF_SLOTBONDING_50_KBPS_PHY 0x1 // should be max 4 bits, is used in 6P cell options

// define the PHY that is used to scan and associate to the network
// this PHY should equal the PHY of the minimal cell
#define TSCH_CONF_SLOTBONDING_ASSOCIATE cc1200_802154g_863_870_fsk_50kbps
//#define TSCH_CONF_SLOTBONDING_ASSOCIATE cc1200_868_4gfsk_1000kbps

/* Force a topology using the tsch_topology.* functionality. */
#define TSCH_CONF_FORCE_TOPOLOGY 1

/* Force a star topology with only the coordinator transmitting an EB. */
#define TSCH_CONF_STAR_TOPOLOGY 0
/* IEEE802.15.4 PANID */

///// START CHANGE PANID /////

//#define IEEE802154_CONF_PANID 0x0353 //

/* Do not start TSCH at init, wait for NETSTACK_MAC.on() */
#define TSCH_CONF_AUTOSTART 1

#define TSCH_CONF_WITH_SIXTOP              0

/* 6TiSCH minimal schedule length.
 * Larger values result in less frequent active slots: reduces capacity and saves energy. */
//#define TSCH_SCHEDULE_CONF_DEFAULT_LENGTH 17


///// START CHANGE SLOTFRAMELENGTH /////


/* Set coordinator node */
//#define COORDINATOR_ID 58144
//#define COORDINATOR_ID 4006

#define TSCH_CONF_BURST_MAX_LEN 0

#define TSCH_CONF_JOIN_HOPPING_SEQUENCE TSCH_HOPPING_SEQUENCE_1_1_sb
/* Define channel hopping sequence */
#define TSCH_CONF_DEFAULT_HOPPING_SEQUENCE TSCH_HOPPING_SEQUENCE_3_8_sb


///// START CHANGE MAX_FRAME_RETRIES /////


/*******************************************************/
/************* Other system configuration **************/
/*******************************************************/

/* Logging */
#define LOG_CONF_LEVEL_RPL                         LOG_LEVEL_NONE
#define LOG_CONF_LEVEL_TCPIP                       LOG_LEVEL_NONE
#define LOG_CONF_LEVEL_IPV6                        LOG_LEVEL_NONE
#define LOG_CONF_LEVEL_6LOWPAN                     LOG_LEVEL_NONE
#define LOG_CONF_LEVEL_MAC                         LOG_LEVEL_INFO
#define LOG_CONF_LEVEL_FRAMER                      LOG_LEVEL_NONE
#define TSCH_LOG_CONF_PER_SLOT                     1

#endif /* PROJECT_CONF_H_ */
