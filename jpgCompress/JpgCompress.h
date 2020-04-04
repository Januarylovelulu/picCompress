#ifndef JPGCOMPRESS_H
#define JPGCOMPRESS_H

#include <QObject>
#include <QFile>
#include <QDataStream>
#include <QImage>
#include "ImgControlBase.h"
#include "Dct.h"
#include "Quantify.h"

const quint16 JPG_TITLE = 0xFFD8;

class JpgCompress : public ImgControlBase
{
public:
    JpgCompress(QString imgPathName);
    ~JpgCompress();

    bool compress(QString imgPathName);
    bool checkImage(QString imgPathName);

    bool readJpg(QString imgPathName);

    static bool isJPG(QString imgPathName);

protected:
    bool initialData();
    void run();

    void scanImgRGB(); // 扫描图片，为color赋值
    void rgbToYCbCr();

private:
    QString imgPathName;
    QImage *jpg;
    QVector<QVector<QVector<int>>> color; // 三维数组，color[x][y][z]，z的长度为3，z的index=0、1、2分别对应R G B或者Y Cb Cr

    bool judgePath(QString &imgPathName);
};

#endif // JPGCOMPRESS_H
