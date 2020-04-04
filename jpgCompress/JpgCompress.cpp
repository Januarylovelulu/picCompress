#include "JpgCompress.h"

#include <QDebug>
#include <QTime>

JpgCompress::JpgCompress(QString imgPathName):
    imgPathName(imgPathName)
{
    jpg = nullptr;
}

JpgCompress::~JpgCompress()
{
    if(jpg)
        delete jpg;
}

bool JpgCompress::isJPG(QString imgPathName)
{
    QFile file(imgPathName);
    if (!file.open(QIODevice::ReadOnly)){
        return false;
    }
    QDataStream in(&file);
    quint16 n;
    in >> n;
    file.close();
    if(!(n==JPG_TITLE))
        return false;
    else
        return true;
}

bool JpgCompress::compress(QString imgPathName)
{
    Q_UNUSED(imgPathName);
    QTime time;

    time.start();
    scanImgRGB();
    qDebug()<<"扫描颜色 运行时间:"<<time.elapsed()/1000.0<<"s";

    qDebug()<<"";

    time.restart();
    rgbToYCbCr();
    qDebug()<<"RGB转YUV 运行时间:"<<time.elapsed()/1000.0<<"s";

    qDebug()<<"";

    time.restart();
    Dct::dct(color);
    qDebug()<<"DCT变换 运行时间:"<<time.elapsed()/1000.0<<"s";

    qDebug()<<"";

    time.restart();
    Quantify::quantify(color);
    qDebug()<<"图片量化 运行时间:"<<time.elapsed()/1000.0<<"s";

    return true;
}

bool JpgCompress::checkImage(QString imgPathName)
{
    if(isJPG(imgPathName))
        return true;
    else
        return false;
}

bool JpgCompress::readJpg(QString imgPathName)
{
    if(!jpg)
        jpg = new QImage();
    if(jpg->load(imgPathName))
        return true;
    else
        return false;
}

bool JpgCompress::initialData()
{
    if(!readJpg(imgPathName))
        return false;
    return true;
}

void JpgCompress::run()
{
    if(!initialData())
        return;
    compress(imgPathName);
}

void JpgCompress::scanImgRGB()
{
    for(int a=0;a<jpg->width();a++){
        color.push_back(QVector<QVector<int>>());
        for(int b=0;b<jpg->height();b++){
            color[a].push_back(QVector<int>());
            QColor c = jpg->pixelColor(a,b);
            color[a][b].push_back(c.red());
            color[a][b].push_back(c.green());
            color[a][b].push_back(c.blue());
        }
    }
}

void JpgCompress::rgbToYCbCr()
{
    for(int a=0;a<jpg->width();a++){
        for(int b=0;b<jpg->height();b++){
            int R = color[a][b][0];
            int G = color[a][b][1];
            int B = color[a][b][2];
            int Y = 0.299000*R + 0.587000*G + 0.114000*B;
            int Cb = -0.168736*R - 0.331264*G + 0.500002*B + 128;
            int Cr = 0.500000*R - 0.418688*G - 0.081312*B + 128;
            color[a][b][0] = Y;
            color[a][b][1] = Cb;
            color[a][b][2] = Cr;
        }
    }
}

bool JpgCompress::judgePath(QString &imgPathName)
{
    if(imgPathName.isEmpty()){
        if(this->imgPathName.isEmpty()){
            qDebug()<<"JpgCompress: the image's pathName is empty, return false.";
            return false;
        }
        else {
            qDebug()<<"JpgCompress: the image's pathName is empty, but class's is not empty, return true.";
            imgPathName = this->imgPathName;
        }
    }
    else {
        if(!isJPG(imgPathName)){
            qDebug()<<"JpgCompress: the path("<<imgPathName<<")is not a legal Png file, return false";
            return false;
        }
    }
    return true;
}
