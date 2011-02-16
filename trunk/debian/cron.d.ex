#
# Regular cron jobs for the jderobot package
#
0 4	* * *	root	[ -x /usr/bin/jderobot_maintenance ] && /usr/bin/jderobot_maintenance
