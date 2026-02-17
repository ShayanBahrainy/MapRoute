#! /bin/bash
read -p "Which executable: " executable
cd build/
cmake --build .
sudo setcap cap_net_raw+ep $executable
./$executable
cd ..