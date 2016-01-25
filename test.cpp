#include <iostream>
#include <cmath>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>

#include "include/zeabus/vision/utilities.h"

using namespace cv;

double getContourAngle(const Contour& contour){
    // int bucket[360];
    return CV_PI/2;
}

void process(Mat& img){

    cv::cvtColor(img, img, CV_BGR2YUV);

    Contours contours;
    std::vector<Vec4i> objectHierarchy;
    findContoursInRange(img, contours, objectHierarchy, 2, true);

    cv::cvtColor(img, img, CV_YUV2BGR);
    drawContours(img, contours, -1, red, CV_FILLED);


    for(Contour contour : contours){

        double angle = getContourAngle(contour);

        /// Preview code
        const int arrowLength = 30;

        Point center(0,0);
        for(Point point : contour){
            center += point;
            circle(img, point, 1, green, 1);
        }
        center.x /= contour.size();
        center.y /= contour.size();
        Point to(
            center.x + std::cos(angle) * arrowLength,
            center.y + std::sin(angle) * arrowLength
        );
        circle(img, center, 3, yellow, 2);
        arrowedLine(img, center, to, green, 2);

    }

}

int main()
    {
    cv::VideoCapture cap;
    cap.open(0);

    if( !cap.isOpened() )
    {
        std::cerr << "***Could not initialize capturing...***\n";
        std::cerr << "Current parameter's value: \n";
        return -1;
    }

    cv::Mat frame;
    while(1){
        cap >> frame;
        if(frame.empty()){
            std::cerr<<"frame is empty"<<std::endl;
            break;
        }

        process(frame);

        cv::imshow("", frame);
        cv::waitKey(10);
    }

    return 1;
}
