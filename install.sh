#!/bin/bash

USER=pi
USERDIR=/home/$USER
BOOTCOLOUR="000"

echo "Installing LedBorg, please wait..."

# Unload in case of reinstalls
sudo rmmod ledborg

# Copy files
cp ledborg.desktop $USERDIR/Desktop/
cp ledborg.ko $USERDIR/
cp ledborg_gui $USERDIR/
cp ledborg_gui.ico $USERDIR/
chown $USER $USERDIR/Desktop/ledborg.desktop $USERDIR/ledborg.ko $USERDIR/ledborg_gui $USERDIR/ledborg_gui.ico
chmod a+x $USERDIR/ledborg_gui

# Setup driver config
sudo bash -c "echo 'KERNEL==\"ledborg\", NAME=\"ledborg\", MODE=\"0666\"' > /etc/udev/rules.d/20-ledborg.rules"

# Setup driver autoload
sudo cp ledborg-service.sh /etc/init.d/ledborg.sh
sudo chmod +x /etc/init.d/ledborg.sh
sudo update-rc.d ledborg.sh defaults 100
echo $BOOTCOLOUR > $USERDIR/ledborg_bootcolour
chown $USER $USERDIR/ledborg_bootcolour

# Test load
sleep 1
sudo insmod $USERDIR/ledborg.ko
echo "010" > /dev/ledborg
echo "LedBorg installed, LedBorg should now be green"

