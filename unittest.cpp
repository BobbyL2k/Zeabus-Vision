#include <iostream>
#include <vector>

#include <cmath>

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

    displayContainer(std::cout, bucket.begin(), bucket.end());
    displayContainer(std::cout, processedBucket.begin(), processedBucket.end());
    std::cout << std::endl;

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

int main(){
    std::vector<int> data = {0,0,0,1,0,0,0};
    std::vector<int> kernel = {1,0,1};
    displayContainer(std::cout, data.begin(), data.end());
    std::vector<int> result = applyKernel(data, kernel, KernelFlag::repeatEdge);
    displayContainer(std::cout, result.begin(), result.end());
    result = applyKernel(result, kernel, KernelFlag::repeatEdge);
    displayContainer(std::cout, result.begin(), result.end());
    return 0;
}
