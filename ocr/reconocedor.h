#ifndef RECONOCEDOR_H
#define RECONOCEDOR_H
#include <QtCore/QCoreApplication>
#include <opencv2/highgui/highgui.hpp>
#include <cv.h>
#include <highgui.h>
#include <opencv/cv.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "bforceknn.h"
#define KLASES 2
#define PIX 255
#define ITE 45
#define XAXIS 64
#define YAXIS 63
#define EPS 1.0e-9
#define SCALE[] {2,3,4,5,6,7}
//#define ROT[] {1,4,5,6}
#define ROT 4
#define SC 6
#define KGABOR 2
#define FEATURE 2
#define ALPHA "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define DBNAME "Letra"
#define LETTER 26

using namespace std;
using namespace cv;
class reconocedor
{
public:
    Mat filtros[SC*ROT];
    int siz;
    Mat grayimage;
    vector<pair<string ,string > >documentos;
    bforceKNN knnclass;

    reconocedor();
    void getKMeans(Mat &grayimage2,Mat *vect);
    void getKGaborFeatures(Mat &input,string cl,ofstream &doc,Mat &test);
    cv::Mat mkKernel(int ks, double sig, double th, double lm, double ps);
    void getGaborFeatures(Mat &input,string cl,ofstream &doc);
    void genFilters();
    void setGrayimage(Mat input);
    string recLetra();
    string checking(QString);
};

#endif // RECONOCEDOR_H
