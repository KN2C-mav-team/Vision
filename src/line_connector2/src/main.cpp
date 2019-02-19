#include "Headers/includes.h"
#include "Headers/ProgramHandeler.h"

using namespace std;

int main(int argc, char** argv){

    qDebug()<<"line_connector2 has started";
    ros::init(argc, argv, "line_connect2");
    ProgramHandeler ph;
    ph.image_sub = ph.it_.subscribe("/usb_cam/image_raw", 1,
                                    &ProgramHandeler::imageCallBack, &ph);
    ph.subscriber = ph.nh_.subscribe("/qr_detector/Qr_Datas",1 ,
                                    &ProgramHandeler::Callback, &ph);

    ros::spin();

    return 0;
}


