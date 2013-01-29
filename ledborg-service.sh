#!/bin/bash
### BEGIN INIT INFO
# Provides:          ledborg
# Required-Start:
# Required-Stop:
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: Load LedBorg driver
# Description:       Load LedBorg driver, providing /dev/ledborg
### END INIT INFO

DRIVERPATH=/home/pi/ledborg.ko
COLOURPATH=/home/pi/ledborg_bootcolour

case "$1" in
	start|"")
		echo 'Loading ledborg driver'
		insmod $DRIVERPATH
		cat $COLOURPATH > /dev/ledborg
		;;
	restart|reload|force-reload)
		echo 'Reloading ledborg driver'
		rmmod $DRIVERPATH
		insmod $DRIVERPATH
		cat $COLOURPATH > /dev/ledborg
		;;
	stop)
		echo 'Unloading ledborg driver'
		rmmod $DRIVERPATH
		;;
	*)
		echo "Usage ledborg.sh [start|restart|stop]" >&2
		exit 3
		;;
esac

