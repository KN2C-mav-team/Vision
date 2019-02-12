#include "detectwindow.h"

Mat DetectWindow::sub;//

DetectWindow::DetectWindow(QObject *parent)
    : QObject(parent)
{
    // cap = new Capture();
    win_pub=win_nh.advertise<window_pkg::WinMsg>("Window_data",1);
    sub_mode=2;
#ifdef _SPLIT_ON
    sub_mode=0;
#endif

    show = false;
    backprojMode = false;
    selectObject = false;
    trackObject = 0;
    showHist = true;
    vmin = 10, vmax = 256, smin = 50, erosion_size=0;
    hsize = 16;
    hranges[0] = 0;
    hranges[1] = 180;
    phranges = hranges;

#ifdef _INIT
    namedWindow( "Window Detection", CV_WINDOW_AUTOSIZE );
    namedWindow( "backproj", CV_WINDOW_AUTOSIZE );

    qDebug("ok1");

    createTrackbar( "Vmin", "Window Detection", &vmin, 256, 0 );
    createTrackbar( "Vmax", "Window Detection", &vmax, 256, 0 );
    createTrackbar( "Smin", "Window Detection", &smin, 256, 0 );
    //#endif
    histimg = Mat::zeros(200, 320, CV_8UC3);
    paused = false;
    hist=Mat::zeros(hsize,1,CV_32F);

    //blue
    hist.at<float>(0,9)=255;
    hist.at<float>(0,10)=255;
    hist.at<float>(0,11)=255;

    hit_border=0;
    scale=0;


#ifdef _SHOW_HIST
    namedWindow( "Histogram", 0 );
#endif
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
#endif


#ifdef _ERODE_DILATE_ON

    erosion_elem = EROSION_ELEM;
    erosion_size = EROSRION_SIZE;
    dilation_elem = DILATION_ELEM;
    dilation_size = DILATION_SIZE;
    // max_elem = 2;
    //max_kernel_size = 21;
    int erosion_type;
    if( erosion_elem == 0 ){ erosion_type = MORPH_RECT; }
    else if( erosion_elem == 1 ){ erosion_type = MORPH_CROSS; }
    else if( erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }

    ERODE_elem = getStructuringElement( erosion_type,
                                        Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                        Point( erosion_size, erosion_size ) );

    int dilation_type;
    if( dilation_elem == 0 ){ dilation_type = MORPH_RECT; }
    else if( dilation_elem == 1 ){ dilation_type = MORPH_CROSS; }
    else if( dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }

    DILATE_elem = getStructuringElement( dilation_type,
                                         Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                         Point( dilation_size, dilation_size ) );

#endif
#ifdef _INIT
    /// Create Erosion Trackbar
    createTrackbar( "Erosion Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "Window Detection",
                    &erosion_elem, 2);

    createTrackbar( "Erosion Kernel size:\n 2n +1", "Window Detection",
                    &erosion_size, 21);

    /// Create Dilation Trackbar
    createTrackbar( "Dilation Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "Window Detection",
                    &dilation_elem, 2);

    createTrackbar( "Dilation Kernel size:\n 2n +1", "Window Detection",
                    &dilation_size, 21);
#endif

}


void DetectWindow::bubbleSort(int arr[], int n)
{

    bool swapped = true;
    int j = 0;
    int tmp;
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

void DetectWindow::bubbleSort_point(Point arr[], int n)
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



void DetectWindow::getImage(Mat raw_image)
{
    qDebug() << "detect: " <<QThread::currentThreadId();
    double exec_time = (double)getTickCount();

    detectFrame=raw_image;
    qDebug()<<"frame received";

    if (!detectFrame.empty())
    {


        //#ifdef _SHOW_HIST
        //        histimg = Scalar::all(0);
        //        int binW = histimg.cols / hsize;
        //        Mat buf(1, hsize, CV_8UC3);
        //        for( int i = 0; i < hsize; i++ )
        //            buf.at<Vec3b>(i) = Vec3b(saturate_cast<uchar>(i*180./hsize), 255, 255);
        //        cvtColor(buf, buf, CV_HSV2BGR);

        //        for( int i = 0; i < hsize; i++ )
        //        {
        //            int val = saturate_cast<int>(hist.at<float>(i)*histimg.rows/255);
        //            rectangle( histimg, Point(i*binW,histimg.rows),
        //                       Point((i+1)*binW,histimg.rows - val),
        //                       Scalar(buf.at<Vec3b>(i)), -1, 8 );
        //        }
#ifdef _INIT
        if (show)
        {
            imshow( "Histogram", histimg );
            waitKey(1);
        }
#endif
        //#endif

        switch (sub_mode) {
        case 0:
            roi.x=0;
            roi.y=0;
            roi.width=detectFrame.cols;
            roi.height=(detectFrame.rows)/2;

            break;
        case 1:
            roi.x=0;
            roi.y=(detectFrame.rows)/2;
            roi.width=detectFrame.cols;
            roi.height=(detectFrame.rows)/2;
            break;

        case 2:
            roi.x=0;
            roi.y=0;
            roi.width=detectFrame.cols;
            roi.height=(detectFrame.rows);

            break;
        default:
            break;
        }

        sub=detectFrame(roi);
        cvtColor(sub, hsv, CV_BGR2HSV);

        int _vmin = vmin, _vmax = vmax;
        inRange(hsv, Scalar(0, smin, MIN(_vmin,_vmax)),
                Scalar(180, 256, MAX(_vmin, _vmax)), mask);
        int ch[] = {0, 0};
        hue.create(hsv.size(), hsv.depth());
        mixChannels(&hsv, 1, &hue, 1, ch, 1);


        calcBackProject(&hue, 1, 0, hist, backproj, &phranges);
        backproj &= mask;

#ifdef _SMOOTH_ON
        medianBlur(backproj,backproj,3);  //change the filter
#endif

#ifdef _ERODE_DILATE_ON

        Erosion();
        Dilation();

#endif

        Rect border(0,0,backproj.cols,backproj.rows);
        rectangle(backproj,border,Scalar(255,255,255),_BORDER);
        //   line(backproj,Point(0,0),Point(_FRAME_WIDTH,0),Scalar(255,255,255),_BORDER);
        //   line(backproj,Point(0,_FRAME_HEIGTH),Point(_FRAME_WIDTH,_FRAME_HEIGTH),Scalar(255,255,255),_BORDER);
        //    line(backproj,Point(0,0),Point(0,480),Scalar(255,255,255),10);
        //    line(backproj,Point(640,0),Point(640,480),Scalar(255,255,255),10);


#ifdef _INIT
        if (show)
        {
            imshow ("backproj",backproj);
            waitKey(1);
        }
#endif

        cv::findContours(backproj,contours,hierarchy,CV_RETR_TREE,CV_CHAIN_APPROX_NONE);
        vector<Point> approxCurve[contours.size()];

        for (unsigned int a=0; a<contours.size(); a++)
        {

            //qDebug() << "size: " << contourArea(contours[a]);
            if (contourArea(contours[a]) > (backproj.cols-2*_BORDER)*(backproj.rows-2*_BORDER))
            {
                contours[a].clear();
                continue;
            }

            if (contourArea(contours[a]) < _MIN_CONTOUR_SIZE)
            {
                contours[a].clear();
                continue;
            }


            //        if ( hierarchy[a][2] ==-1)
            //        {
            //            contours[a].clear();
            //            continue;
            //        }

            approxPolyDP(Mat(contours[a]), approxCurve[a], _APPROXCURE_EPSILON, true);

            if (approxCurve[a].size() != 4)
            {
                contours[a].clear();
                continue;
            }

            if (!cv::isContourConvex(approxCurve[a]))
            {
                contours[a].clear();
                continue;
            }

            //        for (unsigned int w = 0; w < approxCurve[a].size(); w++)
            //        {
            //            line(sub,approxCurve[a][w%approxCurve[a].size()],approxCurve[a][(w+1)%approxCurve[a].size()],Scalar(0,0,255),2,1);

            //        }
            //putText(sub,QString::number(a).toLatin1().data(),approxCurve[a][0], FONT_HERSHEY_SIMPLEX,1,Scalar(0,255,0),1,8);

            if (!(approxCurve[a][1].x-approxCurve[a][0].x<_PIXEL_ANGLE_THR || approxCurve[a][1].y-approxCurve[a][0].y<_PIXEL_ANGLE_THR) || !(approxCurve[a][2].x-approxCurve[a][1].x<_PIXEL_ANGLE_THR || approxCurve[a][2].y-approxCurve[a][1].y<_PIXEL_ANGLE_THR))
            {
                contours[a].clear();
                continue;
            }



            Rect inside = boundingRect(approxCurve[a]);
            int white_count=countNonZero(backproj(inside));
            if (white_count>_WHTIEPIX_THR*inside.area())
            {
                contours[a].clear();
                continue;
            }



        }

        int arr_count=0;
        for (unsigned int a=0; a<contours.size(); a++)
        {
            if (contours[a].size()==0)
                continue;
            window_num[arr_count]=a;
            arr_count++;

        }

        qDebug() << "arr_count: " << arr_count;


        switch (arr_count) {
        case 0:
            break;
        case 3:
            for (int j=0; j<arr_count; j++)
            {
                three_windows_center[j].x=0;
                three_windows_center[j].y=0;
                for (unsigned int i=0;i<4;i++)
                {
                    line(sub,approxCurve[window_num[j]][i],approxCurve[window_num[j]][(i+1)%4],Scalar(50,200,0),2);
                    three_windows_center[j]+=approxCurve[window_num[j]][i];


                }

                three_windows_center[j].x=three_windows_center[j].x/4;
                three_windows_center[j].y=three_windows_center[j].y/4;

                three_window_num_border[j]=window_num[j];


            }

            for (int i=0; i<arr_count; i++)
            {
                three_windows_center_sorted[i]=three_windows_center[i];
            }

            bubbleSort_point(three_windows_center_sorted,arr_count);

            for (int i=0; i<arr_count; i++)
            {
                if (three_windows_center[i]==three_windows_center_sorted[0])
                {
                    left_border=three_window_num_border[i];
                }

                else if (three_windows_center[i]==three_windows_center_sorted[2])
                {
                    right_border=three_window_num_border[i];
                }

                else
                {
                    middle=three_window_num_border[i];
                }

            }

            hit_border=0;
            for (unsigned int i=0; i<4; i++)
            {
                if (approxCurve[left_border][i].x<_BORDER)
                {
                    hit_border=1;
                    break;
                }

            }

            if (hit_border==1)
            {
                hit_border=0;
                for (unsigned int i=0; i<4; i++)
                {
                    if (approxCurve[right_border][i].x>sub.cols-_BORDER)
                    {
                        hit_border=1;
                        break;
                    }

                }
            }

            if (hit_border==1)
            {
                circle(sub,three_windows_center_sorted[1],6,Scalar(100,0,255),-1,8,0);

                mean_window_center=three_windows_center_sorted[1].x;

                side1_y=approxCurve[middle][1].y-approxCurve[middle][0].y;
                side2_y=approxCurve[middle][2].y-approxCurve[middle][1].y;
                side1_x=approxCurve[middle][1].x-approxCurve[middle][0].x;
                side2_x=approxCurve[middle][2].x-approxCurve[middle][1].x;


                if (side2_y>side1_y)
                {
                    line(sub,approxCurve[middle][0],approxCurve[middle][1],Scalar(50,200,200),2);
                    scale=std::abs(side1_x);
                }
                else
                {
                    line(sub,approxCurve[middle][1],approxCurve[middle][2],Scalar(50,200,200),2);
                    scale=std::abs(side2_x);

                }

                break;

            }

            else
            {

                qDebug() << "3 windows is noise. Set to default";

            }



        default:

#ifdef _ELIMINATE_EXTERNAL
            for (int i=0; i<arr_count; i++)
            {
                inside_polygons=1;
                for (int j=0; j<4; j++)
                {
                    if (pointPolygonTest(contours[window_num[i]],approxCurve[window_num[(i+1)%arr_count]][j],false)<=0)
                    {
                        inside_polygons=0;
                        break;
                    }
                }

                if (inside_polygons==1)
                {
                    contours[window_num[i]].clear();
                }

            }

#endif
            arr_count=0;
            for (unsigned int a=0; a<contours.size(); a++)
            {
                if (contours[a].size()==0)
                    continue;
                contour_cols_size[arr_count]=contours[a].size();
                arr_count++;

            }

            bubbleSort(contour_cols_size,arr_count);


            mean_window_center=0;
            for (unsigned int a=0; a<contours.size(); a++)
            {


                if (contours[a].size()==0)
                    continue;


                if (contours[a].size()!=contour_cols_size[arr_count-1])
                    continue;



                hit_border=0;
                for (unsigned int i=0; i<approxCurve[a].size(); i++)
                {
                    if (approxCurve[a][i].x<_BORDER || approxCurve[a][i].x>sub.cols-_BORDER)
                    {
                        hit_border=1;
                        break;
                    }

                }

                window_center.x=0;
                window_center.y=0;
                for (unsigned int i=0;i<approxCurve[a].size();i++)
                {
                    //if (show)
                    {
                        line(sub,approxCurve[a][i],approxCurve[a][(i+1)%4],Scalar(50,200,0),2);
                    }

                    window_center+=approxCurve[a][i];

                }

                window_center.x=window_center.x/approxCurve[a].size();
                window_center.y=window_center.y/approxCurve[a].size();

                //if (show)
                {
                    circle(sub,window_center,6,Scalar(150,100,0),-1,8,0);

                }

                mean_window_center+=window_center.x;

                side1_y=approxCurve[a][1].y-approxCurve[a][0].y;
                side2_y=approxCurve[a][2].y-approxCurve[a][1].y;
                side1_x=approxCurve[a][1].x-approxCurve[a][0].x;
                side2_x=approxCurve[a][2].x-approxCurve[a][1].x;

                if (hit_border==0)
                {
                    if (side2_y>side1_y)
                    {
                        line(sub,approxCurve[a][0],approxCurve[a][1],Scalar(50,200,200),2);
                        scale=std::abs(side1_x);
                    }
                    else
                    {
                        line(sub,approxCurve[a][1],approxCurve[a][2],Scalar(50,200,200),2);
                        scale=std::abs(side2_x);

                    }
                }

            }

            line(sub,Point(mean_window_center,sub.rows/2),Point(sub.cols/2,sub.rows/2),Scalar(200,50,200),2);
            circle(sub,Point(sub.cols/2,sub.rows/2),6,Scalar(200,200,50),-1,8,0);


            break;
        }

        qDebug() << "scale: " << scale;
        //        imshow("sub",sub);
        //         waitKey(2);

        //        if (scale<_FIRST_LEVEL_SCALE)
        //            dilation_size=_FIRST_LEVEL_DILATE;
        //        else
        //            dilation_size=3;


        if (arr_count!=0)
        {
#ifdef _SERIAL_ENABLED
            // transmit->Fill_Data(4,int(mean_window_center-sub.cols/2),0,scale,255);
            msg.setpoint=int(mean_window_center-sub.cols/2);
            msg.flag=0;
            msg.scale=scale;
            msg.upperbound=255;
            win_pub.publish(msg);
            //transmit->Fill_Data(3,1,2,3);
            qDebug() << "delta_x: " << mean_window_center-sub.cols/2;

#endif
        }




        //status_msg.data = 17;
        //ROS_INFO("status: %d", status_msg.data);
        // chatter_pub.publish(msg);



        else
        {
#ifdef _SERIAL_ENABLED
            //transmit->Fill_Data(4,mean_window_center-sub.cols/2,0,scale,0);
            msg.setpoint=int(mean_window_center-sub.cols/2);
            msg.flag=0;
            msg.scale=scale;
            msg.upperbound=0;
            win_pub.publish(msg);
            //transmit->Fill_Data(3,1,2,3);
            qDebug() << "no window" ;
#endif
        }


#ifdef _INIT
        if (show)
        {
            imshow("Window Detection",sub);
            waitKey(1);
        }

        //CamOut.write(detectFrame);
        else
            destroyAllWindows();
#endif

        if (arr_count==0)
        {
            switch (sub_mode) {
            case 0:
                sub_mode=1;
                break;
            case 1:
                sub_mode=0;
                break;
            case 2:
                break;
            default:
                break;
            }
        }

#ifdef _WIFI_ENABLED
        emit dataReady();
#endif

        exec_time = ((double)getTickCount() - exec_time)*1000./getTickFrequency();
        qDebug() << "time: " << exec_time;

    }

}

void DetectWindow::Erosion()
{
#ifdef _INIT
    int erosion_type;
    if( erosion_elem == 0 ){ erosion_type = MORPH_RECT; }
    else if( erosion_elem == 1 ){ erosion_type = MORPH_CROSS; }
    else if( erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }

    ERODE_elem = getStructuringElement( erosion_type,
                                        Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                        Point( erosion_size, erosion_size ) );
#endif

    /// Apply the erosion operation
    erode( backproj, backproj, ERODE_elem );
}

void DetectWindow::Dilation()
{
#ifdef _INIT
    int dilation_type;
    if( dilation_elem == 0 ){ dilation_type = MORPH_RECT; }
    else if( dilation_elem == 1 ){ dilation_type = MORPH_CROSS; }
    else if( dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }

    qDebug() << "dilate size:            " << dilation_size;

    DILATE_elem = getStructuringElement( dilation_type,
                                         Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                         Point( dilation_size, dilation_size ) );
#endif
    /// Apply the dilation operation
    dilate( backproj, backproj, DILATE_elem );
}

void DetectWindow::dontshow()
{
    show = false;
}

void DetectWindow::doshow()
{
#ifdef _INIT
    namedWindow( "Window Detection", CV_WINDOW_AUTOSIZE );
    namedWindow( "backproj", CV_WINDOW_AUTOSIZE );
    createTrackbar( "Vmin", "Window Detection", &vmin, 256, 0 );
    createTrackbar( "Vmax", "Window Detection", &vmax, 256, 0 );
    createTrackbar( "Smin", "Window Detection", &smin, 256, 0 );
#ifdef _SHOW_HIST
    namedWindow( "Histogram", 0 );
#endif

#ifdef _ERODE_DILATE_ON
    createTrackbar( "Erosion Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "Window Detection",
                    &erosion_elem, 2);

    createTrackbar( "Erosion Kernel size:\n 2n +1", "Window Detection",
                    &erosion_size, 21);

    createTrackbar( "Dilation Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "Window Detection",
                    &dilation_elem, 2);

    createTrackbar( "Dilation Kernel size:\n 2n +1", "Window Detection",
                    &dilation_size, 21);
#endif
#endif
    show = true;
}


DetectWindow::~DetectWindow()
{

}

