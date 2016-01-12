#include <utility>
#include <vector>
#include <cmath>
#include "opencv2/opencv.hpp"


namespace cv{

    typedef std::vector<Point> Contour;
    typedef std::vector<Contour> Contours;

    typedef Scalar Color;

    const Color red(0,0,255), green(0,255,0), blue(255,0,0), yellow(0,255,255);

    bool pointInsideContour(Point p , Contour c){
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

    void smoothEdges(Mat inputImage, Mat outputImage, int size, int minSize){

        Mat firstPassEllipse = getStructuringElement( MORPH_ELLIPSE, Size(minSize, minSize)),
        secondPassEllipse = getStructuringElement( MORPH_ELLIPSE, Size(size + minSize, size + minSize)),
        thirdPassEllipse = getStructuringElement( MORPH_ELLIPSE, Size(size, size));

        erode( inputImage, outputImage, firstPassEllipse );
        dilate( outputImage, outputImage, secondPassEllipse );
        erode( outputImage, outputImage, thirdPassEllipse );
    }

}
