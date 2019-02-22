#include "colordetection.h"

int bins = 25;

Mat yellow;

ColorDetection::ColorDetection(QObject *parent) : QObject(parent)
{

}

Mat ColorDetection::detectRed(Mat src, Scalar scalar1, Scalar scalar2,
                              Scalar scalar3, Scalar scalar4)
{
    Mat detected_lower;
    Mat detected_higher;
    Mat hsv;
    cvtColor(src,hsv,CV_BGR2HSV);

    inRange(hsv,scalar1,scalar2,detected_lower);

    inRange(hsv,scalar3,scalar4,detected_higher);


    //inRange(hsv,Scalar(0, 100, 100), cv::Scalar(10, 255, 255)/*scalar1,scalar2*/,detected_lower);

    //inRange(hsv,Scalar(160, 100, 100), cv::Scalar(179, 255, 255)/*scalar3,scalar4*/,detected_higher);

    Mat red = detected_lower | detected_higher;


    return red;
}

Mat ColorDetection::detectColor(Mat src ,Scalar scalar1, Scalar scalar2)
{


    Mat hsv;
    int a=clock();

    cvtColor(src,hsv,CV_BGR2HSV);

    int b=clock();
    inRange(hsv,scalar1,scalar2,yellow);

    return yellow;

}

Mat ColorDetection::do_Hist_And_Backproj(Mat hue,int color)
{



    Mat backproj;

    int hsize=64;

    if(!hue.empty() ){

        //Get Histogram

        Mat hist=Mat::zeros(hsize,1,CV_32F);

        Mat  histImg = Mat::zeros(320, 240, CV_8UC3);
        histImg =  Scalar::all(0);
        float hue_range[] = { 0, 180 };
        const float* ranges = { hue_range };
        int binW = histImg.cols / hsize;

        Mat buf(1, hsize, CV_8UC3);
        for( int i = 0; i < hsize; i++ )
            buf.at<Vec3b>(i) = Vec3b(saturate_cast<uchar>(i*180./hsize), 255, 255);



        switch(color){
        case YELLOW :

            //YELLOW RANGE


            for(int k=4;k<13 ;k++)
                hist.at<float>(0,k)=255;


            break;
        case RED :

            //RED RANGE

            for(int k=0;k<13 ;k++)
                hist.at<float>(0,k)=255;

            for(int j=hsize-1;j>60 ;j--)
               hist.at<float>(0,j)=255;

            break;
        }

            cvtColor(buf, buf, CV_HSV2BGR);


            for( int i = 0; i < hsize; i++ )
            {
                int val = saturate_cast<int>(hist.at<float>(i)*histImg.rows/255);
                rectangle( histImg, Point(i*binW,histImg.rows),
                           Point((i+1)*binW,histImg.rows - val),
                           Scalar(buf.at<Vec3b>(i)), -1, 8 );
            }



           // namedWindow("Histogram",CV_WINDOW_NORMAL);
           // imshow( "Histogram", histImg );



            calcBackProject( &hue, 1, 0, hist, backproj, &ranges, 1, true );
            /// Draw the backproj
            //       imshow( "BackProj", backproj );

        }




    return backproj;
}

Mat ColorDetection::backprojectionResult(Mat src,int color)
{

    Mat hue;
    Mat hsv;

    cvtColor(src,hsv,CV_BGR2HSV);
    hue.create( hsv.size(), hsv.depth() );
    int ch[] = { 0, 0 };
    mixChannels( &hsv, 1, &hue, 1, ch, 1 );


    Mat backproj= do_Hist_And_Backproj(hue,color);


    //imshow("detected",detected);

    //medianBlur(backproj,backproj,3);  //change the filter

    //imshow("backproj",backproj);


    return backproj;
}
