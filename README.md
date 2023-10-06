Engineering materials
====
## Introduction

This repository provides information about the building and programming process of our self-driving car for the Future Engineers 2023 competition. Since this is our third year participating in this competition we have updated our robot even more to be more controllable, compact and precise, remaking it almost entirely.

## Content

* `team-photos` contains 2 photos of the team (an official one and one funny photo).
* `vehicle-photos` contains 6 photos of the vehicle (from every side, top and bottom).
* `video` contains the video.md file with the link to a video with a driving demonstration.
* `schematics` contains schematic diagrams of the electromechanical components, illustrating all the elements (electronic components and motors) used in the vehicle and how they connect to each other.
* `src` contains code of control software for all components which were programmed to participate in the competition.
* `models` contains all the 3D-printed parts of the vehicle. 
* `other` contains the PCBs of the vehicle and the datasheets of all the components.
* `YouTube Channel` All public videos are uploaded at https://www.youtube.com/channel/UCMpIuC9MiNL4U_FDTKrd1CQ

## Structure

This year we are reusing the structure from last year, a RC toy car (the WLtoys k989). We removed the rear differential case to make it front drive, and we put two optical encoders connected to the rear wheels in the new free space.
But the most important change is that we removed the aluminium base and replaced it with our new PCB. This PCB is made by us: we insolated a photo-sensible PCB, put green solder mask on it and soldered all the connections and vias. The components are almost all soldered in SMD (surface mounted) by hand, we even had to use a digital microscope.

## Electronics

The robot consists of three ATMEGA32U4 microcontrollers, so it can be seen as three modules. One microcontroller as the Master, another as the I2C Slave and the last as the Sensors Slave. We chose to use three microcontrollers because we feared that they would be saturated and we could not upload the main code, and in the end we were right, the programs for each use around the 70 - 80% of the maximum program memory.

### Sensors slave

This microcontroller reads the data from three HC-SR04 ultrasonic sensors (in the front, right side and left side) and of three encoders (two on the rear wheels and one directly connected to the motor). It also controls the motor via one IFX9201SG H-bridge motor controller.
The microcontroller connects to the master via SPI communication to send the data from the sensors and receive commands to drive the motor.

![Prueba](https://www.adslzone.net/app/uploads-adslzone.net/2019/04/borrar-fondo-imagen-1200x675.jpg)
### I2C slave
 
This microcontroller connects to all the sensors who use I2C communication: one MPU6050 gyroscope sensor, two VL53L1X TOF sensors (left and right) and the HuskyLens camera. It also controls 10 WS2810B LEDs that act as the vehicle’s headlights, turn signals and brake lights, and a servo that rotates a small mirror that allows the camera to see from left to right.
This microcontroller also sends its sensor data via SPI communication to the master and receives the commands to turn the LEDs on or off.
 
### Master

This microcontroller is in charge of the main program which uses all the information received from the slaves to control the entire robot (mainly the direction servo and the motor) to complete the challenge.

## Programming modules

Having in mind that there are three programs (for each microcontroller), we can also group parts of all the code in groups regarding its purpose.

### Movement

The movement of the vehicle is done by a series of instructions: The Master 32U4 sends the Sensors 32U4 the command to start the motor at a specific speed, with or without a PID controller that adjusts the speed of the motor based on its encoder so its exactly the one required. The master also changes the speed of the motor doing a exponential acceleration or deceleration when needed. Then reads the data of the MPU6050 from the I2C 32U4 to adjust the direction servo with another PID controller to drive straight. Or it moves the servo to turn the car if needed.

### Detecting walls

We mainly use the ultrasonic and TOF sensors to detect distance to walls, to know if we passed a corner and to know the turn direction.
When we pass a corner one of the lateral sensors detects a higher distance, that means there is no wall in that side, that way we know we have to turn to that side.
They can also be used to detect a signal without knowing the color.
The problems with these sensors are that the ultrasonic are very slow for our possible speeds and because the walls are black, little light is reflected to the TOF sensors (the walls we used in Ibiza reflected almost nothing) and the readings may be erratic or imprecise, but we do not know of other types of distance sensors to read walls.

### Detecting the color signals

We use a HuskyLens AI camera with the capability to recognize faces and objects, but most importantly for us, it can recognize colors. Its mounted on a 3D-printed case with an integrated servo that turns a little mirror that reflects the image received by the camera, this way it can view from left to right, around 180º. This allows us to mount the camera facing downwards and make it more compact. The only problem is that this method transposes the image and it needs to be corrected in code.
When the robot detects that it passed a corner, it turns the mirror to view what is in the next sector and the camera reads the color and position of the signals. We simplified the number of possible positions of the signals to 11, ignoring if they are in the left or right lane (only taking into account if they are in the rear, middle or front), because our vehicle is small enough that it can pass between any position of the signals.
The I2C 32U4, the one with the camera, then sends the number we assigned of the specific position in which the signals are to the Master, which saves this position for the other two laps (this way we can go faster because we do not need to read the positions again) and turns the car to position itself on the correct side.

## Debugging with graphs and telemetry

For debugging purposes we included a HC-05 bluetooth module in the vehicle, only used during testing and debuggin, during the competition it will be disconnected. For ease of analyzing the information sent by the robot we use a graphing application made with Python by our friend Maria Pilligua uploaded to her [GitHub repository](https://github.com/mpilligua/app_wro). With this we can see values like the speed or the distance counted from the encoders and view it in the style of Formula 1 telemetry which is more practical for comparing it to earlier tests.

## Code building, compiling and uploading process

We programmed all using the Arduino IDE like any Arduino board, but because we use the 32U4 microcontrollers directly we have to flash the bootloader to each 32U4. We flashed the Arduino Leonardo bootloader because it uses this microcontroller and we are very familiar with it. The process for flashing it requires you to connect the 32U4 to another Arduino using the ICSP communication, then the normal Arduino acts as an ISP programmer (using the example code ArduinoISP) and flashes your 32U4 with the selected bootloader.
Because this was our first time doing this we used this [SparkFun guide](https://learn.sparkfun.com/tutorials/installing-an-arduino-bootloader/all) and this [Arduino guide](https://docs.arduino.cc/built-in-examples/arduino-isp/ArduinoISP).
