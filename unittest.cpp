#include <iostream>
#include <vector>

#include <cmath>

#include "include/zeabus/vision/utilities.h"

using namespace cv;

int main(){
    // std::vector<int> data = {0,0,0,1,0,0,0};
    // std::vector<int> kernel = {1,0,1};
    // displayContainer(std::cout, data.begin(), data.end());
    // std::vector<int> result = applyKernel(data, kernel, KernelFlag::repeatEdge);
    // displayContainer(std::cout, result.begin(), result.end());
    // result = applyKernel(result, kernel, KernelFlag::repeatEdge);
    // displayContainer(std::cout, result.begin(), result.end());

    int testrun = 0;
    int testpass = 0;

    for(double angle=0; angle<180; angle++){

        if(angle == 90)
            continue;

        double rad = angle *CV_PI /180;

        double rad2 = CV_PI/2 - rad;

        const int int_scale = 100;

        Point
            zero  (0                       , 0                       ),
            up    (std::cos(rad) *int_scale, std::sin(rad) *int_scale),
            right (std::cos(rad2)*int_scale, std::sin(rad2)*int_scale);

        up *= 3;

        // VD(angle);
        // VD(getLineAngle(zero, up) * RAD2DEG - angle);

        Contour contour;
        contour.push_back(zero);
        contour.push_back(zero+right);
        contour.push_back(zero+right+up);
        contour.push_back(zero+up);

        if( std::abs((angle) - (getContourAngle(contour) * RAD2DEG) ) > 1 ){
            std::cout << std::endl;
            VD(angle);
            VD(getContourAngle(contour) * RAD2DEG);
            // displayContainer(std::cout, contour.begin(), contour.end());
            VD(std::abs((angle) - (getContourAngle(contour) * RAD2DEG) ));
            std::cout << std::endl;
        }else{
            std::cout << angle << " " << getContourAngle(contour) * RAD2DEG << " Pass" << std::endl;
            testpass++;
        }

        testrun++;
    }

    VD((double)testpass/testrun*100);

    return 0;
}
