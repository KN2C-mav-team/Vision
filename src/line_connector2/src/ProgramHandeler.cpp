#include "Headers/ProgramHandeler.h"



ProgramHandeler::ProgramHandeler()
    :it_(nh_){
    namedWindow(WINDOW,WINDOW_AUTOSIZE);
    //intial this
    quad_current_direction = "N";
    lock_qr = false;
    detected_qr = false;
    lock_points = false;
    optical_flow_lock = false;
    rotation_initiated = false;
    epsilon = 5;
    erode_factor = 0;
    dilate_factor = 1;
    max_L = 255;min_L = 90;
    max_H = 255;min_H = 0;
    max_S = 255;min_S = 0;
    black_max_L = 255;black_min_L = 172;
    black_max_H = 255;black_min_H = 0;
    black_max_S = 255;black_min_S = 0;
    createTrackbar("erode",WINDOW,&erode_factor,10);
    createTrackbar("dilate",WINDOW,&dilate_factor,10);
    createTrackbar("max_L",WINDOW,&max_L,255);
    createTrackbar("min_L",WINDOW,&min_L,255);
    createTrackbar("black_max_L",WINDOW,&black_max_L,255);
    createTrackbar("black_min_L",WINDOW,&black_min_L,255);
    createTrackbar("epsilon",WINDOW,&epsilon,20);
    //    createTrackbar("min_H",WINDOW,&min_H,255);
    //    createTrackbar("min_S",WINDOW,&min_S,255);
    //    createTrackbar("max_H",WINDOW,&max_H,255);
    //    createTrackbar("max_S",WINDOW,&max_S,255);
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
    if(quad_current_direction == "N"){

        if(qr_data == "E"){
            output = "right";
            return output;
        } else if(qr_data == "N"){
            output = "forward";
            return output;
        } else if(qr_data == "W"){
            output = "left";
            return output;
        } else if(qr_data == "S"){
            output = "backward";
            return output;
        } else {
            output = "bullshit";
            return output;
        }

    } else if(quad_current_direction == "E"){
        if(qr_data == "E"){
            output = "forward";
            return output;
        } else if(qr_data == "N"){
            output = "left";
            return output;
        } else if(qr_data == "W"){
            output = "backward";
            return output;
        } else if(qr_data == "S"){
            output = "right";
            return output;
        } else {
            output = "bullshit";
            return output;
        }

    } else if(quad_current_direction == "W"){
        if(qr_data == "E"){
            output = "backward";
            return output;
        } else if(qr_data == "N"){
            output = "right";
            return output;
        } else if(qr_data == "W"){
            output = "forward";
            return output;
        } else if(qr_data == "S"){
            output = "left";
            return output;
        } else {
            output = "bullshit";
            return output;
        }

    } else if(quad_current_direction == "S"){
        if(qr_data == "E"){
            output = "left";
            return output;
        } else if(qr_data == "N"){
            output = "backward";
            return output;
        } else if(qr_data == "W"){
            output = "right";
            return output;
        } else if(qr_data == "S"){
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

    imshow(WINDOW,raw_image);
    imshow("contoursOfWhite",contoursOfWhite);
    imshow("black_contours",black_drawing);
    imshow("black_threshold",black_threshold);
    imshow("Final Approx",final_approx);
    imshow("white_threshold",white_threshold);
    imshow("wholeContersOfWhite",wholeContoursOfWhite);
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
    bool point_found = false;
    inRange(oneLineInput,Scalar(250,250,250),Scalar(255,255,255),oneLineInput);
    findContours(oneLineInput,contours,hierarchy,CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE,Point(0,0));
    approx.resize(contours.size());
    if(contours.size() == 1){
        if(contours[0].size() < 20){
            point_found = true;
        }
    }
    for(int i=0; i<contours.size();i++){
        approxPolyDP(contours[i], approx[i], (double)epsilon, true);
    }
    for(int i=0;i<approx.size();i++){
        for(int j=0;j<approx[i].size();j++){
            circle(oneLineInput,approx[i][j],6,Scalar(255,0,0),CV_FILLED,8,0);
        }
    }
    circle(oneLineInput,image_center,5,Scalar(255,0,0),CV_FILLED,5,0);
    if(approx.size()>0){
        Point col;
        if(!point_found){
            col = handleFinalPoints(approx[0]);
        } else {
            Point p = contours[0][0];
            col = p;
        }
        if(col.x<10000 && col.x > -10 &&
                col.y<10000 && col.y > -10){
            circle(oneLineInput,col,10,Scalar(255,255,255),1,8);
            line(oneLineInput,col,image_center,Scalar(255,255,255),1,8);
            if(col.x > image_center.x){
                dist = eq.point_length(image_center, col);
            } else {
                dist =-eq.point_length(image_center, col);
            }
            //     qDebug()<<"polar distance from center"<<dist<<"px";
//            double delta_y =-(approx[0][0].y - approx[0][1].y);
//            double delta_x = (approx[0][0].x - approx[0][1].x);
//            ang = qAtan( (delta_x) / (delta_y) ) * 180/M_PI;
            setAngel(approx[0][0],approx[0][1]);
            qDebug()<<"arc-tan of detected line"<< ang << "degrees";
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
    vector<vector<Point>> contours,approx;
    vector<Vec4i> hierarchy;
    vector<Point> found_points;
    int k = 0;
    found_points.clear();
    Mat drawing = Mat::zeros(white_input.size(),CV_8UC3);
    Mat final_drawing = Mat::zeros(white_input.size(),CV_8UC3);
    //    if(lock_qr){
    //        Rect r = boundingRect(qr_rect);
    //        rectangle(drawing,r.tl(),r.br(),Scalar(0,0,0),CV_FILLED);
    //        rectangle(white_input,r.tl(),r.br(),Scalar(0,0,0),CV_FILLED);
    //    }
    findContours(white_input, contours, hierarchy,
                 CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0,0));
    approx.resize(contours.size());
    for(int i=0;i<contours.size();i++){
        drawContours(drawing,contours,i,Scalar(100,0,100),1,8,hierarchy,0,Point());
    }
    for(int i=0;i < contours.size();i++){
        if(hierarchy[i][2]<0){
            if (contourArea(contours[i])<MAXIMUM_RECTANGLE_AREA
                    && contourArea(contours[i])>MINIMUM_RECTANGLE_AREA ){
                approxPolyDP(contours[i], approx[i], (double)epsilon, true);
                sort(approx[i].begin(), approx[i].end() , [] (Point a,Point b){
                    return a.y >= b.y;
                });

                if((approx[i].size() == 4 && checkRect(approx[i]))
                        /*&& isInsideBlackArea(approx[i] , black_drawing, blackContours,
                                                          blackHierarchy) */){
                    for(int j=0;j<4;j++){
                        circle(drawing,approx[i][j],4,Scalar(255,255,0),-1,3);
                    }
                    found_points.push_back(Point(
                                               (approx[i][0].x +
                                               approx[i][1].x +
                                           approx[i][2].x +
                            approx[i][3].x)/4 ,
                            (approx[i][0].y +
                            approx[i][1].y +
                            approx[i][2].y +
                            approx[i][3].y)/4
                            ));
                }
            }
        }
    }

    for(int i=0;i < found_points.size();i++){
        circle(drawing,found_points[i],2,Scalar(255,255,255),CV_FILLED,8);
        circle(final_drawing,found_points[i],3,Scalar(255,255,255),CV_FILLED,8);
    }
    if(found_points.size() > 0){
        sort(found_points.begin(), found_points.end() , [] (Point a,Point b){
            return a.y > b.y;
        });
        for(int i=0;i<found_points.size();i++){
            putText(drawing,std::to_string(i),found_points[i],1,2,Scalar(255,255,0),1,8);
        }
        handleRouting(drawing, final_drawing, found_points);
    }
    white_input = final_drawing.clone();
    return drawing;
}

bool ProgramHandeler::checkRect(vector<Point> input){
    sort(input.begin(),input.end() , [] (Point a,Point b){
        return a.y >= b.y;
    });
    LineEquations eq;
    double x1 = eq.point_length(input[0],input[2]);
    double y1 = eq.point_length(input[1],input[3]);
    double x2 = eq.point_length(input[0],input[1]);
    double y2 = eq.point_length(input[2],input[3]);
    if(input.size() != 4){
        return false;
    }
    //    qDebug()<<"width = "<<abs(x1 - y1);
    //    qDebug()<<"height = "<<abs(x2 - y2);
    if( (abs(x1 - y1) < 7 )&& ( abs(x2 - y2) < 7) /*&&
         abs((abs(x1 - y1) - abs(x2 - y2))) > 1*/){
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
    Point center = Point(
                (points[0].x +
                points[1].x +
            points[2].x +
            points[3].x )/4 ,
            (points[0].y +
            points[1].y +
            points[2].y +
            points[3].y )/4
            );

    for(int i=0;i<blackCn.size();i++){
        for(int i=0;i<4;i++){
            circle(blackDr,points[i],5,Scalar(255,0,0),-1,2);
        }
        if( pointPolygonTest(blackCn[i],center,false) == 1){
            drawContours(blackDr,
                         blackCn,i,Scalar(0,0,255),1,8,blackHi,0,Point());
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
                         blackCn,i,Scalar(255,255,255),1,8,blackHi,0,Point());
            circle(blackDr,image_center,5,Scalar(0,100,100),-1,3);
            qDebug()<<"center contour area"<<contourArea(blackCn[i]);
        }
    }
    return true;
}

void ProgramHandeler::handleRouting(Mat &comp_drawing, Mat &fin_drawing, vector<Point> &points){

    if(!detected_qr){//straigh forward
        selectUpAndDownPoints(points);
        for(int i=0; i < points.size() - 1; i++){
            connectPoints(comp_drawing, points);
            connectPoints(fin_drawing, points);
        }
        return;
    } else {
        //find the current direction
        string qr_dir = findDirection();
        //qDebug()<<"ang"<<ang;
        if(qr_dir == "right"){
            selectRightAndDownPoints(points);
            setAngel(points[0],points[1]);
            if(ang > 25 ){
                lock_qr = true;

                circle(comp_drawing,rotationTarget,10,Scalar(255,0,0),3,5);
                circle(comp_drawing,rotationBase,10,Scalar(0,255,0),3,5);
            } else {
                //take the new direction
                takeNewDirections();
                rotation_initiated = false;
                lock_qr = false;
            }
            connectPoints(comp_drawing, points);
            connectPoints(fin_drawing, points);
        }

        if(qr_dir == "left"){
            selectLeftAndDownPoints(points);
            setAngel(points[0],points[1]);
            if(ang < -25 ){
                lock_qr = true;
                circle(comp_drawing,rotationTarget,10,Scalar(255,0,0),3,5);
                circle(comp_drawing,rotationBase,10,Scalar(0,255,0),3,5);
            } else {
                //take the new direction
                takeNewDirections();
                rotation_initiated = false;
                lock_qr = false;
            }
            connectPoints(comp_drawing, points);
            connectPoints(fin_drawing, points);
        }

    }
}

void ProgramHandeler::selectRightAndDownPoints(vector<Point> &points){
    if(!rotation_initiated){
        Point farRightPoint;
        farRightPoint = points[0];
        selectedRotationPoints.clear();
        selectedRotationPoints.push_back(points[0]);
        for(int i=1;i<points.size();i++){
            if( points[i].x > farRightPoint .x)  {
                farRightPoint = points[i];
            }
        }
        selectedRotationPoints.push_back(farRightPoint);
        points = selectedRotationPoints;
        rotation_initiated = true;
        return;
    } else {
        LineEquations eq;
        vector<Point>finalSelected;
        finalSelected.clear();
        for(int i=0;i<points.size();i++){
            if(eq.point_length(selectedRotationPoints[1],points[i]) < 50){
                rotationTarget = points[i];
            }
            if(eq.point_length(selectedRotationPoints[0],points[i]) < 50){
                rotationBase = points[i];
            }
        }
        selectedRotationPoints[1] = rotationTarget;
        selectedRotationPoints[0] = rotationBase;
        points.clear();
        points.push_back(rotationTarget);
        points.push_back(rotationBase);
    }
}

void ProgramHandeler::selectLeftAndDownPoints(vector<Point> &points){
    if(!rotation_initiated){
        Point farLeftPoint;
        farLeftPoint = points[0];
        selectedRotationPoints.clear();
        selectedRotationPoints.push_back(points[0]);
        for(int i=1;i<points.size();i++){
            if( points[i].x < farLeftPoint .x)  {
                farLeftPoint = points[i];
            }
        }
        selectedRotationPoints.push_back(farLeftPoint);
        points = selectedRotationPoints;
        rotation_initiated = true;
        return;
    } else {
        LineEquations eq;
        vector<Point>finalSelected;
        finalSelected.clear();
        for(int i=0;i<points.size();i++){
            if(eq.point_length(selectedRotationPoints[1],points[i]) < 30){
                rotationTarget = points[i];
            }
            if(eq.point_length(selectedRotationPoints[0],points[i]) < 30){
                rotationBase = points[i];
            }
        }
        selectedRotationPoints[1] = rotationTarget;
        selectedRotationPoints[0] = rotationBase;
        points.clear();
        points.push_back(rotationTarget);
        points.push_back(rotationBase);
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
        line(drawing,points[i],points[i+1],Scalar(255,255,255),1,8);
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
    publisher = nh_.advertise<line_connector2::my_msg>("line_connector2/datas",1000);
    msg.distance = dist;
    msg.angel = ang;
    publisher.publish(msg);

}

void ProgramHandeler::setAngel(Point p0,Point p1){

    double delta_y =-(p0.y - p1.y);
    double delta_x = (p0.x - p1.x);
    ang = qAtan( (delta_x) / (delta_y) ) * 180/M_PI;

}
