#/bin/sh

# Launch the VMs
virsh start v1
virsh start v2
virsh start v3

# Waiting for the VMs fully stablized
sleep 120

# Reset the USB devices

do echo "${i##*/}" | sudo tee "${i%/*}/unbind"; sleep 5; echo "${i##*/}" | sudo tee "${i%/*}/bind"; done

