#! /bin/bash
cd build/
cmake --build .
sudo setcap cap_net_raw+ep maproute
./maproute
cd ..