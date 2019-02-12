
#include "detectwindow.h"
Mat DetectWindow::sub;//

DetectWindow::DetectWindow(QObject *parent)
    : QObject(parent)
{
    // cap = new Capture();
    win_pub=win_nh.advertise<window_pkg::WinMsg>("Window_data",1); //Creating a handle to publish messages to a topic window_data
    sub_mode=2;
#ifdef _SPLIT_ON
    sub_mode=0;
#endif

    show = false;
    backprojMode = false;
    selectObject = false;
    trackObject = 0;
    showHist = true;
    vmin = 0, vmax = 256, smin = 66, erosion_size=0;dilation_size=0;
    hsize = 16;
    hranges[0] = 0;
    hranges[1] = 180;
    phranges = hranges;
   last_point_counter=0;

#ifdef _INIT
    namedWindow( "Window Detection", CV_WINDOW_AUTOSIZE );
    namedWindow( "backproj", CV_WINDOW_AUTOSIZE );

    qDebug("ok1");

    //createTrackbar( "Vmin", "Window Detection", &vmin, 256, 0 );
    //createTrackbar( "Vmax", "Window Detection", &vmax, 256, 0 );
    //createTrackbar( "Smin", "Window Detection", &smin, 256, 0 );
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
        buf.at<Vec3b>(i) = Vec3b(saturate_cast<uchar>(i*180./hsize), 255, 255);//make colors
    cvtColor(buf, buf, CV_HSV2BGR);

    for( int i = 0; i < hsize; i++)
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
    //createTrackbar( "Erosion Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "Window Detection",
               //     &erosion_elem, 2);

    //createTrackbar( "Erosion Kernel size:\n 2n +1", "Window Detection",
                 //   &erosion_size, 21);

    /// Create Dilation Trackbar
    //createTrackbar( "Dilation Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "Window Detection",
                   // &dilation_elem, 2);

    //createTrackbar( "Dilation Kernel size:\n 2n +1", "Window Detection",
                   // &dilation_size, 21);
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
	mean_window_center=0;
	window_center_y=0;


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

        //    if (!(approxCurve[a][1].x-approxCurve[a][0].x<_PIXEL_ANGLE_THR || approxCurve[a][1].y-approxCurve[a][0].y<_PIXEL_ANGLE_THR) || !(approxCurve[a][2].x-approxCurve[a][1].x<_PIXEL_ANGLE_THR || approxCurve[a][2].y-approxCurve[a][1].y<_PIXEL_ANGLE_THR))
          //  {
            //    contours[a].clear();
             //   continue;
           // }

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

#ifdef _PRINT_LOG
        qDebug() << "arr_count: " << arr_count;

#endif
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

#ifdef  _DRAW_DETECTED_WINDOW
                    line(sub,approxCurve[window_num[j]][i],approxCurve[window_num[j]][(i+1)%4],Scalar(50,200,0),2);
#endif
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
		window_center_y=three_windows_center_sorted[1].y;

                side1_y=approxCurve[middle][1].y-approxCurve[middle][0].y;
                side2_y=approxCurve[middle][2].y-approxCurve[middle][1].y;
                side1_x=approxCurve[middle][1].x-approxCurve[middle][0].x;
                side2_x=approxCurve[middle][2].x-approxCurve[middle][1].x;


                if (side2_y>side1_y)
                {
                    line(sub,approxCurve[middle][0],approxCurve[middle][1],Scalar(255,255,255),8);
                    scale=std::abs(side1_x);
                    last_scale=scale;
                }
                else
                {
                    line(sub,approxCurve[middle][1],approxCurve[middle][2],Scalar(255,255,255),2);
                    scale=std::abs(side2_x);
                    last_scale=scale;

                }

                break;
            }

            else
            {
#ifdef _PRINT_LOG

                qDebug() << "3 windows is noise. Set to default";
#endif

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

            switch(arr_count)
            {
            case 2:
            {
                
                for (unsigned int a=0; a<arr_count; a++)
                {
                    
                    window_center.x=0;
                    window_center.y=0;
                    
                    for (unsigned int i=0;i<approxCurve[window_num[a]].size();i++)
                    {
                        
                        window_center+=approxCurve[window_num[a]][i];
                        
                    }
                    
                    
                    window_center.x=window_center.x/approxCurve[window_num[a]].size();
                    window_center.y=window_center.y/approxCurve[window_num[a]].size();
                    
                    two_windows_center[a]=window_center;
                    
                }
                
                
                
                
                edge_out_mode=false;
		one_edge_mode=false;
                
                bool first_come=true;
                
                //   for (int i=0; i<arr_count; i++)
                // {
                //   two_windows_center_sorted[i]=two_windows_center[i];
                // }
                
                
                //  bubbleSort_point(two_windows_center_sorted,2);
                
                
                
                
                
                
#ifdef _DRAW_TWO_WINDOWS
                
                for (unsigned int w = 0; w < approxCurve[window_num[0]].size(); w++)
                {
                    line(sub,approxCurve[window_num[0]][w%approxCurve[window_num[0]].size()],approxCurve[window_num[0]][(w+1)%approxCurve[window_num[0]].size()],Scalar(255,0,0),2,1);
                    
                }
                for (unsigned int w = 0; w < approxCurve[window_num[1]].size(); w++)
                {
                    line(sub,approxCurve[window_num[1]][w%approxCurve[window_num[1]].size()],approxCurve[window_num[1]][(w+1)%approxCurve[window_num[1]].size()],Scalar(255,0,0),2,1);
                    
                }
                
#endif
                
                //////
                
                int up_right_window_point_y[2]={0,0};
                int down_right_window_point_y[2]={0,0};
                int up_left_window_point_y[2]={0,0};
                int down_left_window_point_y[2]={0,0};
                
                
                
                
                
                for(int j=0;j<2;j++)
                {
                    
                    
                    for(int i=0;i<approxCurve[window_num[j]].size();i++)
                    {
                        if(two_windows_center[j].x < approxCurve[window_num[j]][i].x && two_windows_center[j].y > approxCurve[window_num[j]][i].y )
                        {
                            up_right_window_point_y[j]=approxCurve[window_num[j]][i].y;
                            
                            
                        }
                        if(two_windows_center[j].x > approxCurve[window_num[j]][i].x && two_windows_center[j].y > approxCurve[window_num[j]][i].y )
                        {
                            up_left_window_point_y[j]=approxCurve[window_num[j]][i].y;
                            
                            
                        }
                        
                        if(two_windows_center[j].x < approxCurve[window_num[j]][i].x && two_windows_center[j].y < approxCurve[window_num[j]][i].y )
                        {
                            down_right_window_point_y[j]=approxCurve[window_num[j]][i].y;
                            
                            
                        }
                        if(two_windows_center[j].x > approxCurve[window_num[j]][i].x && two_windows_center[j].y < approxCurve[window_num[j]][i].y )
                        {
                            down_left_window_point_y[j]=approxCurve[window_num[j]][i].y;
                            
                            
                        }
                        
                        
                    }
                    
                }
                for(int i=0;i<2;i++)
                {
                    qDebug()<<down_left_window_point_y[i]<<" ";
                    qDebug()<<down_right_window_point_y[i]<<" ";
                    qDebug()<<up_left_window_point_y[i]<<" ";
                    qDebug()<<up_right_window_point_y[i]<<endl;
                    
                }
                int border_count[2]={0,0};
           
		
                
                
                for(int j=0;j<2;j++)
                {
                    if(up_left_window_point_y[j] <=_BORDER && up_right_window_point_y[j] <=_BORDER)
                    {
                        
                        border_count[j]++;

                        
                    }
                    ////
                    if(up_left_window_point_y[j] <=_BORDER && down_left_window_point_y[j] >=sub.rows -_BORDER)
                    {
			//left_edge_exists[j]=false;
                        
                        border_count[j]++;
                        
                    }
                    
                    if(down_left_window_point_y[j] >=sub.rows -_BORDER && down_right_window_point_y[j] >=sub.rows - _BORDER)
                    {
                        
                        border_count[j]++;
                        
                    }
                    /////
                    if(down_right_window_point_y[j] >=sub.rows -_BORDER && up_right_window_point_y[j] <=_BORDER)
                    {
			//right_edge_exists[j]=false;
                        
                        border_count[j]++;
                        
                    }
                    
                    
                }
                for(int i=0;i<2;i++)
                {
                    if(border_count[i]>=3 && border_count[1-i]>=3)
                    {
                        one_edge_mode=true;
                        
                        
#ifdef _PRINT_LOG
                        qDebug()<<"one edge mode is true"<<endl;
#endif
                        break;
                        
                    }
                    
                    else if(border_count[i]>=3 && (border_count[1-i]<=2))
                    {
                        edge_out_mode=true;
                        
                        window_index=1-i;
#ifdef _PRINT_LOG
                        qDebug()<<"edge out mode is true"<<endl;
                        
#endif
                        break;
                        
                    }
                    
                }
                
                
                
                
                
                
                if(one_edge_mode)
                {
                    
                    
                    for (unsigned int a=0; a<arr_count; a++)
                    {
                        
                        
                        hit_border=0;
                        int index=window_num[a];
                        for (unsigned int i=0; i<approxCurve[index].size(); i++)
                        {
                            
                            
                            if (approxCurve[index][i].x<_BORDER || approxCurve[index][i].x>sub.cols-_BORDER)
                            {
                                hit_border=1;
                                break;
                            }
                            
                        }
                        
                        
                        
                    
                    
                    
                    
                    
                    
                    if(abs(two_windows_center[0].x-suggested_point_x)<abs(two_windows_center[1].x-suggested_point_x))
                    {
                        mean_window_center==two_windows_center[0].x;
			window_center_y=two_windows_center[0].y;
                        float x=two_windows_center[0].x;
                        float y=two_windows_center[0].y;
                        if (show)
                        {
                            for (unsigned int w = 0; w < approxCurve[window_num[0]].size(); w++)
                            {
                                line(sub,approxCurve[window_num[0]][w%approxCurve[window_num[0]].size()],approxCurve[window_num[0]][(w+1)%approxCurve[window_num[0]].size()],Scalar(0,0,255),2,1);
                                
                            }
                            
                            circle(sub,two_windows_center[0],6,Scalar(150,100,0),-1,8,0);
                           // angle
                        }
                        float tetha1=(CV_PI/2+atan((y-0)/  (x-320)))*180/CV_PI;
                        float tetha2=(CV_PI/2-atan((y-0)/  (x-0)))*180/CV_PI;
                        
#ifdef _PRINT_LOG
                        qDebug()<<"tetha1 = "<<tetha1<<endl;
                        qDebug()<<"tetha3 = "<<tetha2<<endl;
#endif
                       // msg.angle=(int)tetha1-tetha2;
		        msg.angle=side1_y-side2_y; // added by kazemipour
                        scale=last_scale;
                        
                        
                    }
                    else
                    {
                        mean_window_center=two_windows_center[1].x;
			window_center_y=two_windows_center[1].y;
                        float x=two_windows_center[1].x;
                        float y=two_windows_center[1].y;
                        if (show)
                        {
                            for (unsigned int w = 0; w < approxCurve[window_num[1]].size(); w++)
                            {
                                line(sub,approxCurve[window_num[1]][w%approxCurve[window_num[1]].size()],approxCurve[window_num[1]][(w+1)%approxCurve[window_num[1]].size()],Scalar(0,0,255),2,1);
                                
                            }
                            
                            
                            circle(sub,two_windows_center[1],6,Scalar(150,100,0),-1,8,0);
                            
                        }
                        float tetha1=(CV_PI/2+atan((y-0)/  (x-320)))*180/CV_PI;
                        float tetha2=(CV_PI/2-atan((y-0)/  (x-0)))*180/CV_PI;
                        
#ifdef _PRINT_LOG
                        qDebug()<<"tetha1 = "<<tetha1<<endl;
                        qDebug()<<"tetha2 = "<<tetha2<<endl;
#endif
                        // msg.angle=(int)tetha1-tetha2;
		        msg.angle=side1_y-side2_y; // added by kazemipour
                        scale=last_scale;
                    }
                    
                }
}
                if(edge_out_mode)
                {
                    window_center=two_windows_center[window_index];
                    mean_window_center=window_center.x;
		    window_center_y=window_center.y;
                    float x=window_center.x;
                    float y=window_center.y;
                    if (show)
                    {
                        
                        circle(sub,window_center,6,Scalar(150,100,0),-1,8,0);
                        
                    }
                    float tetha1=(CV_PI/2+atan((y-0)/  (x-320)))*180/CV_PI;
                    float tetha2=(CV_PI/2-atan((y-0)/  (x-0)))*180/CV_PI;
                    
#ifdef _PRINT_LOG
                    qDebug()<<"tetha1 = "<<tetha1<<endl;
                    qDebug()<<"tetha2 = "<<tetha2<<endl;
#endif
                    // msg.angle=(int)tetha1-tetha2;
		    msg.angle=side1_y-side2_y; // added by kazemipour
                    scale=last_scale;
                    
                }
                
                
                if(one_edge_mode || edge_out_mode)
                    break;
                
                else
                {
                    
                    
                    
                    
                    qDebug()<<"two windows is noise.set to default"<<endl;
                }
            }
                
                
                
                
            default:
            {
#ifdef _PRINT_LOG
                 qDebug()<<" default mode"<<endl;
#endif



                mean_window_center=0;
		window_center_y=0;
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
                           // line(sub,approxCurve[a][i],approxCurve[a][(i+1)%4],Scalar(50,200,0),2);
                        }

                        window_center+=approxCurve[a][i];

                    }

                    window_center.x=window_center.x/approxCurve[a].size();
                    window_center.y=window_center.y/approxCurve[a].size();

                    float x=window_center.x;
                    float y=window_center.y;
                    int up_right_window_point_y=0;
                    int down_right_window_point_y=0;
                    int up_left_window_point_y=0;
                    int down_left_window_point_y=0;




                    for(int i=0;i<approxCurve[a].size();i++)
                    {
                        if(window_center.x < approxCurve[a][i].x && window_center.y > approxCurve[a][i].y )
                        {
                            up_right_window_point_y=approxCurve[a][i].y;


                        }
                        if(window_center.x > approxCurve[a][i].x && window_center.y > approxCurve[a][i].y )
                        {
                            up_left_window_point_y=approxCurve[a][i].y;


                        }

                        if(window_center.x < approxCurve[a][i].x && window_center.y < approxCurve[a][i].y )
                        {
                            down_right_window_point_y=approxCurve[a][i].y;


                        }
                        if(window_center.x > approxCurve[a][i].x && window_center.y < approxCurve[a][i].y )
                        {
                            down_left_window_point_y=approxCurve[a][i].y;


                        }


                    }

                

                    int border_count=0;
                    if(up_left_window_point_y <=_BORDER && up_right_window_point_y <=_BORDER)
                    {

                        border_count++;


                    }

                    if(up_left_window_point_y <=_BORDER && down_left_window_point_y >=sub.rows -_BORDER)
                    {


                        border_count++;

                    }

                    if(down_left_window_point_y >=sub.rows -_BORDER && down_right_window_point_y >=sub.rows - _BORDER)
                    {

                        border_count++;

                    }

                    if(down_right_window_point_y >=sub.rows -_BORDER && up_right_window_point_y <=_BORDER)
                    {


                        border_count++;

                    }


                


                    if(border_count>=3)
                    {
                        arr_count=0;
                        msg.flag=6;


#ifdef _PRINT_LOG
                        qDebug()<<"invalid window in default mode"<<endl;
#endif


                    }
                    float tetha1=(CV_PI/2+atan((y-0)/  (x-320)))*180/CV_PI;
	            float tetha2=(CV_PI/2-atan((y-0)/  (x-0)))*180/CV_PI;

#ifdef _PRINT_LOG
                        qDebug()<<"tetha1 = "<<tetha1<<endl;
                        qDebug()<<"tetha2 = "<<tetha2<<endl;
#endif
                        // msg.angle=(int)tetha1-tetha2;
		        msg.angle=side1_y-side2_y; // added by kazemipour



                    if (show)
                    {
                        circle(sub,window_center,6,Scalar(150,100,0),-1,8,0);

                    }

                    mean_window_center+=window_center.x;
	            window_center_y=window_center.y;

if(arr_count!=0)
{

                    side1_y=approxCurve[a][1].y-approxCurve[a][0].y;
                    side2_y=approxCurve[a][2].y-approxCurve[a][1].y;
                    side1_x=approxCurve[a][1].x-approxCurve[a][0].x;
                    side2_x=approxCurve[a][2].x-approxCurve[a][1].x;

                    if (hit_border==0)
                    {
                        if (side2_y>side1_y)
                        {
                           
 				line(sub,approxCurve[a][0],approxCurve[a][1],Scalar(0,0,255),2);
                            scale=std::abs(side1_x);
                            last_scale=scale;
                        }
                        else
                        {
			    line(sub,approxCurve[a][1],approxCurve[a][2],Scalar(0,0,255),2);
                            scale=std::abs(side2_x);
                            last_scale=scale;

                        }
                    }
}
                }



                break;}
            }

            break;
        }
//#ifdef _PRINT_LOG
        qDebug() << "scale: " << scale;
        //        imshow("sub",sub);
        //         waitKey(2);

        //        if (scale<_FIRST_LEVEL_SCALE)
        //            dilation_size=_FIRST_LEVEL_DILATE;
        //        else
        //            dilation_size=3;

//#endif


         if (arr_count!=0)
        {
#ifdef _SERIAL_ENABLED
            // transmit->Fill_Data(4,int(mean_window_center-sub.cols/2),0,scale,255);
                        msg.setpoint=int(mean_window_center-sub.cols/2);
                        msg.setpointy=int(window_center_y-sub.rows/2);
                        msg.scale=scale;
                        msg.upperbound=255;
                        win_pub.publish(msg);
            //transmit->Fill_Data(3,1,2,3);
#ifdef _PRINT_LOG
            qDebug()<<"dx "<<mean_window_center<<endl;
	    qDebug()<<"dy "<<int(window_center_y-sub.rows/2)<<endl;
#endif


            last_points_detected[last_point_counter]=mean_window_center;
            last_point_counter++;
            if(last_point_counter==_LAST_POINTS_NUM)
            {

                float suggested_point_x1=0;


                float weight_sum=0;
                for(int i=0;i<_LAST_POINTS_NUM;i++)
                {
                    qDebug()<<"last point suggested"<<last_points_detected[i]<<endl;
                    suggested_point_x1+=(1-((_LAST_POINTS_NUM-i-1)*0.1))*(float)last_points_detected[i];
                    weight_sum+=(1-((_LAST_POINTS_NUM-i-1)*0.1));



                }
                qDebug()<<"weight sum"<<weight_sum<<endl;
                suggested_point_x=int(suggested_point_x1/weight_sum);
                qDebug()<<"suggested point"<<suggested_point_x<<endl;
                last_point_counter=0;
            }

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

//#ifdef _WIFI_ENABLED
 //       emit dataReady();
//#endif

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
   // createTrackbar( "Vmin", "Window Detection", &vmin
                 //   , 256, 0 );
   // createTrackbar( "Vmax", "Window Detection", &vmax, 256, 0 );
   // createTrackbar( "Smin", "Window Detection", &smin, 256, 0 );
#ifdef _SHOW_HIST
    namedWindow( "Histogram", 0 );
#endif

#ifdef _ERODE_DILATE_ON
  //  createTrackbar( "Erosion Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "Window Detection",
              //      &erosion_elem, 2);

   // createTrackbar( "Erosion Kernel size:\n 2n +1", "Window Detection",
                 //   &erosion_size, 21);

   // createTrackbar( "Dilation Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "Window Detection",
                  //  &dilation_elem, 2);

   // createTrackbar( "Dilation Kernel size:\n 2n +1", "Window Detection",
                //    &dilation_size, 21);
#endif
#endif
    show = true;
}


DetectWindow::~DetectWindow()
{

}

