#include "bforceknn.h"
#include <opencv2/highgui/highgui.hpp>
#include <cv.h>
#include <highgui.h>
#include <opencv/cv.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;
using namespace cv;

bforceKNN::bforceKNN()
{


}

float bforceKNN::distance(Mat vectinput,Mat vect)
{

    float result=0.0;
    for(int y=0;y<vectinput.cols;y++)
    {
        result+=pow(vect.at<float>(0,y)-vectinput.at<float>(0,y),2);
    }
    result=sqrt(result);

return result;
}
bool bforceKNN::myfuncion(pair<float, int> &input, pair<float, int> &input2)
{
return (input.first>input2.first);
}
bool myfuncion2(const pair<float, int> &input,const  pair<float, int> &input2)
{
return (input.first<input2.first);
}
void bforceKNN::find_k(Mat vectinput, Mat traindata, Mat labeldata, int k)
{
    //cout<<"entre a find_k"<<endl;
    vector<pair<float,int> >lista;
    for(int u=0;u<traindata.rows;u++)
    {   cv::Mat n(1,traindata.cols,CV_32F);
        for(int j=0;j<traindata.cols;j++){
        n.at<float>(0,j)=traindata.at<float>(u,j);
            }
        float dis=distance(vectinput,n);
        //cout<<"sali de distance"<<endl;
        pair<float,int> p;
        p.first=dis;
        p.second=u;
        lista.push_back(p);
    }
    sort(lista.begin(),lista.end(),myfuncion2);

    for(int t=0;t<k;t++)
    {
    //    cout<<lista[t].first<<"id "<<lista[t].second<<" ";
        cout<<(char)labeldata.at<float>(0,lista[t].second)<<endl;
    }
    cout<<"fin "<<lista.size()<<endl;

}

