#!/bin/bash

sudo apt remove nova-api nova-conductor nova-consoleauth nova-novncproxy nova-scheduler nova-placement-api -y

sudo apt remove nova-compute -y
sudo apt remove glance -y
sudo apt remove keystone -y
sudo apt remove  python-openstackclient -y

sudo apt autoclean -y
sudo apt autoremove -y


sudo rm -rf /opt/stack
sudo rm -rf /usr/lib/python2.7/dist-packages/oslo*
sudo rm -rf /usr/lib/python2.7/dist-packages/nova*
sudo rm -rf /usr/lib/python2.7/dist-packages/glance*
sudo rm -rf /usr/lib/python2.7/dist-packages/keystone*
sudo rm -rf /usr/lib/python2.7/dist-packages/openstack*

sudo rm -rf /etc/sudoers.d/glance*
sudo rm -rf /etc/sudoers.d/nova*

sudo rm -rf /usr/bin/keystone*
sudo rm -rf /usr/bin/glance*
sudo rm -rf /usr/bin/nova*

sudo rm -rf /etc/apt/sources.list.d/cloudarchive-newton.list*
