#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>
#include <iostream>
#include <vector>
#include <math.h>
#define PI 3.14

using namespace std;

void parabola(IplImage* img, double height, double angle, double rate, CvScalar color, int thickness, int shift){
    double rate_x = rate * cos(angle*PI/180);
    double rate_y = rate * sin(angle*PI/180);
    vector<CvPoint> pt;
    double t = 0;
    double location_x = 0;
    double location_y = 150;
    while(true){
        location_x += rate_x*t;
        location_y -= rate_y*t-0.5*9.8*pow(t,2);
        if(location_x > 200 || location_y > 300 || location_y < 0){
            break;
        }
        pt.push_back(cvPoint((int)location_x, (int)location_y));
        t+=0.1;
    }
    for(int i = 0;i < pt.size()-1;i++){
        cvLine(img, pt[i], pt[i+1], color, thickness, CV_AA, shift);
        //cvLine(Image1,FromPoint1,ToPoint1,Color,Thickness,CV_AA,Shift);
    }
    //cvLine(img, cvPoint(0,0), cvPoint(200,300), thickness, CV_AA, shift);
};

int main()
{
    IplImage *Image1,*Image2;
    CvPoint FromPoint1,ToPoint1;
    CvScalar Color;
    int Thickness;
    int Shift;

    CvSize ImageSize1 = cvSize(200,300);
    Image1 = cvCreateImage(ImageSize1,IPL_DEPTH_8U,3);

    Color=CV_RGB(255,0,0);
    Thickness=3;
    Shift=0;
    parabola(Image1, 150, 80, 10, Color, Thickness, Shift);
    //cvLine(Image1,cvPoint(0,0), cvPoint(200,300),Color,Thickness,CV_AA,Shift);

    /*double rate_x = 10*cos(45);
    double rate_y = 10*sin(45);
    vector<CvPoint> pt;
    double t = 0;
    double location_x = 0;
    double location_y = 150;
    while(true){
        location_x += rate_x*t;
        location_y -= rate_y*t-0.5*9.8*pow(t,2);
        if(location_x > 200 || location_y > 300 || location_y < 0){
            break;
        }
        pt.push_back(cvPoint((int)location_x, (int)location_y));
        t+=0.1;
    }
    for(int i = 0;i < pt.size()-1;i++){
        cvLine(Image1, pt[i], pt[i+1], Color, Thickness, CV_AA, Shift);
        //cvLine(Image1,FromPoint1,ToPoint1,Color,Thickness,CV_AA,Shift);
    }*/



    cvNamedWindow("cvLine shift",1);
    cvShowImage("cvLine shift",Image1);

    cvWaitKey(0);
}
