#!/bin/bash

premake5 gmake
make all -j 4 && ./Bin/Debug/Sandbox 


