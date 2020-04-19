#!/bin/bash

premake5 gmake config=Release platform=Linux
make all -j 4 && ./Bin/Debug/Sandbox 


