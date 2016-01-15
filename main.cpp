#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <ctime>
#include <mutex>
#include <atomic>

#include <boost/asio/ip/udp.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/thread.hpp>

#ifdef _CV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#else
#include "utils.h"
#endif

#include "common.h"
#include "asf_stream.h"
#include "pool_send.h"

using namespace std;
using namespace cv;

void save2file(const bytearray &data, string fn)
{
	if(!data.size())
		return;

	fstream stream;
	stream.open(fn, ios_base::binary | ios_base::out);

	if(stream.is_open()){
		stream.write(&data[0], data.size());
		stream.close();
	}
}

#ifndef ARM

#include "widget.h"

#endif

int main(int argc, char* argv[])
{
	///////////////////////////////////

//	for(size_t num = 0; num < 1000; num++){
//		cout << "-------test " << num << "----------" << endl;
//		asf_stream asf;

//		bytearray d, d2;
//		d.resize(128000);

//		for(size_t i = 0; i < d.size(); i++){
//			d[i] = i;
//		}

//		vectorstream vs = asf.generate_stream(d);

//		for(size_t i = 0; i < vs.size(); i++){
//			d2 = asf.add_packet(vs[i]);
//			if(d2.size())
//				cout << d2.size() << endl;
//		}

//		if(d == d2){
//			cout << "test ok" << endl;
//		}else{
//			cout << "test fail" << endl;
//		}
//	}


	///////////////////////////////////

	VideoCapture capture(0);

	int res = 0;

	if(!capture.isOpened()){
		cout << "device not found\n";
		return 1;
	}

#ifndef ARM
	QApplication app(argc, argv);

	Widget w(&capture);
	w.show();

	res = app.exec();

#else
	pool_send pool;

#ifdef _CV
	FileStorage xml("config.xml", FileStorage::READ);
	if(xml.isOpened()){
		string address = xml["address"];
		int port = xml["port"];
		pool.set_address(address, port);
	}else{
		FileStorage xml("config.xml", FileStorage::WRITE);
		xml << "address" << pool.address() << "port" << pool.port();
	}
#endif

	boost::thread thread(boost::bind(&pool_send::run, &pool));

//	if(capture.isOpened()){
//		capture.set(CV_CAP_PROP_FRAME_WIDTH,640);
//		capture.set(CV_CAP_PROP_FRAME_HEIGHT,480);
//	}

	int key = 0;
	while(1){
		Mat m;
		capture >> m;

		pool.push_frame(m);
		//encode_frame(m, data, 20);
		//save2file(data, "test.j2k");

		_msleep(25);
		//key = waitKey(25);

		if(key > 0)
			cout << key << endl;

		if(key == 27){
			pool.close();
			break;
		}

	}

	thread.join();

#endif

	return res;
}

