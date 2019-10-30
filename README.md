# wssa-lab3

This repo is created to support the third lab for 18-847: Wireless Software Systems Architecture. The subject of this particular lab is serial communications on embedded devices, namely, I2C and SPI. 

The lab has three part, corresponds to part1, part2, and part3-X folders. Part1 is an I2C driver for the magnetometer on the SparkFun LSM303C IMU breakout board. Part2 is a SPI driver for the FXOS8700CQ magnetometer, which is also part of a SparkFun breakout board. Each of these two parts references an internal register to ensure the device is being communicated with properly, and then continuously polls the X, Y, and Z values for the magnetometer. Parts 3-1 and 3-2 are subsets of 1 and 2, but are focused characterizing energy consumption as a result of I2C vs. SPI. 

The remaining files of this repo are supporting information that was necessary to finish this lab and write the report.
