#!/bin/sh

java -cp h2*.jar org.h2.tools.Server -baseDir . -tcp -tcpAllowOthers
