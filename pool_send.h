#ifndef POOL_SEND
#define POOL_SEND

#include <vector>
#include <string>
#include <atomic>
#include <mutex>
#include <queue>

#include <boost/thread.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/placeholders.hpp>

#include "common.h"

#ifndef _CV
#include "utils.h"
#else
#include <opencv2/core/core.hpp>
#endif

#include "asf_stream.h"

template< int C >
class thread_pool{
public:
	class thread_one{
	public:
		boost::thread thread;
		bool done;
		std::atomic_bool doit;

		thread_one(){
			done = false;
			doit = false;
			thread = boost::thread(boost::bind(&thread_one::run, this));
		}
		void run(){
			while(!done){
				if(doit){

				}else{
					_msleep(2);
				}
			}
		}
		void close(){
			done = true;
		}
	};
};

struct Frame{
	Frame();
	Frame(const cv::Mat& mat);
	Frame(const Frame& frame);
	void run();

	boost::thread thread;
	int compression;
	cv::Mat image;
	bytearray data;
	bool done;
};

class pool_send{
public:
	pool_send();
	~pool_send();

	void set_address(const std::string& address, ushort port);
	std::string address() const;
	u_short port() const;

	void push_frame(const cv::Mat& mat);
	void run();
	void run2();
	void close();

	void check_frames();
	void send_data(const bytearray& data);
	void write_handler(boost::system::error_code error, size_t size);

private:
	bool m_done;
	std::queue< Frame > m_pool;
	std::mutex m_mutex;
	int m_serial;
	asf_stream m_asf;
	boost::asio::ip::udp::socket *m_socket;
	boost::asio::ip::udp::endpoint m_destination;
	boost::asio::io_service m_io;
};

#endif // POOL_SEND

