#ifndef PNGCOMPRESS_H
#define PNGCOMPRESS_H

#include <QObject>
#include <QThread>
#include <QVector>
#include "ImgControlBase.h"

const quint64 PNG_TITLE = 0x89504E470D0A1A0A;

class PngCompress : public ImgControlBase
{
    Q_OBJECT
public:
    PngCompress(QString imgPathName);
    ~PngCompress();

public:
    bool compress(QString imgPathName = "");

    // 获取处理后的pathName,并复制到class内的imgPathName，为空则不是png，或者名字有错
    // 参数为空时，获取class内的imgPathName
    QString getPathName();
    void setPathName(QString imgPathName);

    static bool isPng(QString &imgPathName);

    bool savePng(QString imgPathName);

    bool checkImage(QString imgPathName);

protected:
    bool readPng(QString imgPathName = "");

    void initialData();
    void run();

private:
    QString imgPathName;

    QVector<QVector<quint8>> idatList;

    bool judgePath(QString &imgPathName);
};

#endif // PNGCOMPRESS_H
