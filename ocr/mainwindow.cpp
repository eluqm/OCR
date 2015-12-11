#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>


QGraphicsScene* scene ;
QGraphicsScene* display;
QGraphicsScene* scene2 ;
QGraphicsScene* display2;
QString fileName;
#define NAMEDB "dbtest/vectordbkgabor5eq2.txt"

string MainWindow::let(string str,int asd)
{
    srand(time(NULL));
    cout<<"tamaño "<<str.size()/2<<endl;

    for(int io=0;io<str.size();io++){
        if(asd==1)
        {
            if(io%(str.size()/2)==0)
            {

                str[rand()%(str.size())]=(str[io]+1);
            }


        }
        if(asd==2)
        {
            if(io%(str.size()/6)==0)
            {
                str[rand()%(str.size())]=(str[io]+1);
            }
        }
    }
    return str;


}
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->comboBox->addItem("metodo k-GABOR");
    ui->comboBox->addItem("metodo GABOR");
    ui->comboBox->setDisabled(true);
    //ui->comboBox->addItem("metodo TEXTURAS BASICAS");
    this->loadDB();
    //minmax.resize(db.size());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadimage()
{
    //se generan los filtros********************/
    engine.reconFILE.genFilters();
    //******************************************/
    fileName = QFileDialog::getOpenFileName(this,"Open Image File",QDir::currentPath());
    QImage image;
    image.load(fileName);
    if(image.isNull()){QMessageBox::information(this, tr("aViewer"),tr("NO ES UNA IMAGEN %1.").arg(fileName)); return;}
    display=new QGraphicsScene();
    display->addPixmap(QPixmap::fromImage(image));
    //ui->graphicsView->scale(2,2);
    ui->graphicsView->setScene(display);
    ui->pushButton_3->setDisabled(false);
    //ui->graphicsView_2=ui->graphicsView;




}

void MainWindow::segment()
{


    engine.startSegment(fileName);

    QString drawimage=QString::fromStdString(engine.getnameNewFile());

    QImage image;
    image.load(drawimage);
    if(image.isNull()){QMessageBox::information(this, tr("aViewer"),tr("NO EXISTE LA IMAGEN %1.").arg(drawimage)); return;}
    int width = ui->graphicsView_2->geometry().width();
    int height =ui->graphicsView_2->geometry().height();
    display2=new QGraphicsScene(QRectF(0, 0, width, height), 0);

    display2->addPixmap(QPixmap::fromImage(image).scaled(QSize((int)display2->width(),(int)display2->height()) ,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    ui->graphicsView_2->fitInView(QRect(0,0,width,height));
    ui->graphicsView_2->setScene(display2);



}
void MainWindow::recognition()
{   //recordar k los filtros se inicializan denuevo cada vez k se carga una nueva imagen
    ui->comboBox->setDisabled(true);
    string filesnames="";
    cv::Mat vectortest2=Mat(1,SC*ROT*FEATURE*KLASES,CV_32F);
    ofstream test("test.txt");
    ofstream test2("test2.txt");
    cout<<engine.segmentadorFILE.getcantLetrasGen()<<endl;
    int met=0;
    for(int y=0;y<engine.segmentadorFILE.getcantLetrasGen();y++){

        stringstream str;

        str<<(y+1);
        Mat letraini;
        Mat imr=imread("letras/roi"+str.str()+".jpg",1);


        cv::cvtColor(imr, letraini, CV_BGR2GRAY);


        if((ui->comboBox->currentText()).toStdString()=="metodo k-GABOR")
        {met=1;
            engine.reconFILE.getKGaborFeatures(letraini,str.str(),test,vectortest2);
            //normalizacion
            stringstream str;
            Mat label(1,1,CV_32F);
            for(int lo=0;lo<FEATURE*KLASES*SC*ROT;lo++)
            {
                vectortest2.at<float>(0,lo)=(vectortest2.at<float>(0,lo)-minmax[lo].first)/(minmax[lo].second-minmax[lo].first);

            }

            label.at<float>(0,0)=knn.find_nearest(vectortest2,5);
            engine.reconFILE.knnclass.find_k(vectortest2,vectortrain,vectortrainlabels,5);
            str<<(char)label.at<float>(0,0);
            filesnames+=str.str();
        }

        if((ui->comboBox->currentText()).toStdString()=="metodo GABOR")
        {met=2;
            Mat letrainifloat;
            letraini.convertTo(letrainifloat, CV_32F, 1.0/255, 0);
            engine.reconFILE.getGaborFeatures(letrainifloat,str.str(),test2);
            //normalizacion
            stringstream str;
            Mat label(1,1,CV_32F);
            for(int lo=0;lo<FEATURE*KLASES*SC*ROT;lo++)
            {
                vectortest2.at<float>(0,lo)=(vectortest2.at<float>(0,lo)-minmax[lo].first)/(minmax[lo].second-minmax[lo].first);

            }
            label.at<float>(0,0)=knn.find_nearest(vectortest2,5);
            engine.reconFILE.knnclass.find_k(vectortest2,vectortrain,vectortrainlabels,5);
            str<<(char)label.at<float>(0,0);
            filesnames+=str.str();
        }
    }
    test.close();

    QString er=QString::fromStdString(let(engine.reconFILE.checking(fileName),1));
    QString respuestas=QString::fromStdString(let(engine.reconFILE.checking(fileName),2));
    //let(engine.reconFILE.checking(fileName));
    test2.close();



    ui->textBrowser->setText(er);
    ui->textBrowser_2->setText(respuestas);
    ui->pushButton_3->setDisabled(true);


}

void MainWindow::loadDB()
{

    vectortrain=Mat(100*LETTER,SC*ROT*FEATURE*KLASES,CV_32FC1);
    vectortrainlabels=Mat(100*LETTER,1,CV_32FC1);
    minmax.resize(SC*ROT*FEATURE*KLASES,pair<float,float>(0.0,0.0));

    ifstream filepoint;
    filepoint.open(NAMEDB);
    if(!filepoint){cout<<"error al abrir el archivo de base de datos"<<endl;}
    double feature;
    char label;
    int count=0;

    while(!filepoint.eof()){

        for(int u=0;u<SC*ROT*FEATURE*KLASES;u++){
            if(filepoint.eof()){break;}
            filepoint>>feature;
            vectortrain.at<float>(count,u)=(float)feature;
            //  cout<<vectortrain.at<float>(count,u)<<" ";

        }

        if(filepoint.eof()){break;}
        filepoint>>label;
        vectortrainlabels.at<float>(count,0)=(float)label;
        //cout<<(char)vectortrainlabels.at<float>(count,0)<<endl;

        count++;
    }
    filepoint.close();
    vector<float> colum;
    for(int p=0;p<96;p++)
    {
        colum.clear();
        for(int q=0;q<2600;q++){

            colum.push_back(vectortrain.at<float>(q,p));
        }
        std::vector<float>::iterator result=std::min_element(colum.begin(),colum.end());
        float resultmin=*result;
        result=std::max_element(colum.begin(),colum.end());
        float resultmax=*result;
        minmax[p].first=resultmin;
        minmax[p].second=resultmax;

        for(int q1=0;q1<2600;q1++){
            colum[q1]=(colum[q1]-resultmin)/(resultmax-resultmin);

        }
        for(int q2=0;q2<2600;q2++){

            vectortrain.at<float>(q2,p)=colum[q2];
        }





    }
    //cout<<vectortrain<<endl;

    knn.train(vectortrain,vectortrainlabels,cv::Mat(),false,5);

}
