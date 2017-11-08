/*
 * main.cpp
 *
 *  Created on: 2017年10月25日
 *      Author: yfji
 */

#include "DigitDetector.h"
#include "TwoLayerNNFaster.h"

cv::Mat biMap, not_biMap;

vector<std::tuple<int, double, cv::Rect>> findPrintDigitAreas(TwoLayerNNFaster &nn, DigitDetector &detector, Mat &frame)
{
	detector.binarize(frame, biMap);
	cv::bitwise_not(biMap, not_biMap);
	vector<Rect> digits = detector.getCandidateBndBoxes(not_biMap);
	vector<std::tuple<int, double, cv::Rect>> res;
	for (size_t i = 0; i < digits.size(); ++i)
	{
		Rect &r = digits[i];
		Mat roi = biMap(r);
		int pred;
		double prob;
		nn.predict(roi, pred, prob);
		if (pred >= 0 and pred < 10 and prob > 0.9)
		{
			res.push_back(std::make_tuple(pred, prob, Rect(r)));
		}
	}
	return res;
}

int ksize_dilate = 11;
int ksize_erode = 11;
Mat dilate_kernel = cv::getStructuringElement(cv::MORPH_RECT, Size(ksize_dilate, ksize_dilate), Point(-1, -1));
Mat erode_kernel = cv::getStructuringElement(cv::MORPH_RECT, Size(ksize_erode, ksize_erode), Point(-1, -1));
vector<cv::Mat> findLedDigitAreasROI(TwoLayerNNFaster &nn, DigitDetector &detector, Mat &frame)
{
	//detector.binarizeHSV(frame,biMap);
	Mat img = frame.clone();
	detector.computeSaliencyFT(img, biMap);
	cv::dilate(biMap, biMap, dilate_kernel);
	cv::erode(biMap, biMap, erode_kernel);
	// imshow("bi", biMap);
	cv::bitwise_not(biMap, not_biMap);
	vector<Rect> digits = detector.getCandidateBndBoxes(biMap);
	vector<cv::Mat> digits_roi;
	for (size_t i = 0; i < digits.size(); ++i)
	{
		Rect &r = digits[i];
		Mat roi = not_biMap(r);
		digits_roi.push_back(roi);
	}

	return digits_roi;
}

vector<std::tuple<int, double, cv::Rect>> findLedDigitAreas(TwoLayerNNFaster &nn, DigitDetector &detector, Mat &frame)
{
	//detector.binarizeHSV(frame,biMap);
	Mat img = frame.clone();
	detector.computeSaliencyFT(img, biMap);
	cv::dilate(biMap, biMap, dilate_kernel);
	cv::erode(biMap, biMap, erode_kernel);
	imshow("bi", biMap);
	cv::bitwise_not(biMap, not_biMap);
	vector<Rect> digits = detector.getCandidateBndBoxes(biMap);
	vector<std::tuple<int, double, cv::Rect>> res;
	for (size_t i = 0; i < digits.size(); ++i)
	{
		Rect &r = digits[i];
		Mat roi = not_biMap(r);
		int pred;
		double prob;
		nn.predict(roi, pred, prob);
		if (pred >= 0 and pred < 10 and prob > 0.7)
		{
			res.push_back(std::make_tuple(pred, prob, Rect(r)));
		}
	}
	return res;
}