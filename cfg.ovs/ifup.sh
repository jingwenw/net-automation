#!/bin/sh

switch="br-nodpdk"
ifconfig $1 up

# The post should have been added in ovs_setup.
#ovs-vsctl add-port ${switch} $1
