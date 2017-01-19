#ifndef WIDGET_H
#define WIDGET_H

#include <QtGui/QFont>
#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QImage>
#include <QtGui/QPushButton>
#include <QtGui/QMouseEvent>
#include <vector>
#include <string>
#include <QMovie>

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
		QLabel *lbl, *select1, *select2, *select3;
		QMovie *opening, *menu_video, *enter_video;
		QLabel *ranking[10];
		QLabel *screen;
		void load_opening();
		void load_menu();
		void load_enter();
		void draw_line();
		void ranking_page();
		void object_tracking();
		void mouseMoveEvent(QMouseEvent *event);
		void mousePressEvent(QMouseEvent *event);
		void mouseReleaseEvent(QMouseEvent *event);
		void mouseDoubleClickEvent(QMouseEvent *event);

	private:
		QTimer *timer;
		IplImage* pImage;
		IplImage *target, *target_hsv, *target_hue, *result, *mask;
		IplImage *small, *small_temp;
		CvHistogram* hist;
		CvRect windowIn, ROI;
		CvBox2D track_box;
		CvConnectedComp track_comp;
		CvCapture *pCapture;
		int nFrmNum, fps, ref_point, menu_shift, menu_buf, rate, state, HistogramBins, x1, y1, x2, y2;
		int menu_select;
		int rank_shift;	
		float GrayLevelRange[2];
		float *HistogramRange[1];
		bool is_press, detect_exit;
		string rank_list[10];
		int rank_score[10];
		CvHaarClassifierCascade *cascade;
		CvMemStorage *storage;
		
};

#endif // WIDGET_H
