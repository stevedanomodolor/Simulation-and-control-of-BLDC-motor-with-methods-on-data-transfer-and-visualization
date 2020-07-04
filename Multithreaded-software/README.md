**Important**
This software is based on a library that is being developed by the robotic research group in Ku Leuven. An official version will be published in the near future with explanation and documentation on how to use it.



Motor_simulation given certain input
=========================================

### Execution
The main is located in file main_program.c. You run the built executable
by simply calling it without any arguments:



### Dependencies
#### CGRAPH
This application uses the libraries cgraph and cdt as a graph library
for the creation of a petrinet. Those libraries are included in the
graphviz library.

#### DISRUPTOR

This package is required for the "lock-free data structures" mechanism used on the data channels.
It is included in the `external` directory. (Where also the EtherCat library, 'SOEM', can be found.)


### Build process
First delete the build folder if there is any
To build the project execute the following steps:
* Create a build folder: ``mkdir build``.
* Enter that folder: ``cd build``.
* Generate makefile from cmake: ``cmake ..``
* Compile: ``make``
* For this simulation run ./motor_simulation



The application consists of three activities divided over 2 separate threads.
The application_mediator activity and the commandline activity are deployed
in one thread and the motor_simulation_control activity is deployed in the second thread.

The simulation returns the current computed value of the motor given certain input:
The input are the following


****** USER_INTERFACE*********************************************************
******** Sliders**********
* Vbus[V]
____Value range: 0 - 36
- **q** - decrease value
- **w** - increase value

* Load Torque[Nm] - The unit that appear while modifying is Nm * 10 and has being taking into account in the simulation code.
____Value range: 0 - 10
- **a** - decrease value
- **s** - increase value

* Load Inertia [kg*m²] - Similarly to the torque output, the value shown is the inertia * 100(Also has being taking into consideration during the computation ).
_____Value range: 0 - 10
**z** decrease value
**x** increase value

* Speed ref[rpm] - Velocity setpoint
_____Value range: 0 - 10
**d** decrease value
**f** increase value

* Position_ref[Degree] - Position setpoint - only valid in position mode
_____Value range: 0 - 360
**c** increase_value
**v** decrease_value

********* buttons********
-----modes
This simulation currently operates in three mode, openloop, FOC_speed and FOC_pos.
* OpenLoop
**h** activate mode

* FOC_velocity
**j** activate mode

* FOC_postion
**k** activate mode


---- other buttons
* Pause
**p**

* restart - reset all parametres
**r**

* exit application
**e**

* toggle
**t**
