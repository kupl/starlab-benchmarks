#!/bin/bash

INFER_PATH=/home/saver/project/footpatch/infer-linux64-v0.9.3/infer/bin/infer
MAKE_CMD="make -j20"

export TIMEFORMAT='%3R'

rm /tmp/td_candidates/*
source RECORD

make clean

CMD="sleep 2"
{ time \
  $INFER_PATH -j 20 --headers --no-filtering -- $MAKE_CMD \
    ; } 2> time.txt

source PLAY

make clean

CMD="sleep 2"
{ time \
  $INFER_PATH -j 20 --headers --no-filtering -- $MAKE_CMD \
    ; } 2> time.txt

chmod -R 755 *

rm /tmp/*footpatch*
