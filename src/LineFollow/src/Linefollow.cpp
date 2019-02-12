#include "Linefollow.h"

Linefollow::Linefollow(QObject *parent) : QObject(parent)
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
    //  //  hist.at<float>(0,2)=255;
    //    //  hist.at<float>(0,6)=255;
    //    //  hist.at<float>(0,3)=255;
    hist.at<float>(0,red)=255;
    hist.at<float>(0,1)=255;
    hist.at<float>(0,15)=255;
    //    //    hist.at<float>(0,8)=255;
    //    //    hist.at<float>(0,blue)=255;
    //    //    hist.at<float>(0,10)=255;
    //    //    hist.at<float>(0,11)=255;
    
    //    histimg = Scalar::all(0);
    //    int binW = histimg.cols / hsize;
    //    Mat buf(1, hsize, CV_8UC3);
    //    for( int i = 0; i < hsize; i++ )
    //        buf.at<Vec3b>(i) = Vec3b(saturate_cast<uchar>(i*180./hsize), 255, 255);
    //    cvtColor(buf, buf, CV_HSV2BGR);
    
    //    for( int i = 0; i < hsize; i++ )
    //    {
    //        int val = saturate_cast<int>(hist.at<float>(i)*histimg.rows/255);
    //        rectangle( histimg, Point(i*binW,histimg.rows),
    //                   Point((i+1)*binW,histimg.rows - val),
    //                   Scalar(buf.at<Vec3b>(i)), -1, 8 );
    //    }

    // video  =VideoWriter("~/Desktop/output5.avi",CV_FOURCC('M','J','P','G'),camera.cap.get(CV_CAP_PROP_FPS), Size(640,480));
#ifdef show_hist
    imshow("histogram",histimg);
#endif
    
      linePub=lineSet_nh.advertise<LineFollow::lineMsg>("line_data",1);

    
}



void Linefollow::contours_handler(Mat BP_image)
{
    
    msg.transmission_flag=-1;
    medianBlur(BP_image,BP_image,5);
    findContours(BP_image,contours,hierarchy,CV_RETR_TREE,CV_CHAIN_APPROX_NONE);
    std::vector<std::vector<Point> > contours_poly( contours.size() );
    for (unsigned int i=0;i<contours.size();i++)
    {
        approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
        
    }
    
    std::vector<Moments> mu(contours_poly.size() );
    for(unsigned int i = 0; i < contours_poly.size(); i++ )
        mu[i] = moments( contours_poly[i], false );
    
    ///  Get the mass centers:
    std::vector<Point2f> mc( contours_poly.size() );
    for(unsigned int i = 0; i < contours_poly.size(); i++ )
        mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );

    float nearby[contours_poly.size()];
    for(uint i=0;i<contours_poly.size();i++)
        nearby[i]=dist(mc[i],prev_mc);
    bubbleSort(nearby,contours_poly.size());
    
    
    for (unsigned int i=0;i<contours_poly.size();i++)
    {
        //        for(uint j=0;j<contours_poly.size();j++)
        //            circle(sub,contours_poly[i][j],3,Scalar(100,100,0),2);
        poly = boundingRect(contours_poly[i]);
        //  rotate_poly=minAreaRect( contours_poly[i]);
        white_count=countNonZero(backproj(((poly))));

        
        if(contourArea(contours_poly[i]) >minArea  /*&&contours_poly[i].size()==4 */&&white_count>0.75*minArea && dist(mc[i],prev_mc) == nearby[0] )
        {


            
            //  circle(sub,mc[i],8,Scalar(100,100,0),-1);
            
            for(uint w=0;w<contours_poly[i].size();w++)
            {
                //  circle(sub,contours_poly[i][w],3,Scalar(255,255,255),2);
                
                
                
                if( contours_poly[i][w].x == sub.cols-1 && contours_poly[i][w].y < mc[i].y )
                {

                    p2=contours_poly[i][w];
                    // circle(sub,p2,8,color,-1);
                    //                    Point sorted[contours_poly[i].size()];
                    //                    for(unsigned int a=0;a<contours_poly[i].size();a++)
                    //                        sorted[a]=contours_poly[i][a];
                    //                    bubbleSort_point_x(sorted,contours_poly[i].size());
                    //                    p2=(sorted[contours_poly[i].size()]+sorted[contours_poly[i].size()-1])*0.5;

                    //  qDebug()<<"right 1 angle = "<<int((PI/2+atan((mc[i].y-p2.y)/(mc[i].x-p2.x)))*180/PI);
                    angle=-int((PI/2+atan((mc[i].y-p2.y)/(mc[i].x-p2.x)))*180/PI);
                    msg.direction=255;
                    msg.angle=angle;
                    msg.dx=sub.cols/2-p2.x;
                    msg.transmission_flag=1;

                    
                    break;
                    
                    
                }
                
                else if( contours_poly[i][w].x == 0 && contours_poly[i][w].y <= mc[i].y)
                {

                    //                    p2=contours_poly[i][w];
                    //                    circle(sub,p2,8,Scalar(200,0,200),-1);
                    // qDebug()<<"left 1 angle = "<<int((PI/2-atan((mc[i].y-p2.y)/(mc[i].x-p2.x)))*180/PI);

                    Point sorted[contours_poly[i].size()];
                    for(unsigned int a=0;a<contours_poly[i].size();a++)
                        sorted[a]=contours_poly[i][a];
                    bubbleSort_point_x(sorted,contours_poly[i].size());
                    p2=(sorted[0]+sorted[1])*0.5;
                    angle=int((PI/2-atan((mc[i].y-p2.y)/(mc[i].x-p2.x)))*180/PI);
                    msg.direction=255;
                    msg.angle=angle;
                    msg.dx=sub.cols/2-p2.x;
                    msg.transmission_flag=1;
                    break;
                    
                }
                
                else if(contours_poly[i][w].y <=3 && contours_poly[i][w].x<mc[i].x  )
                {
                    //  p2=contours_poly[i][w];
                    //  circle(sub,p2,8,Scalar(200,0,200),-1);
                    //   qDebug()<<"left angle = "<<(PI/2-atan((mc[i].y-p2.y)/(mc[i].x-p2.x)))*180/PI;
                    //                    forward_left=true;
                    //                    if(forward_right == true)
                    //                    {
                    //                        p2= 0.5*(p2+contours_poly[i][w]);
                    //                        forward_right=false;
                    
                    //                    }
                    //                    else
                    //                        p2=contours_poly[i][w];
                    
                    Point sorted[contours_poly[i].size()];
                    for(unsigned int a=0;a<contours_poly[i].size();a++)
                        sorted[a]=contours_poly[i][a];
                    bubbleSort_point_y(sorted,contours_poly[i].size());
                    p2=(sorted[0]+sorted[1])*0.5;
                    //  circle(sub,p2,8,color,-1);
                    float tetha=(PI/2-atan((mc[i].y-p2.y)/(mc[i].x-p2.x)))*180/PI;
                    if(abs(tetha-180)>10)
                    {
                        if(p2.x<mc[i].x)
                        {
                            angle=int((PI/2-atan((mc[i].y-p2.y)/(mc[i].x-p2.x)))*180/PI);

                            msg.direction=255;
                            msg.angle=angle;
                            msg.dx=sub.cols/2-p2.x;
                            msg.transmission_flag=1;
                        }
                    }
                    else
                    {
                        //  qDebug()<< "straight angle " <<0;
                        angle=0;
                        msg.direction=255;
                        msg.angle=angle;
                        msg.dx=sub.cols/2-p2.x;
                        msg.transmission_flag=1;
                    }
                    
                    break;
                    
                    
                    //    break;
                    
                }
                
                else if(contours_poly[i][w].y <=3  && contours_poly[i][w].x>mc[i].x)
                {
                    //p2=contours_poly[i][w];
                    // circle(sub,p2,8,Scalar(200,0,200),-1);
                    // qDebug()<<"right 2 angle = "<<(PI/2+atan((mc[i].y-p2.y)/(mc[i].x-p2.x)))*180/PI;
                    //                    forward_right=true;
                    //                    if(forward_left == true)
                    //                    {
                    //                        p2= 0.5*(p2+contours_poly[i][w]);
                    //                        forward_left=false;
                    
                    //                    }
                    
                    //                    else
                    //                        p2=contours_poly[i][w];
                    
                    //   break;
                    
                    Point sorted[contours_poly[i].size()];
                    for(unsigned int a=0;a<contours_poly[i].size();a++)
                        sorted[a]=contours_poly[i][a];
                    bubbleSort_point_y(sorted,contours_poly[i].size());
                    p2=(sorted[0]+sorted[1])*0.5;
                    //   circle(sub,p2,8,color,-1);
                    
                    //  qDebug()<<"right 2 angle"<<int((PI/2+atan((mc[i].y-p2.y)/(mc[i].x-p2.x)))*180/PI);
                    angle=-int((PI/2+atan((mc[i].y-p2.y)/(mc[i].x-p2.x)))*180/PI);
                    msg.direction=255;
                    msg.angle=angle;
                    msg.dx=sub.cols/2-p2.x;
                    msg.transmission_flag=1;
                    break;
                    
                }
                
                
                else
                {
                    Point sorted[contours_poly[i].size()];
                    for(unsigned int a=0;a<contours_poly[i].size();a++)
                        sorted[a]=contours_poly[i][a];
                    bubbleSort_point_y(sorted,contours_poly[i].size());
                    p2=(sorted[0]+sorted[1])*0.5;
                    for(uint a=0;a<contours_poly[i].size();a++)
                    {
                        if(sorted[a].x==sub.cols-1 || sorted[a].x== 0 )
                            p2=sorted[a];
                    }
                    //                    circle(sub,p2,8,Scalar(200,0,200),-1);
                    if(p2.x<mc[i].x)
                    {
                        Point sorted[contours_poly[i].size()];
                        for(unsigned int a=0;a<contours_poly[i].size();a++)
                            sorted[a]=contours_poly[i][a];
                        bubbleSort_point_x(sorted,contours_poly[i].size());
                        p2=(sorted[0]+sorted[1])*0.5;

                        // qDebug()<<"left 3 angle "<<int((PI/2-atan((mc[i].y-p2.y)/(mc[i].x-p2.x)))*180/PI);
                        angle=int((PI/2-atan((mc[i].y-p2.y)/(mc[i].x-p2.x)))*180/PI);
                        //  circle(sub,p2,8,Scalar(200,0,200),-1);
                        msg.direction=255;
                        msg.angle=angle;
                    }
                    else
                    {
                        //                        Point sorted[contours_poly[i].size()];
                        //                        for(unsigned int a=0;a<contours_poly[i].size();a++)
                        //                            sorted[a]=contours_poly[i][a];
                        //                        bubbleSort_point_x(sorted,contours_poly[i].size());
                        //                        p2=(sorted[contours_poly[i].size()]+sorted[contours_poly[i].size()-1])*0.5;

                        // qDebug()<<"right 3 angle"<<int((PI/2+atan((mc[i].y-p2.y)/(mc[i].x-p2.x)))*180/PI);
                        angle=-int((PI/2+atan((mc[i].y-p2.y)/(mc[i].x-p2.x)))*180/PI);
                        // circle(sub,p2,8,Scalar(200,0,200),-1);
                        msg.direction=255;
                        msg.angle=angle;
                    }

                    msg.dx=sub.cols/2-p2.x;
                    msg.transmission_flag=1;
                    break;
                    
                }
                
                
                
                
            }
            
            
          //  line(sub,mc[i],p2,Scalar(150,100,50),3);
            prev_mc=mc[i];
          //  linePub.publish(msg);
            
            
        }
        
    }
    
    //  d2=ros::Time::now().toSec();
    //qDebug()<<"Ros delay = "<<d2-d1;
   // qDebug()<<"Delay = "<<d.elapsed();
   //  putText(sub,QString("angle = "+QString::number(angle)).toLatin1().data(),Point(3,50),CV_FONT_HERSHEY_PLAIN,1,Scalar(0,250,0),2);

   //  imshow("sub",sub);
    //  video.write(sub);
  //   waitKey(2);
    
}

void Linefollow::Morphology_Operations()
{
    //![operation]
    int operation = morph_operator + 2;
    //![operation]
    
    Mat element = getStructuringElement( morph_elem, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
    /// Apply the specified morphology operation
    morphologyEx( backproj, backproj, operation, element );
}

float Linefollow::dist(Point A, Point B)
{
    int dx=B.x-A.x;
    int dy=B.y-A.y;
    float distance=sqrt(dx*dx+dy*dy);
    return distance;
}

void Linefollow::bubbleSort_point_y(Point arr[], int n)
{
    bool swapped = true;
    int j = 0;
    Point tmp;
    while (swapped) {
        swapped = false;
        j++;
        for (int i = 0; i < n - j; i++) {
            if (arr[i].y > arr[i + 1].y ) {
                tmp = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = tmp;
                swapped = true;
            }
        }
    }
}


void Linefollow::bubbleSort_point_x(Point arr[], int n)
{
    bool swapped = true;
    int j = 0;
    Point tmp;
    while (swapped) {
        swapped = false;
        j++;
        for (int i = 0; i < n - j; i++) {
            if (arr[i].x > arr[i + 1].x) {
                tmp = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = tmp;
                swapped = true;
            }
        }
    }
}


void Linefollow::bubbleSort(float arr[], int n)
{
    bool swapped = true;
    int j = 0;
    float tmp;
    while (swapped) {
        swapped = false;
        j++;
        for (int i = 0; i < n - j; i++) {
            if (arr[i] > arr[i + 1]) {
                tmp = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = tmp;
                swapped = true;
            }
        }
    }
}

void Linefollow::zbarCB(const std_msgs::String &msg )
{
    color_string=msg.data;
    if(color_string == "http://en.m.wikipedia.org")
    {
        hist.at<float>(0,red)=255;
        hist.at<float>(0,1)=255;
        hist.at<float>(0,15)=255;
    }
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

    begin=true;


}

void Linefollow::erosion_dilation()
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



void Linefollow::CalcHist(Mat raw_image)
{

    //   if(begin)
    //   {
   //  d.start();
    // d1=ros::Time::now().toSec();
   // Size size(160,120);
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
    //    }

    
}
