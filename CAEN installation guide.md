## Dependencies

Install these dependencies before installing the CAEN libraries:

1. sudo dnf install kernel-devel kernel-headers kernel-modules dkms
2. sudo dnf install java-1.8.0-openjdk

Then add the Java JDK directory to your path by opening and editing your user profile file
```
cd ~
vim .bash_profile
```

Then add the following lines to the file

```
# Make sure that the directory below exists, it might vary slightly depending on your Fedora
export JAVA_HOME=/usr/lib/jvm/java-1.8.0-openjdk-1.8.0.252.b09-0.fc31.x86_64
export PATH=$JAVA_HOME/bin:$PATH
```

Close and save. Then do `source .bash_profile`. For the changes to be consistent across all of your user space, log out and back in again.

## CAEN libraries and applications versions

- CAENVMElib-2.50
- CAENComm-1.2
- CAENDPPLib-1.7.2
- CAENDigitizer-2.15.0
- CAENUSBdrvB-1.5.3
- A3818Drv-1.6.3
- CoMPASS-1.4.2
- CAENUpgrader-1.6.6
- Firmware DPP-PHA_rev4.22

## Installation order

Navigate to the Linux folder inside the CAEN dependencies folder, then go into each of the sub-folders and follow the installation instructions for each of the components. Install the components in this order:

1. CAEN VME library
2. CAEN Comm library
3. CAEN DPP library
4. CAEN Digitizer library
5. CAEN USB driver
6. CAEN A3818 driver
7. CoMPASS application
8. CAENUpgrader application


## Some notes on the A3818 driver compatibility

The program does not immediately compile on kernel versions after 5.4 because the kernel PCI aplication interface, and the kernel file operations application interface both received changes with kernel > 5.4

The two changes relevant to us are:
- the system file header linux/pci-aspm.h was absorbed into linux/pci.h (https://bugs.gentoo.org/701200)
- the type 'struct file_operations' defined in linux/fs.h was refactored into 'struct proc_ops', and its data members were also refactored (https://lore.kernel.org/netdev/20191225172546.GB13378@avx2/)

Solution:

1. Include linux/pci.h instead of linux/pci-aspm.h as follows:

`include <linux/pci-aspm.h>` becomes -> `#include <linux/pci.h>`

2. In the A3818 kernel module source, refactor:
```
static struct file_operations a3818_procdir_fops = {
  read: a3818_procinfo
};
```
into:
```
static struct proc_ops a3818_procdir_fops = {
  proc_read: a3818_procinfo
};
```

### Note

The A3818 driver in this folder has already the previous fixed applied. Only compile and install
## USB driver post-installation steps

You have to restart the networking service if it is enabled on the computer. To check if it is enabled do:

```bash
systemctl status networking
```

If the response is 

```bash
Unit networking.service could not be found
```

Then the networking service is not loaded on the kernel space and you can proceed. Otherwise, follow the following steps:

```bash
# Bring down the network 
service networking stop
# Unload the driver module from the kernel
# Find the name of the module... 
lspci -v 
# and look for "Kernel driver in use:"
modprobe -r <driver module>
#Reload the udev rules 
udevadm control --reload-rules
#Trigger the new rules 
udevadm trigger
#Load driver modprobe <driver module>
#Restart the network 
service networking start
#(optional) Re-run any iptables scripts that referenced the interface name before it was up.
```

## Other notes

The CAEN Digitizer library and the CAEN DPP library are incompatible with each other, use on or the other. We use the CAEN DPP library.


## Testing

Both the application and its dependencies have been tested on Fedora 31, kernel 5.6.19



