JdeRobot Official Image
========

This is the official Docker image of [JdeRobot project](http://jderobot.org), a software framework for developing applications in robotics, computer vision.

It contains all the necessary components to be used with [Teaching Robotics](http://jderobot.org/Teaching_robotics_with_JdeRobot):
* JdeRobot package installed
* Gzweb prepare to use
* Videos for be used with camserver component
* Nano TextEditor installed ...

# Usage
* Download and run: 
```sh
docker run -tiP --rm -p 7681:7681 jderobot/jderobot
```
* Camserver with video: 
```sh
docker run -tiP --rm -p 9000:9000 jderobot/jderobot video [video_name]
```
* Show all posibles videos: 
```sh
docker run -tiP --rm -p 9000:9000 jderobot/jderobot lsvideos
```
* Run a world with gazebo: 
```sh
docker run -tiP --rm -p 7681:7681 jderobot/jderobot
rungzserver [world_name]
# After seeing the traces of the plugin
rungzweb
```


# Notes
 See all mapped ports:
* linux:
```sh
docker ps | tail -n 1 | perl -lae '$,="\n";foreach(@F){/tcp,?$/&&push(@x,$_)};print(@x)'
```
* Windows: you need to use Kitematic


[![Introrob_py + GzWeb in Windows](https://img.youtube.com/vi/YoJYahFtEyg/0.jpg)](https://www.youtube.com/watch?v=YoJYahFtEyg)
