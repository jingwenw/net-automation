#!/bin/bash

#
# Prepare the global environments used for TCNG build tools.
#
# NOTE: the followings can be pre-configured, otherwise default will be used:
#    - $WORKSPACE
#    - $TCNG_REPO_ASIC
#    - $TCNG_REPO_MIDDLEWARE
#    - $TCNG_REPO_TUNETSYS
#    - source $TCNG_CFG_DIR/cfg.tcng/source_me_first_tcng_env.sh
#

if [ ! -n "$TCNG_CFG_DIR" ]
then
    export TCNG_CFG_DIR=${HOME}/workspace/TCNG_QA/ci
fi

echo "Going to setup the environment for TCNG Framework"
source $TCNG_CFG_DIR/cfg.tcng/source_me_first_tcng_env.sh

if [ ! -n "$TCNG_REPO_ASIC" ]
then
    TCNG_REPO_ASIC=ssh://hguser@hg//var/hg/TCNG_ASIC
fi

if [ ! -n "$TCNG_REPO_MIDDLEWARE" ]
then
    TCNG_REPO_MIDDLEWARE=ssh://hguser@hg//var/hg/TCNG_Middleware
fi

if [ ! -n "$TCNG_REPO_TUNETSYS" ]
then
    TCNG_REPO_TUNETSYS=ssh://hguser@hg//var/hg/TUNetSysShared
fi

echo "  Now to checkout the external repos..."
cd $TNT_WORKSPACE_TOPDIR/external_repos

echo "    checking $TCNG_REPO_ASIC ..."
if [ ! -e TCNG_ASIC ]
then
    hg clone $TCNG_REPO_ASIC
else
    cd TCNG_ASIC
    hg pull --update
    cd ..
fi

echo "    checking $TCNG_REPO_MIDDLEWARE ..."
if [ ! -e TCNG_Middleware ]
then
    hg clone $TCNG_REPO_MIDDLEWARE
else
    cd TCNG_Middleware
    hg pull --update
    cd ..
fi

echo "    checking $TCNG_REPO_TUNETSYS ..."
if [ ! -e TUNetSysShared ]
then
    hg clone $TCNG_REPO_TUNETSYS
else
    cd TUNetSysShared
    hg pull --update
    cd ..
fi

cd $TNT_WORKSPACE_TOPDIR
pwd

echo "  To build the image and qa framework ..."
$TCNG_CFG_DIR/myex.pl --config $TCNG_CFG_DIR/cfg.tcng/tcng_build_all.cfg

if [ $? != 0 ]
then
    exit $?
fi

echo "  To build QA Framework and create the tarball for later QA testing ..."
$TCNG_CFG_DIR/myex.pl --config $TCNG_CFG_DIR/cfg.tcng/tcng_create_qa_package.cfg

echo "  To run the unit test ..."
$TCNG_CFG_DIR/myex.pl --config $TCNG_CFG_DIR/cfg.tcng/testsuite_unit_full.xml

