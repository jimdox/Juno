#!/bin/bash

premake5 gmake
make config=release all -j 4 && ./Bin/Release/Sandbox 


