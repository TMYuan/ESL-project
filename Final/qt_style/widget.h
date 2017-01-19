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
#define PI 3.14
using namespace std;
class PainterWidget : public QWidget
{
    Q_OBJECT
	protected:
        void paintEvent(QPaintEvent*);
        void drawTrace();
		void face_detect(IplImage* img,CvHaarClassifierCascade *cascade,CvMemStorage *storage);
		void cvxCopyIplImage(const IplImage *pIplImage,QImage &qImage);
	public:
		PainterWidget();
		~PainterWidget();
	private:
		QTimer *timer;
        double timeNow, height, angle, rate;
        int thickness, shift;
        CvPoint pt1, pt2;
        IplImage* img;
		CvHaarClassifierCascade *cascade;
		CvMemStorage *storage;
};

#endif // WIDGET_H
