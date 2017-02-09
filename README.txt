/*
 *     Copyright (c) 2017
 *     Author: James Wang.
 *     All rights reserved.
 *
 */

This is an automation tool to perform the following routine tasks(MYEX_PATH is set as the root of this tool dir):

- Build DPDK SDK from the open source code from Intel by:
  $MYEX_PATH/myex.pl --config $MYEX_PATH/cfg.ovs/dpdk_build.cfg (running under the DPDK SDK source dir)

- Build and setup Open vSwitch from source code with DPDK support by:
  $MYEX_PATH/myex.pl --config $MYEX_PATH/cfg.ovs/ovs_setup.cfg (running under the Open vSwitch source dir)

- Build and setup OpenStack (Ocata) on the multiple servers, controller, compute1, block1:
  . $MYEX_PATH/cfg.os/os_setup.sh (running under anywhere)

And details can be found in the specific cfg files.


