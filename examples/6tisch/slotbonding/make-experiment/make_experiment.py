#!/usr/bin/python

import json
from hostnames import hostnames_to_nodeid

EXPERIMENT_BASE_DIR = '../experiments'
EXPERIMENT_NAME = 'initial-test'
EXPERIMENT_FULL_PATH = '{0}/{1}'.format(EXPERIMENT_BASE_DIR, EXPERIMENT_NAME)
MAX_ITERATIONS = 3

def make_iteration_file(experiment_name, iteration, coordinator, schedule):
    full_path_iteration = "{0}/{1}.json".format(EXPERIMENT_FULL_PATH, iteration)
    data = dict()
    data['experiment_name'] = experiment_name
    data['iteration'] = iteration
    data['hostnames'] = hostnames_to_nodeid
    data['coordinator'] = coordinator
    data['schedule'] = schedule
    with open(full_path_iteration, 'w') as outfile:
        json.dump(data, outfile)

def make_schedule(iteration):
    coordinator = 0
    schedule = dict()
    schedule[0] = {'parent': None, 'rx': [{'phy': 'TSCH_SLOTBONDING_50_KBPS_PHY', 'ts': 5, 'ch': 0, 'addr': 1}, {'phy': 'TSCH_SLOTBONDING_50_KBPS_PHY', 'ts': 10, 'ch': 0, 'addr': 1}], 'tx': []}  # node 0
    schedule[1] = {'parent': 0, 'rx': [], 'tx': [{'phy': 'TSCH_SLOTBONDING_50_KBPS_PHY', 'ts': 5, 'ch': 0, 'addr': 0}, {'phy': 'TSCH_SLOTBONDING_50_KBPS_PHY', 'ts': 10, 'ch': 0, 'addr': 0}]}  # node 1
    return schedule, coordinator

def main():
    import os
    if not os.path.exists(EXPERIMENT_FULL_PATH):
        os.makedirs(EXPERIMENT_FULL_PATH)
    else:
        raise Exception('The experiment directory already existed.')
    for iteration in range(MAX_ITERATIONS):
        schedule, coordinator = make_schedule(iteration)
        make_iteration_file(EXPERIMENT_NAME, iteration, coordinator, schedule)

if __name__ == "__main__":
    main()