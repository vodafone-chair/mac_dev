cd build
gnome-terminal -- ./channel_server.exe

gnome-terminal -- ./base_station_node.exe 1 14001

gnome-terminal -- ./ue_node.exe 2 60004
gnome-terminal -- ./ue_node.exe 3 60005
#gnome-terminal -- ./ue_node.exe 4 60006
#gnome-terminal -- ./ue_node.exe 5 60007
