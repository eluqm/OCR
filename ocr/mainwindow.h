#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include "segmentador.h"
#include "motorocr.h"
#include "paramocr.h"
#include "readfile.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    //segmentador imaseg;
    MotorOcr engine;
    paramOCR parametros;
    readFile fileLEctor;


    vector<vector<int> > db;
    cv::KNearest knn;
    cv::Mat vectortrain;
    cv::Mat vectortrainlabels;
    cv::Mat vectortrainsimple;
    cv::Mat vectortrainlabelssimple;
    //para normalizar apartir de la base de datos
    vector<pair<float,float> > minmax;
    cv::Mat vectortest;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void loadimage();
    void segment();
    void recognition();
    void loadDB();
    string let(string str,int metodo);
    
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
