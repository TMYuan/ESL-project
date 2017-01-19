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
	public:
		PainterWidget();
		~PainterWidget();
		
    
	private:
		QPushButton *btn_next;
		QPushButton *btn_last;
		vector<QImage> vec_img;
		int index;
};

#endif // WIDGET_H
