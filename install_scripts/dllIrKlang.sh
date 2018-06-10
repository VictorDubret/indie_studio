#!/bin/sh

wget http://www.ambiera.at/downloads/irrKlang-64bit-1.6.0.zip
unzip irrKlang-64bit-1.6.0.zip

cd irrKlang-64bit-1.6.0/include

sudo cp * /usr/include
cd ..

cd bin/linux-gcc-64
sudo cp * /usr/bin
cd ../../..

rm -rf irrKlang-64bit-1.6.0
rm -f irrKlang-64bit-1.6.0.zip
