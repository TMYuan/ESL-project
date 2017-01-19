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


PainterWidget::PainterWidget()
{
	play=false;
	face=false;
	wait_sec=1000;
    btn_next=new QPushButton(this);
    btn_next->setText(">>");
	btn_next->setFont(QFont("Courier", 18, QFont::Bold));
	btn_next->setGeometry(650,250,110,50);
	QObject::connect(btn_next, SIGNAL(clicked()), this, SLOT(img_next()));

	btn_last=new QPushButton(this);
    btn_last->setText("<<");
	btn_last->setFont(QFont("Courier", 18, QFont::Bold));
	btn_last->setGeometry(200,250,110,50);
	QObject::connect(btn_last, SIGNAL(clicked()), this, SLOT(img_last()));
	
	btn_play=new QPushButton(this);
    btn_play->setText("Start");
	btn_play->setFont(QFont("Courier", 18, QFont::Bold));
	btn_play->setGeometry(400,250,110,50);
	QObject::connect(btn_play, SIGNAL(clicked()), this, SLOT(is_play()));

	btn_fast=new QPushButton(this);
    btn_fast->setText("+");
	btn_fast->setFont(QFont("Courier", 18, QFont::Bold));
	btn_fast->setGeometry(650,350,110,50);
	QObject::connect(btn_fast, SIGNAL(clicked()), this, SLOT(faster()));

	btn_slow=new QPushButton(this);
    btn_slow->setText("-");
	btn_slow->setFont(QFont("Courier", 18, QFont::Bold));
	btn_slow->setGeometry(200,350,110,50);
	QObject::connect(btn_slow, SIGNAL(clicked()), this, SLOT(slower()));

	label=new QLabel(this);
	label->setText("1X");
	label->setFont(QFont("Courier", 18, QFont::Bold));
	label->setGeometry(400,350,110,50);

	btn_face=new QPushButton(this);
    btn_face->setText("Off");
	btn_face->setFont(QFont("Courier", 18, QFont::Bold));
	btn_face->setGeometry(200,150,110,50);
	QObject::connect(btn_face, SIGNAL(clicked()), this, SLOT(is_face()));
	
	index=0;
	vec_img.push_back("01.jpg");
	vec_img.push_back("02.jpg");
	vec_img.push_back("03.jpg");
	vec_img.push_back("04.jpg");
	vec_img.push_back("05.jpg");
	
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(img_next()));
	timer->stop();

	cascade =(CvHaarClassifierCascade*)cvLoad("haarcascade_frontalface_alt.xml",0,0,0); 
	storage =cvCreateMemStorage(0);
}
PainterWidget::~PainterWidget()
{

}

void PainterWidget::img_next()
{
	index++;
	index=index%5;
	update();
}
void PainterWidget::img_last()
{
	index--;
	if(index==-1)
		index=4;
	update();
}
void PainterWidget::is_play(){
	play=!play;
	if(play){
		timer->start(wait_sec);
		btn_play->setText("Stop");
	}
	else{
		timer->stop();
		btn_play->setText("Start");
	}
	update();
}
void PainterWidget::slower(){
	double rate;
	timer->stop();
	wait_sec= wait_sec*2;
	rate=1000.0/wait_sec;
	label->setText(QString::number(rate)+"X");
	if(play){
		timer->start(wait_sec);
	}
	update();
}
void PainterWidget::faster(){
	double rate;
	timer->stop();
	wait_sec= wait_sec/2;
	rate=1000.0/wait_sec;
	label->setText(QString::number(rate)+"X");
	if(play){
		timer->start(wait_sec);
	}
	update();
}
void PainterWidget::paintEvent(QPaintEvent*event) {
	IplImage* img_temp=cvLoadImage(vec_img[index],1);
	if(face){
		face_detect(img_temp,cascade,storage);
	}
	QImage img;
	cvxCopyIplImage(img_temp,img);
	QPainter  painter(this);
	QRectF target(0, 0, 800, 400);
	QRectF source(0, 0, img.width(), img.height());
	painter.drawImage(target, img, source);
}
void PainterWidget::is_face(){
	face=!face;
	if(face){
		btn_face->setText("On");
	}
	else{
		btn_face->setText("Off");
	}
}

void PainterWidget::face_detect(IplImage* img,CvHaarClassifierCascade *cascade,CvMemStorage *storage){
	if( !cascade ){
	    fprintf(stderr, "ERROR: Could not load classifier cascade\n");
	}
	// Be careful size of detection area
	CvSeq* faces=cvHaarDetectObjects(img,cascade,storage,1.1,2,CV_HAAR_DO_CANNY_PRUNING,cvSize(0,0),cvSize(100,100));
	CvPoint pt1, pt2;
	if(faces){
	    for(int a = 0 ; a < faces->total ; a++){
			CvRect* rectangle = (CvRect*)cvGetSeqElem(faces, a);
			pt1.x = rectangle->x;
			pt2.x = rectangle->x + rectangle->width;
			pt1.y = rectangle->y;
			pt2.y = rectangle->y + rectangle->height;
			CvRect rect1 = cvRect(pt1.x,pt1.y,rectangle->width,rectangle->height);			
			cvRectangle(img, pt1, pt2, CV_RGB(255,0,0), 3, 8, 0);
		}	
	}
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
