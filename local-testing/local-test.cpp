#include "zeabus/vision/utilities.h"
#include <iostream>
#include <cstdio>

#ifndef BREAKER
#define BREAKER std::cout << "break" << std::endl
#endif
#ifndef BREAKERx
#define BREAKERx(x) std::cout << "break" << x << std::endl
#endif

typedef std::vector<cv::Point> Contour;

int main(){
    Contour contour;
    contour.push_back(cv::Point( 3, 0));
    contour.push_back(cv::Point( 7, 2));
    contour.push_back(cv::Point( 8, 1));
    contour.push_back(cv::Point(15, 2));
    contour.push_back(cv::Point(12, 6));
    contour.push_back(cv::Point( 9, 9));
    contour.push_back(cv::Point( 6, 6));
    contour.push_back(cv::Point( 3, 9));
    contour.push_back(cv::Point( 0, 6));
    contour.push_back(cv::Point( 0, 3));

    for(int y=-2; y<12; y++){
        for(int x=-2; x<16; x++){
            cv::Point point(x,y);
            int v=0;

            bool behindLine = false,
                behindPoint = false,
                onLine = false;
            for(int c=0; c<contour.size(); c++){
                behindPoint = behindPoint ^ cv::pointAIsInFrontOfPointB(contour[c], point);
                behindLine = behindLine ^ cv::lineIsInFrontOfPoint(contour[c], contour[(c+1)%contour.size()], point);
                onLine = onLine | cv::pointIsOnLine(point, contour[c], contour[(c+1)%contour.size()]);
            }
            v += 1 * behindPoint;
            v += 10 * behindLine;
            v += 100 * onLine;
            // v += 100 * cv::pointInContour(point, contour);
            printf("%3d ", v);
        }
        std::cout << std::endl;
    }
    for(int y=-2; y<12; y++){
        for(int x=-2; x<16; x++){
            cv::Point point(x,y);
            bool result = cv::pointInContour(point, contour);
            bool isPoint = false;
            for(int c=0; c<contour.size(); c++){
                if(contour[c].x == x && contour[c].y == y){
                    isPoint = true;
                    break;
                }
            }
            printf("%s%s ", isPoint?"P":" ", result?"P":"-");
        }
        std::cout << std::endl;
    }
    return 0;
}
