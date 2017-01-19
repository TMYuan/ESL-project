#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>

int main(){
    IplImage *img, *img1;
    img= cvLoadImage("jenna.jpg",0);
    img1= cvCreateImage(cvGetSize(img), IPL_DEPTH_16S, 1);
    cvNamedWindow("input",1);
    cvShowImage("input",img);
    cvSobel(img, img1, 0, 1, 7);
    cvNamedWindow("output",1);
    cvShowImage("output",img1);
    cvWaitKey(0);
    return 0 ;
}
