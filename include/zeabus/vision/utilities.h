#include <utility>
#include <vector>
#include <cmath>

#include "opencv2/opencv.hpp"


namespace cv{

    typedef std::vector<Point> Contour;

    bool pointInContour(const Point &point, const Contour &contour);
    bool pointAIsInFrontOfPointB(const Point &a, const Point &b);
    bool lineIsInFrontOfPoint(const Point &lineFrom, const Point &lineTo, const Point &point);
    bool pointIsOnLine(const Point &point, const Point &a, const Point &b);

    typedef std::vector<Point> Contour;

    bool pointInContour(const Point &point, const Contour &contour){
        bool resultInside = false;
        bool resultBorder = false;
        for(int c=0; c<contour.size(); c++){
            Point pa = contour[c];
            Point pb = contour[ (c+1) % contour.size() ];
            Point pc = contour[ (c-1+contour.size()) % contour.size() ];

            if( ((pb.y < pa.y && pa.y < pc.y)
                ||
                 (pb.y > pa.y && pa.y > pc.y))
                &&
                pointAIsInFrontOfPointB(pa, point)){
                resultInside = !resultInside;
            }
            if(lineIsInFrontOfPoint(pa, pb, point)){
                resultInside = !resultInside;
            }
            if(
                !resultBorder && pointIsOnLine(point, pa, pb)
            ){
                resultBorder = true;
                break;
            }
        }
        return resultInside || resultBorder;
    }

    bool pointAIsInFrontOfPointB(const Point &a, const Point &b){
        return a.y == b.y && a.x >= b.x;
    }

    int gcd(int a, int b){
        if(b<0)
            b = -b;
        if(a<0)
            a = -a;
        if(a<b){
            int temp = a;
            a = b;
            b = temp;
        }
        while(b!=0){
            int temp = b;
            b = a%b;
            a = temp;
            // std::cout << a << " : " << b << std::endl;
        }
        // std::cout << "END of GCD" << std::endl;
        return a;
    }

    bool pointIsOnLine(const Point &point, const Point &lineFrom, const Point &lineTo){
        Point high(
            std::max(lineFrom.x, lineTo.x),
            std::max(lineFrom.y, lineTo.y));
        Point low(
            std::min(lineFrom.x, lineTo.x),
            std::min(lineFrom.y, lineTo.y));
        if(
            low.x <= point.x && point.x <= high.x
            &&
            low.y <= point.y && point.y <= high.y)
        {
            int dpx = lineFrom.x - point.x,
                dpy = lineFrom.y - point.y,
                dlx = lineFrom.x - lineTo.x,
                dly = lineFrom.y - lineTo.y;
            if(dpx == 0 && dpy == 0)
                return true;
            int gcdp = gcd(dpx, dpy),
                gcdl = gcd(dlx, dly);
            dpx /= gcdp;
            dpy /= gcdp;
            dlx /= gcdl;
            dly /= gcdl;
            return dpx == dlx && dpy == dly;
        }
        return false;
    }

    bool lineIsInFrontOfPoint(const Point &lineFrom, const Point &lineTo, const Point &point){

        int top     = std::max(lineFrom.y, lineTo.y),
            bottom  = std::min(lineFrom.y, lineTo.y);
            // right   = max(lineFrom.x, lineTo.x),
            // left    = min(lineFrom.x, lineTo.x);
        if(top > point.y && bottom < point.y){
            int deltaX = lineFrom.x - lineTo.x;
            // cout << lineFrom << " " << lineTo << endl;
            if(deltaX!=0){
                double m = (double)( lineFrom.y - lineTo.y ) / (deltaX);
                double c = lineFrom.y - (m * lineFrom.x);
                // if(m < 0)
                //     cout << "M>0 " << point << " " << ((point.y <= point.x * m + c)?"True ":"False ") << point.y << " " << point.x * m +c << endl;
                // else if (m > 0)
                //     cout << "M<0 " << " " << ((point.y >= point.x * m + c)?"True":"False") << endl;
                // else
                //     cout << "M=0 " << " " << ((point.y == c)?"True":"False") << endl;
                if( (m < 0 && point.y < point.x * m + c)
                    ||
                    (m > 0 && point.y > point.x * m + c) ){
                    return true;
                }
            }else if(lineFrom.x > point.x){
                return true;
            }
        }
        return false;
    }

}
