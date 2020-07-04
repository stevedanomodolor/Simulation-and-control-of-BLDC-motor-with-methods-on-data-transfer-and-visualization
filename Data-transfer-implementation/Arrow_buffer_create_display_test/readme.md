This example shows an implementation of how to create an arrow byte data and display it within the same program


Dependencies:
ARROW_CGLIB library
installation procedures can be found at this location - https://arrow.apache.org/install/
Important - If an error shows that the arrow library is not found, go to the cmakelist and modify the library's location to the location it is installed in your system:
"include_directories(/usr/lib/x86_64-linux-gnu/glib-2.0/include /usr/include/glib-2.0)"



It is Advisable to delete the build folder first to prevent building error
Instruction

To run:
create a folder called build
- mkdir build

go into the folder and run the following

- cd build
- cmake ..
- make

