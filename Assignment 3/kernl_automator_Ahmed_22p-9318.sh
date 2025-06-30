#!/bin/bash
set -e #exit on any error
set -o pipefail #exit if pipe fails
#variable
KERNEL_URL="https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.11.8.tar.xz"
KERNEL_ARCHIVE="linux-6.11.8.tar.xz"
KERNEL_DIR="linux-6.11.8"
SHA256SUM_URL="https://cdn.kernel.org/pub/linux/kernel/v6.x/sha256sums.asc"
#I want to do configuration in the kernel and then compile it but it was asking certificate of signing keys
#CONFIG_OPTIONS=(
  #"CONFIG_BT=n"#disabling Bluetooth drivers
  #"CONFIG_ETHERNET=n"#disabling ethernet drivers
  #"CONFIG_INPUT_TOUCHSCREEN=n"#disabling touchscreen drivers
  #"CONFIG_INPUT_TABLET=n"#disabling pen drivers
  #"CONFIG_USB_PRINTER=n"#disabling USB printer support
  #"CONFIG_SOUND=n"#disabling sound support
  #"CONFIG_VIDEO_V4L2=n"#disabling video support
  #"CONFIG_WLAN=n"#disabling wireless LAN drivers
#)
#function to record/log messages
log() 
{
  echo "[cout] $1"
}
#installing dependencies
log "Installing dependencies"
sudo apt update && sudo apt install -y \
  build-essential \
  libncurses-dev \
  bison \
  flex \
  libssl-dev \
  wget \
  bc \
  xz-utils \
  gnupg \
  libelf-dev
  
#downloading kernel
log "downloading kernel"
wget -O $KERNEL_ARCHIVE $KERNEL_URL
wget -O sha256sums.asc $SHA256SUM_URL
#verify integrity of downloaded kernel
log "verifying integrity"
grep $(basename $KERNEL_ARCHIVE) sha256sums.asc | sha256sum -c -
if [ $? -ne 0 ]; 
then
  log "process of itegrity check failed! Exiting"
  exit 1
fi
#extracting kernel
log "extracting kernel"
tar -xf $KERNEL_ARCHIVE
#moving to kerel directory
cd $KERNEL_DIR
#as I have mentioned above that I am trying to compile the customized configuration but it was asking for certificate of signing keys so I do the default configuration
log "using default kernel configuration"
make defconfig  #it automatically create default configuration
#compiling kernel
log "compiling kernel(may take a while sir pplease wait)"
make -j$(nproc) #this command will use all available cores of your cpu
#compile kernel module
log "compiling modules of kernel"
make modules
#installing kernel and modules
log "installing kernel and modules"
sudo make modules_install
sudo make install
#updating grub
log "Updating GRUB bootloade"
sudo update-grub
#removing temporary files and directories created while downloading,compling and installing kernel
log "removing temporary files"
cd ..
rm -rf $KERNEL_ARCHIVE $KERNEL_DIR sha256sums.asc
#rebooting/restarting and notify
log "kernel installation completed successfully!"
read -p "do U want to reboot to apply the new kernel? (y/n): " REBOOT
if [[ "$REBOOT" == "y" || "$REBOOT" == "Y" ]]; 
then
  log "rebooting now..."
  sudo reboot
else
  log "when U will reboot the pc will automatically run on new kernel"
fi
