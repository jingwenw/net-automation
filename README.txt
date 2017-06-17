/*
 *     Copyright (c) 2017
 *     Author: James Wang.
 *     All rights reserved.
 *
 */

This is an automation tool to perform the following routine tasks(MYEX_PATH is set as the root of this tool dir):

Description:
    This tool is to execute a batch of cmds according to the given config file:
        - to run the cmds in a .cfg file - format in the sample cfg files
        - to run the testcases in a XML files - format in the sample xml files

    Usage:

        myex.pl [--config <config_file> | --help]

        --config : To specify the configurations how to build/install actions.
                   The default is myex.pl.cfg under the same dir.
            NOTE :
                   If the configure file is a xml, it would be parsed as
                   testing suite file, which should include <TestCase> tag
                   under <ProductRegression>

        --help   : print this help
        --debug  : Pause before each command
        --details: Log details for each command to screen (;pwd appended)

Examples:

- Build DPDK SDK from the open source code from Intel by:
  $MYEX_PATH/myex.pl --config $MYEX_PATH/cfg.ovs/dpdk_build.cfg (running under the DPDK SDK source dir)

- Build and setup Open vSwitch from source code with DPDK support by:
  $MYEX_PATH/myex.pl --config $MYEX_PATH/cfg.ovs/ovs_setup.cfg (running under the Open vSwitch source dir)

- Build and setup OpenStack (Ocata) on the multiple servers, controller, compute1, block1:
  . $MYEX_PATH/cfg.os/os_setup.sh (running under anywhere)

And details can be found in the specific cfg files.


