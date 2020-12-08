//
// Created by mosaic on 23.10.20.
//

#ifndef SLOTBONDING_TSCH_TOPOLOGY_H
#define SLOTBONDING_TSCH_TOPOLOGY_H

/********** Includes **********/

#include "contiki.h"
#include "net/linkaddr.h"

/********** Functions *********/

/* Checks if given linkaddr is allowed for incoming EBs. */
int tsch_allowed_eb(const linkaddr_t *);
/* Install static cells in schedule after being associated. */
int tsch_install_cells(void);

/* Are we coordinator of the TSCH network? */
extern int tsch_is_coordinator;
/* Are we associated to a TSCH network? */
extern int tsch_is_associated;
/* A global variable that hosts the node ID */
extern uint16_t node_id;

#endif //SLOTBONDING_TSCH_TOPOLOGY_H
