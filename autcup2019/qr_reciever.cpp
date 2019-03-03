#include "qr_reciever.h"

qr_reciever::qr_reciever(QObject *parent) : QObject(parent)
{

}

void qr_reciever::imageCallback(cv::Mat raw_image)
{
    // imshow("QR",raw_image);
    decoder(raw_image);

    //  waitKey(3);


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
            //  cout<<"symbol : "<<symbol->get_count()<<endl;
            string type = symbol->get_type_name();
            string data = symbol->get_data();
#ifdef SAY_DATA
         //   cout<<"Type : "<<type<<endl;
            cout<<"Data : "<<data<<endl;
#endif
            for(int i=0;i<symbol->get_location_size();i++){
                locs.push_back(Point(symbol->get_location_x(i), symbol->get_location_y(i) ));
            }
            emit QR_signal(type, data, locs, detected);
        }
    } else if(detected == 0){
        emit QR_signal("","",locs,detected);
    }
}
//    if(!locs.empty()){
//        Rect rec = boundingRect(locs);
//        rectangle(input, rec, Scalar(0,0,255), 3,5);
//    }
//}

