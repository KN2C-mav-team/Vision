#include "hdetection.h"
#define H_AREA 4000

int Y_H11;
int Y_S11;
int Y_V11;
int Y_H21;
int Y_S21;
int Y_V21;

HDetection::HDetection(QObject *parent) : QObject(parent)
{

    Y_H11=103;//32
    Y_S11=90;//70
    Y_V11=53;//104
    Y_H21=130;
    Y_S21=255;
    Y_V21=255;

    //img= imread("/home/kimia/Desktop/FIRA_Pix/H4.jpg" );
    //resize(img,img,Size(640,480));

    namedWindow("Blue Detection",WINDOW_NORMAL);

    createTrackbar("H1 :","Blue Detection",&Y_H11,179,on_trackbar);
    createTrackbar("S1 :","Blue Detection",&Y_S11,255,on_trackbar);
    createTrackbar("V1 :","Blue Detection",&Y_V11,255,on_trackbar);
    createTrackbar("H2 :","Blue Detection",&Y_H21,179,on_trackbar);
    createTrackbar("S2 :","Blue Detection",&Y_S21,255,on_trackbar);
    createTrackbar("V2 :","Blue Detection",&Y_V21,255,on_trackbar);
    on_trackbar(0,0);
}

void HDetection::on_trackbar(int, void *)
{

}

void HDetection::hLandingCallBack(cv::Mat img)
{
    qDebug("here");
    bool found=false;

    Mat hsv,out;
    Mat rgb=img.clone();
    cvtColor(img,hsv,CV_BGR2HSV);

    inRange(hsv,Scalar(Y_H11,Y_S11,Y_V11),Scalar(Y_H21,Y_S21,Y_V21),out);//blue
    imshow("blue",out);
    Mat threshold_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    /// Detect edges using Threshold
    threshold( out, threshold_output, 100, 255, THRESH_BINARY );
    /// Find contours
    findContours( threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    /// Approximate contours to polygons + get bounding rects and circles
    vector<vector<Point> > contours_poly( contours.size() );
    //qDebug()<<"cont size "<<contours.size();
    //cvtColor(threshold_output,output,CV_GRAY2BGR);
    for( int i = 0; i < contours.size(); i++ )
    {

        approxPolyDP( Mat(contours[i]), contours_poly[i], 8, true );

        //qDebug()<<"CONTOUR SIZE : "<<contours_poly[i].size();
        int cont_size= contours_poly[i].size();

        if(cont_size ==8 || cont_size ==11 || cont_size ==12 ||  cont_size ==13 || cont_size ==14){
            qDebug()<<cont_size<<" Vertex";
            //sort by min y

            qDebug()<<"contour area : "<<contourArea(contours[i]);
            if(contourArea(contours[i])> H_AREA){
                sort(contours_poly[i].begin(),contours_poly[i].end(),myobject1);
                int size=contours_poly[i].size();
qDebug("after sort");
                double m1 =getSlope(contours_poly[i][0],contours_poly[i][3]);
qDebug("after slop1");
qDebug()<<"size -4"<<size-4;
                double m2 =getSlope(contours_poly[i][size-1],contours_poly[i][size-4]);
qDebug("after slop2");
                qDebug()<<"m1-m2 : "<<m1-m2;
                if(abs(m1-m2)<5){
                    qDebug("H Found");
                    //drawContours(rgb,contours[i],i,Scalar(125,125,200),3);
                    //imshow("BLUE_RGB",rgb);
                    emit landingPadCallBack(1);
                    found=true;
                    break;
                }
            }
        }
    }if(!found)
        emit landingPadCallBack(0);
}


double HDetection::getSlope(Point p1,Point p2){


    return (p2.y - p1.y) / (double)(p2.x - p1.x);

}
