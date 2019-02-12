#include "include/HistAndBackproj.h"

HistAndBackproj::HistAndBackproj(QObject *parent) : QObject(parent)
{
  //  transmit=new Quad_Board();
    RNG rng(12345);
    color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
    namedWindow( "back", WINDOW_AUTOSIZE);
    createTrackbar( "Erosion Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "back",
                    &erosion_elem, 2);
    //
    createTrackbar( "Erosion Kernel size:\n 2n +1", "back",
                    &erosion_size, 21);
    createTrackbar( "Dilation Kernel size:\n 2n +1", "back",
                    &dilation_size, 21);
    createTrackbar( "Vmin", "back", &vmin, 256, 0 );
    createTrackbar( "Vmax", "back", &vmax, 256, 0 );
    createTrackbar( "Smin", "back", &smin, 256, 0 );
    createTrackbar("Min Area","back",&minArea,10000);
    namedWindow( window_name, WINDOW_AUTOSIZE ); // Create window
    //![window]

    //![create_trackbar1]
    /// Create Trackbar to select Morphology operation
    createTrackbar("Operator:\n 0: Opening - 1: Closing  \n 2: Gradient - 3: Top Hat \n 4: Black Hat", window_name, &morph_operator, max_operator/*, Morphology_Operations*/ );
    //![create_trackbar1]

    //![create_trackbar2]
    /// Create Trackbar to select kernel type
    createTrackbar( "Element:\n 0: Rect - 1: Cross - 2: Ellipse", window_name,
                    &morph_elem, max_elem
                    /*Morphology_Operations */);
    //![create_trackbar2]

    //![create_trackbar3]
    /// Create Trackbar to choose kernel size
    createTrackbar( "Kernel size:\n 2n +1", window_name,
                    &morph_size, max_kernel_size
                    /*Morphology_Operations*/ );
    //![create_trackbar3]
    ////////////////////////////////////////////////////////////
    hsize = 16;
    hranges[0] = 0;
    hranges[1] = 180;
    phranges = hranges;
    histimg = Mat::zeros(200, 320, CV_8UC3);
    hist=Mat::zeros(hsize,1,CV_32F);
    //  hist.at<float>(0,2)=255;
    //  hist.at<float>(0,6)=255;
    //  hist.at<float>(0,3)=255;
     hist.at<float>(0,red)=255;
     hist.at<float>(0,1)=255;
     hist.at<float>(0,15)=255;
    // hist.at<float>(0,9)=255;
   // hist.at<float>(0,blue)=255;

    histimg = Scalar::all(0);
    int binW = histimg.cols / hsize;
    Mat buf(1, hsize, CV_8UC3);
    for( int i = 0; i < hsize; i++ )
        buf.at<Vec3b>(i) = Vec3b(saturate_cast<uchar>(i*180./hsize), 255, 255);
    cvtColor(buf, buf, CV_HSV2BGR);

    for( int i = 0; i < hsize; i++ )
    {
        int val = saturate_cast<int>(hist.at<float>(i)*histimg.rows/255);
        rectangle( histimg, Point(i*binW,histimg.rows),
                   Point((i+1)*binW,histimg.rows - val),
                   Scalar(buf.at<Vec3b>(i)), -1, 8 );
    }
#ifdef show_hist
    imshow("histogram",histimg);
#endif

    polePub=poleSet_nh.advertise<pole_handler::poleMsg>("pole_data",1);

}

void HistAndBackproj::contours_handler(Mat BP_image)
{
    medianBlur(BP_image,BP_image,5);
    findContours(BP_image,contours,hierarchy,CV_RETR_TREE,CV_CHAIN_APPROX_NONE);
    std::vector<std::vector<Point> > contours_poly( contours.size() );
    for (unsigned int i=0;i<contours.size();i++)
    {
        approxPolyDP( Mat(contours[i]), contours_poly[i], 5, true );
    }

    std::vector<Moments> mu(contours_poly.size() );
    for(unsigned int i = 0; i < contours_poly.size(); i++ )
        mu[i] = moments( contours_poly[i], false );

    ///  Get the mass centers:
   std::vector<Point2f> mc( contours_poly.size() );
    for(unsigned int i = 0; i < contours_poly.size(); i++ )
        mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );

    for (unsigned int i=0;i<contours_poly.size();i++)
    {
        poly = boundingRect(contours_poly[i]);
        white_count=countNonZero(backproj(((poly))));
        if(contourArea(contours_poly[i]) >minArea /*&& contours_poly[i].size()==4*/ &&white_count>0.75*minArea)
        {
            allowed=0;
           // circle(sub,Point(poly.x+poly.width/2,poly.y+poly.height/2),8,Scalar(255,0,0),-1);
           // rectangle(sub,poly,color,2);
            //  qDebug()<<"set point = "<<int(mc[i].x-160);
            // qDebug()<<"number of poles ="<<mc.size();
            int error= int(mc[i].x-160);
            if(error<0)
            {
                //  transmit->Fill_Data(3,Stop,error,-160);
             //   qDebug()<<"Wait ";
                msg.order=Stop;
                msg.error=int(error);
                msg.setpoint=-160;
		msg.Area=contourArea(contours_poly[i]);
                polePub.publish(msg);
            }
            else
            {
                //transmit->Fill_Data(3,Stop,error,160);
              //  qDebug()<<"Wait ";
                msg.order=Stop;
                msg.error=int(error);
                msg.setpoint=160;
		msg.Area=contourArea(contours_poly[i]);
                polePub.publish(msg);
            }
        }
        else
            allowed=1;
    }
    if(allowed)
    {
        //  transmit->Fill_Data(3,0,0,0);
        qDebug()<<"Go forward ";
      //  qDebug()<<"allowed = "<<allowed;
        msg.order=2;
        msg.error=0;
        msg.setpoint=0;
        msg.Area=0;
        polePub.publish(msg);
    }
   // imshow("sub",sub);
   // waitKey(2);

}

void HistAndBackproj::Morphology_Operations()
{
    //![operation]
    int operation = morph_operator + 2;
    //![operation]

    Mat element = getStructuringElement( morph_elem, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
    /// Apply the specified morphology operation
    morphologyEx( backproj, backproj, operation, element );
}

void HistAndBackproj::erosion_dilation()
{
    int erosion_type;
    if( erosion_elem == 0 ){ erosion_type = MORPH_RECT; }
    else if( erosion_elem == 1 ){ erosion_type = MORPH_CROSS; }
    else if( erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }


    int dilation_type;
    if( dilation_elem == 0 ){ dilation_type = MORPH_RECT; }
    else if( dilation_elem == 1 ){ dilation_type = MORPH_CROSS; }
    else if( dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }

    ERODE_elem = getStructuringElement( erosion_type,
                                        Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                        Point( erosion_size, erosion_size ) );


    DILATE_elem = getStructuringElement( dilation_type,
                                         Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                         Point( dilation_size, dilation_size ) );
    medianBlur(backproj,backproj,3);
    erode( backproj, backproj, ERODE_elem );
    dilate( backproj, backproj, DILATE_elem );
    Morphology_Operations();
#ifdef show_backproj
    imshow("back",backproj);
#endif
    contours_handler(backproj);


}



void HistAndBackproj::CalcHist(Mat raw_image)
{   
	Size size(320,240);
	resize(raw_image,sub,size);
    cvtColor(sub, hsv, CV_BGR2HSV);

    int _vmin = vmin, _vmax = vmax;
    inRange(hsv, Scalar(0, smin, MIN(_vmin,_vmax)),
            Scalar(180, 256, MAX(_vmin, _vmax)), mask);
    int ch[] = {0, 0};
    hue.create(hsv.size(), hsv.depth());
    mixChannels(&hsv, 1, &hue, 1, ch, 1);

    calcBackProject(&hue, 1, 0, hist, backproj, &phranges);

    backproj &= mask;
    erosion_dilation();

}

