# Simulation-and-control-of-BLDC-motor-with-methods-on-data-transfer-and-visualization.

Each folder contains its readMe file specifying how to build the project. It is worth noting that this project was done in Linux and has not been tested in windows, feel free to make changes as you see fit. Some part of this project is still under development, especially, the peer to peer connection which requires a signaling server be implemented. 
### **Explanation for each implementation**
1.**Javascript-C-Version**- This is a design of a real-time motor simulation web application written in C for the heavy computational task but compiled on the web using Javascript for the user interface. To run the code on the web,  WebAssmebly was used.

2.**Multithreaded-software** - A multi-threaded asynchronous motor simulation application written in C using the motor simulation algorithms. 

3.**Data-transfer-implementation**- An implementation of a data format that is language independent and a transport layer that would help broadcast data to a web application for real-time visualization.




## Future work 
Future work will go into using implementating the multithreaded application for a real robotic platform. The idea is too create a robotic software application for controlling and broadcasting data from the **PR2** robot updating the ethernet drivers to the latest EtherCat drivers. This would require a redesign of the software to take into account things like the communication with the motor EtherCAT drivers replacing the motor simulation algorithm or the communication between the newly added threads using techniques like buffers. Using the web visualization software **perspective** in the javascript-C -version, the data format **Apache Arrow** and the peer to peer data sending method **Webrtc**, the final robotic application should include a broadcasting thread that allows the robot data to be visualized on the web.

## Important
The deployment architecture library used in the Multithreaded software is still under development by the robotic research group in Ku Leuven. A complete open-source version of this software with documentation and explanation will be published soon. 
Feel free to report a bug you find in the software. 
