#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <QDebug>
#include <QTime>
#include <math.h>
#include <QObject>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <string.h>
#include <qmath.h>
#include <zbar.h>
#include <geometry_msgs/Point.h>

#include "Headers/qr_reciever.h"
#include <qr_detector/qr_msg.h>

using namespace std;
using namespace cv;
