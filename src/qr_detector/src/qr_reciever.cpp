#include "Headers/qr_reciever.h"



qr_reciever::qr_reciever()
    :image_transport(node_handler){
    publisher = node_handler.advertise<qr_detector::qr_msg>("/qr_detector/Qr_Datas",1);
}

qr_reciever::~qr_reciever(){
    destroyAllWindows();
}

void qr_reciever::imageCallback(const sensor_msgs::ImageConstPtr & msg){

    try{
        cv_ptr = cv_bridge::toCvCopy(msg,sensor_msgs::image_encodings::BGR8);
    } catch(cv_bridge::Exception& e){
        ROS_ERROR("cv_bridge exception : ",e.what());
        return ;
    }
    Mat raw_image = cv_ptr->image.clone();
    decoder(raw_image);
    imshow("raw_image",raw_image);
    waitKey(3);

}

void qr_reciever::decoder(Mat &input){

    Mat gray_input;
    cvtColor(input,gray_input,CV_BGR2GRAY);

    zbar::ImageScanner z_scanner;
    z_scanner.set_config(zbar::ZBAR_NONE,zbar::ZBAR_CFG_ENABLE,1);

    zbar::Image z_image(input.cols, input.rows, "Y800",
                        (uchar *)gray_input.data, input.cols * input.rows);
    int detected =z_scanner.scan(z_image);

    vector<Point> locs;
    if(detected == 1){
        for(zbar::Image::SymbolIterator symbol = z_image.symbol_begin();
            symbol != z_image.symbol_end();
            ++symbol){
            cout<<"symbol : "<<symbol->get_count()<<endl;
            string type = symbol->get_type_name();
            string data = symbol->get_data();
            cout<<"Type : "<<type<<endl;
            cout<<"Data : "<<data<<endl;
            for(int i=0;i<symbol->get_location_size();i++){
                locs.push_back(Point(symbol->get_location_x(i), symbol->get_location_y(i) ));
            }
            publish(type, data, locs, detected);
        }
    } else if(detected == 0){
        publish("","",locs,detected);
    }
    Rect rec = boundingRect(locs);
    rectangle(input, rec, Scalar(0,0,255), 3,5);
}

void qr_reciever::publish(const string type,
                          const string data,const vector<Point> locations,int detected){

    qr_detector::qr_msg msg;
    msg.type = type;
    msg.data = data;
    geometry_msgs::Point PointsToSend;
    Point* sent_points = new Point[locations.size()];
    for (int i=0;i<locations.size();i++){
        sent_points[i] = locations[i];
        PointsToSend.x = sent_points[i].x;
        PointsToSend.y = sent_points[i].y;
        PointsToSend.z = 0;
        msg.qr_points.push_back(PointsToSend);
    }
    msg.qr_nums = detected;
    publisher.publish(msg);

}
