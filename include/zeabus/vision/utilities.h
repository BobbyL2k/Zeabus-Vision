#include <utility>
#include <vector>
#include <cmath>
#include "opencv2/opencv.hpp"

#define DEBUG
#ifdef DEBUG
#include <iostream>
#define VD(X) std::cout << #X << " = " << X << std::endl
#else
#define VD(X)
#endif


namespace cv{

    typedef std::vector<Point> Contour;
    typedef std::vector<Contour> Contours;

    typedef Scalar Color;

    const double
    RAD2DEG = 180 / CV_PI,
    DEG2RAD = CV_PI / 180;

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

        // Contours tempContours;

        findContours(localImg, contours, objectHierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_TC89_L1);

        // contours.clear();
        // for(Contour contour : tempContours){
        //     Contour toAdd;
        //     for(int c=0; c<contour.size(); c=+10){
        //         toAdd.push_back(contour[c]);
        //     }
        //     contours.push_back(toAdd);
        // }

    }

    void findContoursInRange(InputArray img, Contours& contours, std::vector<Vec4i> objectHierarchy, double sdRange, bool invert){
        Color colorMean, colorStdDev;
        meanStdDev(img, colorMean, colorStdDev);
        findContoursInRange(img, contours, objectHierarchy, sdRange, invert, colorMean, colorStdDev);
    }

    int mod(int a, int b){
        int r = a % b;
        return r < 0 ? r + b : r;
    }
    double modd(double a, double b){
        return a - floor(a/b)*b;
    }

    enum KernelFlag {repeatEdge, loopEdge, discardEdge};

    template <typename T>
    std::vector<T> applyKernel(const std::vector<T> &input,const std::vector<T> &kernel, KernelFlag flag){
        CV_Assert(kernel.size() % 2 == 1);
        CV_Assert(flag == repeatEdge);
        std::vector<T> result(input.size());
        for(int i=0; i<input.size(); i++){
            int eResult = 0;
            // VD(i);
            for(int k=0; k<kernel.size(); k++){
                const int iIndex = mod(i + k - kernel.size()/2, input.size());
                // VD(k);VD(i + k - kernel.size()/2);VD(iIndex);
                eResult += input[iIndex] * kernel[k];
            }
            result[i] = eResult;
        }
        return result;
    }

    template <typename ContainerIterator>
    void displayContainer(std::ostream &os, ContainerIterator begin, ContainerIterator end){
        os << "[ ";
        while(begin != end){
            os << *begin << ' ';
            begin++;
        }
        os << ']' << std::endl;
    }

    /// Return in Radian [0 : PI]
    double getLineAngle(const Point& from, const Point& to){
        Point delta(from - to);

        double angle = std::atan2(delta.y, delta.x);
        if(angle < 0){
            angle += CV_PI;
        }

        return angle;
    }

    double getContourAngle(const Contour& contour){
        const int BUCKET_SPACE = 360;
        const int HALF_CIRCLE_BUCKET_SPACE = BUCKET_SPACE;

        std::vector<double> bucket(BUCKET_SPACE);
        // double processedBucket[BUCKET_SPACE];
        for(int c=0; c<BUCKET_SPACE; c++)
            bucket[c] = 0;

        for(int c=0; c<contour.size(); c+=10){
            Point
                from(contour[c]),
                to(contour[(c+1) % contour.size()]),
                delta(from - to);

            int angle = getLineAngle(from, to) *HALF_CIRCLE_BUCKET_SPACE /(CV_PI);
            // std::cout << angle << std::endl;
            bucket[angle] += delta.x*delta.x + delta.y*delta.y;
        }

        const std::vector<double> kernel = {1,2,3,2,1};
        // std::vector<double> kernel;
        // for(int c=1; c<BUCKET_SPACE/72; c++){
        //     kernel.push_back(const_reference __x)
        // }
        std::vector<double> processedBucket = applyKernel(bucket, kernel, KernelFlag::repeatEdge);

        // displayContainer(std::cout, bucket.begin(), bucket.end());
        // displayContainer(std::cout, processedBucket.begin(), processedBucket.end());
        // std::cout << std::endl;

        int maxIndex = 0;
        double maxValue = processedBucket[0];
        for(int c=1; c<BUCKET_SPACE; c++){
            if(processedBucket[c] > maxValue){
                maxIndex = c;
                maxValue = processedBucket[c];
            }
        }

        double angle = ((double)maxIndex) / HALF_CIRCLE_BUCKET_SPACE * CV_PI;

        // if(angle < 0)
        //     angle += CV_PI;

        return angle;
    }

    /// UNTESTED
    /// TODO test RepeatingArray
    // template <typename Element>
    // class RepeatingArray {
    // private:
    //     Element *array;
    //     int arraySize;
    // public:
    //     RepeatingArray (int arraySize) : arraySize{arraySize} {
    //         array = new Element[arraySize];
    //     }
    //     virtual ~RepeatingArray (){
    //         delete array;
    //     }
    //     Element& operator[] (int index){
    //         index = mod(index, arraySize);
    //         return array[index];
    //     }
    //     Element operator[] (int index) const{
    //         index = mod(index, arraySize);
    //         return array[index];
    //     }
    // };
}
