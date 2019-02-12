#include "ros/ros.h"
#include "std_msgs/String.h"
//#include "serial_handler/pole.h"
#include <include/quad_board.h>

int main(int argc, char **argv)
{

  ros::init(argc, argv, "serial_handler");

  ros::NodeHandle nh_serial,nh_serial2,nh_serial3;
  Quad_Board transmit;
 // ros::Subscriber sub_pole = nh_serial.subscribe("pole_data", 1,&Quad_Board::poleSerial_CB,&transmit);
  ros::Subscriber sub_optFlow = nh_serial2.subscribe("opticalFlow_data", 1,&Quad_Board::optFlow_CB,&transmit);
  ros::Subscriber sub_window = nh_serial3.subscribe("Window_data", 1,&Quad_Board::window_CB,&transmit);

  ros::spin();

  return 0;
}

