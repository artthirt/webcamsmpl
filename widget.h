#ifndef WIDGET
#define WIDGET

#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtCore/QTimer>
#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>
#include <QtGui/QImage>

#ifdef _CV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#else
#include "utils.h"
#endif

#include "jpeg_encode.h"

class Widget: public QWidget{
	Q_OBJECT
public:
	Widget(cv::VideoCapture* cap)
		: QWidget()
		, m_cap(cap)
	{
		connect(&m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
		m_timer.start(25);
	}

public slots:
	void timeout(){
		if(!m_cap)
			return;
		*m_cap >> m_mat;

		jpeg_encode enc;
		bytearray data;
		enc.encode(m_mat, data, 80);
		write_file("tmp.jpg", data);

		update();
	}

protected:
	virtual void paintEvent(QPaintEvent*){
		QPainter painter(this);
		QImage image;
		if(m_mat.empty())
			return;

		image = QImage(m_mat.cols, m_mat.rows, QImage::Format_ARGB32);
		for(int i = 0; i < m_mat.rows; i++){
			int index = i * m_mat.cols * 3;
			cv::Vec3b *sc = (cv::Vec3b*)&m_mat.data[index];
			QRgb* sco = (QRgb*)image.scanLine(i);
			for(int j = 0; j < m_mat.cols; j++){
				sco[j] = qRgb(sc[j][2], sc[j][1], sc[j][0]);
			}
		}
		image = image.scaled(QSize(width(), height()), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

		painter.drawImage(0, 0, image);
	}

private:
	cv::VideoCapture* m_cap;
	QTimer m_timer;
	cv::Mat m_mat;
};

#endif // WIDGET

