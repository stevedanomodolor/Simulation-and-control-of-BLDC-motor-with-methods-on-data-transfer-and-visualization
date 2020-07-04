#Motor simulation javascript + c

Dependencies
**EMSCRIPTEN**
- Before running the code, emscripten software must be installed.  The installation process can be found in the following link
https://emscripten.org/docs/getting_started/downloads.html
https://gist.github.com/WesThorburn/00c47b267a0e8c8431e06b14997778e4
Issue while installing emscripten( I have not found the solution yet)
Every time you exit the console while using emscripten, you should
run this command in the emsdk terminal the same location you ran it during installation
**source ./emsdk_env.sh**

**Issues while installing emscripten**
*If you get this error while installing**

```
Installing SDK 'sdk-releases-upstream-ae5001fac3849895a873e422a2a80afc90f3b798-64bit'..
Installing tool 'node-12.9.1-64bit'..
Error: Downloading URL 'https://storage.googleapis.com/webassembly/emscripten-releases-builds/deps/node-v12.9.1-linux-x64.tar.xz': <urlopen error [SSL: CERTIFICATE_VERIFY_FAILED] certificate verify failed: unable to get local issuer certificate (_ssl.c:1076)>
Warning: Possibly SSL/TLS issue. Update or install Python SSL root certificates (2048-bit or greater) supplied in Python folder or https://pypi.org/project/certifi/ and try again.
Installation failed!
```

*solution*

* Go to the emsdk.py file and add this command,
`<
import ssl
ssl._create_default_https_context = ssl._create_unverified_context
>`

*Issue running  cmake - "finding compiler path"*
* This can be solved following the instruction in this web
[stackoverflow](https://stackoverflow.com/questions/56230175/how-to-find-compiler-path-automatically-in-cmake)

*fatal error: emscripten.h: No such file or directory*
* The solution can be found here:
[github](https://gist.github.com/WesThorburn/00c47b267a0e8c8431e06b14997778e4)




**How to build the project**


1. In the root directory, set the CMAKE Toolchain file by running:
CMAKE_TOOLCHAIN_FILE=/home/user/emsdk/upstream/emscripten/CMake/Modules/Platform/Emscripten.cmake

2. if there is already a build folder, you should delete it and run the following  commands from the root directory
      1. $ mkdir build
      2. $ cd build
      3. $ emcmake cmake .. -DJS_ONLY=ON
      4. $ make
This step would generate an "a.js" file in the javascript folder( it replaces the file if it already there):


3. Go to the JavaScript folder. To prevent any issue, you should delete the yarn.lock and  node_modules. Then run the following commands
      1. $ yarn install (regenerates the deleted files)
      2. $ npm start

```The last command should start the server in port 8080.
Open any browser and go to this address.
**http://localhost:8080/**
```
