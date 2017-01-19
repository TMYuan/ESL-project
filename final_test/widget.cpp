#include "widget.h"
#include <QtGui/QApplication>
#include <QtGui/QPushButton>
#include <QtGui/QFont>
#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QImage>
#include <QtGui/QMouseEvent>
#include <QPainter>

#include <unistd.h>
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
	menu_exit = false;
	is_press = false;
	detect_exit = false;
	pImage = NULL;
	pCaptureO = NULL;
	pCaptureE = NULL;
	pCaptureM = NULL;
	state = 0;
	nFrmNum = 0;
	HistogramBins = 256;
	ROI = cvRect(0, 60, 320, 180);
	storage = cvCreateMemStorage(0);

	timer = new QTimer(this);

	if (!(pCaptureO = cvCaptureFromFile("Opening.mp4"))) {//open the video
		fprintf(stderr, "Cannot open Opening\n");
	}
	if (!(pCaptureE = cvCaptureFromFile("Enter_Video.mp4"))){ //open the video
		fprintf(stderr, "Cannot open Enter_Video\n");
	}
	if (!(pCaptureM = cvCaptureFromFile("Menu_Video.mp4"))){ //open the video
		fprintf(stderr, "Cannot open Menu_Video\n");
	}
	if (!(pCaptureT = cvCaptureFromFile("Object_Tracking.mp4"))){ //open the video
		fprintf(stderr, "Cannot open Object_Tracking\n");
	}
	lbl = new QLabel(this);
	lbl->setText("");
	lbl->setFont(QFont("Courier", 50, QFont::Bold));
	lbl->setGeometry(200, 310, 250, 90);


	connect(timer, SIGNAL(timeout()), this, SLOT(update()));

	setMouseTracking(true);
}

void PainterWidget::load_opening()
{
	if (pImage = cvQueryFrame(pCaptureO)){
		nFrmNum++;
		if (nFrmNum == 1){
			small = cvCreateImage(cvSize(800, 400), pImage->depth, pImage->nChannels);
			fps = cvGetCaptureProperty(pCaptureO, CV_CAP_PROP_FPS);
			frameW = cvGetCaptureProperty(pCaptureO, CV_CAP_PROP_FRAME_WIDTH);
			frameH = cvGetCaptureProperty(pCaptureO, CV_CAP_PROP_FRAME_HEIGHT);
			fourcc = cvGetCaptureProperty(pCaptureO, CV_CAP_PROP_FOURCC);
			rate = 1000 / fps;
			timer->start(rate);
		}
		cvResize(pImage, small, CV_INTER_AREA);
	}
	else{
		timer->stop();
		cvReleaseCapture(&pCaptureO);
		++state;
		nFrmNum = 0;
		update();
	}
}

void PainterWidget::load_menu()
{
	if (pImage = cvQueryFrame(pCaptureM)){
		nFrmNum++;
		if (nFrmNum == 1){
			small_temp = cvCreateImage(cvSize(800, 400), pImage->depth, pImage->nChannels);
			fps = cvGetCaptureProperty(pCaptureM, CV_CAP_PROP_FPS);
			frameW = cvGetCaptureProperty(pCaptureM, CV_CAP_PROP_FRAME_WIDTH);
			frameH = cvGetCaptureProperty(pCaptureM, CV_CAP_PROP_FRAME_HEIGHT);
			fourcc = cvGetCaptureProperty(pCaptureM, CV_CAP_PROP_FOURCC);
			rate = 1000 / fps;
			timer->start(rate);
		}
		cvResize(pImage, small, CV_INTER_AREA);
	}
	else{
		timer->stop();
		cvReleaseCapture(&pCaptureM);
		cvResize(small, small_temp, CV_INTER_AREA);//store the original picture
		++state;
		timer->start(100);
	}
}

void PainterWidget::load_enter()
{
	if (nFrmNum == 0){
		pImage = cvQueryFrame(pCaptureE);
		nFrmNum++;
		if (nFrmNum == 1){
			connect(timer, SIGNAL(timeout()), this, SLOT(load_enter()));
			fps = cvGetCaptureProperty(pCaptureE, CV_CAP_PROP_FPS);
			frameW = cvGetCaptureProperty(pCaptureE, CV_CAP_PROP_FRAME_WIDTH);
			frameH = cvGetCaptureProperty(pCaptureE, CV_CAP_PROP_FRAME_HEIGHT);
			fourcc = cvGetCaptureProperty(pCaptureE, CV_CAP_PROP_FOURCC);
			rate = 1000 / fps;
			timer->start(rate);
		}
		cvResize(pImage, small, CV_INTER_AREA);
	}
	else{
		cvReleaseCapture(&pCaptureE);
		timer->stop();
		/*
		pImage = cvLoadImage("shoot.jpg", -1);
		cvResize(pImage, small, CV_INTER_AREA);
		*/
		++state;
		nFrmNum = 0;
		update();
	}
}

void PainterWidget::draw_line()
{
	static int i = 0;
	static int j = 0;
	if (i<301 && j<601){
		cvResize(small_temp, small, CV_INTER_AREA);
		cvLine(small, cvPoint(1, i), cvPoint(799, i), CV_RGB(255, 255, 255), 1, CV_AA, 0);
		cvLine(small, cvPoint(j, 1), cvPoint(j, 399), CV_RGB(255, 255, 255), 1, CV_AA, 0);
		i += 15;
		j += 30;
	}
	else{
		//Load menu
		menu_exit = true;
		setMouseTracking(true);
		lbl->setText("<font color='White'>Start</font>");
		timer->stop();
	}
}

void PainterWidget::object_tracking()
{
	if (pImage = cvQueryFrame(pCaptureT)){
		nFrmNum++;
		if (nFrmNum == 1){
			small = cvCreateImage(cvSize(800, 400), pImage->depth, pImage->nChannels);
			small_temp = cvCreateImage(cvSize(800, 400), pImage->depth, pImage->nChannels);
			fps = cvGetCaptureProperty(pCaptureT, CV_CAP_PROP_FPS);
			frameW = cvGetCaptureProperty(pCaptureT, CV_CAP_PROP_FRAME_WIDTH);
			frameH = cvGetCaptureProperty(pCaptureT, CV_CAP_PROP_FRAME_HEIGHT);
			fourcc = cvGetCaptureProperty(pCaptureT, CV_CAP_PROP_FOURCC);
			rate = 1000 / fps;
			timer->start(rate);
		}
		cvResize(pImage, small, CV_INTER_AREA);

		if (detect_exit){
			cvCvtColor(small, target_hsv, CV_BGR2HSV);
			cvSplit(target_hsv, target_hue, NULL, NULL, NULL);
			cvCalcBackProject(&target_hue, result, hist);
			cvAnd(result, mask, result, 0);
			if(windowIn.width == 0 || windowIn.height == 0){
				windowIn =cvRect(0, 0, 100, 100);
			}
			cvCamShift(result, windowIn,
				cvTermCriteria(CV_TERMCRIT_EPS, 100, 1),
				&track_comp, &track_box);
			windowIn = track_comp.rect;

			//if (backproject_mode)
			//	cvCvtColor(result, pImage, CV_GRAY2BGR);

			if (small->origin)
				track_box.angle = -track_box.angle;
			cvEllipseBox(small, track_box, CV_RGB(255, 0, 0), 3, CV_AA, 0);
		}

	}
	else{
		timer->stop();
		cvReleaseCapture(&pCaptureM);
		cvResize(small, small_temp, CV_INTER_AREA);//store the original picture
		++state;
		timer->start(100);
	}
}

void PainterWidget::mouseMoveEvent(QMouseEvent *event)
{
	if (state == 2){
		if (event->x() < 600 && event->y() > 300){
			lbl->setFont(QFont("Courier", 75, QFont::Bold));
		}
		else{
			lbl->setFont(QFont("Courier", 50, QFont::Bold));
		}
	}
}

void PainterWidget::mousePressEvent(QMouseEvent *event)
{
	//is_press = true;
	if (state == 2){

	}
	else if (state == 4){
		timer->stop();
		x1 = event->x();
		y1 = event->y();
	}
}
void PainterWidget::mouseReleaseEvent(QMouseEvent *event)
{
	//is_press = false;
	if (state == 2){

	}
	else if (state == 4){
		x2 = event->x();
		y2 = event->y();
		windowIn = cvRect(x1, y1, x2 - x1, y2 - y1);
		if (!detect_exit){
			target_hsv = cvCreateImage(cvGetSize(small), IPL_DEPTH_8U, 3);
			target_hue = cvCreateImage(cvGetSize(small), IPL_DEPTH_8U, 1);
			mask = cvCreateImage(cvGetSize(small), 8, 1);
			result = cvCreateImage(cvGetSize(small), IPL_DEPTH_8U, 1);
			cvInRangeS(target_hsv, cvScalar(0, 30, MIN(10, 255), 0), cvScalar(180, 256, MAX(10, 255), 0), mask);

			cvCvtColor(small, target_hsv, CV_BGR2HSV);
			cvSplit(target_hsv, target_hue, NULL, NULL, NULL);
			cvSetImageROI(target_hue, windowIn);
			cvSetImageROI(mask, windowIn);
			hist = cvCreateHist(1, &HistogramBins, CV_HIST_ARRAY, HistogramRange, 1);
			cvCalcHist(&target_hue, hist, 0, mask);
			//caculate the histogram of target object
			detect_exit = true;
			cvResetImageROI(target_hue);
			cvResetImageROI(mask);
		}

		timer->start(rate);
	}
}
void PainterWidget::mouseDoubleClickEvent(QMouseEvent *event){
	if (state == 2){
		if (event->x() < 600 && event->y() > 300){
			lbl->setText("");
			menu_exit = false;
			nFrmNum = 0;
			++state;
			update();
		}
	}
	else if (state == 0){
		timer->stop();
		cvReleaseCapture(&pCaptureO);
		++state;
		nFrmNum = 0;
		update();
	}
	else if (state == 1){
		timer->stop();
		cvReleaseCapture(&pCaptureM);
		cvResize(small, small_temp, CV_INTER_AREA);//store the original picture
		++state;
		timer->start(100);
	}
}


PainterWidget::~PainterWidget()
{
	cvReleaseCapture(&pCaptureO);
	cvReleaseCapture(&pCaptureM);
	cvReleaseCapture(&pCaptureE);
	cvReleaseCapture(&pCaptureT);
}

void PainterWidget::paintEvent(QPaintEvent*event) {
	if (state == 0){ load_opening(); }
	else if (state == 1){ load_menu(); }
	else if (state == 2){ draw_line(); }
	else if (state == 3){ load_enter(); }
	else if (state == 4){ object_tracking(); }
	QImage img;
	cvxCopyIplImage(small, img);
	QPainter  painter(this);
	painter.drawImage(0, 0, img);
}

void PainterWidget::cvxCopyIplImage(const IplImage *pIplImage, QImage &qImage)
{   //implement the conversion function here

	if (qImage.isNull())
	{
		int w = pIplImage->width;
		int h = pIplImage->height;
		qImage = QImage(w, h, QImage::Format_RGB32);
	}

	int x, y;
	for (x = 0; x < pIplImage->width; ++x)
	{
		for (y = 0; y < pIplImage->height; ++y)
		{
			CvScalar color = cvGet2D(pIplImage, y, x);

			int r = color.val[2];
			int g = color.val[1];
			int b = color.val[0];

			qImage.setPixel(x, y, qRgb(r, g, b));
		}
	}
}


