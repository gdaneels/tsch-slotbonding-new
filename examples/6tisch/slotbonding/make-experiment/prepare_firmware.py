import json
import sys
import fileinput

data = dict()

def read_out_experiment(experiment):
    global data
    with open(experiment) as json_file:
        data = json.load(json_file)
        # print(data)

def node_id_to_hostname(hostname=None):
    if hostname is None:
        raise Exception('Given hostname is None.')
    for node_id, info in data["hostnames"].items():
        if info['hostname'] == hostname:
            return node_id
    raise Exception('Did not find the node_id for hostname {0}'.format(hostname))

def get_addr(node_id):
    if node_id not in data["hostnames"]:
        raise Exception('Node id is not in hostnames dictionary present.')
    return data["hostnames"][node_id]['addr']

def make_allowed_address(node_id=None):
    if node_id not in data['schedule']:
        raise Exception('Node ID = {0} not in schedule for make_allowed_address of prepare_firmware!'.format(node_id))
    statement = None
    if node_id != data['coordinator']:
        statement = "static const linkaddr_t allowed_address = {0};".format(get_addr(data['schedule'][node_id]['parent']))
    else: # in the case the node is the coordinator, the addr is not use but it should be defined, so just take yourself
        statement = "static const linkaddr_t allowed_address = {0};".format(get_addr(node_id))
    return statement

def make_tx(node_id=None):
    if node_id not in data['schedule']:
        raise Exception('Node ID = {0} not in schedule for make_tx of prepare_firmware!'.format(node_id))
    statement = "static const force_topology_allocation tx_allocations[TSCH_TOPOLOGY_MAX_TX_ALLOCATIONS] = {"
    if len(data['schedule'][node_id]['tx']) > 0:
        for cell in data['schedule'][node_id]['tx']:
            statement += "{{{timeslot}, {channel}, {address}, {phy}}},".format(timeslot=cell['ts'], channel=cell['ch'], address=get_addr(cell['addr']), phy=cell['phy'])
    statement += "};"
    return statement

def make_rx(node_id=None):
    if node_id not in data['schedule']:
        raise Exception('Node ID = {0} not in schedule for make_rx of prepare_firmware!'.format(node_id))
    statement = "static const force_topology_allocation rx_allocations[TSCH_TOPOLOGY_MAX_RX_ALLOCATIONS] = {"
    if len(data['schedule'][node_id]['rx']) > 0:
        for cell in data['schedule'][node_id]['rx']:
            statement += "{{{timeslot}, {channel}, {address}, {phy}}},".format(timeslot=cell['ts'], channel=cell['ch'], address=get_addr(cell['addr']), phy=cell['phy'])
    statement += "};"
    return statement

def make_numtx(node_id=None):
    if node_id not in data['schedule']:
        raise Exception('Node ID = {0} not in schedule for make_numtx of prepare_firmware!'.format(node_id))
    statement = "#define TSCH_TOPOLOGY_MAX_TX_ALLOCATIONS "
    if len(data['schedule'][node_id]['tx']) > 0:
        statement += "{0}".format(len(data['schedule'][node_id]['tx']))
    else:
        statement += "0"
    return statement

def make_numrx(node_id=None):
    if node_id not in data['schedule']:
        raise Exception('Node ID = {0} not in schedule for make_numrx of prepare_firmware!'.format(node_id))
    statement = "#define TSCH_TOPOLOGY_MAX_RX_ALLOCATIONS "
    if len(data['schedule'][node_id]['rx']) > 0:
        statement += "{0}".format(len(data['schedule'][node_id]['rx']))
    else:
        statement += "0"
    return statement

def replace(filename, search_text, replace_with):
    with fileinput.FileInput(filename, inplace = True, backup ='.bak') as f:
        for line in f:
            if search_text.strip() == line.strip():
                print(replace_with, end="")
            else:
                print(line, end="")

def make_coordinator(node_id=None):
    path_to_node_file = "../../../../examples/6tisch/multi-phy-slotbonding/node.c"
    if node_id is None:
        raise Exception('Node ID is None in make_coordinator of prepare_firmware!')
    statement = "static linkaddr_t coordinator_addr = {0};".format(get_addr(data['coordinator']))
    if node_id == data['coordinator']:
        print("This node={0} becomes the coordinator.".format(node_id))
    replace(path_to_node_file, "///// START CHANGE COORDINATOR /////", statement)

def make_destaddr(node_id=None):
    path_to_node_file = "../../../../examples/6tisch/multi-phy-slotbonding/node.c"
    if node_id is None:
        raise Exception('Node ID is None in make_destaddr of prepare_firmware!')
    statement = None
    if node_id != data['coordinator']:
        statement = "static linkaddr_t dest_addr = {0};".format(get_addr(data['schedule'][node_id]['parent']));
    else:
        statement = "static linkaddr_t dest_addr = {0};".format(get_addr(node_id)); # put it to its own address, NOT correct, but will be ignored.
    replace(path_to_node_file, "///// START CHANGE DESTADDR /////", statement)

def make_frameretries():
    path_to_node_file = "../../../../examples/6tisch/multi-phy-slotbonding/project-conf.h"
    statement = "#define TSCH_CONF_MAC_MAX_FRAME_RETRIES {0}".format(data['r_max'])
    replace(path_to_node_file, "///// START CHANGE MAX_FRAME_RETRIES /////", statement)

def make_slotframelength():
    path_to_node_file = "../../../../examples/6tisch/multi-phy-slotbonding/project-conf.h"
    statement = "#define TSCH_SCHEDULE_CONF_DEFAULT_LENGTH {0}".format(data['slotframe_size'])
    replace(path_to_node_file, "///// START CHANGE SLOTFRAMELENGTH /////", statement)

def make_panid():
    path_to_node_file = "../../../../examples/6tisch/multi-phy-slotbonding/project-conf.h"
    statement = "#define IEEE802154_CONF_PANID {0}".format(data['pan_id'])
    replace(path_to_node_file, "///// START CHANGE PANID /////", statement)

def adjust_topology(node_id=None):
    if node_id is None:
        raise Exception('Node ID is None in adjust_topology of prepare_firmware!')
    path_to_topoloy_file = "../../../../os/net/mac/tsch/tsch-topology.c"
    replace(path_to_topoloy_file, "///// START CHANGE PARENT /////", make_allowed_address(node_id))
    replace(path_to_topoloy_file, "///// START CHANGE NUMTX /////", make_numtx(node_id))
    replace(path_to_topoloy_file, "///// START CHANGE NUMRX /////", make_numrx(node_id))
    replace(path_to_topoloy_file, "///// START CHANGE TX /////", make_tx(node_id))
    replace(path_to_topoloy_file, "///// START CHANGE RX /////", make_rx(node_id))

def main():
    if len(sys.argv) < 3:
        raise Exception('Not enough arguments for prepare_firmware.py (only {0} arguments).'.format(len(sys.argv)))
    experiment_path = sys.argv[1]
    hostname = str(sys.argv[2]).split(".")[0] # only the first part is important
    read_out_experiment(experiment_path)
    node_id = node_id_to_hostname(hostname=hostname)
    print("Preparing the experiment at path={0} for node with node ID={1}".format(experiment_path, node_id))
    adjust_topology(node_id=node_id)
    make_coordinator(node_id=node_id)
    make_destaddr(node_id=node_id)
    make_panid()
    make_slotframelength()
    make_frameretries()

if __name__ == "__main__":
    main()