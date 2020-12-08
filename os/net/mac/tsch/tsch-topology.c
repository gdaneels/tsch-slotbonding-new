//
// Created by mosaic on 23.10.20.
//

#include "net/mac/tsch/tsch-const.h"
#include "net/mac/tsch/tsch-types.h"
#include "net/mac/tsch/tsch-schedule.h"
#include "tsch-topology.h"

/* Log configuration */
#include "sys/log.h"
#define LOG_MODULE "TSCH Top"
#define LOG_LEVEL LOG_LEVEL_MAC

///// START CHANGE NUMTX /////


#define TSCH_TOPOLOGY_MAX_TX_ALLOCATIONS 1


///// START CHANGE NUMRX /////


#define TSCH_TOPOLOGY_MAX_RX_ALLOCATIONS 0


///// START CHANGE PARENT /////


static const linkaddr_t allowed_address = {{0x00, 0x12, 0x4b, 0x00, 0x19, 0x32, 0xe3, 0x20}};
//static const linkaddr_t allowed_address = {{0x00, 0x12, 0x4b, 0x00, 0x19, 0x32, 0xe4, 0xb2}};


typedef struct {
	uint16_t timeslot_offset;
	uint16_t channel_offset;
	linkaddr_t addrtofrom;
	uint8_t phy;
} force_topology_allocation;



///// START CHANGE TX /////


static const force_topology_allocation tx_allocations[TSCH_TOPOLOGY_MAX_TX_ALLOCATIONS] = {
		{4, 0, {{0x00, 0x12, 0x4b, 0x00,  0x19, 0x32, 0xe3, 0x20}}, TSCH_SLOTBONDING_50_KBPS_PHY}
};

//static const force_topology_allocation tx_allocations[TSCH_TOPOLOGY_MAX_TX_ALLOCATIONS] = {
//		{8, 0, {{0x00, 0x12, 0x4b, 0x00, 0x19, 0x32, 0xe4, 0xb2}}, v},
//};

//static const force_topology_allocation tx_allocations[TSCH_TOPOLOGY_MAX_TX_ALLOCATIONS] = {};



///// START CHANGE RX /////



//static const force_topology_allocation rx_allocations[TSCH_TOPOLOGY_MAX_RX_ALLOCATIONS] = {
//		{8, 0, {{0x00, 0x12, 0x4b, 0x00,  0x14, 0xd5, 0x2b, 0xab}}, TSCH_SLOTBONDING_1000_KBPS_PHY},
//};
//
//static const force_topology_allocation rx_allocations[TSCH_TOPOLOGY_MAX_RX_ALLOCATIONS] = {
//		{10, 0, {{0x00, 0x12, 0x4b, 0x00, 0x14, 0xd5, 0x2b, 0xab}}, TSCH_SLOTBONDING_50_KBPS_PHY}, {37, 0, {{0x00, 0x12, 0x4b, 0x00,  0x19, 0x32, 0xe4, 0xb2}}, TSCH_SLOTBONDING_50_KBPS_PHY}, {53, 0, {{0x00, 0x12, 0x4b, 0x00,  0x19, 0x32, 0xe2, 0x22}}, TSCH_SLOTBONDING_50_KBPS_PHY},
//};
//
//static const force_topology_allocation rx_allocations[TSCH_TOPOLOGY_MAX_RX_ALLOCATIONS] = {
//		{4, 0, {{0x00, 0x12, 0x4b, 0x00,  0x19, 0x32, 0xe4, 0xb2}}, TSCH_SLOTBONDING_1000_KBPS_PHY}, {8, 0, {{0x00, 0x12, 0x4b, 0x00,  0x14, 0xd5, 0x2b, 0xab}}, TSCH_SLOTBONDING_50_KBPS_PHY},
//};
//static const force_topology_allocation rx_allocations[TSCH_TOPOLOGY_MAX_RX_ALLOCATIONS] = {
//		{4, 0, {{0x00, 0x12, 0x4b, 0x00,  0x19, 0x32, 0xe4, 0xb2}}, TSCH_SLOTBONDING_50_KBPS_PHY},
//};
//
static const force_topology_allocation rx_allocations[TSCH_TOPOLOGY_MAX_RX_ALLOCATIONS] = {};

int
tsch_allowed_eb(const linkaddr_t *src_addr) {
	if (!tsch_is_coordinator) {
//		LOG_PRINT("In TSCH allowed packet.\n");
		if (!linkaddr_cmp(src_addr, &allowed_address)) {
//		LOG_PRINT("Packet from wrong source node.\n");
			return 0;
		}
	}
	return 1;
}

int
tsch_install_tx_cells(void) {
	int i;
//	if (!tsch_is_associated) {
//		LOG_PRINT("Can not install TX cells, not associated yet.\n");
//		return 0;
//	}
	for(i = 0; i < TSCH_TOPOLOGY_MAX_TX_ALLOCATIONS; i += 1) {
		tsch_schedule_add_link(tsch_schedule_slotframe_head(),
							   LINK_OPTION_TX,
							   LINK_TYPE_NORMAL,
							   &tx_allocations[i].addrtofrom,
							   tx_allocations[i].timeslot_offset,
							   tx_allocations[i].channel_offset,
							   tx_allocations[i].phy);
	}
	return 1;
}

int
tsch_install_rx_cells(void) {
	int i;
//	if (!tsch_is_coordinator) {
//		LOG_PRINT("Can not install RX cells, not associated/coordinator (yet).\n");
//		return 0;
//	}
	for(i = 0; i < TSCH_TOPOLOGY_MAX_RX_ALLOCATIONS; i += 1) {
		tsch_schedule_add_link(tsch_schedule_slotframe_head(),
							   LINK_OPTION_RX,
							   LINK_TYPE_NORMAL,
							   &rx_allocations[i].addrtofrom,
							   rx_allocations[i].timeslot_offset,
							   rx_allocations[i].channel_offset,
							   rx_allocations[i].phy);
	}
	return 1;
}
