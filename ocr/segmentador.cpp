#include "segmentador.h"
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
//#define DOCUMENT "pr2.JPG"
#define LIMITEALTURALETRA 5
int _brightness = 148;//128//98
int _contrast = 179;//189//179
double UMBRALPROM = 0.2;
double UMBRALPROM_VERT=0.2;


using namespace std;
using namespace cv;

segmentador::segmentador()
{
    //posLetter(MAXPAL,pair<int,int>(0,0));

    posLetter.resize(MAXPAL,pair<int,int>(0,0));
    posLine.resize(MAXLINES,pair<int,int>(0,0));
}
void segmentador::loadImage(QString str)
{
    //nombre de la imagen
    String str2=str.toStdString();//direccion de archivo elegido
    cv::Mat image=cv::imread(str2,1);
    nameimage=str2;
    cv::cvtColor(image,grayimage,CV_BGR2GRAY);

    lut=cv::Mat(1,256,CV_8UC1);
    cantLineas=0;
    cantLetrasGeneral=0;
    posLetter2.clear();
    posLetter.clear();
    posLine.clear();
    matrix.clear();
    matrixPal.clear();
    cantPal.clear();

}



//CvMat*lut_mat;

//cv::Mat grayimage;
//cv::Mat des_gray;



/*
         * algoritmo por Werner D. Streidt
         * (http://visca.com/ffactory/archives/5-99/msg00021.html)
        Change contrast and brightness of an image


\texttt{dst} (I) \leftarrow \texttt{lut(src(I) + d)}

where

d = \fork{0}{if \texttt{src} has depth \texttt{CV\_8U}}{128}{if \texttt{src} has depth \texttt{CV\_8S}}
*/
float segmentador::absFloat(float val){
    return (val<0) ? -val : val;
}
int segmentador::getcantLetrasGen(){return cantLetrasGeneral;}
void segmentador::graficarUnRectangulo(int X1, int Y1, int X2,int Y2){
    stringstream str;
    cv::Mat imgRoi=img2(cv::Rect(Point(X1+1,Y1+1),Point(X2,Y2)));
    cv::rectangle(img2,cvPoint(X1,Y1),cvPoint(X2,Y2),cvScalar(255,0,0),1);
    cantLetrasGeneral++;
    str<<cantLetrasGeneral;

    cv::imwrite("letras/roi"+str.str()+".jpg",imgRoi);
}
pair<int,int> segmentador::resizePal(){

    vector<vector<int> > copia_matrixPal;
    pair<int,int> retorno;
    int primero=0;
    int segundo=matrixPal.size();//0
    int suma=0;
    int i=0,j=0;
    //bool entrada=false;//aun no se detecto ceros arriba
    //por arriba
    for (i=0;i<matrixPal.size();i++){
        suma=0;
        for (j=0;j<matrixPal[i].size();j++){
            suma+=matrixPal[i][j];
        }
        if (suma==0)
            primero=i;
        else
            break;
    }

    retorno.first=primero;
    //por abajo
    for (i=matrixPal.size()-1;i>=0;i--){
        suma=0;
        for (j=0;j<matrixPal[i].size();j++){
            suma+=matrixPal[i][j];
        }
        if (suma==0)
            segundo=i;
        else
            break;
    }
    retorno.second=segundo;

    //todo esta copado
    if (primero==0 && segundo+1 >= matrixPal.size())
        return retorno;

    if (segundo-primero <= LIMITEALTURALETRA){// se descarta punto etc
        matrixPal.clear();
        return retorno;
    }
    copia_matrixPal.resize(segundo-primero);
    //Resize de memoria
    for (i=0;i< segundo-primero;i++)
        copia_matrixPal[i].resize(matrixPal[i].size());
    //copia_matrixPal.resize(segundo-primero,matrixPal[0].size(),0);
    for (i=primero;i<segundo;i++)
    {
        for (j=0;j<matrixPal[i].size();j++)
        {
            copia_matrixPal[i-primero][j]=matrixPal[i][j];
        }
    }
    matrixPal=copia_matrixPal;
    return retorno;
}
void segmentador::loadWord(int firstX,int firstY, int secondX, int secondY){
    int j,actual;
    cout<<"entre a loadword"<<endl;
    /*
    int salto=(secondX-firstX)*4.8;
    secondX=firstX+(secondX-firstX)*0.65;
    */
    firstX=(int)firstX/2;
    secondX=(int)secondX/2;
    matrixPal.resize(secondY-firstY);
    for ( j=0;j<secondY-firstY;j++)
        matrixPal[j].resize(secondX-firstX);
    for (int i=firstY;i<secondY;i++){
        for (int j=firstX;j<secondX;j++){
            //actual=absFloat(((uchar *)(dst_image->imageData+(i)*dst_image->widthStep))[j*2])/1.00000e+002;//-65//-j
            actual=absFloat(des_gray.at<uchar>(i,j*2))/1.00000e+002;//?????
            if(actual>UMBRALPIX)
                matrixPal[i-firstY][j-firstX]=0;
            else
                matrixPal[i-firstY][j-firstX]=1;
        }
    }
}

void segmentador::delnoise()
{

    int brightness = _brightness - 100;
    int contrast = _contrast - 100;
    int i;



    if( contrast > 0 )
    {
        double delta = 127.*contrast/100;
        double a = 255./(255. - delta*2);
        double b = a*(brightness - delta);
        for( i = 0; i < 256; i++ )
        {
            int v = cvRound(a*i + b);
            if( v < 0 )
                v = 0;
            if( v > 255 )
                v = 255;
            lut.at<uchar>(0,i) = (uchar)v;
        }
    }
    else
    {
        double delta = -128.*contrast/100;
        double a = (256.-delta*2)/255.;
        double b = a*brightness + delta;
        for( i = 0; i < 256; i++ )
        {
            int v = cvRound(a*i + b);
            if( v < 0 )
                v = 0;
            if( v > 255 )
                v = 255;
            lut.at<uchar>(0,i) = (uchar)v;
        }
    }
    /********************************************************************
    double alpha;
    int beta;
    cv::Mat mat2=Mat::zeros(grayimage.size(),grayimage.type());
    std::cout<<"* Enter the alpha value [1.0-3.0]: ";std::cin>>alpha;
    std::cout<<"* Enter the beta value [0-100]: "; std::cin>>beta;
    for( int y = 0; y < grayimage.rows; y++ )
    { for( int x = 0; x < grayimage.cols; x++ )
         { //for( int c = 0; c < 3; c++ )

      mat2.at<uchar>(y,x)=
         saturate_cast<uchar>( alpha*( grayimage.at<uchar>(y,x)) + beta );

    }
    }
*/
    cv::LUT(grayimage, lut, des_gray);//tranformada
    //imshow("imagen",des_gray);
    cout<<"original"<<des_gray.cols<<endl;
    cout<<"original"<<des_gray.rows<<endl;


}
void segmentador::findLine()
{   matrix.resize(des_gray.rows,vector<int>(des_gray.cols,0));

    double suma=0;
    double actual;
    double val=cv::threshold(des_gray,des_gray,0,255,CV_THRESH_BINARY|CV_THRESH_OTSU)/100.0;
    cout<<val<<endl;
    bool entrada=true;
    int i;
    cout<<"dentro de findline"<<endl;
    for (i=0;i<des_gray.rows;i++){
        suma=0;
        for (int j=0;j<des_gray.cols;j++){
            //actual=absFloat(((uchar *)(dst_image->imageData+i*dst_image->widthStep))[j/* *3+2*/])/1.00000e+002;
            actual=absFloat(des_gray.at<uchar>(i,j))/1.00000e+002;
            //cout<<actual<<endl;

            if(actual>val)
                matrix[i][j]=0;
            else
                matrix[i][j]=1;
            suma+=actual;
        }
        if (i==0)
            UMBRALPROM=suma/des_gray.cols;//primera linea

        if (entrada==true && (suma/des_gray.cols)<UMBRALPROM)//se espera linea// sepuede mejorar el umbral
        {
            entrada=false;
            posLine[cantLineas].first=i;//entrada
        }
        else
            if (entrada==false && (suma/des_gray.cols)>=UMBRALPROM){
                posLine[cantLineas++].second=i;//salida
                entrada=true;
            }
    }//de for

    if (entrada==false){
        posLine[cantLineas++].second=i-1;//ultima linea de la imagen
    }
    cout<<"lineas : "<<cantLineas<<endl;

}
void segmentador::findColum()
{
    posLetter2.resize(cantLineas);
    int i;
    for (i=0;i<cantLineas;i++){
        posLetter2[i].resize(MAXPAL);
    }

    cantPal.resize(cantLineas);
    int z=0,pal,j;
    bool entrada=true;
    double suma=0;
    double actual;

    for (pal=0;pal<cantLineas;pal++){
        for (j=0;j<grayimage.cols;j++){
            suma=0;
            //mx_all=0;
            for(i = posLine[pal].first; i <posLine[pal].second ;i++ ){
                //actual=absFloat(((uchar *)(dst_image->imageData+i*dst_image->widthStep))[j/*3+2*/])/1.00000e+002;
                actual=absFloat(des_gray.at<uchar>(i,j))/1.00000e+002;
                suma+=actual;
            }
            suma=suma/(posLine[pal].second-posLine[pal].first);
            if (j==0)
                //UMBRALPROM_VERT=suma/(posLine[pal].second-posLine[pal].first);
                UMBRALPROM_VERT=2.23124*1.13;//Umbral para letra imprenta
            // UMBRALPROM_VERT=2.23126*1.07;//umbral para letra corrida
            //UMBRALPROM_VERT=2.23126*1.12;//umbral para letra corrida
            //UMBRALPROM_VERT=2.23126*1.11;//umbral para letra corrida

            if (entrada==true && suma/*  /(posLine[pal].second-posLine[pal].first)*/<=UMBRALPROM_VERT)//se espera palabra
            {
                entrada=false,
                        posLetter2[pal][cantPal[pal]].first=j;
            }
            else
                if (entrada==false && suma/* /(posLine[pal].second-posLine[pal].first)*/>UMBRALPROM_VERT){
                    posLetter2[pal][cantPal[pal]++].second=j;
                    entrada=true;
                }
            //std::cout<<"Suma en palabra"<<pal<<" linea "<<j<<" "<<absFloat(suma/(posLine[i].second-posLine[i].first))<<endl;
        }

    }



}


//funcion que llama a calcularVecinos..tbn se usa cuando ya se tiene localizada una letra
void segmentador::preProcesarNeoGCognitron(int firstX,int firstY, int secondX, int secondY){

    if (abs(firstX-secondX)<3 || abs(firstY-secondY)<7)// ruido se descarta//umbral de descarte
        return;

    loadWord(firstX,firstY,secondX,secondY);
    int i,j;
    if (matrixPal.size()==0 || matrixPal[0].size()==0)
        return;

    pair<int,int> FactorDeReuccionCuadrado=resizePal();
    cout<<"entre.."<<endl;
    graficarUnRectangulo(firstX,firstY+FactorDeReuccionCuadrado.first,secondX,
                         firstY+FactorDeReuccionCuadrado.second);
    cout<<"salir.."<<endl;


}
void segmentador::drawRect(bool mostrarletras=NOMOSTRARLETRAS,bool mostrarlineas=MOSTRARLINEAS)
{

    img2=imread(nameimage,-1);
    int i;
    if (mostrarlineas==MOSTRARLINEAS){
        for (i=0;i<cantLineas;i++){
            cv::rectangle(img2,cvPoint(3,posLine[i].first),cvPoint(img2.cols-3,posLine[i].second),cvScalar(255,0,0),1);

        }
    }
    cout<<"antes de preprocesarNEO"<<endl;

    if (mostrarletras==MOSTRARLETRAS){
        //cout<<"entre..."<<endl;
        int z;
        for (i=0;i<cantLineas;i++){
            for (z=0;z<cantPal[i];z++){

                preProcesarNeoGCognitron(posLetter2[i][z].first,posLine[i].first,posLetter2[i][z].second,posLine[i].second);
            }

        }
    }
    cout<<"sali de preprocesarNEO"<<endl;
    namenewimage="imagedraw.jpg";
    imwrite(namenewimage,img2);

}
string segmentador::process()
{


    delnoise();
    cout<<"salide noise"<<endl;
    findLine();
    cout<<"sali de findline"<<endl;
    findColum();
    cout<<"sali de findcolum"<<endl;
    drawRect(false,true);
    cout<<"sali de draw"<<endl;

    return namenewimage;
}
