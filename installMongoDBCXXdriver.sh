#!/bin/bash
git clone https://github.com/mongodb/mongo-cxx-driver.git
export MYDIR=`pwd`
cd mongo-cxx-driver
git checkout legacy
scons --prefix=$MYDIR --64 install
