#include "Headers/ProgramHandeler.h"



ProgramHandeler::ProgramHandeler()
    :it_(nh_){
    namedWindow(WINDOW,WINDOW_AUTOSIZE);
    publisher = nh_.advertise<line_connector2::my_msg>("line_connector2/datas",1000);
    //intial this
    quad_current_direction = QUAD_STARTING_DIRECTION;
    lock_qr = false;
    detected_qr = false;
    lock_points = false;
    optical_flow_lock = false;
    rotation_initiated = false;
    epsilon = 5;
    erode_factor = STARTING_ERODE;
    dilate_factor = STARTING_DILATE;
    max_L = MAX_SCALAR;min_L = 90;
    max_H = MAX_SCALAR;min_H = 0;
    max_S = MAX_SCALAR;min_S = 0;
    black_max_L = MAX_SCALAR;black_min_L = 172;
    black_max_H = MAX_SCALAR;black_min_H = 0;
    black_max_S = MAX_SCALAR;black_min_S = 0;
    createTrackbar("erode",WINDOW,&erode_factor,MAX_ERODE);
    createTrackbar("dilate",WINDOW,&dilate_factor,MAX_DILATE);
    createTrackbar("max_L",WINDOW,&max_L,MAX_SCALAR);
    createTrackbar("min_L",WINDOW,&min_L,MAX_SCALAR);
    //    createTrackbar("black_max_L",WINDOW,&black_max_L,MAX_SCALAR);
    //    createTrackbar("black_min_L",WINDOW,&black_min_L,MAX_SCALAR);
    createTrackbar("epsilon",WINDOW,&epsilon,MAX_EPSILON);
    //    createTrackbar("min_H",WINDOW,&min_H,MAX_SCALAR);
    //    createTrackbar("min_S",WINDOW,&min_S,MAX_SCALAR);
    //    createTrackbar("max_H",WINDOW,&max_H,MAX_SCALAR);
    //    createTrackbar("max_S",WINDOW,&max_S,MAX_SCALAR);
}

ProgramHandeler::~ProgramHandeler(){
    destroyAllWindows();
}

void ProgramHandeler::Callback(const line_connector2::qr_msg::ConstPtr& msg){
    if(!lock_qr){
        if(msg->qr_nums != 0){
            detected_qr = true;
            qr_data = msg -> data;
            qr_data = qr_data.substr(0,1);
            qr_rect.clear();
            for(int i=0;i<msg->qr_points.size();i++){
                Point p;
                p.x = msg ->qr_points[i].x;
                p.y = msg ->qr_points[i].y;
                qr_rect.push_back(p);
            }
            LineEquations eq;
            qr_center = eq.findMeanPoint(qr_rect);
            // cout<<"connector node says : qr type = "<< msg -> type<<endl;
            //            cout<<"connector node says : qr data = "<< qr_data <<endl;
            //            cout<<"current direction : "<<quad_current_direction <<endl;
            //            cout<<"next direction : "<<findDirection()<<endl;

        } else {
            detected_qr = false;

        }
    } else {
        detected_qr = true;
        //qr locsation
        if(msg -> qr_nums != 0){
            qr_rect.clear();
            for(int i=0;i<msg->qr_points.size();i++){
                Point p;
                p.x = msg ->qr_points[i].x;
                p.y = msg ->qr_points[i].y;
                qr_rect.push_back(p);
            }
        }
    }
}

string ProgramHandeler::findDirection(){
    string output = "not_intialaized";
    if(quad_current_direction == NORTH){

        if(qr_data == EAST){
            output = "right";
            return output;
        } else if(qr_data == NORTH){
            output = "forward";
            return output;
        } else if(qr_data == WEST){
            output = "left";
            return output;
        } else if(qr_data == SOUTH){
            output = "backward";
            return output;
        } else {
            output = "bullshit";
            return output;
        }

    } else if(quad_current_direction == EAST){
        if(qr_data == EAST){
            output = "forward";
            return output;
        } else if(qr_data == NORTH){
            output = "left";
            return output;
        } else if(qr_data == WEST){
            output = "backward";
            return output;
        } else if(qr_data == SOUTH){
            output = "right";
            return output;
        } else {
            output = "bullshit";
            return output;
        }

    } else if(quad_current_direction == WEST){
        if(qr_data == EAST){
            output = "backward";
            return output;
        } else if(qr_data == NORTH){
            output = "right";
            return output;
        } else if(qr_data == WEST){
            output = "forward";
            return output;
        } else if(qr_data == SOUTH){
            output = "left";
            return output;
        } else {
            output = "bullshit";
            return output;
        }

    } else if(quad_current_direction == SOUTH){
        if(qr_data == EAST){
            output = "left";
            return output;
        } else if(qr_data == NORTH){
            output = "backward";
            return output;
        } else if(qr_data == WEST){
            output = "right";
            return output;
        } else if(qr_data == SOUTH){
            output = "forward";
            return output;
        } else {
            output = "bullshit";
            return output;
        }
    }

    return output;
}

void ProgramHandeler::takeNewDirections(){
    quad_current_direction = qr_data;
}

void ProgramHandeler::imageCallBack(const sensor_msgs::ImageConstPtr& msg){
    time.start();
    try{
        cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);

    }catch (cv_bridge::Exception& e){
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }

    Mat raw_image = cv_ptr->image.clone();
    Mat white_threshold = raw_image.clone();
    Mat black_threshold = raw_image.clone();
    Mat contoursOfWhite,wholeContoursOfWhite;
    Mat final_approx;

    image_center.x = raw_image.cols/2;
    image_center.y = raw_image.rows/2;

    whiteThsOut(white_threshold);
    blackThsOut(black_threshold);

    //just for test
    Mat black_drawing = Mat::zeros(black_threshold.size(),CV_8UC3);

    contoursOfWhite = white_threshold.clone();
    wholeContoursOfWhite = findWhiteContours(contoursOfWhite,
                                             black_threshold,black_drawing);
    final_approx = contoursOfWhite.clone();
    makeOneLine(final_approx);
    //makeOneLine2(final_drawing);

    imshow(WINDOW,raw_image);
    //    imshow("contoursOfWhite",contoursOfWhite);
    //    imshow("black_contours",black_drawing);
    imshow("Final Approx",final_approx);
    imshow("white_threshold",white_threshold);
    imshow("wholeContersOfWhite",wholeContoursOfWhite);
    qDebug()<<"delay time : "<<time.elapsed();
    waitKey(3);
}

Point ProgramHandeler::handleFinalPoints(vector<Point> & input){

    Point p0 = input[0];
    Point p1 = input[1];
    LineEquations equator;
    double a_factor, b_factor, b_pole_factor, a_pole_factor;
    equator.find_line_equation(p0, p1, a_factor, b_factor);
    equator.find_pole_equation(image_center, a_factor, a_pole_factor, b_pole_factor);

    Point collusion = equator.find_twolines_collusion(a_factor, -(b_factor),
                                                      a_pole_factor, -(b_pole_factor));
    return collusion;

}

void ProgramHandeler::makeOneLine(Mat &oneLineInput){
    //TODO:clean this
    LineEquations eq;
    vector<Vec4i> hierarchy;
    vector<vector<Point>> contours,approx;
   // bool point_found = false;
    inRange(oneLineInput,Scalar(MAX_SCALAR - 5,MAX_SCALAR - 5,MAX_SCALAR - 5),
            Scalar(MAX_SCALAR,MAX_SCALAR,MAX_SCALAR),oneLineInput);
    findContours(oneLineInput,contours,hierarchy,CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE,Point(0,0));
    approx.resize(contours.size());
    //    if(contours.size() == 1){
    //        if(contours[0].size() < 20){
    //            point_found = true;//intentional
    //        }
    //    }
    for(int i=0; i<contours.size();i++){
        approxPolyDP(contours[i], approx[i], (double)epsilon, true);
    }
    for(int i=0;i<approx.size();i++){
        for(int j=0;j<approx[i].size();j++){
            circle(oneLineInput,approx[i][j],6,Scalar(MAX_SCALAR,0,0),CV_FILLED,8,0);
        }
    }
    circle(oneLineInput,image_center,5,Scalar(MAX_SCALAR,0,0),CV_FILLED,5,0);
    if(approx.size()>0){
        Point col;
        col = handleFinalPoints(approx[0]);
        //        if(!point_found){
        //            col = handleFinalPoints(approx[0]);
        //        } else {
        //            Point p = contours[0][0];
        //            col = p;
        //        }
        if(col.x<10000 && col.x > -10 &&
                col.y<10000 && col.y > -10){
            circle(oneLineInput,col,10,Scalar(MAX_SCALAR,MAX_SCALAR,MAX_SCALAR),1,8);
            line(oneLineInput,col,image_center,Scalar(MAX_SCALAR,MAX_SCALAR,MAX_SCALAR),1,8);
            if(col.x > image_center.x){
                dist = eq.point_length(image_center, col);
            } else {
                dist =-eq.point_length(image_center, col);
            }
            //     qDebug()<<"polar distance from center"<<dist<<"px";
            setAngel(approx[0][0],approx[0][1]);
            //     qDebug()<<"arc-tan of detected line"<< ang << "degrees";
            publish(dist,ang);
        }
    }
}

Mat ProgramHandeler::findWhiteContours(Mat &white_input , Mat &blkThs, Mat &black_drawing){
    //black
    vector<vector<Point>> blackContours;
    vector<Vec4i> blackHierarchy;
    findContours(blkThs,blackContours,blackHierarchy,
                 CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
    for(int i=0;i<blackContours.size();i++){
        drawContours(black_drawing,
                     blackContours,i,Scalar(100,100,0),1,8,blackHierarchy,0,Point());
    }

    //white
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    vector<Point> found_points;
    found_points.clear();
    Mat drawing = Mat::zeros(white_input.size(),CV_8UC3);
    Mat final_drawing = Mat::zeros(white_input.size(),CV_8UC3);
    findContours(white_input, contours, hierarchy,
                 CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0,0));
    for(int i=0;i<contours.size();i++){
        drawContours(drawing,contours,i,Scalar(100,0,100),1,8,hierarchy,0,Point());
    }

    choosePoints(contours, hierarchy, found_points, drawing);

    if(found_points.size() > 0){
        sort(found_points.begin(), found_points.end() , [] (Point a,Point b){
            return a.y > b.y;
        });
        for(int i=0;i<found_points.size();i++){
            putText(drawing,std::to_string(i),found_points[i],1,2,Scalar(MAX_SCALAR,MAX_SCALAR,0),1,8);
        }
        final_found_points.clear();
        handleRouting(drawing, final_drawing, found_points);
    }
    white_input = final_drawing.clone();
    return drawing;
}

void ProgramHandeler::choosePoints(vector<vector<Point>>contours ,vector<Vec4i> hierarchy,
                                   vector<Point>&found_points, Mat &drawing){
    LineEquations eq;
    vector<vector<Point>> approx;
    approx.resize(contours.size());
    for(int i=0;i < contours.size();i++){
        if(hierarchy[i][2]<0){
            if (contourArea(contours[i])<MAXIMUM_RECTANGLE_AREA
                    && contourArea(contours[i])>MINIMUM_RECTANGLE_AREA ){
                approxPolyDP(contours[i], approx[i], (double)epsilon, true);
                sort(approx[i].begin(), approx[i].end() , [] (Point a,Point b){
                    return a.y >= b.y;
                });

                if((approx[i].size() < 9 && checkRect(approx[i]))
                        /*&& isInsideBlackArea(approx[i] , black_drawing, blackContours,
                                                                                              blackHierarchy) */){
                    for(int j=0;j<4;j++){
                        circle(drawing,approx[i][j],4,Scalar(MAX_SCALAR,MAX_SCALAR,0),-1,3);
                    }
                    found_points.push_back(eq.findMeanPoint(approx[i]));
                }
            }
        }
    }
    for(int i=0;i < found_points.size();i++){
        circle(drawing,found_points[i],2,Scalar(MAX_SCALAR,MAX_SCALAR,MAX_SCALAR),CV_FILLED,8);
    }
}

bool ProgramHandeler::checkRect(vector<Point> input){
    sort(input.begin(),input.end() , [] (Point a,Point b){
        return a.y >= b.y;
    });
    if(input.size() > 8){
        return false;
    }
    LineEquations eq;
    double x1 = eq.point_length(input[0],input[2]);
    double y1 = eq.point_length(input[1],input[3]);
    double x2 = eq.point_length(input[0],input[1]);
    double y2 = eq.point_length(input[2],input[3]);

    if( (abs(x1 - y1) < 7 )&& ( abs(x2 - y2) < 7) ){
        return true;
    }

    return false;

}


bool ProgramHandeler::isInsideBlackArea(vector<Point> points, Mat &blackDr,
                                        vector<vector<Point>> &blackCn,
                                        vector<Vec4i> &blackHi){
    if(points.size() != 4){
        return false;
    }
    LineEquations eq;
    Point center = eq.findMeanPoint(points);

    for(int i=0;i<blackCn.size();i++){
        for(int i=0;i<4;i++){
            circle(blackDr,points[i],5,Scalar(MAX_SCALAR,0,0),-1,2);
        }
        if( pointPolygonTest(blackCn[i],center,false) == 1){
            drawContours(blackDr,
                         blackCn,i,Scalar(0,0,MAX_SCALAR),1,8,blackHi,0,Point());
            circle(blackDr,image_center,5,Scalar(0,0,100),-1,3);
            if( contourArea(blackCn[i]) < 12000 ){
                // qDebug()<<"unwanted contour with area"<<contourArea(blackCn[i]);
                return false;
            } else {
                //qDebug()<<"wanted contour with area"<<contourArea(blackCn[i]);
            }
        }
        if( pointPolygonTest(blackCn[i],image_center,false) == 1){
            drawContours(blackDr,
                         blackCn,i,Scalar(MAX_SCALAR,MAX_SCALAR,MAX_SCALAR),1,8,blackHi,0,Point());
            circle(blackDr,image_center,5,Scalar(0,100,100),-1,3);
            qDebug()<<"center contour area"<<contourArea(blackCn[i]);
        }
    }
    return true;
}

void ProgramHandeler::handleRouting(Mat &comp_drawing, Mat &fin_drawing, vector<Point> &points){

    if(!detected_qr){//straigh forward
        selectUpAndDownPoints(points);
        connectPoints(comp_drawing, points);
        final_found_points.push_back(points[0]);
        final_found_points.push_back(points.back());
        connectPoints(fin_drawing, final_found_points);
        return;
    } else {
        //find the current direction
        string qr_dir = findDirection();
        if(qr_dir == "right"){
            if(detectedLinesOf("right",points)){
                selectRightAndDownPoints(points);
                setAngel(points[0],points[1]);
                if((!reachedTargetPoint(rotationTarget))){
                    lock_qr = true;
                    circle(comp_drawing,Point(rotationTarget.x + 20, rotationTarget.y),10,Scalar(MAX_SCALAR,0,0),3,5);
                    circle(comp_drawing,Point(rotationBase.x - 65,rotationBase.y - 80),10,Scalar(0,MAX_SCALAR,0),3,5);
                } else {
                    //take the new direction
                    takeNewDirections();
                    rotation_initiated = false;
                    lock_qr = false;
                }
                connectPoints(comp_drawing, points);
                final_found_points.push_back(points[0]);
                final_found_points.push_back(points.back());
                connectPoints(fin_drawing, final_found_points);
            }

        }

        if(qr_dir == "left"){
            if(detectedLinesOf("left",points)){
                selectLeftAndDownPoints(points);
                setAngel(points[0],points[1]);
                if((!reachedTargetPoint(rotationTarget)) ){
                    lock_qr = true;
                    circle(comp_drawing,Point(rotationTarget.x - 20, rotationTarget.y),10,Scalar(MAX_SCALAR,0,0),3,5);
                    circle(comp_drawing,Point(rotationBase.x + 65,rotationBase.y - 80),10,Scalar(0,MAX_SCALAR,0),3,5);
                } else {
                    //take the new direction
                    takeNewDirections();
                    rotation_initiated = false;
                    lock_qr = false;
                }
                connectPoints(comp_drawing, points);
                final_found_points.push_back(points[0]);
                final_found_points.push_back(points.back());
                connectPoints(fin_drawing, final_found_points);
            }
        }

        if(qr_dir == "forward"){
            points[0] = qr_center;
            circle(comp_drawing,Point(points[0].x,points[0].y-30),10,Scalar(0,MAX_SCALAR,0),3,5);
            selectUpAndDownPoints(points);
            connectPoints(comp_drawing, points);
            final_found_points.push_back(Point(points[0].x,points[0].y-30));
            final_found_points.push_back(points.back());
            connectPoints(fin_drawing, final_found_points);
        }

    }
}

bool ProgramHandeler::detectedLinesOf(string dir, const vector<Point> points){
    if(dir == "left"){
        for(int i=0;i<points.size();i++){
            if(points[i].x < 140){
                return true;
            }
        }
        return false;
    } else if (dir == "right"){
        for(int i=0;i<points.size();i++){
            if(points[i].x > 190){
                return true;
            }
        }
        return false;


    }
}

bool ProgramHandeler::reachedTargetPoint(const Point target){

    LineEquations eq;
    if(eq.point_length(image_center,target) < 35){
        return true;
    }
    return false;

}

void ProgramHandeler::selectRightAndDownPoints(vector<Point> &points){
    if(!rotation_initiated){
        Point farRightPoint;
        farRightPoint = points[0];
        selectedRotationPoints.clear();
        selectedRotationPoints.push_back(Point(points[0].x - 60 , points[0].y - 60));
        for(int i=1;i<points.size();i++){
            if( points[i].x > farRightPoint .x)  {
                farRightPoint = points[i];
            }
        }
        selectedRotationPoints.push_back(Point(farRightPoint.x + 20 , farRightPoint.y));
        points = selectedRotationPoints;
        rotation_initiated = true;
        return;
    } else {
        LineEquations eq;
        vector<Point>finalSelected;
        finalSelected.clear();
        for(int i=0;i<points.size();i++){
            if(eq.point_length(selectedRotationPoints[1],Point(points[i].x + 20 ,points[i].y)) < 45){
                rotationTarget = points[i];
            }
            if(eq.point_length(selectedRotationPoints[0], Point(points[i].x - 60 ,points[i].y - 60)) < 45){
                rotationBase = points[i];
            }
        }
        selectedRotationPoints[1] = Point(rotationTarget.x + 20, rotationTarget.y);
        selectedRotationPoints[0] = Point(rotationBase.x - 60,rotationBase.y - 60);
        points.clear();
        points.push_back(Point(rotationTarget.x + 20, rotationTarget.y));
        points.push_back(Point(rotationBase.x - 60,rotationBase.y - 60));
    }
}

void ProgramHandeler::selectLeftAndDownPoints(vector<Point> &points){
    if(!rotation_initiated){
        Point farLeftPoint;
        farLeftPoint = points[0];
        selectedRotationPoints.clear();
        selectedRotationPoints.push_back(Point(points[0].x + 65 , points[0].y - 80));
        for(int i=1;i<points.size();i++){
            if( points[i].x < farLeftPoint .x)  {
                farLeftPoint = points[i];
            }
        }
        selectedRotationPoints.push_back(Point(farLeftPoint.x - 20 , farLeftPoint.y));
        points = selectedRotationPoints;
        rotation_initiated = true;
        return;
    } else {
        LineEquations eq;
        vector<Point> finalSelected;
        finalSelected.clear();
        for(int i=0;i<points.size();i++){
            if(eq.point_length(selectedRotationPoints[1],Point(points[i].x - 20 ,points[i].y)) < 50){
                rotationTarget = points[i];
            }
            if(eq.point_length(selectedRotationPoints[0], Point(points[i].x + 65 ,points[i].y - 80)) < 50){
                rotationBase = points[i];
            }
        }
        selectedRotationPoints[1] = Point(rotationTarget.x - 20, rotationTarget.y);
        selectedRotationPoints[0] = Point(rotationBase.x + 65, rotationBase.y - 80);
        points.clear();
        points.push_back(Point(rotationTarget.x - 20, rotationTarget.y));
        points.push_back(Point(rotationBase.x + 65,rotationBase.y - 80));
    }
}

void ProgramHandeler:: selectUpAndDownPoints(vector<Point> &points){
    vector<Point> selected;
    selected.clear();
    selected.push_back(points[0]);
    for(int i=1;i<points.size();i++){
        if( (points[i].x <= points[0].x + 50) && (points[i].x >= points[0].x - 50) ) {
            selected.push_back(points[i]);
        }
    }
    points = selected;
}

void ProgramHandeler::connectPoints(Mat& drawing,vector<Point> &points){
    for(int i=0; i < points.size() - 1; i++){
        line(drawing,points[i],points[i+1],Scalar(MAX_SCALAR,MAX_SCALAR,MAX_SCALAR),1,8);
    }
}

void ProgramHandeler::whiteThsOut(Mat &whtInput){
    handleBlurs(whtInput);
    cvtColor(whtInput,whtInput,CV_BGR2HLS);
    inRange(whtInput,Scalar(min_H,min_L,min_S),Scalar(max_H,max_L,max_S),whtInput);
}

void ProgramHandeler::blackThsOut(Mat &blkInput){
    Inverter inv;
    handleBlurs(blkInput);
    inv.invert_channels(&blkInput,&blkInput);
    cvtColor(blkInput,blkInput,CV_BGR2HLS);
    inRange(blkInput,Scalar(black_min_H,black_min_L,black_min_S),
            Scalar(black_max_H,black_max_L,black_max_S),blkInput);
}

void ProgramHandeler::handleBlurs(Mat &unfiltered){
    // medianBlur(unfiltered,unfiltered,5);
    erode(unfiltered,unfiltered,
          getStructuringElement(MORPH_ELLIPSE,Size( 2*erode_factor+1, 2*erode_factor+1 ),Point(-1,-1)));
    dilate(unfiltered,unfiltered,
           getStructuringElement(MORPH_ELLIPSE,Size( 2*dilate_factor+1, 2*dilate_factor+1 ),Point(-1,-1)));
}

void ProgramHandeler::publish(const double dist,const double ang){

    line_connector2::my_msg msg;
    msg.distance = dist;
    msg.angel = ang;
    publisher.publish(msg);

}

void ProgramHandeler::setAngel(Point p0,Point p1){

    double delta_y =-(p0.y - p1.y);
    double delta_x = (p0.x - p1.x);
    ang = qAtan( (delta_x) / (delta_y) ) * 180/M_PI;

}
