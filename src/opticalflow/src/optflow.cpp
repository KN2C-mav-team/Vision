#include <include/optflow.h>


OptFlow::OptFlow(QObject *parent) : QObject(parent)
{
    status_pub= opt_nh.advertise<std_msgs::Int8>("opt_status_output", 1);
    needToInit = true;
    termcrit = TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 20, 0.03);

    output_pub = opt_nh.advertise<opticalflow::OpticalMsg>("optical_output", 1);

    connect(this, SIGNAL(imageReadySig()),this, SLOT(getImage()));



}

void OptFlow::getImage()
{


    cvtColor(colorframe, grayframe, CV_BGR2GRAY);
    double exec_time = (double)getTickCount();
    OptFlow_LK();
    exec_time = ((double)getTickCount() - exec_time)*1000./getTickFrequency();
    // qDebug() << "time: " << exec_time;


}

void OptFlow::OptFlow_LK()
{

    if( needToInit )
    {
        goodFeaturesToTrack(grayframe, points[1], MAX_COUNT, 0.01, 10, Mat(), 3, 0, 0.04);
        if (!points[1].empty())
            cornerSubPix(grayframe, points[1], subPixWinSize, Size(-1,-1), termcrit);
    }
    else if( !points[0].empty() )
    {
        vector<uchar> status_opt;
        vector<float> err;
        if(prevGray.empty())
            grayframe.copyTo(prevGray);
        calcOpticalFlowPyrLK(prevGray, grayframe, points[0], points[1], status_opt, err, winSize,
                3, termcrit, 0, 0.001);


        //
        Features_counter =0;
        delta_x = 0;
        delta_y = 0;

        for( unsigned int i = 0 ; i < points[1].size(); i++ )
        {
            if( status_opt[i] )
            {
                points[1][Features_counter++] = points[1][i];
#ifdef _GRAPH
                line(colorframe, points[0][i], points[1][i], Scalar(0,255,0), 1, 8, 0);
#endif
                Point2f p1 = points[0][i], p2 = points[1][i];
                delta_x += (p1.x - p2.x);
                delta_y += (p1.y - p2.y);
            }
        }
        delta_x = delta_x/(double)Features_counter;
        delta_y = delta_y/(double)Features_counter;


        points[1].resize(Features_counter);



        //transmiter->Fill_Data(3,(int)(delta_x *100),(int)(delta_y *100),255);
        //transmiter->Fill_Data(3,1,2,3);
        output.delta_x=delta_x *100;
        output.delta_y=delta_y *100;
        if (points[1].size()!=0)
        {

            output.status=255;
        }

        else
        {
            output.status=0;
        }

        


        //qDebug() << delta_x;


    }

output_pub.publish(output);

    if(points[1].size() < (unsigned int)THRESH_REFRESH)
        needToInit = true;
    else
        needToInit = false;

#ifdef _GRAPH
    imshow("LK Demo", colorframe);

#endif

    //if (cv::waitKey(1)>0) exit(0);

    std::swap(points[1], points[0]);
    cv::swap(prevGray, grayframe);
    status.data=2;
    status_pub.publish(status);

}


OptFlow::~OptFlow()
{
    //    delete ui;
}

void OptFlow::imageCb_bgr(const sensor_msgs::ImageConstPtr &msg)
{
    try
    {
        colorframe = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8)->image;
        double exec_time2 = (double)getTickCount();
        resize(colorframe,colorframe,Size(160,120));
        exec_time2 = ((double)getTickCount() - exec_time2)*1000./getTickFrequency();
        // qDebug() << "resize time: " << exec_time2;
        emit imageReadySig();
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }
}
