#include "include/detection.h"

Mat Detection::frame1;
int Detection::px;
int Detection::py;

Detection::Detection(QObject *parent) : QObject(parent)
{
    dilationkernel_= 5;
    erosinkernel_= 3;
accuracy_=8;
    output_pub = marker_nh.advertise<marker_node::MarkerDetMsg>("Marker_output", 1);
    image_pub=marker_nh.advertise<sensor_msgs::Image>("Marker_image",1);
    image_binary_pub=marker_nh.advertise<sensor_msgs::Image>("Marker_binary_image",1);
    connect(this,SIGNAL(img_ready()),this,SLOT(main_function()));
    kernal=getStructuringElement(MORPH_CROSS,Size(dilationkernel_,dilationkernel_),Point( 1, 1 ));
    kernall=getStructuringElement(MORPH_RECT,Size(erosinkernel_,erosinkernel_),Point( 1, 1 ));
    invers=0;
    aa=2;
}

Detection::~Detection()
{

}

void Detection::imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
    try
    {

        frame1=cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8)->image;
        //waitKey(5);
        emit img_ready();
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
    }
}

void Detection::img_set(const marker_node::MarkerSetMsgConstPtr &msg1)
{
    if(msg1->data==0)
    {
        imgenable=msg1->mode;
        qDebug() << "msg1->mode";
    }
    else if(msg1->data==1)
    {
        if(msg1->swich==1)
        {
            exit(0);
            qDebug() << "exit";
        }
        else if(msg1->swich==0)
        {
            if(fork() == 0)
            {
                // Child process will return 0 from fork()
                printf("I'm the child process.\n");
                system("rosrun window_node window");
                exit(0);
            }
            else
            {
                // Parent process will return a non-zero value from fork()
                printf("I'm the parent.\n");
            }
            exit(0);
        }
    }
    else if (msg1->data==2)
    {
        dilationkernel_= msg1->dilate;
        qDebug() << dilationkernel_;
        kernal=getStructuringElement(MORPH_RECT,Size((dilationkernel_*2)+1,(dilationkernel_*2)+1),Point( 1, 1 ));
    }
    else if (msg1->data==3)
    {
        invers=msg1->invers;
        qDebug() << invers;
    }
else if (msg1->data==4)
    {
        accuracy_=msg1->accuracy;
        qDebug() << accuracy_;
    }
}

void Detection::main_function()
{
    if(!frame1.empty())
    {

#ifdef time_
        double t = (double)getTickCount();
#endif
        //*************
        cvtColor(frame1,gray,COLOR_BGR2GRAY);

        threshold(gray,binary, 0.0, 255.0, THRESH_BINARY+THRESH_OTSU);
        erode(binary,binary,kernall,Point(-1,-1),3,2,1);
        dilate(binary,binary,kernal,Point(-1,-1),3,2,1);
        line(binary,Point(0,0),Point(gray.cols,0),255,8,4);
        line(binary,Point(gray.cols,0),Point(gray.cols,gray.rows),255,8,4);
        line(binary,Point(0,gray.rows),Point(gray.cols,gray.rows),255,8,4);
        line(binary,Point(0,gray.rows),Point(0,0),255,8,4);
        binary.copyTo(binary1);
        findContours(binary1,contours,hierarchy,CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE);
        flag=0;
#ifdef checking_
        for (unsigned int k = 0; k < contours.size(); k++)
        {
            approxPolyDP(contours[k],shape1,approxpoly_,true);
            if(shape1.size() < sizemin_ch || shape1.size() > sizemax_ch || contourArea(contours[k]) < 400 || contourArea(contours[k]) > 630*470 )
            {
                contours[k].clear();
            }
        }
        area=0;
        coun=0;
        check=0;
        for(unsigned int l = 0; l < contours.size(); l++)
        {
            if(contours[l].size() != 0 && contourArea(contours[l]) < 630*470 )
            {

#endif
#ifndef checking_
                for (unsigned int l = 0; l < contours.size(); l++)
                {
                    approxPolyDP(contours[l],shape1,approxpoly_,true);
                    if(shape1.size() > sizemin_ && shape1.size() < sizemax_)
                    {
#endif

                        approxPolyDP(contours[l],shape1,approxpoly_,true);
                        recttt=minAreaRect(shape1);
                        recttt.points(rect_points);
                        tlx=recttt.center.x;
                        tly=recttt.center.y;
                        oy=ox=rect_points[0];

                        if(invers)
                        {
                            flag1=0;
                            for(int w=0;w<4;w++)
                            {
                                deltax1=abs(rect_points[0].x-rect_points[1].x);
                                deltax2=abs(rect_points[1].x-rect_points[2].x);
                                deltay1=abs(rect_points[0].y-rect_points[1].y);
                                deltay2=abs(rect_points[1].y-rect_points[2].y);

                                for(int w=0;w<4;w++)
                                {
                                    if(rect_points[w].x <= ox.x)
                                    {
                                        oy=ox;
                                        ox=rect_points[w];
                                    }
                                    else if(rect_points[w].x < oy.x)
                                    {
                                        oy=rect_points[w];
                                    }

                                }
                                if(ox.y > oy.y)
                                {
                                    result=ox;
                                }
                                else
                                {
                                    result=oy;
                                }
                                if(deltax1 > deltax2)
                                {
                                a1=deltax1;
                                }
                                else
                                {
                                    a1=deltax2;
                                }
                                if(deltay1 > deltay2)
                                {
                                a2=deltay1;
                                }
                                else
                                {
                                    a2=deltay2;
                                }
                            }

qDebug() << "inverse";
                        }
                        if(!invers)
                        {
                            deltax1=abs(rect_points[0].x-rect_points[1].x);
                            deltax2=abs(rect_points[1].x-rect_points[2].x);
                            deltay1=abs(rect_points[0].y-rect_points[1].y);
                            deltay2=abs(rect_points[1].y-rect_points[2].y);

                            for(int w=0;w<4;w++)
                            {
                                if(rect_points[w].x >= ox.x)
                                {
                                    oy=ox;
                                    ox=rect_points[w];
                                }
                                else if(rect_points[w].x > oy.x)
                                {
                                    oy=rect_points[w];
                                }

                            }
                            if(ox.y > oy.y)
                            {
                                result=ox;
                            }
                            else
                            {
                                result=oy;
                            }
                            if(deltax1 > deltax2)
                            {
                            a1=deltax1;
                            }
                            else
                            {
                                a1=deltax2;
                            }
                            if(deltay1 > deltay2)
                            {
                            a2=deltay1;
                            }
                            else
                            {
                                a2=deltay2;
                            }
                        }
//qDebug() << a1 << a2;
 if(binary.cols-2 > (int)(tlx+a1/4) && binary.rows-2 > (int)(tly+a2/4) && 2 < (int)(tlx-a1/4) && 2 < (int)(tly-a2/4))
                        {
                            Rect roi4((int)tlx-a1/4,(int)tly-a2/4,(int)a1/2,(int)a2/2);
                            Mat sub1(binary,roi4);
                            rectangle(frame1,roi4,Scalar(255,0,0),1, 8);
                            if(countNonZero(sub1)<(recttt.size.area()/4)*(10-accuracy_)/10)
                            {
                        r0=a1;
                        r1=(sqrt(a1*a1+a2*a2))/2;
                        d=sqrt((tlx - result.x)*(tlx - result.x) + (tly - result.y)*(tly - result.y));
                        if(d < r0+r1 && d > abs(r0-r1))
                        {

                            a = ((a1*a1) - ((a1*a1+a2*a2)/4) + (d*d))/(2*d);
                            h = sqrt((a1*a1)- (a*a));
                            px2=(((result.x - tlx)*a)/d)+tlx;
                            py2=(((result.y - tly)*a)/d)+tly;
                            x3 = (int)(px2 + h*(result.y - tly)/d);
                            y3 = (int)(py2 - h*(result.x - tlx)/d);
                           // circle(frame1,Point(x3,y3),5,Scalar(0,0,255));
//                            if(invers)
//                            {
//                                x3-=aa*dilationkernel_;
//                                y3+=aa*dilationkernel_;
//                            }
//                            if(!invers)
//                            {
//                                x3+=aa*dilationkernel_;
//                                y3-=aa*dilationkernel_;
//                            }
                            if(d < a2+r1 && d > abs(r1-a2))
                            {
                                a = ((a2*a2) - ((a1*a1+a2*a2)/4) + (d*d))/(2*d);
                                h = sqrt((a2*a2)- (a*a));
                                px2=(((result.x - tlx)*a)/d)+tlx;
                                py2=(((result.y - tly)*a)/d)+tly;
                                x4 =(int)(px2 - h*(result.y - tly)/d);
                                y4 = (int)(py2 + h*(result.x - tlx)/d);
//                                if(invers)
//                                {
//                                    x4-=aa*dilationkernel_;
//                                    y4-=aa*dilationkernel_;
//                                }
//                                if(!invers)
//                                {
//                                    x4+=aa*dilationkernel_;
//                                    y4+=aa*dilationkernel_;
//                                }
                                // circle(frame1,Point(x4,y4),5,Scalar(255,0,0));
                                if((int)(x3-a1/4) > 2 && (int)(y3-a2/4) > 2 && (int)(x4-a1/4) > 2 && (int)(y4-a2/4)> 2)
                                {
                                    if(frame1.cols-2 > (int)(x3+a1/4) && frame1.rows-2 > (int)(y3+a2/4) && frame1.cols-2 > (int)(x4+a1/4) && frame1.rows-2 > (int)(y4+a2/4))
                                    {
                                        // x=binary.at<uchar>(Point(tlx,tly));
                                        Rect roi1((int)(x3-a1/4),(int)(y3-a2/4),(int)a1/2,(int)a2/2);
                                        Mat sub1(binary,roi1);

                                        rectangle(frame1,roi1,Scalar(255,0,0),1, 8);

                                        if(countNonZero(sub1)>(recttt.size.area()/4)*accuracy_/10)
                                        {
                                            Rect roi2((int)(x4-a1/4),(int)(y4-a2/4),(int)a1/2,(int)a2/2);
                                            Mat sub1(binary,roi2);

                                            rectangle(frame1,roi2,Scalar(255,0,0),1, 8);

                                            if(countNonZero(sub1)>(recttt.size.area()/4)*accuracy_/10)
                                            {
                                                d=sqrt((x3 - x4)*(x3 - x4) + (y3 - y4)*(y3 - y4));
                                                if(d < a1+a2 && d > abs(a2-a1))
                                                {
                                                   // qDebug() << "d";
                                                    a = ((a2*a2) - (a1*a1) + (d*d))/(2*d);
                                                    h = sqrt((a2*a2)- (a*a));
                                                    px2=((x4 - x3)/d*a) + x3;
                                                    py2=((y4 - y3)/d*a) + y3;

                                                    if(invers)
                                                    {
                                                        if(y4<y3)
                                                        {
                                                            x5=(int)(px2 + h*(y4 - y3)/d - aa*dilationkernel_);
                                                            y5=(int)(py2 - h*(x4 - x3)/d + aa*dilationkernel_);
                                                        }
                                                        else
                                                        {
                                                            x5=(int)(px2 - h*(y4 - y3)/d - aa*dilationkernel_);
                                                            y5=(int)(py2 + h*(x4 - x3)/d + aa*dilationkernel_);
                                                        }
qDebug() << "inverse";
                                                    }
                                                    if(!invers)
                                                    {
                                                        if(y4>y3)
                                                        {
                                                            x5=(int)(px2 + (y4 - y3)/d*h + aa*dilationkernel_);
                                                            y5=(int)(py2 - (x4 - x3)/d*h + aa*dilationkernel_);
                                                        }
                                                        else
                                                        {
                                                            x5=(int)(px2 -(y4 - y3)/d*h + aa*dilationkernel_);
                                                            y5=(int)(py2 +(x4 - x3)/d*h + aa*dilationkernel_);
                                                        }

                                                    }
                                                   //  circle(frame1,Point(x5,y5),5,Scalar(0,0,255));
                                                    if(binary.cols-2 > (int)(x5+a1/4) && binary.rows-2 > (int)(y5+a2/4) && 2 < (int)(x5-a1/4) && 2 < (int)(y5-a2/4))
                                                    {
                                                        Rect roi3((int)x5-a1/4,(int)y5-a2/4,(int)a1/2,(int)a2/2);
                                                        Mat sub1(binary,roi3);


                                                        rectangle(frame1,roi3,Scalar(255,0,0),1, 8);
                                                        if(countNonZero(sub1)<(recttt.size.area()/4)*(10-accuracy_)/10)
                                                        {

                                                            for(unsigned int y=0;y<contours.size();y++)
                                                            {
                                                                if(contours[y].size()!= 0 && contourArea(contours[y])< 630*470)
                                                                {
                                                                    if(pointPolygonTest(contours[y],roi3.br(),false )>0)
                                                                    {
                                                                        coun++;
                                                                        qDebug() << recttt.size.area();

                                                                        if(area < recttt.size.area())
                                                                        {
                                                                            shape3=shape2;
                                                                            shape2=shape1;
                                                                            area=recttt.size.area();
                                                                        }
                                                                        else
                                                                        {
                                                                            shape3=shape1;
                                                                        }
                                                                    }
                                                                }
                                                            }

                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
}
}
                check=0;
                if(area != 0 )
                {
                    if(coun==2)
                    {

                        rrect1=minAreaRect(shape2);
                        rrect2=minAreaRect(shape3);

                        if(rrect1.center.x + center_ < rrect2.center.x || rrect1.center.x- center_ > rrect2.center.x)
                        {

                            if(rrect1.size.area()*2>rrect2.size.area() && rrect2.size.area()*2>rrect1.size.area())
                            {
                                result.x=(rrect1.center.x+rrect2.center.x)/2;
                                result.y=(rrect1.center.y+rrect2.center.y)/2;
                                circle(frame1,result,5,Scalar(0,255,0),5);
                                flag=1;
                                check=1;
                            }
                        }
                    }
                    if(check!=1)
                    {

                        recttt=minAreaRect(shape2);
                        recttt.points(rect_points);
                        oy=ox=rect_points[0];
                        if(invers)
                        {
                            for(int w=0;w<4;w++)
                            {
                                if(rect_points[w].x <= ox.x)
                                {
                                    oy=ox;
                                    ox=rect_points[w];
                                }
                                else if(rect_points[w].x < oy.x)
                                {
                                    oy=rect_points[w];
                                }
                            }
                            if(ox.y>oy.y)
                            {
                                result=ox;
                            }
                            else
                            {
                                result=oy;
                            }
qDebug() << "inverse";
                        }
                        if(!invers)
                        {
                            for(int w=0;w<4;w++)
                            {
                                if(rect_points[w].x >= ox.x)
                                {
                                    oy=ox;
                                    ox=rect_points[w];
                                }
                                else if(rect_points[w].x > oy.x)
                                {
                                    oy=rect_points[w];
                                }
                            }
                            if(ox.y>oy.y)
                            {
                                result=ox;
                            }
                            else
                            {
                                result=oy;
                            }
                        }
                        for (int j = 0; j < 4; j++)
                            line(frame1, rect_points[j], rect_points[(j + 1) % 4],Scalar(0,0,255),1, 8);
                        circle(frame1,result,5,Scalar(255,0,0),5);
                        flag=1;
                    }
                }
                else
               {
                    //qDebug() << "elseeeeee";

                    area=0;
                    coun=0;
                    check=0;
                    for(unsigned int l = 0; l < contours.size(); l++)
                    {
                        if(contours[l].size() != 0 && contourArea(contours[l]) < 630*470 )
                        {


#ifndef checking_
                            for (unsigned int l = 0; l < contours.size(); l++)
                            {
                                approxPolyDP(contours[l],shape1,approxpoly_,true);
                                if(shape1.size() > sizemin_ && shape1.size() < sizemax_)
                                {
#endif
                                    approxPolyDP(contours[l],shape1,approxpoly_,true);
                                      for ( unsigned int w = 0; w < shape1.size(); w++)
                                                                        {
                                                                            line(frame1,shape1[w%shape1.size()],shape1[(w+1)%shape1.size()],Scalar(255,0,0),2,4);
                                                                        }
                                    recttt=minAreaRect(shape1);
                                    recttt.points(rect_points);
                                    tlx=recttt.center.x;
                                    tly=recttt.center.y;
                                    oy=ox=rect_points[0];


                                    if(invers)
                                    {
                                        deltax1=abs(rect_points[0].x-rect_points[1].x);
                                        deltax2=abs(rect_points[1].x-rect_points[2].x);
                                        deltay1=abs(rect_points[0].y-rect_points[1].y);
                                        deltay2=abs(rect_points[1].y-rect_points[2].y);

                                        for(int w=0;w<4;w++)
                                        {
                                            if(rect_points[w].x >= ox.x)
                                            {
                                                oy=ox;
                                                ox=rect_points[w];
                                            }
                                            else if(rect_points[w].x > oy.x)
                                            {
                                                oy=rect_points[w];
                                            }

                                        }
                                        if(ox.y < oy.y)
                                        {
                                            result=ox;
                                        }
                                        else
                                        {
                                            result=oy;
                                        }
                                        if(deltax1 > deltax2)
                                        {
                                        a1=deltax1;
                                        }
                                        else
                                        {
                                            a1=deltax2;
                                        }
                                        if(deltay1 > deltay2)
                                        {
                                        a2=deltay1;
                                        }
                                        else
                                        {
                                            a2=deltay2;
                                        }
qDebug() << "inverse";
                                    }
                                    if(!invers)
                                    {
                                        deltax1=abs(rect_points[0].x-rect_points[1].x);
                                        deltax2=abs(rect_points[1].x-rect_points[2].x);
                                        deltay1=abs(rect_points[0].y-rect_points[1].y);
                                        deltay2=abs(rect_points[1].y-rect_points[2].y);


                                        for(int w=0;w<4;w++)
                                        {
                                            if(rect_points[w].x <= ox.x)
                                            {

                                                oy=ox;
                                                ox=rect_points[w];
                                            }
                                            else if(rect_points[w].x < oy.x)
                                            {

                                                oy=rect_points[w];
                                            }

                                        }
                                        if(ox.y < oy.y)
                                        {
                                           result=ox;
                                        }
                                        else
                                        {
                                            result=oy;
                                        }

                                        if(deltax1>deltax2)
                                        {
                                        a1=deltax1;
                                        }
                                        else
                                        {
                                            a1=deltax2;
                                        }
                                        if(deltay1>deltay2)
                                        {
                                        a2=deltay1;
                                        }
                                        else
                                        {
                                            a2=deltay2;
                                        }
                                    }
                                    // circle(frame1,oz1,5,Scalar(0,0,255),5);
//                                      circle(frame1,oy,5,Scalar(0,255,0),5);
//                                        circle(frame1,ox,5,Scalar(255,0,0),5);
 if(binary.cols-2 > (int)(tlx+a1/4) && binary.rows-2 > (int)(tly+a2/4) && 2 < (int)(tlx-a1/4) && 2 < (int)(tly-a2/4))
                        {
                            Rect roi4((int)tlx-a1/4,(int)tly-a2/4,(int)a1/2,(int)a2/2);
                            Mat sub1(binary,roi4);
                            rectangle(frame1,roi4,Scalar(0,255,0),1, 8);
                            if(countNonZero(sub1)<(recttt.size.area()/4)*(10-accuracy_)/10)
                            {
                                    r0=(sqrt(a1*a1+a2*a2))/2;
                                    r1=a2;
                                    d=sqrt((tlx - result.x)*(tlx - result.x) + (tly - result.y)*(tly - result.y));
                                    if(d < r0+r1 && d > abs(r0-r1))
                                    {
                                        a = ((a2*a2) - ((a1*a1+a2*a2)/4) + (d*d))/(2*d);
                                        h = sqrt((a2*a2)- (a*a));
                                        px2=(((result.x - tlx)*a)/d)+tlx;
                                        py2=(((result.y - tly)*a)/d)+tly;
                                        x3 = (int)(px2 - (result.y - tly)/d*h);
                                        y3 = (int)(py2 + (result.x - tlx)/d*h);
//                                        if(invers)
//                                        {
//                                            x3+=aa*dilationkernel_;
//                                            y3+=aa*dilationkernel_;
//                                        }
//                                        if(!invers)
//                                        {
//                                            x3+=aa*dilationkernel_;
//                                            y3-=aa*dilationkernel_;
//                                        }
                                        // circle(frame1,Point(x3,y3),5,Scalar(255,0,0));
                                        if(d < a1+r0 && d > abs(r0-a1))
                                        {
                                            a = ((a1*a1) - ((a1*a1+a2*a2)/4) + (d*d))/(2*d);
                                            h = sqrt((a1*a1)- (a*a));
                                            px2=(((result.x - tlx)*a)/d)+tlx;
                                            py2=(((result.y - tly)*a)/d)+tly;
                                            x4 =(int)(px2 + h*(result.y - tly)/d);
                                            y4 = (int)(py2 - h*(result.x - tlx)/d);
//                                            if(invers)
//                                            {
//                                                x4+=aa*dilationkernel_;
//                                                y4-=aa*dilationkernel_;
//                                            }
//                                            if(!invers)
//                                            {
//                                                x4-=aa*dilationkernel_;
//                                                y4+=aa*dilationkernel_;
//                                            }

                                           // circle(frame1,Point(x4,y4),5,Scalar(255,0,0));
                                            if((int)(x4-a1/4) > 2 && (int)(y4-a2/4) > 2 && (int)(x3-a1/4) > 2 && (int)(y3-a2/4)> 2)
                                            {
                                                if(frame1.cols-2 > (int)(x3+a1/4) && frame1.rows-2 > (int)(y3+a2/4) && frame1.cols-2 > (int)(x4+a1/4) && frame1.rows-2 > (int)(y4+a2/4))
                                                {
                                                    //x=binary.at<uchar>(Point(tlx,tly));
                                                    Rect roi1((int)(x3-a1/4),(int)(y3-a2/4),(int)a1/2,(int)a2/2);
                                                    Mat sub1(binary,roi1);

                                                    rectangle(frame1,roi1,Scalar(0,255,0),1, 8);

                                                    if(countNonZero(sub1)>(recttt.size.area()/4)*accuracy_/10)
                                                    {
                                                        Rect roi2((int)(x4-a1/4),(int)(y4-a2/4),(int)a1/2,(int)a2/2);
                                                        Mat sub1(binary,roi2);

                                                        rectangle(frame1,roi2,Scalar(0,255,0),1, 8);

                                                        if(countNonZero(sub1)>(recttt.size.area()/4)*accuracy_/10)
                                                        {

                                                           // qDebug() << "22222222222222222";
                                                            d=sqrt((x3 - x4)*(x3 - x4) + (y3 - y4)*(y3 - y4));
                                                            if(d < a1+a2 && d > abs(a2-a1))
                                                            {
                                                              //  qDebug() << "3333333333333";
                                                                a = ((a2*a2) - (a1*a1) + (d*d))/(2*d);
                                                                h = sqrt((a2*a2)- (a*a));
                                                                px2=((x4 - x3)/d*a) + x3;
                                                                py2=((y4 - y3)/d*a) + y3;

                                                                if(invers)
                                                                {
                                                                    if(y4<y3)
                                                                    {
                                                                        x5=(int)(px2 - h*(y4 - y3)/d + aa*dilationkernel_);
                                                                        y5=(int)(py2 + h*(x4 - x3)/d - aa*dilationkernel_);
                                                                    }
                                                                    else
                                                                    {
                                                                        x5=(int)(px2 + h*(y4 - y3)/d + aa*dilationkernel_);
                                                                        y5=(int)(py2 - h*(x4 - x3)/d - aa*dilationkernel_);
                                                                    }
qDebug() << "invers";
                                                                }
                                                                if(!invers)
                                                                {

                                                                    if(y4 < y3)
                                                                    {
                                                                        x5=(int)(px2 + (y4 - y3)/d*h - aa*dilationkernel_);
                                                                        y5=(int)(py2 - (x4 - x3)/d*h - aa*dilationkernel_);
                                                                    }
                                                                    else
                                                                    {
                                                                        x5=(int)(px2 -(y4 - y3)/d*h - aa*dilationkernel_);
                                                                        y5=(int)(py2 +(x4 - x3)/d*h - aa*dilationkernel_);
                                                                    }
                                                                }
                                                                //circle(frame1,Point(x5,y5),5,Scalar(0,255,0),5);
                                                                    if(binary.cols-2 > (int)(x5+a1/4) && binary.rows-2 > (int)(y5+a2/4) && 2 < (int)(x5-a1/4) && 2 < (int)(y5-a2/4))
                                                                    {

                                                                        Rect roi3((int)x5-a1/4,(int)y5-a2/4,(int)a1/2,(int)a2/2);
                                                                        Mat sub1(binary,roi3);

                                                                        rectangle(frame1,roi3,Scalar(0,255,0),1, 8);

                                                                        if(countNonZero(sub1)<(recttt.size.area()/4)*(10-accuracy_)/10)
                                                                        {

                                                                            for(unsigned int y=0;y<contours.size();y++)
                                                                            {
                                                                                if(contours[y].size()!= 0 && contourArea(contours[y])< 630*470)
                                                                                {
                                                                                    if(pointPolygonTest(contours[y],roi3.tl(),false )>0)
                                                                                    {


                                                                                        coun++;

                                                                                        if(area < recttt.size.area())
                                                                                        {
                                                                                            shape3=shape2;
                                                                                            shape2=shape1;
                                                                                            area=recttt.size.area();
                                                                                        }
                                                                                        else
                                                                                        {
                                                                                            shape3=shape1;
                                                                                        }


                                                                                    }
                                                                                }
                                                                            }

                                                                        }

                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
}
}

                            check=0;
                            if(area != 0 )
                            {
                                if(coun==2)
                                {

                                    rrect1=minAreaRect(shape2);
                                    rrect2=minAreaRect(shape3);

                                    if(rrect1.center.x + center_ < rrect2.center.x || rrect1.center.x - center_ > rrect2.center.x)
                                    {

                                        if(rrect1.size.area()*2>rrect2.size.area() && rrect2.size.area()*2>rrect1.size.area())
                                        {

                                            result.x=(rrect1.center.x+rrect2.center.x)/2;
                                            result.y=(rrect1.center.y+rrect2.center.y)/2;
                                            circle(frame1,result,5,Scalar(0,255,0),5);
                                            flag=1;
                                            check=1;
                                        }
                                    }
                                }
                                if(check!=1)
                                {

                                    recttt=minAreaRect(shape2);
                                    recttt.points(rect_points);
                                    oy=ox=rect_points[0];



                                    if(invers)
                                    {
                                        for(int w=0;w<4;w++)
                                        {
                                            if(rect_points[w].x >= ox.x)
                                            {
                                                oy=ox;
                                                ox=rect_points[w];
                                            }
                                            else if(rect_points[w].x > oy.x)
                                            {
                                                oy=rect_points[w];
                                            }
qDebug() << "inverse";
                                        }
                                        if(ox.y<oy.y)
                                        {
                                            result=ox;
                                        }
                                        else
                                        {
                                            result=oy;
                                        }
                                    }
                                    if(!invers)
                                    {
                                        for(int w=0;w<4;w++)
                                        {
                                            if(rect_points[w].x  <= ox.x)
                                            {
                                                oy=ox;
                                                ox=rect_points[w];
                                            }
                                            else if(rect_points[w].x < oy.x)
                                            {
                                                oy=rect_points[w];
                                            }
                                        }
                                        if(ox.y < oy.y)
                                        {
                                            result=ox;
                                        }
                                        else
                                        {
                                            result=oy;
                                        }
                                    }

                                    for (int j = 0; j < 4; j++)
                                        line(frame1, rect_points[j], rect_points[(j + 1) % 4],Scalar(0,0,255),1, 8);
//                                    circle(frame1,ox,7,Scalar(0,255,0),5);
//                                    circle(frame1,oy,9,Scalar(255,0,0),5);
                                    circle(frame1,result,5,Scalar(0,0,255),5);

                                    flag=1;
                                }
                            }

}
                        if(flag==1)
                        {
                            output.delta_x=(result.x-gray.cols/2);
                            output.delta_y=(result.y-gray.rows/2);
                            output.status=255;
                            output_pub.publish(output);
                            //qDebug() << "delta_x: " << output.delta_x<< "delta_y: " << output.delta_y << "status"<<output.status;
                        }
                        else
                        {
                            output.status=0;
                            output_pub.publish(output);
                            //qDebug() << "delta_x: " << output.delta_x<< "delta_y: " << output.delta_y << "status"<<output.status;
                        }
#ifdef show_
                        imshow("image11",frame1);
                        imshow("image",binary);
#endif
                        if(imgenable==1)
                        {
                            pyrDown(frame1,frame1);
                            imageMsg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame1).toImageMsg();

                            image_pub.publish(imageMsg);
                            pyrDown(binary,binary);
                            imageMsg = cv_bridge::CvImage(std_msgs::Header(), "mono8", binary).toImageMsg();
                            image_binary_pub.publish(imageMsg);
                            //qDebug() << "pub";
                        }
#ifdef time_
                        t = ((double)getTickCount() - t)*1000/getTickFrequency();
                        qDebug() << t;
#endif
                        //qDebug() << "marker";
                    }
                }
