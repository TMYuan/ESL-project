#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>

int main(){
    IplImage *circle;
    circle = cvCreateImage(cvSize(1000,600),IPL_DEPTH_8U, 3);
    cvCircle(circle, cvPoint(400,200), 100, CV_RGB(255,255,255), 3, CV_AA, 0);
    cvCircle(circle, cvPoint(600,200), 100, CV_RGB(255,0,0), 3, CV_AA, 0);
    cvCircle(circle, cvPoint(200,200), 100, CV_RGB(0,0,255), 3, CV_AA, 0);
    cvCircle(circle, cvPoint(300,300), 100, CV_RGB(255,255,0), 3, CV_AA, 0);
    cvCircle(circle, cvPoint(500,300), 100, CV_RGB(0,255,0), 3, CV_AA, 0);
    cvShowImage("output",circle);
    cvWaitKey(0);
    return 0 ;
}
