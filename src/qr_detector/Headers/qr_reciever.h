#ifndef QRREC_H
#define QRREC_H
#include "Headers/includes.h"

class qr_reciever : public QObject{
    Q_OBJECT
public:
    qr_reciever();
    ~qr_reciever();
    void imageCallback(const sensor_msgs::ImageConstPtr&);

    ros::NodeHandle node_handler;
    ros::Publisher publisher;
    image_transport::ImageTransport image_transport;
    image_transport::Subscriber image_sub;
    cv_bridge::CvImagePtr cv_ptr;

private:

    void decoder(Mat &);
    void publish(const string , const string , const vector<Point> , int );
};

#endif
