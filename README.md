![Muca Logo](https://raw.githubusercontent.com/wiki/muca-board/Muca/Images/muca_black.png)

# Muca: Multi Touch boards

This The  Arduino library for Muca.
For more information on the project and tutorial, visit the [wiki](https://github.com/muca-board/Muca/wiki/) or the [website](https://muca.cc)


## Setup

Download the library as a zip folder and install it as described here: 
https://www.arduino.cc/en/Guide/Libraries ("Importing a .zip Library")

### Working

Connect the board following the tutorial on the [wiki](https://github.com/muca-board/Muca/wiki/Getting-started-with-Muca). 

Run the example `Muca_Raw.ino`.  To display the results,  launch`Muca_Raw_Processing.pde`.

**Important**: On the processing file, don't forget to change `SERIAL_PORT` to your active board. 


### Known Issues

* The "TouchPoints" detection is not working yet, needs some calibration. 

