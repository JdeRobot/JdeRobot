PiBot is an educational robot designed by JdeRobot team. It is composed by a RaspberryPI board, a PiCam camera, several common (and cheap) sensors and actuators, a battery and a frame of 3D printable pieces.

It is open hardware.

Its drivers are provided as open source, so students may program PiBot applications in Python. In addition, they can program a simulated PiBot in Gazebo too. The robot model and plugin are also provided with the same programming interface as the real PiBot.

![real Pibot][PiBot-real]
![simulated Pibot][PiBot-sim]

[PiBot-sim]: http://jderobot.github.io/JdeRobot/pibot-2.png "Simulated PiBot"
[PiBot-real]: http://jderobot.github.io/JdeRobot/pibot-1.jpg "Real PiBot"


# Programming interface

The Hardware Abstraction Layer (HAL) is composed of four sections: raw sensors, raw actuators, cooked sensors and cooked actuators.

| Function | Meaning|
| ------ |------|
| | Robot movement|
| | Left Motor movement |
| | Right Motor movement |
| | Servo motor movement |
| | Get IR measurement |
| | Get US measurement |
| | Get camera image |
| | Get colored object in image |
| | Get front distance to obstacles from image |



# Shopping list

* Raspberry Pi 3
* Micro SD de 16GB
* Battery (3A output, 20000mAh)
* 3 motors: like [Parallax Servo Feedback 360º](https://www.parallax.com/product/900-00360)
* PiCam camera
* IR sensors
* US sensor HC-SR04 model


# PiBot in action

<a href="http://www.youtube.com/watch?feature=player_embedded&v=WUeVjef1p6U" target="_blank"><img src="http://img.youtube.com/vi/WUeVjef1p6U/0.jpg" 
alt="Real PiBot moving" width="240" height="180" border="10" /></a>