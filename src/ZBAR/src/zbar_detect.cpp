#include "include/zbar_detect.h"

zbar_detect::zbar_detect(QObject *parent) : QObject(parent)
{


 zbarPub=zbar_nh.advertise<std_msgs::String>("zbar_data",1);

}

void zbar_detect::decode(Mat &im, std::vector<decodedObject> &decodedObjects)
{
    // Create zbar scanner
       zbar::ImageScanner scanner;
//
       // Configure scanner
       scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);

       // Convert image to grayscale
       Mat imGray;
       cvtColor(im, imGray,CV_BGR2GRAY);

       // Wrap image data in a zbar image
       Image image(im.cols, im.rows, "Y800", (uchar *)imGray.data, im.cols * im.rows);

       // Scan the image for barcodes and QRCodes
       int n = scanner.scan(image);

       // Print results
       for(Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol)
       {
           decodedObject obj;

           obj.type = symbol->get_type_name();
           obj.data = symbol->get_data();
           //if(obj.data == "red")
           msg.data=obj.data;
           zbarPub.publish(msg);
           waitKey(100);
           ros::shutdown();

           // Print type and data
           cout << "Type : " << obj.type << endl;
           cout << "Data : " << obj.data << endl << endl;

           // Obtain location
           for(int i = 0; i< symbol->get_location_size(); i++)
           {
               obj.location.push_back(Point(symbol->get_location_x(i),symbol->get_location_y(i)));
           }

           decodedObjects.push_back(obj);
       }
}

void zbar_detect::display(Mat &im, vector<decodedObject> &decodedObjects)
{
    for(int i = 0; i < decodedObjects.size(); i++)
    {
        vector<Point> points = decodedObjects[i].location;
        vector<Point> hull;

        // If the points do not form a quad, find convex hull
        if(points.size() > 4)
            convexHull(points, hull);
        else
            hull = points;

        // Number of points in the convex hull
        int n = hull.size();

        for(int j = 0; j < n; j++)
        {
            line(im, hull[j], hull[ (j+1) % n], Scalar(255,0,0), 3);
        }

    }

    // Display results
   // imshow("Results", im);
    // waitKey(2);
}

void zbar_detect::detector(Mat img)
{
    src=img;

    // Variable for decoded objects
    std::vector<decodedObject> decodedObjects;

    // Find and decode barcodes and QR codes
    decode(src, decodedObjects);

    // Display location
    display(src, decodedObjects);

}
