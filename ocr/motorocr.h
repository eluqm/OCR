#ifndef MOTOROCR_H
#define MOTOROCR_H
#include "segmentador.h"
#include "reconocedor.h"

class MotorOcr
{
public:
    string drawimage;
    segmentador segmentadorFILE;
    reconocedor reconFILE;
    MotorOcr();

    void startSegment(QString str);
    void startRec(QString name,int numletras);
    string getnameNewFile();
    void setDrawImage(string str);
};

#endif // MOTOROCR_H
