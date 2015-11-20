#!/bin/bash
set -e

# non interactive
echo 'debconf debconf/frontend select Noninteractive' | debconf-set-selections

# install system deps
echo " ---> Updating ubuntu"
apt-get -y -qq --force-yes update
apt-get -y -qq --force-yes install apt-utils aptitude
apt-get -y -qq --force-yes upgrade
apt-get -y -qq --force-yes install curl libcurl4-nss-dev libssl-dev
apt-get -y -qq --force-yes install build-essential g++ scons
apt-get -y -qq --force-yes install libboost-dev libboost-regex-dev libboost-thread-dev
apt-get -y -qq --force-yes install git
apt-get -y -qq --force-yes install vim
apt-get -y -qq --force-yes install iputils-ping net-tools
