LOG_DIRECTORY=/users/gdaneels/logs
PERMANENT_STORAGE=/groups/ilabt-imec-be/tsch-slotbonding

NODEID=$1
EXPERIMENT_NAME=$2
ITERATION=$3

# copy the log and output files
cp -r ${LOG_DIRECTORY} ${PERMANENT_STORAGE}
# remove the logs
rm -r ${LOG_DIRECTORY}

