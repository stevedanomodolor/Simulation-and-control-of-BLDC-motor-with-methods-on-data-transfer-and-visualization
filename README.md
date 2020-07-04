# Simulation-and-control-of-BLDC-motor-with-methods-on-data-transfer-and-visualization.

Each folder contains its own readMe file specifying how to build the project. It is worth noting that this project was done in Linux and has not been tested in windows, feel free to make changes as you see fit. 
Some part of this project is still under development, especially, the peer to peer connection which requires a signaling server be implemented. 


## Future work 
Future work will go into using implementing the multithreaded application for a real robotic platform. This would require a redesign of the software to take into account things like the communication with the motor EtherCAT drivers or the communication between the newly added threads. Using the web visualization software **perspective** in the javascript-C -version, the data format **Apache Arrow** and the peer to peer data sending method **Webrtc**, the final robotic application should include a broadcasting thread that allows the robot data to be visualized on the web.

## Important
The deployment architecture library is still under development by the robotic research group in Ku Leuven. A complete open-source version of this software with documentation and explanation will be published soon. 
Feel free to report a bug you find in the software. 
