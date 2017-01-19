#include "widget.h"
#include <QtGui/QApplication>
#include <QtGui/QPushButton>
#include <QtGui/QFont>
#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QImage>
#include <QPainter>

#include <QTimer>
#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
using namespace cv;
using namespace std;

PainterWidget::PainterWidget(){
    timeNow =   0;
    height  =   150;
    angle   =   60;
    rate    =   10;
    thickness   =   3;
    shift   =   0;
    img     =   cvCreateImage(cvSize(200,300),IPL_DEPTH_8U,3);
    pt1     =   cvPoint(0, 150);
    pt2     =   pt1;
    
    timer   =   new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(100);
}
PainterWidget::~PainterWidget()
{

}

void PainterWidget::paintEvent(QPaintEvent*event) {
    drawTrace();
	QImage Qimg;
	cvxCopyIplImage(img, Qimg);
	QPainter  painter(this);
	painter.drawImage(0,0,Qimg);
}  

void PainterWidget::drawTrace(){
    double rate_x = rate * cos(angle*PI/180);
    double rate_y = rate * sin(angle*PI/180);
    timeNow +=  0.1;
    pt1     =   pt2;
    pt2.x   =   (int)(pt1.x + rate_x*timeNow);
    pt2.y   =   (int)(pt1.y - (rate_y*timeNow-0.5*9.8*pow(timeNow,2)));
    if(pt2.x > 200 || pt2.y > 300 || pt2.y < 0){
        pt2.x   =   200;
        timeNow =   (200-pt1.x)/rate_x;
        pt2.y   =   (int)(pt1.y - (rate_y*timeNow-0.5*9.8*pow(timeNow,2)));
        timeNow =   0;
        timer->stop();
    }
    cvLine(img, pt1, pt2, CV_RGB(255,0,0), thickness, shift);
}

void PainterWidget::cvxCopyIplImage(const IplImage *pIplImage,QImage &qImage)
{   //implement the conversion function here
	if(qImage.isNull())
        {
                int w = pIplImage->width;
                int h = pIplImage->height;
                qImage = QImage(w, h, QImage::Format_RGB32);
        }

	int x, y;
	for(x = 0; x < pIplImage->width; ++x)
        {
	   for(y = 0; y < pIplImage->height; ++y)
	   {
	      CvScalar color = cvGet2D(pIplImage, y, x);

	      int r = color.val[2];
	      int g = color.val[1];
	      int b = color.val[0];

	      qImage.setPixel(x, y, qRgb(r,g,b));
	   }
    }
}