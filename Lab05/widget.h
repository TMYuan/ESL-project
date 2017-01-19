#ifndef WIDGET_H
#define WIDGET_H

#include <QtGui/QFont>
#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QImage>
#include <QtGui/QPushButton>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
using namespace std;
class PainterWidget : public QWidget
{
    Q_OBJECT
	protected:
        void paintEvent(QPaintEvent*);
	public slots:
		void img_next();
		void img_last();
		void is_play();
		void faster();
		void slower();
		void is_face();

		void face_detect(IplImage* img,CvHaarClassifierCascade *cascade,CvMemStorage *storage);
		void cvxCopyIplImage(const IplImage *pIplImage,QImage &qImage);
	public:
		PainterWidget();
		~PainterWidget();
	private:
		QTimer *timer;
		QPushButton *btn_next;
		QPushButton *btn_last;
		QPushButton *btn_play;
		QPushButton *btn_fast, *btn_slow;
		QPushButton *btn_face;
		QLabel *label;
		bool play, face;
		double wait_sec;
		vector<char*> vec_img;
		int index;
		CvHaarClassifierCascade *cascade;
		CvMemStorage *storage;
};

#endif // WIDGET_H
