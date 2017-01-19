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
	start_throw = false;
	//detect_exit = false;
	is_press = false;
	pImage = NULL;
	pCapture = NULL;
	nFrmNum = 0;
	GrayLevelRange[0] = 0;
	GrayLevelRange[1] = 255;
	HistogramRange[0] = &GrayLevelRange[0];
	HistogramBins = 256;
	storage = cvCreateMemStorage(0);
	pt_temp.x = 0;
	pt_temp.y = 0;
	game_state = 0;
	temp_count = 0;

	timer = new QTimer(this);
	

	if (!(pCapture = cvCaptureFromCAM(0))) {//open the video
		fprintf(stderr, "Cannot open Video\n");
	}

	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	update();
}

void PainterWidget::load_opening()
{
	if ((pImage = cvQueryFrame(pCapture))){
		nFrmNum++;
		if(nFrmNum == 1){
			small = cvCreateImage(cvSize(800, 400), pImage->depth, pImage->nChannels);
			fps = cvGetCaptureProperty(pCapture, CV_CAP_PROP_FPS);
			rate = 1000 / fps;
			timer->start(rate);
			cout<<"Depth : "<<small->depth<<endl;
			cout<<"Channels : "<<small->nChannels<<endl;
		}
		cvResize(pImage, small, CV_INTER_AREA);

		if (game_state == 1){
			cvCvtColor(small, target_hsv, CV_BGR2HSV);
			cvSplit(target_hsv, target_hue, NULL, NULL, NULL);
			cvCalcBackProject(&target_hue, result, hist);
			//cvAnd(result, mask, result, 0);
			cvCamShift(result, windowIn,cvTermCriteria(CV_TERMCRIT_EPS, 10, 1),&track_comp, &track_box);
			//cout<<"windowIn width before:"<<windowIn.width<<endl;
			//cout<<"windowIn height before:"<<windowIn.height<<endl;
			windowIn = track_comp.rect;
			cout<<"windowIn width after:"<<windowIn.width<<endl;
			cout<<"windowIn height after:"<<windowIn.height<<endl;
			cout<<"track box size:"<<track_box.size.width<<"x"<<track_box.size.height<<endl;
			//if (backproject_mode)
			//	cvCvtColor(result, pImage, CV_GRAY2BGR);

			//if (small->origin){track_box.angle = -track_box.angle;}
			cout<<"Center.x : "<< track_box.center.x <<endl;
			cout<<"Center.y : "<< track_box.center.y <<endl;
			if(temp_count % 3 == 2){
				if(pt.empty()){
					pt.push_back(pt_temp);
				} else{
					if(start_throw){
						if(pt[pt.size()-1].x + 5 < pt_temp.x){
							pt.push_back(pt_temp);
						}
						else{
							temp_count = 0;
							start_throw = false;
							game_state = 2;
						}
					}
					else{
						if(pt[pt.size()-1].x + 5 < pt_temp.x){
							pt.push_back(pt_temp);
							start_throw = true;
						}
						else if(pt[pt.size()-1].x < track_box.center.x){
							pt[pt.size()-1] = pt_temp;
						}
					}
				}
				pt_temp.x = 0;
				pt_temp.y = 0;
				++temp_count;
			}
			else{
				if(pt_temp.x == 0 && pt_temp.y == 0){
					pt_temp.x = (int)track_box.center.x;
					pt_temp.y = (int)track_box.center.y;
				}
				else{
					pt_temp.x = (int)((pt_temp.x + track_box.center.x)/2);
					pt_temp.y = (int)((pt_temp.y + track_box.center.y)/2);
				}
				++temp_count;
			}
			cvEllipseBox(small, track_box, CV_RGB(255, 0, 0), 3, CV_AA, 0);
		}
		else if(game_state == 2){
			for(int i = 0; i < pt.size(); ++i){
				cout << "pt[" << i << "] : " << pt[i].x << " and "<< pt[i].y << endl;
			}
			timer->stop();
		}
	}
	else{
		timer->stop();
		disconnect(timer, SIGNAL(timeout()), this, SLOT(load_opening()));
		cvReleaseCapture(&pCapture);
	}
}


void PainterWidget::mouseMoveEvent(QMouseEvent *event)
{

}
void PainterWidget::mousePressEvent(QMouseEvent *event)
{
	is_press = true;
	timer->stop();
	x1 = event->x();
	y1 = event->y();
}
void PainterWidget::mouseReleaseEvent(QMouseEvent *event)
{
	is_press = false;
	x2 = event->x();
	y2 = event->y();
	windowIn = cvRect(x1, y1, x2 - x1, y2 - y1);
	if (game_state == 0){
		target_hsv = cvCreateImage(cvGetSize(small), small->depth, small->nChannels);
		target_hue = cvCreateImage(cvGetSize(small), small->depth, 1);
		//mask = cvCreateImage(cvGetSize(small), small->depth, 1);
		result = cvCreateImage(cvGetSize(small), small->depth, 1);
		//cvInRangeS(target_hsv, cvScalar(0, 30, MIN(10, 255), 0), cvScalar(180, 256, MAX(10, 255), 0), mask);	
		cvCvtColor(small, target_hsv, CV_BGR2HSV);
		cvSplit(target_hsv, target_hue, NULL, NULL, NULL);
		cvSetImageROI(target_hue, windowIn);
		//cvSetImageROI(mask, windowIn);
		hist = cvCreateHist(1, &HistogramBins, CV_HIST_ARRAY, HistogramRange, 1);
		cvCalcHist(&target_hue, hist);
		//caculate the histogram of target object
		cvResetImageROI(target_hue);
		//cvResetImageROI(mask);
		game_state = 1;
	}

	timer->start(rate);
}


PainterWidget::~PainterWidget()
{

}

void PainterWidget::paintEvent(QPaintEvent*event) {
	load_opening();

	QImage img;
	cvxCopyIplImage(small,img);
	QPainter  painter(this);
	painter.drawImage(0,0,img);
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


