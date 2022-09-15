#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <dlib/opencv.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <librealsense2/rs.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <signal.h>

using namespace dlib;
using namespace std;
using namespace cv;

#define FACE_DOWNSAMPLE_RATIO 4
#define SKIP_FRAMES 60
#define BUF_SIZE 100
#define NAME_SIZE 20
#define ARR_CNT 5

const char* ON = "[1]ON";
const char* OFF = "[1]OFF";
char name[NAME_SIZE]="[Default]";
char name_msg[NAME_SIZE + BUF_SIZE +1];
char msg[BUF_SIZE];
double xTurn, yTurn, zTurn;
float dist_to_center;
int ISPStart = 0;

void * recv_msg(void * arg);
void error_handling(char * msg);

void draw_polyline(cv::Mat &img, const dlib::full_object_detection& d, const int start, const int end, bool isClosed = false) {
	std::vector <cv::Point> points;
	for (int i = start; i <= end; ++i) {
		points.push_back(cv::Point(d.part(i).x(), d.part(i).y()));
	}
	//    cv::polylines(img, points, isClosed, cv::Scalar(255,0,0), 2, 16);
	cv::polylines(img, points, isClosed, cv::Scalar(47, 255, 173), 2, 16);
}

void render_face (cv::Mat &img, const dlib::full_object_detection& d) {

	DLIB_CASSERT
		(
		 d.num_parts() == 68,
		 "\t std::vector<image_window::overlay_line> render_face_detections()"
		 << "\n\t Invalid inputs were given to this function. "
		 << "\n\t d.num_parts():  " << d.num_parts()
		);

	draw_polyline(img, d, 0, 16);           // Jaw line
	draw_polyline(img, d, 17, 21);          // Left eyebrow
	draw_polyline(img, d, 22, 26);          // Right eyebrow
	draw_polyline(img, d, 27, 30);          // Nose bridge
	draw_polyline(img, d, 30, 35, true);    // Lower nose
	draw_polyline(img, d, 36, 41, true);    // Left eye
	draw_polyline(img, d, 42, 47, true);    // Right Eye
	draw_polyline(img, d, 48, 59, true);    // Outer lip
	draw_polyline(img, d, 60, 67, true);    // Inner lip
}

std::vector<cv::Point3d> get_3d_model_points() {
	std::vector<cv::Point3d> modelPoints;

	modelPoints.push_back(cv::Point3d(0.0f, 0.0f, 0.0f)); //The first must be (0,0,0) while using POSIT
	modelPoints.push_back(cv::Point3d(0.0f, -330.0f, -65.0f));
	modelPoints.push_back(cv::Point3d(-225.0f, 170.0f, -135.0f));
	modelPoints.push_back(cv::Point3d(225.0f, 170.0f, -135.0f));
	modelPoints.push_back(cv::Point3d(-150.0f, -150.0f, -125.0f));
	modelPoints.push_back(cv::Point3d(150.0f, -150.0f, -125.0f));

	return modelPoints;
}

std::vector<cv::Point2d> get_2d_image_points(full_object_detection &d) {
	std::vector<cv::Point2d> image_points;
	image_points.push_back( cv::Point2d( d.part(30).x(), d.part(30).y() ) );    // Nose tip
	image_points.push_back( cv::Point2d( d.part(8).x(), d.part(8).y() ) );      // Chin
	image_points.push_back( cv::Point2d( d.part(36).x(), d.part(36).y() ) );    // Left eye left corner
	image_points.push_back( cv::Point2d( d.part(45).x(), d.part(45).y() ) );    // Right eye right corner
	image_points.push_back( cv::Point2d( d.part(48).x(), d.part(48).y() ) );    // Left Mouth corner
	image_points.push_back( cv::Point2d( d.part(54).x(), d.part(54).y() ) );    // Right mouth corner
	return image_points;
}

cv::Mat get_camera_matrix(float focal_length, cv::Point2d center) {
	cv::Mat camera_matrix = (cv::Mat_<double>(3,3) << focal_length, 0, center.x, 0 , focal_length, center.y, 0, 0, 1);
	return camera_matrix;
}

/*****************************************/

int main(int argc, char* argv[]) {
	int sock;
	struct sockaddr_in serv_addr;
	char s_msg[NAME_SIZE + BUF_SIZE+2];

	pthread_t recv_id = 0;

	/*make socket*****************************************************************/
	if(argc != 4) {
		printf("Usage : %s <IP> <port> <name>\n", argv[0]);
		exit(1);
	}

	sprintf(name, "%s", argv[3]);
	sock = socket(PF_INET, SOCK_STREAM, 0);

	if(sock == -1) error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connect() error");

	sprintf(msg, "[%s:PASSWD]", name);
	write(sock, msg, strlen(msg));

	/*thread**********************************************************************/
	pthread_create(&recv_id, NULL, recv_msg, (void*)&sock);
	pthread_detach(recv_id);

	/*ISP*************************************************************************/
	try {
		/*distance****************************************************************/
		rs2::pipeline p;
		rs2::config cfg;
		cfg.enable_stream(RS2_STREAM_DEPTH, 640, 480, RS2_FORMAT_Z16, 30);
		cfg.enable_stream(RS2_STREAM_COLOR, 640, 480, RS2_FORMAT_BGR8, 30); //change RGB->BGR
		p.start(cfg);
		rs2::frameset frames;

		cv::Mat temp_small;
		cv::Mat temp_display;

		frontal_face_detector detector = get_frontal_face_detector();
		shape_predictor pose_model;
		deserialize("../shape_predictor_68_face_landmarks.dat") >> pose_model;

		int count = 0;

		while(1){
			while(ISPStart) {
				// Grab a frame

				/*distance********************************************************/
				frames = p.wait_for_frames();
				rs2::depth_frame depth = frames.get_depth_frame();
				auto width = depth.get_width();
				auto height = depth.get_height();
				dist_to_center = depth.get_distance(width / 2, height / 2);

				rs2::frame frame1 = frames.get_color_frame();
				Mat temp(Size(640, 480), CV_8UC3,(void*)frame1.get_data(), cv::Mat::AUTO_STEP);

				cv::resize(temp, temp_small, cv::Size(), 1.0/FACE_DOWNSAMPLE_RATIO, 1.0/FACE_DOWNSAMPLE_RATIO);

				cv_image<bgr_pixel> cimg(temp);
				cv_image<bgr_pixel> cimg_small(temp_small);

				std::vector<dlib::rectangle> faces;
				// Detect faces
				if(count % SKIP_FRAMES == 0) {
					faces = detector(cimg_small);
				}

				// Find the pose of each face.
				std::vector<full_object_detection> shapes;
				full_object_detection shape;

				if(faces.size() != 0)
					for (unsigned long i = 0; i < faces.size(); ++i) {
						dlib::rectangle r(
								(long)(faces[i].left() * FACE_DOWNSAMPLE_RATIO),
								(long)(faces[i].top() * FACE_DOWNSAMPLE_RATIO),
								(long)(faces[i].right() * FACE_DOWNSAMPLE_RATIO),
								(long)(faces[i].bottom() * FACE_DOWNSAMPLE_RATIO)
								);
						shape = pose_model(cimg, r);

						render_face(temp, shape);
						//find point 34
						cout << "No.34" << shape.part(33) << endl;

						/**********solvePnP*************/
						std::vector<cv::Point2d> image_points = get_2d_image_points(shape);
						std::vector<cv::Point3d> model_points = get_3d_model_points();
						double focal_length = temp.cols;
						cv::Mat camera_matrix = get_camera_matrix(focal_length, cv::Point2d(temp.cols/2, temp.rows/2));
						cv::Mat dist_coeffs = cv::Mat::zeros(4, 1, cv::DataType<double>::type);
						cv::Mat rotation_vector;
						cv::Mat translation_vector;

						cv::solvePnP(model_points, image_points, camera_matrix, dist_coeffs, rotation_vector, translation_vector);

						std::vector<cv::Point3d> nose_end_point3D;
						std::vector<cv::Point2d> nose_end_point2D;
						nose_end_point3D.push_back(cv::Point3d(0, 0, 1000.0));

						cv::projectPoints(nose_end_point3D, rotation_vector, translation_vector, camera_matrix, dist_coeffs, nose_end_point2D);
						cv::line(temp,image_points[0], nose_end_point2D[0], cv::Scalar(47, 255, 173), 2);

						/********EulerAngle********/
						cv::Mat rotation_matrix;
						cv::Rodrigues(rotation_vector, rotation_matrix);
						// get eulerAngles
						cv::Vec3d eulerAngles;
						cv::Mat cameraMatrix, rotMatrix, transVect, rotMatrixX, rotMatrixY, rotMatrixZ;
						double *_r = rotation_matrix.ptr<double>();
						double projMatrix[12] = {_r[0], _r[1], _r[2], 0,
							_r[3], _r[4], _r[5], 0,
							_r[6], _r[7], _r[8], 1};

						cv::decomposeProjectionMatrix(cv::Mat(3, 4, CV_64FC1, projMatrix),
								cameraMatrix,
								rotMatrix,
								transVect,
								rotMatrixX,
								rotMatrixY,
								rotMatrixZ,
								eulerAngles);

						xTurn = eulerAngles[0];
						yTurn = eulerAngles[1];
						zTurn = eulerAngles[2];

						//value
						std::cout << "x : " << xTurn << std::endl;
						std::cout << "y : " << yTurn << std::endl;
						std::cout << "z : " << zTurn << std::endl;
						std::cout << "x : " << (int)shape.part(33).x() << std::endl;
						std::cout << "y : " << (int)shape.part(33).y() << std::endl;

						std::cout << "distance : " << dist_to_center << std::endl;

						/*send value**********************************************/
						memset(s_msg,0,sizeof(s_msg));

						int x_face = (int)shape.part(33).x();
						int y_face = (int)shape.part(33).y();

						//Up, Down, Left, Right
						if(y_face < 220) strcat(s_msg,"[3]U");
						else if(y_face > 250) strcat(s_msg,"[3]D");
						else strcat(s_msg,"[3]N");

						if(x_face < 327) strcat(s_msg,"@R");
						else if(x_face > 353) strcat(s_msg,"@L");
						else strcat(s_msg,"@N");

						//Far, Close
						if(dist_to_center > 0.315) strcat(s_msg, "@F\n");
						else if(dist_to_center < 0.265) strcat(s_msg, "@C\n");
						else strcat(s_msg,"@N\n");

						if(write(sock, s_msg, strlen(s_msg))<=0) {
							error_handling("write error");
						}
					}
				cv::imshow("OpenCV Landmark Detector", temp);
				int k = cv::waitKey(100);
			}
			cv::destroyAllWindows();
		}
	}
	catch(serialization_error& e) {
		cout << "You need dlib's default face landmarking model file to run this example." << endl;
		cout << "You can get it from the following URL: " << endl;
		cout << "   http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2" << endl;
		cout << endl << e.what() << endl;
	}
	catch(exception& e) {
		cout << e.what() << endl;
	}
}

void * recv_msg(void * arg) {
	int * sock = (int *)arg;
	int str_len;
	char name_msg[NAME_SIZE + BUF_SIZE+2];

	while(1) {
		memset(name_msg, 0x0, sizeof(name_msg));
		int str_len = read(*sock, name_msg, NAME_SIZE + BUF_SIZE );
		if(str_len <= 0) {
			*sock = -1;
			return NULL;
		}
		name_msg[str_len] = 0;
		fputs(name_msg, stdout);
		if(!strncmp(name_msg, ON, 5)) {
			cout << "ON" << endl;
			ISPStart = 1;
		}
		else if(!strncmp(name_msg, OFF, 6)) {
			cout << "OFF" << endl;
			ISPStart = 0;
		}
	}
}

void error_handling(char * msg) {
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
