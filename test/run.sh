#! /bin/sh
#
# run.sh
# Copyright (C) 2017 khoi <khoi@hkpc>
#
# Distributed under terms of the MIT license.
#

# Set echo on
set -x

../bin/main ~/Videos/out.avi config$1.txt result$1.txt

