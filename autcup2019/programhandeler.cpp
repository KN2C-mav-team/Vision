#include "programhandeler.h"

ProgramHandeler::ProgramHandeler(QObject *parent) : QObject(parent){

    //intial this
    quad_current_direction = QUAD_STARTING_DIRECTION;
    queued_mission = STARTING_QUEUED_MISSION;
    mission_counter = 0;
    lock_qr = false;
    detected_qr = false;
    lock_points = false;
    optical_flow_lock = false;
    rotation_initiated = false;
    straight_lock = false;
    mission = 0;
    qr_flag = -3;
    //next_mission = 0;
    onlyOnePointDetected = false;
    MAXIMUM_RECTANGLE_AREA = 2800;
    MINIMUM_RECTANGLE_AREA = 250;
    epsilon = 5;
    erode_factor = STARTING_ERODE;
    dilate_factor = STARTING_DILATE;
    max_L = MAX_SCALAR;min_L = 90;
    max_H = MAX_SCALAR;min_H = 0;
    max_S = MAX_SCALAR;min_S = 0;
    black_max_L = MAX_SCALAR;black_min_L = 172;
    black_max_H = MAX_SCALAR;black_min_H = 0;
    black_max_S = MAX_SCALAR;black_min_S = 0;
#ifdef DEBUG

    namedWindow(WINDOW,WINDOW_AUTOSIZE);
    createTrackbar("erode",WINDOW,&erode_factor,MAX_ERODE);
    createTrackbar("dilate",WINDOW,&dilate_factor,MAX_DILATE);
    createTrackbar("max_L",WINDOW,&max_L,MAX_SCALAR);
    createTrackbar("min_L",WINDOW,&min_L,MAX_SCALAR);
    createTrackbar("MINIMUM_AREA",WINDOW,&MINIMUM_RECTANGLE_AREA,1000);
    createTrackbar("MAXIMUM_AREA",WINDOW,&MAXIMUM_RECTANGLE_AREA,100000);
    //    createTrackbar("black_max_L",WINDOW,&black_max_L,MAX_SCALAR);
    //    createTrackbar("black_min_L",WINDOW,&black_min_L,MAX_SCALAR);
    createTrackbar("epsilon",WINDOW,&epsilon,MAX_EPSILON);
    //    createTrackbar("min_H",WINDOW,&min_H,MAX_SCALAR);
    //    createTrackbar("min_S",WINDOW,&min_S,MAX_SCALAR);
    //    createTrackbar("max_H",WINDOW,&max_H,MAX_SCALAR);
    //    createTrackbar("max_S",WINDOW,&max_S,MAX_SCALAR);
#endif

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

void ProgramHandeler::imageCallBack(cv::Mat raw_image){

    //    qDebug()<<"PH Thread"<<QThread::currentThreadId();


    time.start();

    image_center.x = raw_image.cols/2;
    image_center.y = raw_image.rows/2;

    Point2f raw_cent(raw_image.cols/2.0F, raw_image.rows/2.0F);
    Mat rot_mat = getRotationMatrix2D(raw_cent, rotation_angle , 1.0);
    warpAffine(raw_image, raw_image, rot_mat, raw_image.size());

    //    transpose(raw_image, raw_image);
    //    flip(raw_image, raw_image, +1);

    Mat white_threshold ;
    white_threshold = raw_image.clone();
    Mat contoursOfWhite,wholeContoursOfWhite;
    Mat final_approx;


    whiteThsOut(white_threshold);
    contoursOfWhite = white_threshold.clone();
    wholeContoursOfWhite = findWhiteContours(contoursOfWhite);
    final_approx = contoursOfWhite.clone();
    makeOneLine(final_approx);

#ifdef VIDEO_LOG
    VideoWriter video("/home/danial/bc/outcpp.avi",CV_FOURCC('M','J','P','G'),10,
                      Size(FRAME_WIDTH,FRAME_HEIGHT));
    video.write(wholeContoursOfWhite);
#endif

#ifdef DEBUG
    imshow(WINDOW,raw_image);
    imshow("Final Approx",final_approx);
    imshow("white_threshold",white_threshold);
    imshow("wholeContersOfWhite",wholeContoursOfWhite);
#endif
#ifdef DELAY
    qDebug()<<"delay time : "<<time.elapsed();
#endif

    // waitKey(3);

}

Mat ProgramHandeler::findWhiteContours(Mat &white_input){
    //white
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    vector<Point> found_points;
    found_points.clear();
#ifdef DEBUG
    Mat drawing = Mat::zeros(white_input.size(),CV_8UC3);
#endif
#ifndef DEBUG
    Mat drawing ;
#endif
    Mat final_drawing = Mat::zeros(white_input.size(),CV_8UC3);
    findContours(white_input, contours, hierarchy,
                 CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0,0));
#ifdef DEBUG
    for(int i=0;i<contours.size();i++){
        drawContours(drawing,contours,i,Scalar(100,0,100),1,8,hierarchy,0,Point());
    }
#endif
    choosePoints(contours, hierarchy, found_points, drawing, white_input);

    if(found_points.size() > 0){
        sortByY(found_points);
        //        for(int i=0;i<found_points.size();i++){
        //            putText(drawing,std::to_string(i),found_points[i],1,2,Scalar(MAX_SCALAR,MAX_SCALAR,0),1,8);
        //        }
        final_found_points.clear();
#ifndef TIME_ROUNTING
        handleRouting(drawing, final_drawing, found_points);
#endif
#ifdef TIME_ROUNTING
        secondHandleRouting(drawing, final_drawing, found_points);
#endif
    }
    white_input = final_drawing.clone();
    return drawing;
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
    vector<vector<Point> > contours,approx;
    inRange(oneLineInput,Scalar(MAX_SCALAR - 5,MAX_SCALAR - 5,MAX_SCALAR - 5),
            Scalar(MAX_SCALAR,MAX_SCALAR,MAX_SCALAR),oneLineInput);
    findContours(oneLineInput,contours,hierarchy,CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE,Point(0,0));
    approx.resize(contours.size());
    for(int i=0; i<contours.size();i++){
        approxPolyDP(contours[i], approx[i], (double)epsilon, true);
    }
#ifdef DEBUG
    for(int i=0;i<approx.size();i++){
        for(int j=0;j<approx[i].size();j++){
            circle(oneLineInput,approx[i][j],6,Scalar(MAX_SCALAR,0,0),CV_FILLED,8,0);
        }
    }
    circle(oneLineInput,image_center,5,Scalar(MAX_SCALAR,0,0),CV_FILLED,5,0);
#endif
    if(approx.size()>0){
        Point col;
        // col = handleFinalPoints(approx[0]);
        try{
            col = eq.findMeanPoint(final_found_points);
            if(col.x<10000 && col.x > -10 &&
                    col.y<10000 && col.y > -10){
#ifdef DEBUG
                circle(oneLineInput,col,10,Scalar(MAX_SCALAR,MAX_SCALAR,MAX_SCALAR),1,8);
                line(oneLineInput,col,image_center,Scalar(MAX_SCALAR,MAX_SCALAR,MAX_SCALAR),1,8);
#endif
                dist = -(image_center.x - col.x);
                //            if(col.x > image_center.x){
                //                dist = eq.point_length(image_center, col);
                //            } else {
                //                dist =-eq.point_length(image_center, col);
                //            }
                //setAngel(image_center,col);
                if(!onlyOnePointDetected){
                    setAngel(approx[0][0],approx[0][1]);
                } else {
                    ang = 0;
                }
#ifdef LOG
                qDebug()<<"polar distance from center"<<dist<<"px";
                qDebug()<<"arc-tan of detected line"<< ang << "degrees";
#endif
                publish(dist,ang);
            }
        } catch (Exception e){
            qDebug()<<"exception : "<<e.what();
        }
    }
}

void ProgramHandeler::choosePoints(vector<vector<Point> >contours ,vector<Vec4i> hierarchy,
                                   vector<Point>&found_points, Mat &drawing,Mat white_input){
    LineEquations eq;
    vector<vector<Point> > approx;
    approx.resize(contours.size());
    for(int i=0;i < contours.size();i++){
        // if(hierarchy[i][2]<0){
        if (contourArea(contours[i])<MAXIMUM_RECTANGLE_AREA
                && contourArea(contours[i])>MINIMUM_RECTANGLE_AREA
                && checkNonZeros(contours[i],white_input)){
            approxPolyDP(contours[i], approx[i], (double)epsilon, true);
            sortByY(approx[i]);

            if((approx[i].size() < 5 && checkRect(approx[i]))
                    /*&& isInsideBlackArea(approx[i] , black_drawing, blackContours,
                                                                                                                                                                                                                                                                                                                                                                                                      blackHierarchy) */){
#ifdef DEBUG
                for(int j=0;j<4;j++){
                    circle(drawing,approx[i][j],4,Scalar(MAX_SCALAR,MAX_SCALAR,0),-1,3);
                }
#endif
                mergePointsByNearest(approx[i],found_points);
            }
            //   }
        }
    }
#ifdef DEBUG
    for(int i=0;i < found_points.size();i++){
        circle(drawing,found_points[i],2,Scalar(MAX_SCALAR,MAX_SCALAR,MAX_SCALAR),CV_FILLED,8);
    }
#endif
}

bool ProgramHandeler::checkNonZeros(vector<Point>points,Mat white_input){

    LineEquations eq;
    Point p = eq.findMeanPoint(points);
    //qDebug()<<white_input.at<uchar>(p.y,p.x);
    if(white_input.at<uchar>(p.y,p.x) == 0){
        return false;
    }
    return true;

}

void ProgramHandeler::mergePointsByNearest(vector<Point>points,vector<Point>&found_points){
    vector<Point> down;
    down.clear();
    down = points;
    vector<Point> up;
    up.clear();

    LineEquations eq;

    Point p0 = points[0];
    double *lengthsToZero = new double[4];
    lengthsToZero[0] = 0;
    up.push_back(p0);
    down.erase(down.begin() + 0);
    for(int i=1;i<4;i++){
        lengthsToZero[i] = eq.point_length(p0,points[i]);
    }
    int min = 10000;
    int minAndis = 0;
    for(int i=1;i<4;i++){
        if(lengthsToZero[i] < min){
            min = lengthsToZero[i];
            minAndis = i;
        }
    }
    up.push_back(points[minAndis]);
    down.erase(down.begin() + minAndis - 1);
    try{
        found_points.push_back(eq.findMeanPoint(up));
        if(down.size() != 0){
            found_points.push_back(eq.findMeanPoint(down));
        } else {
            found_points.push_back(eq.findMeanPoint(up));
        }
    }catch (Exception e){
        qDebug()<<"ex : "<<e.what();
    }
}


bool ProgramHandeler::checkRect(vector<Point> input){
    sortByY(input);
    if(input.size() > 5){
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

void ProgramHandeler::handleRouting(Mat &comp_drawing, Mat &fin_drawing, vector<Point> &points){
    if(!detected_qr){//straigh forward
        if(!straight_lock){
            selectedStraightPoints.clear();
            selectUpAndDownPoints(points);
            straightTarget = points[0];
            straightBase = points.back();
            setAngel(straightBase,straightTarget);
            selectedStraightPoints.push_back(straightBase);
            selectedStraightPoints.push_back(straightTarget);
            if(straightBase == straightTarget){
                onlyOnePointDetected = true;
            } else {
                onlyOnePointDetected = false;
            }
            //straight_lock = true;
        } else {
            LineEquations eq;
            vector<Point>finalSelected;
            finalSelected.clear();
            for(int i=0;i<points.size();i++){
                if(eq.point_length(selectedStraightPoints[1],points[i]) < 45){
                    straightTarget = points[i];
                }
                if(eq.point_length(selectedStraightPoints[0],points[i]) < 45){
                    straightBase = points[i];
                }
            }
            selectedStraightPoints[1] = straightTarget;
            selectedStraightPoints[0] = straightBase;
            if(straightBase == straightTarget){
                onlyOnePointDetected = true;
            } else {
                onlyOnePointDetected = false;
            }
            if(reachedTargetPoint(straightTarget) ){
                straight_lock = false;
            }
        }
#ifdef DEBUG
        circle(comp_drawing,straightTarget,10,Scalar(MAX_SCALAR,0,0),3,5);
        circle(comp_drawing,straightBase,10,Scalar(0,MAX_SCALAR,0),3,5);
        connectPoints(comp_drawing, points);
#endif
        final_found_points.push_back(straightBase);
        final_found_points.push_back(straightTarget);
        connectPoints(fin_drawing, final_found_points);
        return;
    } else {
        dilate_factor = 1;
        //find the current direction
        qr_dir = findDirection();
        if(qr_dir == "right"){
            old_rotation_angle = rotation_angle;
            rotation_angle += 90;
            qr_flag = -1;
#ifdef DEBUG
            qDebug()<<"here1";
#endif
            takeNewDirections();
        }

        if(qr_dir == "left"){
            old_rotation_angle = rotation_angle;
            rotation_angle += -90;
            qr_flag = -2;
#ifdef DEBUG
            qDebug()<<"here3";
#endif
            takeNewDirections();
        }

        if(qr_dir == "backward"){
            old_rotation_angle = rotation_angle;
            rotation_angle += -180;
            qr_flag = -4;
            takeNewDirections();
#ifdef DEBUG
            qDebug()<<"here4";
#endif
        }
        if(qr_dir == "forward"){
            selectedStraightPoints.clear();
            selectUpAndDownPoints(points);
            straightTarget = points[0];
            straightBase = points.back();
            setAngel(straightBase,straightTarget);
            selectedStraightPoints.push_back(straightBase);
            selectedStraightPoints.push_back(straightTarget);
            if(straightBase == straightTarget){
                onlyOnePointDetected = true;
            } else {
                onlyOnePointDetected = false;
            }
            //straight_lock = true;
#ifdef DEBUG
            circle(comp_drawing,straightTarget,10,Scalar(MAX_SCALAR,0,0),3,5);
            circle(comp_drawing,straightBase,10,Scalar(0,MAX_SCALAR,0),3,5);
            connectPoints(comp_drawing, points);
#endif
            final_found_points.push_back(straightBase);
            final_found_points.push_back(straightTarget);
            connectPoints(fin_drawing, final_found_points);
        }

    }
}

bool ProgramHandeler::detectedLinesOf(string dir, const vector<Point> points){
    if (lock_qr){
        return true;
    }

    if(dir == "left"){
        for(int i=0;i<points.size();i++){
            if(points[i].x < 120){
                return true;
            }
        }
        return false;
    } else if (dir == "right"){
        for(int i=0;i<points.size();i++){
            if(points[i].x > 160){
                return true;
            }
        }
        return false;


    }
}

bool ProgramHandeler::reachedTargetPoint(const Point target){
    if(target == image_center){
#ifdef DEBUG
        qDebug()<<"returning false";
#endif

        return false;
    }

    LineEquations eq;
    if(eq.point_length(image_center,target) < 30){
        return true;
    }
    return false;

}

void ProgramHandeler::selectRightAndDownPoints(vector<Point> &points){
    if(!rotation_initiated){
        Point farRightPoint;
        farRightPoint = points[0];
        selectedRotationPoints.clear();
        selectedRotationPoints.push_back(Point(points[0].x + BASE_RIGHT_X_OFFSET ,
                                         points[0].y + BASE_RIGHT_Y_OFFSET));
        for(int i=1;i<points.size();i++){
            if( points[i].x > farRightPoint .x)  {
                farRightPoint = points[i];
            }
        }
        selectedRotationPoints.push_back(Point(farRightPoint.x +
                                               RIGHT_X_OFFSET , farRightPoint.y));
        points = selectedRotationPoints;
        rotation_initiated = true;
        return;
    } else {
        LineEquations eq;
        vector<Point>finalSelected;
        finalSelected.clear();
        for(int i=0;i<points.size();i++){
            if(eq.point_length(selectedRotationPoints[1],Point(points[i].x +
                                                               RIGHT_X_OFFSET ,points[i].y)) < 45){
                rotationTarget = points[i];
            }
            if(eq.point_length(selectedRotationPoints[0],
                               Point(points[i].x + BASE_RIGHT_X_OFFSET ,points[i].y +
                                     BASE_RIGHT_Y_OFFSET)) < 45){
                rotationBase = points[i];
            }
        }
        selectedRotationPoints[1] = Point(rotationTarget.x + RIGHT_X_OFFSET, rotationTarget.y);
        selectedRotationPoints[0] = Point(rotationBase.x + BASE_RIGHT_X_OFFSET,rotationBase.y +
                                          BASE_RIGHT_Y_OFFSET);
        points.clear();
        points.push_back(Point(rotationTarget.x + RIGHT_X_OFFSET, rotationTarget.y));
        points.push_back(Point(rotationBase.x + BASE_RIGHT_X_OFFSET,rotationBase.y +
                               BASE_RIGHT_Y_OFFSET));
    }
}

bool ProgramHandeler::RightAndDownPointsIsValid(const vector<Point> points){
    vector<Point> testPoints = points;

    Point farRightPoint;
    farRightPoint = testPoints[0];
    for(int i=1;i<points.size();i++){
        if( testPoints[i].x > farRightPoint .x)  {
            farRightPoint = testPoints[i];
        }
    }

    if(testPoints[0].y < farRightPoint.y){
        return false;
    }
    return true;

}

void ProgramHandeler::selectLeftAndDownPoints(vector<Point> &points){
    if(!rotation_initiated){
        Point farLeftPoint;
        farLeftPoint = points[0];
        selectedRotationPoints.clear();
        selectedRotationPoints.push_back(Point(points[0].x + BASE_LEFT_X_OFFSET ,
                                         points[0].y + BASE_LEFT_Y_OFFSET));
        for(int i=1;i<points.size();i++){
            if( points[i].x < farLeftPoint .x)  {
                farLeftPoint = points[i];
            }
        }
        selectedRotationPoints.push_back(Point(farLeftPoint.x + LEFT_X_OFFSET ,
                                               farLeftPoint.y + LEFT_Y_OFFSET));
        points = selectedRotationPoints;
        rotation_initiated = true;
        return;
    } else {
        LineEquations eq;
        vector<Point> finalSelected;
        finalSelected.clear();
        for(int i=0;i<points.size();i++){
            if(eq.point_length(selectedRotationPoints[1],
                               Point(points[i].x + LEFT_X_OFFSET ,points[i].y + LEFT_Y_OFFSET)) < 50){
                rotationTarget = points[i];
            }
            if(eq.point_length(selectedRotationPoints[0],
                               Point(points[i].x + BASE_LEFT_X_OFFSET ,points[i].y + BASE_LEFT_Y_OFFSET)) < 50){
                rotationBase = points[i];
            }
        }
        selectedRotationPoints[1] = Point(rotationTarget.x + LEFT_X_OFFSET, rotationTarget.y);
        selectedRotationPoints[0] = Point(rotationBase.x + BASE_LEFT_X_OFFSET, rotationBase.y + BASE_LEFT_Y_OFFSET);
        points.clear();
        points.push_back(Point(rotationTarget.x + LEFT_X_OFFSET, rotationTarget.y));
        points.push_back(Point(rotationBase.x + BASE_LEFT_X_OFFSET,rotationBase.y + BASE_LEFT_Y_OFFSET));
    }
}

bool ProgramHandeler::LeftAndDownPointsIsValid(const vector<Point> points){


    vector<Point> testPoints = points;

    Point farLeftPoint;
    farLeftPoint = testPoints[0];
    for(int i=1;i<points.size();i++){
        if( testPoints[i].x < farLeftPoint .x)  {
            farLeftPoint = testPoints[i];
        }
    }

    if(testPoints[0].y < farLeftPoint.y){
        return false;
    }
    return true;
}

void ProgramHandeler:: selectUpAndDownPoints(vector<Point> &points){

    vector<Point> selected;
    int local_flag = 0;
    selected.clear();
    for(int i=0;i<points.size();i++){
        if( (abs(points[i].x - 160)<= 25) ){
            selected.push_back(points[i]);
            local_flag = 1;
        }
    }
    if(local_flag == 0){
        selected.push_back(points[0]);
    }
    points = selected;
}

void ProgramHandeler::connectPoints(Mat& drawing,vector<Point> &points){
    for(int i=0; i < points.size() - 1; i++){
        line(drawing,points[i],points[i+1],Scalar(MAX_SCALAR,MAX_SCALAR,MAX_SCALAR),1,8);
    }
}

void ProgramHandeler::handleBlurs(Mat &unfiltered){
    // medianBlur(unfiltered,unfiltered,5);
    erode(unfiltered,unfiltered,
          getStructuringElement(MORPH_ELLIPSE,Size( 2*erode_factor+1, 2*erode_factor+1 ),Point(-1,-1)));
    dilate(unfiltered,unfiltered,
           getStructuringElement(MORPH_ELLIPSE,Size( 2*dilate_factor+1, 2*dilate_factor+1 ),Point(-1,-1)));
}

void ProgramHandeler::whiteThsOut(Mat &whtInput)
{
    handleBlurs(whtInput);
    cvtColor(whtInput,whtInput,CV_BGR2HLS);
    inRange(whtInput,Scalar(min_H,min_L,min_S),Scalar(max_H,max_L,max_S),whtInput);
}

void ProgramHandeler::blackThsOut(Mat &blkInput){
    inverter inv;
    handleBlurs(blkInput);
    inv.invert_channels(&blkInput,&blkInput);
    cvtColor(blkInput,blkInput,CV_BGR2HLS);
    inRange(blkInput,Scalar(black_min_H,black_min_L,black_min_S),
            Scalar(black_max_H,black_max_L,black_max_S),blkInput);
}

void ProgramHandeler::setAngel(Point p0,Point p1){

    double delta_y =-(p0.y - p1.y);
    double delta_x = (p0.x - p1.x);
    ang = qAtan( (delta_x) / (delta_y) ) * 180/M_PI;
}

void ProgramHandeler::Callback(const string type , const string data,
                               const vector<Point> locations, int detected){
    if(!lock_qr){
        if(detected != 0){
            next_mission = data.at(8);
            if(((int)next_mission - 48) == queued_mission ){
                mission_counter += 2;
                queued_mission ++;

            }
            detected_qr = true;
            if(mission_counter < 8){
                qr_data = data.at(mission_counter);
            } else {
                qr_data = data.at(6);
            }
            qr_rect.clear();
            for(int i=0;i<locations.size();i++){
                Point p;
                p.x = locations[i].x;
                p.y = locations[i].y;
                qr_rect.push_back(p);
            }
            LineEquations eq;
            qr_center = eq.findMeanPoint(qr_rect);
#ifdef SAY_QR_DATA
            cout << "(int)next_mission - 48 : "<< (int)next_mission - 48 << endl;
            cout << "mission_counter : "<< (int)next_mission - 48 << endl;
            cout << "queued_mission : "<< queued_mission << endl;
            cout << "my qr_data : "<< qr_data << endl;
            cout << "connector node says : qr data = "<< data <<endl;
            //            cout<<"current direction : "<<quad_current_direction <<endl;
            //            cout<<"next direction : "<<findDirection()<<endl;
#endif
        } else {
            detected_qr = false;

        }
    } else {
        detected_qr = true;
        //qr locsation
        if(detected != 0){
            qr_rect.clear();
            for(int i=0;i<locations.size();i++){
                Point p;
                p.x = locations[i].x;
                p.y = locations[i].y;
                qr_rect.push_back(p);
            }
        }
    }
}

void ProgramHandeler::sortByY(vector<Point> &points){
    int  j;
    Point key;
    for (int i = 1; i < points.size(); i++){
        key = points[i];
        j = i-1;
        while (j >= 0 && points[j].y > key.y)
        {
            points[j+1] = points[j];
            j = j-1;
        }
        points[j+1] = key;
    }
}

void ProgramHandeler::publish(const double dist,const double ang){


    if(quad_current_direction == NORTH){
        hassan = 0;
    }
    if(quad_current_direction == EAST){
        hassan = 1;
    }
    if(quad_current_direction == WEST){
        hassan = 3;
    }
    if(quad_current_direction == SOUTH){
        hassan = 2;
    }
    emit connectorPkg(dist,ang,hassan);

}

void ProgramHandeler::gateStatusCallBack(int movement){

    //movement -> 0paii / 1->bala

    if(movement == 1){
        MINIMUM_RECTANGLE_AREA = 150;
    } else {
        MINIMUM_RECTANGLE_AREA = 200;
    }

}

