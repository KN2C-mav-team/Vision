#include <QApplication>
#include <include/optflow.h>
#include <QThread>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ros::init(argc, argv, "opticalflow");

    ros::NodeHandle nh_;
    image_transport::ImageTransport it(nh_);
    image_transport::Subscriber rgb_image_sub_;

    OptFlow w;

    // Subscribe to input video feed for grab rgb and publish output video feed
    rgb_image_sub_ = it.subscribe("/below_cam/image_raw", 1,
                                  &OptFlow::imageCb_bgr, &w);

    ros::spin();
    return 0;
}
