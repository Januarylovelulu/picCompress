#ifndef JPGCOMPRESS_H
#define JPGCOMPRESS_H

#include <QObject>
#include <QFile>
#include <QDataStream>
#include "ImgControlBase.h"

const quint32 JPG_TITLE = 0xFFD8FFE1;

class JpgCompress : public ImgControlBase
{
public:
    JpgCompress(QString imgPathName);
    ~JpgCompress();

    bool compress(QString imgPathName);
    bool checkImage(QString imgPathName);

    static bool isJPG(QString imgPathName);

protected:
    void initialData();
    void run();

private:
    QString imgPathName;

    bool judgePath(QString &imgPathName);
};

#endif // JPGCOMPRESS_H
