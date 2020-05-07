

![alt text](https://github.com/jimdox/Juno/blob/master/Resources/Textures/Juno3D.png) 


## Project Setup
Sandbox is an example project which has everything you need to start simulating some simple phyics scenarios. To get started, all you need to do is to install the latest alpha release of [Premake 5](https://premake.github.io/download.html#v5).

### Running the Sandbox Project
For those unfamiliar with how to use Premake, when you initially download the project, or whenever you create a new file you want to include in your project, you need you will need to generate new Makefiles (using Premake). I've Included a bash script to do this automatically for Linux users, just run `build.sh`.
#### Linux
    to generate makefiles: premake5 gmake 
    then to build project: make config=release all -j [number of cores on machine] 
    and to run:            ./Bin/Release/Sandbox 
#### Windows
    [Windows instructions coming soon...]


### Getting Started with Physics Simulation
In the current version of the project, the templates in the startup menu will now load in some default scenes for you to mess around with. The Particle Simulation template is now using an early implementation of compute shaders, which allows the user to simulate objects fully on the GPU, allowing for much higher particle counts than would otherwise be possible in real-time. 


### Status of Loading Custom .obj Files
In order to correctly load a wavefront file (.obj), apply 'split edges' along any texture seams for the model, this will duplicate vertices along that seam, as the renderer currently can only process unique vertices per any texture coordinate. Additionally, mark all faces as 'shade smooth', as otherwise blender will generate multiple normals per face, which is not currently supported.


### And Finally
This project is still early in development. Cross-platform support is planned, but I am currently exclusively focusing on Linux platforms until I can get around to fixing the Premake side of things to work with Windows and macOS.

    
