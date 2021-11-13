#!/bin/bash

STM32CubeProgrammer_PATH=~/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin

cd ./Tools/ST-LINK_gdbserver/tools/bin
./ST-LINK_gdbserver --verify -c config.txt -cp ${STM32CubeProgrammer_PATH}