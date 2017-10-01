/*
 * imageTrans.cpp
 *
 *  Created on: Sep 14, 2016
 *      Author: odroid
 */

#include "imageTrans.h"
#include "trace.h"

using namespace std;
using namespace cv;

void warpFfine(cv::Mat &inputIm, cv::Mat &tempImg, float angle)
{
	CV_Assert(!inputIm.empty());
	Mat inputImg;
	inputIm.copyTo(inputImg);
	float radian = (float)(angle / 180.0 * CV_PI);
	int uniSize = (int)(max(inputImg.cols, inputImg.rows) * 1.414);
	int dx = (int)(uniSize - inputImg.cols) / 2;
	int dy = (int)(uniSize - inputImg.rows) / 2;
	copyMakeBorder(inputImg, tempImg, dy, dy, dx, dx, BORDER_CONSTANT);
	Point2f center((float)(tempImg.cols / 2), (float)(tempImg.rows / 2));
	Mat affine_matrix = getRotationMatrix2D(center, angle, 1.0);
	warpAffine(tempImg, tempImg, affine_matrix, tempImg.size());
	float sinVal = fabs(sin(radian));
	float cosVal = fabs(cos(radian));
	Size targetSize((int)(inputImg.cols * cosVal + inputImg.rows * sinVal),
					(int)(inputImg.cols * sinVal + inputImg.rows * cosVal));
	int x = (tempImg.cols - targetSize.width) / 2;
	int y = (tempImg.rows - targetSize.height) / 2;
	Rect rect(x, y, targetSize.width, targetSize.height);
	tempImg = Mat(tempImg, rect);
}

std::string get_time()
{
	time_t time_ptr;
	time(&time_ptr);
	tm *ptm = gmtime(&time_ptr);
	char date[60] = {0};
	sprintf(date, "%d-%02d-%02d--%02d:%02d:%02d", (int)ptm->tm_year + 1900,
			(int)ptm->tm_mon + 1, (int)ptm->tm_mday, (int)ptm->tm_hour,
			(int)ptm->tm_min, (int)ptm->tm_sec);
	return std::string(date);
}

std::vector<std::string> state_str;

void drawImage(cv::Mat &image)
{
	char temp_text[50];
	int char_to_recognition = 0;

	static Point pt_src_center(image.cols / 2, image.rows / 2);
	sprintf(temp_text, "target=%d", target_num);
	putText(image, temp_text, Point(10, 60), FONT_HERSHEY_SIMPLEX, 0.6,
			CV_RGB(255, 0, 0), 2);
	sprintf(temp_text, "Pit=%.2f", Pitch);
	putText(image, temp_text, Point(10, 80), FONT_HERSHEY_SIMPLEX, 0.6,
			CV_RGB(255, 0, 0), 2);
	sprintf(temp_text, "Yaw=%.2f", Yaw);
	putText(image, temp_text, Point(10, 100), FONT_HERSHEY_SIMPLEX, 0.6,
			CV_RGB(255, 0, 0), 2);
	sprintf(temp_text, "Roll=%.2f", Roll);
	putText(image, temp_text, Point(10, 120), FONT_HERSHEY_SIMPLEX, 0.6,
			CV_RGB(255, 0, 0), 2);
	sprintf(temp_text, "  %d", state_num);
	putText(image, state_str[state_num] + temp_text, Point(160, 20),
			CV_FONT_HERSHEY_TRIPLEX, 0.6, CV_RGB(0, 255, 0), 1, 8);

	putText(image, number_position_send.number_, Point(10, 140),
			CV_FONT_HERSHEY_TRIPLEX, 0.6, CV_RGB(255, 0, 0), 1, 8);

	sprintf(temp_text, "pt=%d %d", number_position_send.position_.x,
			number_position_send.position_.y);
	putText(image, temp_text, Point(10, 160), FONT_HERSHEY_SIMPLEX, 0.6,
			CV_RGB(255, 0, 0), 2);

	cv::line(image, pt_src_center - Point(10, 0), pt_src_center + Point(10, 0),
			 CV_RGB(0, 255, 0), 2);
	cv::line(image, pt_src_center - Point(0, 10), pt_src_center + Point(0, 10),
			 CV_RGB(0, 255, 0), 2);
}

void init()
{
	state_str.resize(30, "none");
	state_str[SG_LOW_CHECK] = "G_LC";
	state_str[SG_MID_CHECK] = "G_MC";
	state_str[SU_UP1] = "U_UP1";
	state_str[SU_HOLD] = "U_HOLD";
	state_str[SD_RETRY_UP] = "R_UP";
	state_str[SD_RETRY_UP_HOLD] = "R_HOLD";
	state_str[SD_CHECK_TARGET] = "CHK_TARGET";
	state_str[SD_FLY_TARGET] = "FLY_TARGET";
	state_str[TO_START] = "TO_START";

	state_str[SD_HOLD] = "D_HOLD";
	state_str[SD_MISS_SEARCH] = "D_MISS";
	state_str[SD_HOLD2] = "D_HOLD2";
	state_str[SD_HIGH_FAST_DOWN] = "D_FastD";
	state_str[SD_CIRCLE_SLOW_DOWN] = "D_CSD";
	state_str[SD_CIRCLE_HOLD] = "D_CHOLD";
	state_str[SD_CIRCLE_MID_DOWN] = "D_CMD";
	state_str[SD_CHECK_G] = "D_GC";
	state_str[SD_SHUT_DOWN] = "SHUT_DOWN";
	state_str[SD_SAFE] = "D_SAFE";
	state_str[HOLD_BACK] = "HOLD_BACK";
	state_str[BREAK] = "BREAK";
	state_str[SHUT] = "SHUT";
	state_str[GO_HOME] = "GO_HOME";
}

void startWriteVideo(cv::VideoWriter &video_writer)
{
	string user_path = expand_user("~");
	string video_num_path(user_path + "/workspace/characterRecognition/video/video_num.txt");

	int video_num = 0;
	std::ifstream video_num_read;
	video_num_read.open(video_num_path.c_str());
	video_num_read >> video_num;
	video_num_read.close();

	cout << video_num << endl;

	std::ofstream video_num_write;
	video_num_write.open(video_num_path.c_str());
	video_num_write << (video_num + 1);
	video_num_write.close();

	if (video_writer.isOpened())
	{
		video_writer.release();
	}

	std::stringstream ss;
	string video_name;

	ss << video_num;
	ss >> video_name;
	video_name += ".avi";

	video_writer.open(user_path + "/workspace/characterRecognition/video/" + video_name,
					  CV_FOURCC('D', 'I', 'V', 'X'), 15, Size(320, 240));
}

std::string expand_user(std::string path)
{
	if (not path.empty() and path[0] == '~')
	{
		assert(path.size() == 1 or path[1] == '/'); // or other error handling
		char const *home = getenv("HOME");
		if (home or ((home = getenv("USERPROFILE"))))
		{
			path.replace(0, 1, home);
		}
		else
		{
			char const *hdrive = getenv("HOMEDRIVE"),
					   *hpath = getenv("HOMEPATH");
			assert(hdrive); // or other error handling
			assert(hpath);
			path.replace(0, 1, std::string(hdrive) + hpath);
		}
	}
	return path;
}