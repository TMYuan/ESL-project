#ifndef TEST_CAMSHIFT_H
#define TEST_CAMSHIFT_H

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
		void load_opening();

	public:
		PainterWidget();
		~PainterWidget();
		void object_tracking();
		void mouseMoveEvent(QMouseEvent *event);
		void mousePressEvent(QMouseEvent *event);
		void mouseReleaseEvent(QMouseEvent *event);

	private:
		QTimer *timer;

		IplImage* pImage;
		IplImage* target;
		IplImage* target_hsv;
		IplImage* target_hue;
		IplImage* result;
		IplImage* mask;
		IplImage* small;
		IplImage* small_temp;
		CvHistogram* hist;
		CvRect windowIn;
		CvBox2D track_box;
		CvConnectedComp track_comp;
		CvCapture* pCapture;
		int nFrmNum;
		int fps, frameH, frameW, fourcc;
		int rate;
		int HistogramBins;
		int x1, y1, x2, y2;
		float GrayLevelRange[2];
		float *HistogramRange[1];
		bool menu_exit, is_press, detect_exit;
		CvMemStorage *storage;
		int game_state;
		vector<CvPoint> pt;
		CvPoint pt_temp;
		bool start_throw;
		int temp_count;
};

#endif // WIDGET_H
