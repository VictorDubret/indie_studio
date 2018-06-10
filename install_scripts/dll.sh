#!/bin/sh
wget https://datapacket.dl.sourceforge.net/project/irrlicht/Irrlicht%20SDK/1.8/1.8.4/irrlicht-1.8.4.zip
unzip irrlicht-1.8.4.zip
cd irrlicht-1.8.4/source/Irrlicht
make
cd ../../include
sudo cp * /usr/include
cd ../lib/Linux
sudo cp libIrrlicht.a /usr/lib
cd ../../..
rm -rf irrlicht-1.8.4
rm -f irrlicht-1.8.4.zip
