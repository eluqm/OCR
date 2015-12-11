#include "motorocr.h"


MotorOcr::MotorOcr()
{


}

void MotorOcr::startSegment(QString str)
{
    segmentadorFILE.loadImage(str);
    this->setDrawImage(segmentadorFILE.process());
}
void MotorOcr::startRec(QString name,int numpala)
{

}
string MotorOcr::getnameNewFile()
{
return drawimage;
}
void MotorOcr::setDrawImage(string n)
{
    drawimage=n;
}

