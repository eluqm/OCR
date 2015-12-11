#include "reconocedor.h"
#include <QtCore/QCoreApplication>
#include <opencv2/highgui/highgui.hpp>
#include <cv.h>
#include <highgui.h>
#include <opencv/cv.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
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

reconocedor::reconocedor()
{
    ifstream tn("check.txt");
    if(!tn){cout<<"error al abrir el archivo"<<endl;}
    string feature;
    string label;
    int count=0;
    while(!tn.eof()){


            if(tn.eof()){break;}
            tn>>feature;
            tn>>label;
            documentos.push_back(pair<string,string>(feature,label));


        if(tn.eof()){break;}
        //tn>>label;

        count++;
    }

    siz=5;
    tn.close();
}
void reconocedor::setGrayimage(Mat input)
{
    grayimage=input;
}
void reconocedor:: getKMeans(Mat &grayimage2,Mat *vect)
{

    double centrs[KLASES];
    double min, max;
    int minInd, maxInd;
    cv::minMaxIdx(grayimage2,&min,&max,&minInd,&maxInd,Mat());

    for(int l=0;l<KLASES;l++)
    {
         centrs[l]=((l+1.0)*max)/(l+2.0);//centroides

    }

    int p=0;
    double dist;
    float temp;
    int label;
    int val[KLASES];
    int numpix[KLASES];
    int image2[grayimage2.rows][grayimage2.cols];
    while(p<ITE)//numero de iteraciones
    {   for(int ki=0;ki<KLASES;ki++)
        {
            val[ki]=0;
            numpix[ki]=0;
        }

        for(int i=0;i<grayimage2.rows;i++){
            for(int j=0;j<grayimage2.cols;j++)
            {
               dist=100000.0;
               temp=0;
               label=-1;


                for(int k=0;k<KLASES;k++)
                {
                        temp=abs((int)grayimage2.at<uchar>(i,j)-centrs[k]);

                        if(temp<dist){dist=temp;label=k;}

                }
                        numpix[label]++;

                        val[label]+=(int)grayimage2.at<uchar>(i,j);


                        if(p==(ITE-1))
                        {
                            image2[i][j]=label;
                        }

            }




        }


        for(int f=0;f<KLASES;f++){
            centrs[f]=(val[f]+0.0)/(numpix[f]+0.0);


       }



                p++;
    }



    string str="esd";
    for(int b=0;b<KLASES;b++)
    {
        vect[b]=grayimage2.clone();
        vect[b]*=0;
    }


        for(int i=0;i<grayimage2.rows;i++){
            for(int j=0;j<grayimage2.cols;j++)
            {
                //matchin con la imagen original
                vect[image2[i][j]].at<uchar>(i,j)=grayimage2.at<uchar>(i,j);

            }
            }

        str="image";
    for(int m2=0;m2<KLASES;m2++)
    {
        Mat let=vect[m2].clone();
        cv::equalizeHist(let,vect[m2]);               //equalizando cada imagen
        vect[m2].convertTo(vect[m2],CV_32F,1.0/255,0);//transformacion


    }


}
void reconocedor::getKGaborFeatures(Mat &input,string cl,ofstream &doc,cv::Mat &test)
{   Mat imvect;
    Scalar mean1;//media
    Scalar dev1;//desviacion standar


    stringstream str;
    str<<cl;
    Mat Kimage[KLASES];//imagenes clusterizadas
    this->getKMeans(input,Kimage);
   for(int m1=0;m1<SC*ROT;m1++)//SC*ROT FILTROS
    {
       stringstream str2;
       str2.str("");
       str2<<m1;
       stringstream str3;


       for(int m2=0;m2<KLASES;m2++){
           str3.str("");
           str3<<m2;
           cv::filter2D(Kimage[m2], imvect, CV_32FC1, filtros[m1]);
           cv::meanStdDev(imvect,mean1,dev1);



        doc<<mean1.val[0]<<" ";
        test.at<float>(0,((m1*4)+(KLASES*m2)))=mean1.val[0];
        doc<<dev1.val[0]<<" ";
        test.at<float>(0,((m1*4)+(KLASES*m2)+1))=dev1.val[0];
       }
    }
    doc<<cl[0]<<endl;



}
void reconocedor::getGaborFeatures(Mat &input,string cl,ofstream &doc)
{   Mat imvect;
    Scalar mean1;
    Scalar dev1;


    stringstream str;
    str<<cl;
    for(int m1=0;m1<SC*ROT;m1++)
    {
        cv::filter2D(input, imvect, CV_32FC1, filtros[m1]);
        meanStdDev(imvect,mean1,dev1);
        doc<<mean1.val[0]<<" ";
        doc<<dev1.val[0]<<" ";

    }
    doc<<cl[0]<<endl;



}
cv::Mat reconocedor:: mkKernel(int ks, double sig, double th, double lm, double ps)
{
    int hks = (ks-1)/2;

    double theta =th;
    double psi = ps*CV_PI/180;

    double del=1;
    double lmbd = lm;

    double sigma=sig;
    double x_theta;
    double y_theta;

    cv::Mat kernel(ks,ks, CV_32F);
    for (int y=-hks; y<=hks; y++)
    {
        for (int x=-hks; x<=hks; x++)
        {
            x_theta = x*del*cos(theta)+y*del*sin(theta);
            y_theta = -x*del*sin(theta)+y*del*cos(theta);
            kernel.at<float>(hks+y,hks+x) = (float)exp(-0.5*(pow(x_theta,2)+pow(y_theta,2))/pow(sigma,2))* cos(2*CV_PI*x_theta/lmbd + psi);
        }
    }
    return kernel;
}
void reconocedor::genFilters()
{
    float scal[SC];
    float rotations[ROT];
    for(int n=0;n<ROT;n++)
    {
        rotations[n]=(n*CV_PI)/16;
    }
    for(int m=0;m<SC;m++){
        scal[m]=1.5*(pow(1.2,m-1));
    }

    for(int m1=0;m1<SC;m1++)
    {
        stringstream str2;
        stringstream str3;
        str2.str("");
        str2<<m1;
        for(int n1=0;n1<ROT;n1++)
        {

            str3.str("");
            str3<<n1;
            filtros[m1*(ROT) + n1]=mkKernel(siz,scal[m1],rotations[n1],(scal[m1]*CV_PI)/2,0);
        }
    }

}

string reconocedor::checking(QString filename)
{

    string name=filename.toStdString();
    int found= name.rfind('/');
    cout<<found<<endl;
    name=name.substr(found+1,name.size()-(name.size()-found));
    for(int u=0;u<documentos.size();u++)
    {
        if(documentos[u].second==name){name=documentos[u].first;}
    }
    return name;
}


