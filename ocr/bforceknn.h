#ifndef BFORCEKNN_H
#define BFORCEKNN_H
#include <QtCore/QCoreApplication>
#include <opencv2/highgui/highgui.hpp>
#include <cv.h>
#include <highgui.h>
#include <opencv/cv.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace cv;
using namespace std;
class bforceKNN
{
public:
    bforceKNN();
    float distance(cv::Mat vectinput,cv::Mat vect);
    bool myfuncion(pair<float,int> &input,pair<float,int>&input2);
    void find_k(cv::Mat vectinput,cv::Mat traindata,cv::Mat labeldata,int k);


};

#endif // BFORCEKNN_H
