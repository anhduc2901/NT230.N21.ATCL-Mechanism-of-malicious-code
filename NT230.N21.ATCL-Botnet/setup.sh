#!/bin/sh

cd /tmp

# pip for python 2
wget https://bootstrap.pypa.io/pip/2.7/get-pip.py
python2 get-pip.py

# install dependencies
apt update
apt install -y libevent-dev python-all-dev

# needed python packages
pip2 install irckit greenlet gevent
