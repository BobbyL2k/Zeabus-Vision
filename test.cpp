#include <iostream>
#include <cmath>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>

#include "include/zeabus/vision/utilities.h"

using namespace cv;

double getContourAngle(const Contour& contour){
    const int ANGLE_SPACE = 180;
    const int ANGLE_IN_PI = 180;

    std::vector<double> bucket(ANGLE_SPACE);
    // double processedBucket[ANGLE_SPACE];
    for(int c=0; c<ANGLE_SPACE; c++)
        bucket[c] = 0;

    for(int c=1; c<contour.size(); c++){
        Point
            from(contour[c]),
            to(contour[(c+1) % contour.size()]),
            delta(from - to);
        int angle = (int)(std::atan2(delta.y, delta.x) * ANGLE_IN_PI / CV_PI);
        angle = angle + ANGLE_IN_PI;    // shift from [-180,179] to [0,359];

        bucket[angle % ANGLE_SPACE] += delta.x*delta.x + delta.y*delta.y;
    }

    const std::vector<double> kernel = {1,2,3,2,1};
    std::vector<double> processedBucket = applyKernel(bucket, kernel, KernelFlag::repeatEdge);

    // displayContainer(std::cout, bucket.begin(), bucket.end());
    // displayContainer(std::cout, processedBucket.begin(), processedBucket.end());
    // std::cout << std::endl;

    int maxIndex = 0;
    double maxValue = bucket[0];
    for(int c=1; c<ANGLE_SPACE; c++){
        if(bucket[c] > maxValue){
            maxIndex = c;
            maxValue = bucket[c];
        }
    }

    double angle = ((double)maxIndex-180) / 180 * CV_PI;

    return angle;
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

    return 0;
}
