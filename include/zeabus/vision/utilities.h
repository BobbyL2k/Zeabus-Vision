#include <utility>
#include <vector>
#include <cmath>
#include "opencv2/opencv.hpp"


namespace cv{

    typedef std::vector<Point> Contour;
    typedef std::vector<Contour> Contours;

    typedef Scalar Color;

    const int
    BGR_B = 0,
    BGR_G = 1,
    BGR_R = 2,
    RGB_R = 0,
    RGB_G = 1,
    RGB_B = 2,
    YUV_Y = 0,
    YUV_U = 1,
    YUV_V = 2,
    HSV_H = 0,
    HSV_S = 1,
    HSV_V = 2;

    const Color red(0,0,255), green(0,255,0), blue(255,0,0), yellow(0,255,255);

    bool pointInsideContour(const Point& p , const Contour& c){
        return pointPolygonTest(c, p, false) >= 0;
    }

    Scalar heatColor(double value){
        value = value/2;
        if(value < 0)
            value = 0;
        if(value > 1)
            value = 1;
        int compute = value*1024;
        int r, g, b;
        switch ((int)(value*4)) {
            case 0:
            b = 255;
            g = compute;
            r = 0;
            break;
            case 1:
            b = 511 - compute;
            g = 255;
            r = 0;
            break;
            case 2:
            b = 0;
            g = 255;
            r = compute - 512;
            break;
            default:
            b = 0;
            g = 1023 - compute;
            r = 255;
        }
        return Scalar(r,g,b,255);
    }

    void smoothEdges(InputArray inputImage, OutputArray outputImage, int size, int minSize){

        Mat firstPassEllipse = getStructuringElement( MORPH_ELLIPSE, Size(minSize, minSize)),
        secondPassEllipse = getStructuringElement( MORPH_ELLIPSE, Size(size + minSize, size + minSize)),
        thirdPassEllipse = getStructuringElement( MORPH_ELLIPSE, Size(size, size));

        erode( inputImage, outputImage, firstPassEllipse );
        dilate( outputImage, outputImage, secondPassEllipse );
        erode( outputImage, outputImage, thirdPassEllipse );
    }

    void findContoursInRange(InputArray img, Contours& contours, std::vector<Vec4i> objectHierarchy, double sdRange, bool invert, const Color& colorMean, const Color& colorStdDev){

        Mat localImg;

        inRange(img,
            colorMean - colorStdDev*sdRange,
            colorMean + colorStdDev*sdRange,
            localImg);

        const int edgeSmoothingFactorSize = 15, edgeSmoothingFactorSizeMin = 3;
        smoothEdges(localImg, localImg, edgeSmoothingFactorSize, edgeSmoothingFactorSizeMin);

        if(invert){
            bitwise_not(localImg, localImg);
        }

        findContours(localImg, contours, objectHierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
    }

    void findContoursInRange(InputArray img, Contours& contours, std::vector<Vec4i> objectHierarchy, double sdRange, bool invert){
        Color colorMean, colorStdDev;
        meanStdDev(img, colorMean, colorStdDev);
        findContoursInRange(img, contours, objectHierarchy, sdRange, invert, colorMean, colorStdDev);
    }
}
