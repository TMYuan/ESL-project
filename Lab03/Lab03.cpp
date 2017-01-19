#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>

using namespace std;

int main(){
    static CvMemStorage* storage=cvCreateMemStorage(0);
    static CvHaarClassifierCascade* cascade=0;
    IplImage* img[5];
    cvNamedWindow("Player",1);
    img[0]= cvLoadImage("01.jpg", -1);
    img[1]= cvLoadImage("02.jpg", -1);
    img[2]= cvLoadImage("03.jpg", -1);
    img[3]= cvLoadImage("04.jpg", -1);
    img[4]= cvLoadImage("05.jpg", -1);

    cascade= (CvHaarClassifierCascade*)cvLoad("haarcascade_frontalface_alt.xml",0,0,0);

    for(int i=0; i<5; ++i){
        
        cvShowImage("Player", img[i]);
        CvSeq* faces = cvHaarDetectObjects(
            img[i], 
            cascade, 
            storage, 
            1.1, 
            2, 
            CV_HAAR_DO_CANNY_PRUNING,
            cvSize(20,20),
            cvSize(100,100)
        );
        if(faces){
            for(int j=0; j<faces->total; ++j){
                CvPoint pt1, pt2;
                CvRect* rectangle = (CvRect*)cvGetSeqElem(faces, j);
                pt1.x = rectangle->x;
                pt2.x = rectangle->x + rectangle->width;
                pt1.y = rectangle->y;
                pt2.y = rectangle->y + rectangle->height;
                //Draw a red rectanngle
                cvRectangle( img[i], pt1,pt2, CV_RGB(255,0,0), 3, 8, 0 );
            }
        }
        cvWaitKey(2000);
        cvShowImage("Player",img[i]);
        cvWaitKey(2000);
        
    }

    for(int i=0; i<5; ++i){
        cvReleaseImage(&img[i]);
    }
    cvReleaseHaarClassifierCascade(&cascade);
    cvClearMemStorage(storage);
    return 0;
}