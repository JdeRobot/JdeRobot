If you want to compile for the Nao robot you have to do the same things that
for any other component for this robot. Please, follow the guidelines at
Aldebaran Robotics website:

https://community.aldebaran-robotics.com/doc/1-12/dev/cpp/install_guide.html
https://community.aldebaran-robotics.com/doc/1-12/dev/cpp/tutos/using_qibuild.html

The project is named followball, so you have to compile like:

qibuild configure -c <your_toolchain> followball
qibuild make -c <your_toolchain> followball
