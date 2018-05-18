#USE ./MAVLinkServer mavlinkserver.yml
python2.7 ./mavproxy.py $1 --master=10.1.1.191:14550 --console
