#!/bin/bash

#
# Prepare the global environments used for TCNG build tools.
#    And this script needs to be source before any other ci tools/config,
#    in this directory, executing.
#
# NOTE: the followings can be pre-configured, otherwise default will be used:
#    - $WORKSPACE
#    - $TCNG_REPO_ASIC
#    - $TCNG_REPO_MIDDLEWARE
#    - $TCNG_REPO_TUNETSYS
#

if [ ! -n "$TNT_WORKSPACE_TOPDIR" ]
then
    if [ -n "$WORKSPACE" ]
    then
        export TNT_WORKSPACE_TOPDIR=$WORKSPACE
    else
        export TNT_WORKSPACE_TOPDIR=$HOME/workspace/TCNG_Software
    fi
fi

if [ ! -n "$TNT_TCNG_BUILD_TOOLS_TOPDIR" ]
then
    export TNT_TCNG_BUILD_TOOLS_TOPDIR=$HOME/workspace/TCNG_Build_Tools
fi
if [ ! -n "$TNT_TCNG_QA_TOPDIR" ]
then
    export TNT_TCNG_QA_TOPDIR=$HOME/workspace/TCNG_QA
fi

export TNT_EXTERNAL_REPO_MIDDLEWARE=$TNT_WORKSPACE_TOPDIR/external_repos/TCNG_Middleware
export TNT_EXTERNAL_REPO_TUNETSYSSHARED=$TNT_WORKSPACE_TOPDIR/external_repos/TUNetSysShared
export TNT_EXTERNAL_REPO_ASIC=$TNT_WORKSPACE_TOPDIR/external_repos/TCNG_ASIC

export PERL5LIB=$TNT_TCNG_QA_TOPDIR/framework/perlapi:$TNT_EXTERNAL_REPO_TUNETSYSSHARED/perllib

# For TCNG packaging for QA test
if [ ! -n "$TCNG_PACKAGE_ROOT" ]
then
    export TCNG_PACKAGE_ROOT=${TNT_WORKSPACE_TOPDIR}/build/ci/var/workspace/TCNG_Software
fi
if [ -n "$BUILD_NUMBER" ]
then
    export TCNG_PACKAGE_TARBALL="tcng_package_qa_"$TNT_PLATFORM_NAME"_"$BUILD_NUMBER".tar.gz"
else
    export TCNG_PACKAGE_TARBALL="tcng_package_qa_"$TNT_PLATFORM_NAME".tar.gz"
fi
export TCNG_PACKAGE_TARBALL=tcng_package_qa.tar.gz

if [ ! -n "$TNT_PLATFORM_NAME" ]
then
    export TNT_PLATFORM_NAME=tcng_at91cap9_e132mb
fi
export NFS_SERVER_MOUNT=/tmp/nfs

# TCNG Test suite
if [ ! -n "$TCNG_TEST_SUITE" ]
then
    export TCNG_TEST_SUITE=$TNT_TCNG_QA_TOPDIR/testcases/Common/Component/TCNGComponentSmokeTest/TCNGComponentSmokeTest.xml
fi

# TCNG Build Flags
if [ ! -n "$TNT_ORTHOGONAL_INSTALL_VECTOR" ]
then
    if [ "$TNT_PLATFORM_NAME" == "tcng_at91cap9_e132mb" ]
    then
        export TNT_ORTHOGONAL_INSTALL_VECTOR=0
    else
        export TNT_ORTHOGONAL_INSTALL_VECTOR=1
    fi
fi

# Needed for some ERP tcs.
export TNT_ENABLE_PERT_SHELL=1

if [ ! -n "$TNT_LAB_BUILD" ]
then
    export TNT_LAB_BUILD=1
fi

# TCNG Dev_Board Flashing
if [ ! -n "$FLASH_IMAGE_PATH" ]
then
    export FLASH_IMAGE_PATH=${TNT_WORKSPACE_TOPDIR}/binaries/${TNT_PLATFORM_NAME}-armv5l-timesys-linux-gnueabi/${TNT_PLATFORM_NAME}_32MB_image_wh.bin
fi

if [ ! -n "$FLASH_START_ADDR" ]
then
    export FLASH_START_ADDR=0x0
fi
export FLASH_SAM_BA_DEV=/dev/ttyACM0
export FLASH_SAM_BA_BOARD=tcng_e1dev

