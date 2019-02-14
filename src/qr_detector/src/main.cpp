#include "Headers/includes.h"

int main(int argc, char** argv){

    ros::init(argc, argv, "detect_Qr");
    qr_reciever recieve;
    recieve.image_sub = recieve.image_transport.subscribe("/usb_cam/image_raw",1,
                                                          &qr_reciever::imageCallback,&recieve);
    ros::spin();

    return 0;
}


