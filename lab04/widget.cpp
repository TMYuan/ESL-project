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
    btn_next=new QPushButton(this);
    btn_next->setText(">>");
	btn_next->setFont(QFont("Courier", 18, QFont::Bold));
	btn_next->setGeometry(650,100,110,50);
	QObject::connect(btn_next, SIGNAL(clicked()), this, SLOT(img_next()));

	index=0;
	
	QImage img= QImage("01.jpg");
	vec_img.push_back(img);
	img= QImage("02.jpg");
	vec_img.push_back(img);
	img= QImage("03.jpg");
	vec_img.push_back(img);
	img= QImage("04.jpg");
	vec_img.push_back(img);
	img= QImage("05.jpg");
	vec_img.push_back(img);
	/*
		code here
	*/
	btn_last=new QPushButton(this);
	btn_last->setText("<<");
	btn_last->setFont(QFont("Courier", 18, QFont::Bold));
	btn_last->setGeometry(500,100,110,50);
	QObject::connect(btn_last, SIGNAL(clicked()), this, SLOT(img_last()));

	update();
}
PainterWidget::~PainterWidget()
{
	
}
void PainterWidget::img_next()
{
	/*
		code here
	*/
	if(index<4){
		++index;
	}
	update(); //call paintEvent(QPaintEvent*event) function
}
void PainterWidget::img_last()
{
	/*
		code here
	*/
	if(index>0){
		--index;
	}
	update(); //call paintEvent(QPaintEvent*event) function
}
void PainterWidget::paintEvent(QPaintEvent*event) {
	QPainter painter(this);
	painter.drawImage(0,0,vec_img[index]);
}
