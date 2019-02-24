#! /bin/bash

sudo chmod  777 /dev/video0
sudo chmod  777 /dev/video1



#source /opt/ros/indigo/setup.bash 2> /home/odroid/se.log
#source /home/odroid/Linefollow_ws/devel/setup.bash 2>/home/odroid/line.log

#source /home/odroid/robocupap2018/devel/setup.bash 2> /home/odroid/se1.log

#unset DISPLAY XAUTHORITY
#xvfb-run roslaunch serial_handler myLaunch.launch 2> /home/odroid/ros_error.log

export DISPLAY=:0.0


/home/odroid/autcup2019_final/build/./autcup2019  2> /home/odroid/aut.log



# &>> /home/odroid/rosout.log


#roslaunch serial_handler myLaunch.launch 

#./a_optical_didari/optical_console
#./altitude_control/altitude_control 2> altitude_control_error.log


