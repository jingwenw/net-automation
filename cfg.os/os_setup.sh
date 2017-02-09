#!/bin/bash

echo "OpenStack Enviroment setup for controller, compute1 and block1 nodes"
myex.pl --config /home/james/myRepos/packetier-tools/cfg.os/os_net.cfg
if [ $? -eq 0 ]; then echo Ok; else echo FAIL; exit; fi

echo "OpenStack Identity Service Setup for controoler node"
myex.pl --config /home/james/myRepos/packetier-tools/cfg.os/os_keystone.cfg
if [ $? -eq 0 ]; then echo Ok; else echo FAIL; exit; fi

echo "OpenStack Image Service for controller node"
myex.pl --config /home/james/myRepos/packetier-tools/cfg.os/os_glance.cfg
if [ $? -eq 0 ]; then echo Ok; else echo FAIL; exit; fi

echo "OpenStack Compute Service for controller and compute1 nodes"
myex.pl --config /home/james/myRepos/packetier-tools/cfg.os/os_nova.cfg
if [ $? -eq 0 ]; then echo Ok; else echo FAIL; exit; fi
myex.pl --config /home/james/myRepos/packetier-tools/cfg.os/os_nova_compute1.cfg
if [ $? -eq 0 ]; then echo Ok; else echo FAIL; exit; fi

echo "OpenStack Networking service for controller and compute1 nodes"
myex.pl --config /home/james/myRepos/packetier-tools/cfg.os/os_neutron.cfg
if [ $? -eq 0 ]; then echo Ok; else echo FAIL; exit; fi
myex.pl --config /home/james/myRepos/packetier-tools/cfg.os/os_neutron_compute1.cfg
if [ $? -eq 0 ]; then echo Ok; else echo FAIL; exit; fi

echo "OpenStack Dashboard Service  for controller node"
myex.pl --config /home/james/myRepos/packetier-tools/cfg.os/os_horizon.cfg
if [ $? -eq 0 ]; then echo Ok; else echo FAIL; exit; fi

echo "OpenStack Block Storage Service for controller and block1 nodes"
myex.pl --config /home/james/myRepos/packetier-tools/cfg.os/os_cinder.cfg
if [ $? -eq 0 ]; then echo Ok; else echo FAIL; exit; fi
myex.pl --config /home/james/myRepos/packetier-tools/cfg.os/os_cinder_block1.cfg
if [ $? -eq 0 ]; then echo Ok; else echo FAIL; exit; fi

echo "OpenStack Share File System for controller and block1 nodes"
myex.pl --config /home/james/myRepos/packetier-tools/cfg.os/os_manila.cfg
if [ $? -eq 0 ]; then echo Ok; else echo FAIL; exit; fi
myex.pl --config /home/james/myRepos/packetier-tools/cfg.os/os_manila_block1.cfg
if [ $? -eq 0 ]; then echo Ok; else echo FAIL; exit; fi



echo "Now to launch network and router instance"
myex.pl --config /home/james/myRepos/packetier-tools/cfg.os/os_launch_net.cfg
if [ $? -eq 0 ]; then echo ======= All Done Successfully ======; else echo FAIL; exit; fi

echo " NOTE: remember to get the net_ids and floating ips from the previous step and put into the following cfg"
#myex.pl --config /home/james/myRepos/packetier-tools/cfg.os/os_launch_vm.cfg
# if [ $? -eq 0 ]; then echo Ok; else echo FAIL; exit; fi




