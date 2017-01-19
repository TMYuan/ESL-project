#include "widget.h"
#include <QtGui/QApplication>
#include <QtGui/QPushButton>
#include <QtGui/QFont>
#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QImage>
#include <QtGui/QMouseEvent>
#include <QPainter>
#include <string>
#include <QMovie>

#include <unistd.h>
#include <QTimer>
#include <iostream>
#include <fstream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
using namespace cv;
using namespace std;


PainterWidget::PainterWidget()
{
	is_press = false;
	detect_exit = false;
	pImage = NULL;
	pCapture = NULL;
	state = 0;
	menu_shift = 0;
	menu_buf = 0;
	nFrmNum = 0;
	rank_shift = 0;
	HistogramBins = 256;
	ROI = cvRect(0, 60, 320, 180);
	GrayLevelRange[0] = 0;
	GrayLevelRange[1] = 255;
	HistogramRange[0] = &GrayLevelRange[0];
	menu_select = 1;
	storage = cvCreateMemStorage(0);
	/*
	ifstream fin("Rank_List.txt");
	if (!fin){
		cout << "Can't not open the Rank Page"<<endl;
	}

	for (int i = 0; i < 10; ++i){
		fin >> rank_list[i];
		fin >> rank_score[i];
	}
	*/
	
	timer = new QTimer(this);	

	if (!(opening = new QMovie("Opening.gif"))){
		cout << "Can not open the Opening";
	}
	opening->setScaledSize(QSize(800, 400));
	if (!(menu_video = new QMovie("Menu_Video.gif"))){
		cout << "Can not open the Menu_Video";
	}
	menu_video->setScaledSize(QSize(800, 400));
	if (!(enter_video = new QMovie("Enter_Video.gif"))){
		cout << "Can not open the Enter_Videp";
	}
	enter_video->setScaledSize(QSize(800, 400));

	screen = new QLabel(this);
	screen->setGeometry(0, 0, 800, 400);
	screen->setMovie(opening);
	
	lbl = new QLabel(this);
	lbl->setFont(QFont("Courier", 50, QFont::Bold));
	lbl->setGeometry(200, 310, 250, 90);

	select1 = new QLabel(this);
	select1->setFont(QFont("Courier", 25, QFont::Bold));
	select1->setGeometry(610, 50, 250, 90);

	select2 = new QLabel(this);
	select2->setFont(QFont("Courier", 25, QFont::Bold));
	select2->setGeometry(610, 125, 250, 90);

	select3 = new QLabel(this);
	select3->setFont(QFont("Courier", 25, QFont::Bold));
	select3->setGeometry(610, 200, 250, 90);
	
	for (int i = 0; i < 10; ++i){
		ranking[i] = new QLabel(this);
		ranking[i]->setFont(QFont("Courier", 50, QFont::Bold));
		ranking[i]->setGeometry(50, 50, 250, 90);
	}
	
	opening->start();
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
}

void PainterWidget::load_opening()
{
	if (opening->currentFrameNumber() > 100){
		delete opening;
		screen->setMovie(menu_video);
		menu_video->start();
		++state;
	}
}

void PainterWidget::load_menu()
{
	
	if (menu_video->currentFrameNumber() == menu_video->frameCount()-1){
		delete menu_video;
		pImage = cvLoadImage("Menu.jpg", -1);
		small = cvCreateImage(cvSize(800, 400), pImage->depth, pImage->nChannels);
		small_temp = cvCreateImage(cvSize(800, 400), pImage->depth, pImage->nChannels);
		cvResize(pImage, small, CV_INTER_AREA);
		cvResize(pImage, small_temp, CV_INTER_AREA);
		++state;
		timer->start(100);
	}
	
}

void PainterWidget::draw_line()
{
	
	static int i = 0;
	static int j = 0;
	if (i<301 && j<601){
		cvResize(small_temp, small, CV_INTER_AREA);
		cvLine(small, cvPoint(-10, i), cvPoint(1000, i), CV_RGB(255, 255, 255), 1, CV_AA, 0);
		cvLine(small, cvPoint(j, -10), cvPoint(j, 500), CV_RGB(255, 255, 255), 1, CV_AA, 0);
		i += 15;
		j += 30;
	}
	else{
		//Load menu
		setMouseTracking(true);
		lbl->setText("Start");
		select1->setText("<font color='White'>Play</font>");
		select2->setText("<font color='White'>Rank</font>");
		select3->setText("<font color='White'>Exit</font>");
		timer->stop();
	}
}

void PainterWidget::load_enter()
{
	timer->stop();
	screen->setMovie(enter_video);
	enter_video->start();
	if (enter_video->currentFrameNumber() == enter_video->frameCount() - 1){
		delete enter_video;
	}
}

void PainterWidget::ranking_page(){
	for (int i = 1; i < 10; ++i){
		if (rank_shift <= 40 * i)
			ranking[i]->setGeometry(50, 50+ rank_shift, 250, 90);
	}
	if (rank_shift <= 360){
		rank_shift += 10;
	}
}

void PainterWidget::object_tracking()
{
	if ((pImage = cvQueryFrame(pCapture))){
		nFrmNum++;
		if (nFrmNum == 1){
			fps = cvGetCaptureProperty(pCapture, CV_CAP_PROP_FPS);	
			rate = 1000 / fps;
			timer->start(rate);
		}
		cvResize(pImage, small, CV_INTER_AREA);

		if (detect_exit){
			cvCvtColor(small, target_hsv, CV_BGR2HSV);
			cvSplit(target_hsv, target_hue, NULL, NULL, NULL);
			cvCalcBackProject(&target_hue, result, hist);
			cvAnd(result, mask, result, 0);
			cvCamShift(result, windowIn,
				cvTermCriteria(CV_TERMCRIT_EPS, 1000, 1),
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
		cvReleaseCapture(&pCapture);
		cvResize(small, small_temp, CV_INTER_AREA);//store the original picture
		++state;
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
		
		if (event->x() > 600 && event->y() < 300){
			if (is_press){
				menu_shift = menu_buf + event->y() - ref_point;
				//int x_shift;
				//x_shift = menu_shift*menu_shift	*0.01;
				select1->setGeometry(610, 100 + menu_shift/2, 250, 90);
				select2->setGeometry(610, 175 + menu_shift/2, 250, 90);
				select3->setGeometry(610, 250 + menu_shift/2, 250, 90);
			}
		}
	
	}
}
void PainterWidget::mousePressEvent(QMouseEvent *event)
{
	is_press = true;
	if (state == 2){
		if (event->x() > 600 && event->y() < 300){
			ref_point = event->y();	
			select1->setFont(QFont("Courier", 25, QFont::Bold));
			select2->setFont(QFont("Courier", 25, QFont::Bold));
			select3->setFont(QFont("Courier", 25, QFont::Bold));
		}
	}
	else if (state == 4){
		timer->stop();
		x1 = event->x();
		y1 = event->y();
	}
}
void PainterWidget::mouseReleaseEvent(QMouseEvent *event)
{
	is_press = false;
	if (state == 2){
		
		menu_buf = menu_shift;
		if (menu_buf < -50){
			select3->setFont(QFont("Courier", 50, QFont::Bold));
			menu_select = 3;
		}
		else if (menu_buf > 50){
			select1->setFont(QFont("Courier", 50, QFont::Bold));
			menu_select = 1;
		}
		else {
			select2->setFont(QFont("Courier", 50, QFont::Bold));
			menu_select = 2;
		}
		
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
			select1->setText("");
			select2->setText("");
			select3->setText("");
			delete lbl;
			delete select1;
			delete select2;
			delete select3;
			nFrmNum = 0;
			setMouseTracking(false);
			++state;
		}
	}
	else if (state == 0){
		delete opening;
		screen->setMovie(menu_video);
		menu_video->start();
		++state;
	}
	else if (state == 1){
		delete menu_video;
		pImage = cvLoadImage("Menu.jpg", -1);
		small = cvCreateImage(cvSize(800, 400), pImage->depth, pImage->nChannels);
		small_temp = cvCreateImage(cvSize(800, 400), pImage->depth, pImage->nChannels);
		cvResize(pImage, small, CV_INTER_AREA);
		cvResize(pImage, small_temp, CV_INTER_AREA);
		++state;
		timer->start(70);
	}
}


PainterWidget::~PainterWidget()
{
	cvReleaseCapture(&pCapture);
	ofstream fout("Rank_List.txt");
	for (int i = 0; i < 10; ++i){
		fout << rank_list[i] << " " << rank_score;
	}
}

void PainterWidget::paintEvent(QPaintEvent*event) {

	QImage img;
	QPainter  painter(this);

	if (state == 0){ load_opening(); }
	else if (state == 1){ load_menu(); }
	else if (state == 2){ 
		draw_line(); 
		cvxCopyIplImage(small, img);
		painter.drawImage(0, 0, img);
	}
	else if (state == 3){ load_enter(); }
	else if (state == 4){ object_tracking(); }
	else if (state == 5){ ranking_page();}


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


