#ifndef SEGMENTADOR_H
#define SEGMENTADOR_H
#include <QtCore/QCoreApplication>
#include <opencv2/highgui/highgui.hpp>
#include <cv.h>
#include <highgui.h>
#include <opencv/cv.h>
#include <ml.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>
#define ROT 4
#define SC 6
#define FEATURE 2
#define KLASES 2
#define LETTER 26

#define MAXLINES 30
#define MAXPAL      100
#define UMBRALPIX       0.1

#define VECINOSCONPRIORIDAD  true
#define VECINOSMETODOSIMPLE  false

#define MOSTRARLINEAS false
#define MOSTRARLETRAS false

#define NOMOSTRARLETRAS false
#define NOMOSTRARLINEAS false
using namespace std;
using namespace cv;
class segmentador
{

private:
public:
    cv::Mat grayimage;//scala de grises
    cv::Mat imtextformreal;//imagen real para mostrar
    cv::Mat des_gray;
    cv::Mat lut;
    int cantLineas;
    int cantLetrasGeneral;
    string namenewimage;
    string nameimage;
    cv::Mat img2;
    vector<vector<int> > matrix;
    vector<pair<int,int> > posLetter;
    vector<vector<pair<int,int> > >posLetter2;
    vector<vector<int>    > matrixPal;
    vector<int> cantPal;
    vector<pair<int,int> > posLine;

    segmentador();
    void loadImage(QString file);
    float absFloat(float val);
    string process();
    void drawRect(bool mostrarletras,bool mostrarlineas);
    void preProcesarNeoGCognitron(int firstX,int firstY, int secondX, int secondY);
    void findColum();
    void findLine();
    void delnoise();
    void loadWord(int firstX,int firstY, int secondX, int secondY);
    pair<int,int> resizePal();
    void graficarUnRectangulo(int X1, int Y1, int X2,int Y2);
    int getcantLetrasGen();
};

#endif // SEGMENTADOR_H
