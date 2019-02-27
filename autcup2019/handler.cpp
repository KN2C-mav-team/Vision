#include "handler.h"
#include <QThread>

int Y_H1;
int Y_S1;
int Y_V1;
int Y_H2;
int Y_S2;
int Y_V2;

int X_S1;
int X_V1;
int X_S2;
int X_V2;
int X_S11;
int X_V11;
int X_S21;
int X_V21;


int erosion_elem = 0;
int erosion_size = 0;
int dilation_elem = 0;
int dilation_size = 0;
int const max_elem = 2;
int const max_kernel_size = 21;

int dilation_type;
int erosion_type;
Mat image;

//sort ascending
struct myclassY {
    bool operator() (Point pt1, Point pt2) { return (pt1.y < pt2.y);}
} obj1,obj2;


Handler::Handler(QObject *parent) : QObject(parent)
{

    erosion_size=3;
    dilation_size=3;
    up_down =0;
    detector =new ColorDetection();
    uDetector = new UDetection();
    uDetector->setUp_down(1);
    //timer = new QTimer();
    makeTrackbar();
    connect(this,SIGNAL(_SIGNAL_reduceNoise()),this,SLOT(reduceNoise()));
    connect(this,SIGNAL(_SIGNAL_detectYellowRedFrame()),this,SLOT(detectYellowRedFrame()));


    //image=imread("/home/kimia/Desktop/FIRA_Pix/fira.jpg");
    //resize(image,image,Size(320,240));

    //connect(this,SIGNAL(_SIGNAL_Hist_And_Backproj(Mat)),this->detector,SLOT(_SLOT_Hist_And_Backproj(Mat)));


    //timer->start(2);
}

void Handler::makeTrackbar()
{

    Y_H1=12;//0
    Y_S1=65;//70
    Y_V1=87;//104
    Y_H2=179;
    Y_S2=255;
    Y_V2=255;

    X_S1 =100;
    X_V1 =65;
    X_S2 =255;
    X_V2 =255;
    X_S11=100;
    X_V11=100;
    X_S21=255;
    X_V21=255;

   /* namedWindow("Red Detection",WINDOW_NORMAL);

    namedWindow("Yellow Detection",WINDOW_NORMAL);

    namedWindow("Filter Demo",WINDOW_NORMAL);

    namedWindow("YELLOW RED",WINDOW_NORMAL);

    createTrackbar("H1 :","Yellow Detection",&Y_H1,179,on_trackbar);
    createTrackbar("S1 :","Yellow Detection",&Y_S1,255,on_trackbar);
    createTrackbar("V1 :","Yellow Detection",&Y_V1,255,on_trackbar);
    createTrackbar("H2 :","Yellow Detection",&Y_H2,179,on_trackbar);
    createTrackbar("S2 :","Yellow Detection",&Y_S2,255,on_trackbar);
    createTrackbar("V2 :","Yellow Detection",&Y_V2,255,on_trackbar);

    createTrackbar("S1  :","Red Detection",&X_S1 ,255,on_trackbar);
    createTrackbar("V1  :","Red Detection",&X_V1 ,255,on_trackbar);
    createTrackbar("S2  :","Red Detection",&X_S2 ,255,on_trackbar);
    createTrackbar("V2  :","Red Detection",&X_V2 ,255,on_trackbar);
    createTrackbar("S11 :","Red Detection",&X_S11,255,on_trackbar);
    createTrackbar("V11 :","Red Detection",&X_V11,255,on_trackbar);
    createTrackbar("S21 :","Red Detection",&X_S21,255,on_trackbar);
    createTrackbar("V21 :","Red Detection",&X_V21,255,on_trackbar);

    /// Create Erosion Trackbar
    createTrackbar( "Erosion Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "Filter Demo",
                    &erosion_elem, max_elem,
                    Erosion );

    createTrackbar( "Erosion Kernel size:\n 2n +1", "Filter Demo",
                    &erosion_size, max_kernel_size,
                    Erosion );

    /// Create Dilation Trackbar
    createTrackbar( "Dilation Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "Filter Demo",
                    &dilation_elem, max_elem,
                    Dilation );

    createTrackbar( "Dilation Kernel size:\n 2n +1", "Filter Demo",
                    &dilation_size, max_kernel_size,
                    Dilation );
*/
    /// Default start
    Erosion( 0, 0 );
    Dilation( 0, 0 );
}


void Handler::on_trackbar(int value, void *)
{
    //DetectionColor.detect  color by hsv scalar

}

void Handler::imageCallback(cv::Mat frame)
{

    uDetector->setCroped(false);
    bool red_found=false;
    bool yellow_found=false;
    int a=clock();
    //qDebug("frame recieved");
    if(frame.empty())
        qDebug("EMPPPTY");
    raw_frame = frame;

    //raw_frame=image;

    emit _SIGNAL_detectYellowRedFrame();
    //emit red


    emit _SIGNAL_reduceNoise();
    //emit redframe

    int a1=clock();

    ColorDetection::Colors tag=ColorDetection::YELLOW;


    if(!red.empty() || !yellow.empty() ){
        tag = findFirstGate(raw_frame);
        if( tag ==ColorDetection::RED){
            qDebug("RED");
            uDetector->setColor_tag(ColorDetection::RED);
            red_found = uDetector->detectU_shapes(red);

        }
        else if( tag ==ColorDetection::YELLOW){
            qDebug("YELLOW");
            uDetector->setColor_tag(ColorDetection::YELLOW);
            yellow_found =uDetector->detectU_shapes(yellow);
        }

        if(tag ==ColorDetection::RED && !red_found ){
            tag = ColorDetection::YELLOW;
            qDebug("YELLOW");
            uDetector->setColor_tag(ColorDetection::YELLOW);
            yellow_found =uDetector->detectU_shapes(yellow);
        }
        else if(tag ==ColorDetection::YELLOW && !yellow_found ){
            tag = ColorDetection::RED;
            qDebug("RED");
            uDetector->setColor_tag(ColorDetection::RED);
            yellow_found =uDetector->detectU_shapes(red);

        }



        if(red_found){
            up_down = 1;
            uDetector->setUp_down(1);
            emit gateCallBack(1);
            qDebug("-----------  RED GATE FOUND :)  --------------");
        }
        else if(yellow_found){
            up_down = 0;
            uDetector->setUp_down(0);
            emit gateCallBack(-1);
            qDebug("-----------  YELLOW GATE FOUND :)  --------------");
        }
        else{
            emit gateCallBack(0);
            qDebug("-----------  GATE NOT FOUND :(  --------------");
        }

    }
    else{
        emit gateCallBack(1);
        qDebug("-----------  GATE NOT FOUND :(  --------------");
    }
    int b=clock();

    // qDebug()<<"DELAY"<<(b-a1)/double(CLOCKS_PER_SEC)*1000 ;
    qDebug()<<"TOTAL DELAY : "<<((b-a)/double(CLOCKS_PER_SEC))*1000  ;
    //imshow("LIVE",raw_frame);
}


ColorDetection::Colors Handler::findFirstGate(Mat frame)
{
    Rect ROI;
    vector<Point> Y_white_points;
    vector<Point> R_white_points;


    ROI.x = 0;
    ROI.width = frame.cols;
    ROI.height = frame.rows/2;


   /* qDebug()<<"Y rows = "<<yellow.rows;
    qDebug()<<"R rows = "<<red.rows;

    qDebug()<<"Y cols = "<<yellow.cols;
    qDebug()<<"R cols = "<<red.cols;
    qDebug()<<"frame rows = "<<frame.rows;
   */
    //if up = red gate = keep down


    //else if down = yellow gate = keep up


  /*  qDebug()<<"ROI X = "<<ROI.x;
    qDebug()<<"ROI Y = "<<ROI.y
  */          ;
    if(!red.empty() && !yellow.empty()){
        qDebug()<<"Both NOT Empty";

        int y_count= countNonZero(yellow);
        int r_count= countNonZero(red);
        if(y_count >0)
            findNonZero(yellow,Y_white_points);
        if(r_count >0)
            findNonZero(red,R_white_points);

        if(up_down ==1){

            if(R_white_points.size() > Y_white_points.size())
                return ColorDetection::RED;
            else
                return chooseColor(ROI,frame.rows/2);
        }
        else{

            if(R_white_points.size() < Y_white_points.size())
                return ColorDetection::YELLOW;
            else
                return chooseColor(ROI,0);

        }
    }
    else if(!yellow.empty())

        return ColorDetection::YELLOW;

    else if(!red.empty())
        return ColorDetection::RED;

    return ColorDetection::WHITE;
}

ColorDetection::Colors Handler::chooseColor(Rect ROI,int y)
{


    qDebug()<<"In Choose Color";
    vector<Point> Y_white_points;
    vector<Point> R_white_points;

    ROI.y = y;
    Mat Y_croped_frame =yellow(ROI);
    Mat R_croped_frame =red(ROI);
    int y_count= countNonZero(Y_croped_frame);
    int r_count= countNonZero(R_croped_frame);
    qDebug()<<"Y count "<<y_count;
    qDebug()<<"R count "<<r_count;
    if(y_count >0)
        findNonZero(Y_croped_frame,Y_white_points);
    if(r_count >0)
        findNonZero(R_croped_frame,R_white_points);
    if(R_white_points.size() < Y_white_points.size()){
        yellow = Y_croped_frame;
        uDetector->setCroped(true);
        //imshow("Rect",Y_croped_frame);
        return ColorDetection::YELLOW;
    }
    else{
        red = R_croped_frame;
        uDetector->setCroped(true);
        //imshow("Rect",R_croped_frame);
        return ColorDetection::RED;
    }
}

ColorDetection::Colors Handler::compareContoursColor(vector<Point> red_c, vector<Point> yellow_c)
{
    //wait to go up or down 2 second


    vector<Point> p_red,p_yellow;
    approxPolyDP(Mat(red_c),p_red,8,true);
    sort(p_red.begin(),p_red.end(),obj1);
    approxPolyDP(Mat(yellow_c),p_yellow,8,true);
    sort(p_yellow.begin(),p_yellow.end(),obj2);

    if(up_down ==1){
        if(contourArea(red_c) < contourArea(yellow_c)){
            if(p_yellow[0].y < p_red[0].y)
                return ColorDetection::RED;
            else
                return ColorDetection::YELLOW;
        }
        else
            return ColorDetection::RED;

    }
    else{
        if(contourArea(yellow_c) < contourArea(red_c) ){
            if(p_yellow[0].y < p_red[0].y)
                return ColorDetection::YELLOW;
            else
                return ColorDetection::RED;
        }
        else
            return ColorDetection::YELLOW;//red

    }



}

void Handler::detectYellowRedFrame()
{
    int a=clock();


    yellow = detector->detectColor(raw_frame,Scalar(Y_H1,Y_S1,Y_V1),Scalar(Y_H2,Y_S2,Y_V2));
    red    = detector->detectRed(raw_frame,
                                 Scalar(0,X_S1,X_V1),
                                 Scalar(10,X_S2,X_V2),
                                 Scalar(160,X_S11,X_V11),
                                 Scalar(179,X_S21,X_V21));



    //imshow("RED ",red);
    //imshow("YELLOW ",yellow);
    int b1=clock();


    //CHECK NULL

    Mat red_backproj =detector->backprojectionResult(raw_frame,ColorDetection::RED);

    Mat yellow_backproj =detector->backprojectionResult(raw_frame,ColorDetection::YELLOW);

    red &= red_backproj; //and backprojection with mask
    yellow &=yellow_backproj;

    //imshow("YELLOW",yellow);

    //imshow("RED",red);


    int b=clock();

    //qDebug()<<"COLOR DELAY"<<(b-a)/double(CLOCKS_PER_SEC)*1000 ;

}


void Handler::reduceNoise()
{

    if(!yellow.empty()){
        Mat element = getStructuringElement( erosion_type,
                                             Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                             Point( erosion_size, erosion_size ) );

        /// Apply the erosion operation
        erode( yellow, yellow, element );//////////we have to merge red and yellow and reduce both
        //imshow( "Filter Demo", yellow );
        ///
        element = getStructuringElement( dilation_type,
                                         Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                         Point( dilation_size, dilation_size ) );
        /// Apply the dilation operation
        dilate( yellow, yellow, element );
        //imshow( "Filter Demo", yellow );
    }
    if(!red.empty()){
        Mat element = getStructuringElement( erosion_type,
                                             Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                             Point( erosion_size, erosion_size ) );

        /// Apply the erosion operation
        erode( red, red, element );//////////we have to merge red and yellow and reduce both
        //imshow( "Filter Demo RED", red );
        ///
        element = getStructuringElement( dilation_type,
                                         Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                         Point( dilation_size, dilation_size ) );
        /// Apply the dilation operation
        dilate( red, red, element );
        //imshow( "Filter Demo RED", red );
    }
}


void Handler::Erosion( int, void* )
{
    if( erosion_elem == 0 ){ erosion_type = MORPH_RECT; }
    else if( erosion_elem == 1 ){ erosion_type = MORPH_CROSS; }
    else if( erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }

}

/** @function Dilation */
void Handler::Dilation( int, void* )
{
    if( dilation_elem == 0 ){ dilation_type = MORPH_RECT; }
    else if( dilation_elem == 1 ){ dilation_type = MORPH_CROSS; }
    else if( dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }


}

void Handler::wait(int seconds)
{
    clock_t endwait;
    endwait = clock () + seconds * CLOCKS_PER_SEC ;
    while (clock() < endwait) {}

}
