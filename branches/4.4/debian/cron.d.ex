#
# Regular cron jobs for the jde package
#
0 4	* * *	root	[ -x /usr/bin/jde_maintenance ] && /usr/bin/jde_maintenance
