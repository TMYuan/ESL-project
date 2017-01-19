#ifndef WIDGET_H
#define WIDGET_H

#include <QtGui/QFont>
#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QImage>
#include <QtGui/QPushButton>
#include <QtGui/QMouseEvent>
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
		void cvxCopyIplImage(const IplImage *pIplImage,QImage &qImage);


	public:
		PainterWidget();
		~PainterWidget();
		QLabel* lbl;
		void load_opening();
		void load_menu();
		void load_enter();
		void draw_line();
		void object_tracking();
		void mouseMoveEvent(QMouseEvent *event);
		void mousePressEvent(QMouseEvent *event);
		void mouseReleaseEvent(QMouseEvent *event);
		void mouseDoubleClickEvent(QMouseEvent *event);

	private:
		QTimer *timer;

		IplImage* pImage;
		IplImage *target, *target_hsv, *target_hue, *result, *mask;
		IplImage* small;
		IplImage* small_temp;
		CvHistogram* hist;
		CvRect windowIn;
		CvBox2D track_box;
		CvConnectedComp track_comp;
		CvCapture* pCaptureO, *pCaptureM, *pCaptureE, *pCaptureT;
		int nFrmNum;
		int fps, frameH, frameW, fourcc;
		CvRect ROI;
		int rate;
		int state;
		int HistogramBins;
		int x1, y1, x2, y2;
		float GrayLevelRange[2] = { 0, 255 };
		float *HistogramRange[1] = { &GrayLevelRange[0] };
		bool menu_exit, is_press, detect_exit;
		CvHaarClassifierCascade *cascade;
		CvMemStorage *storage;
		
};

#endif // WIDGET_H
