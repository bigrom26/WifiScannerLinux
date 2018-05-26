#!/usr/bin/env bash
xhost +local:`whoami`
cat /proc/net/wireless | perl -ne '/(\w+):/ && print "$1\n"' > MeerkatWifiScanner.conf
