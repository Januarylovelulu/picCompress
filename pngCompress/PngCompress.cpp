#include "PngCompress.h"

#include <QImage>
#include <QFile>
#include <QMap>
#include <QDebug>

PngCompress::PngCompress()
{

}

PngCompress::PngCompress(QString imgPathName)
{
    getPathName(imgPathName);
}

bool PngCompress::compress(QString imgPathName)
{
    if(imgPathName.isEmpty())
        return false;
    QFile file(imgPathName.remove("file://"));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return false;
    }
    QDataStream in(&file);
    quint32 n;
    QImage p_w_picpath;
    QMap<QString, QColor> map;

    in >> n >> p_w_picpath >> map;

    qDebug()<<n;
    qDebug()<<p_w_picpath;
    qDebug()<<map;

    file.close();
    return true;
}

bool PngCompress::compress()
{
    if(!judgePath(imgPathName))
        return false;
    return compress(imgPathName);
}

QString PngCompress::getPathName(QString imgPathName)
{
    if(!judgePath(imgPathName))
        return "";
#ifdef Q_OS_UNIX
    QFile file(imgPathName.remove("file://"));
#else
    QFile file(imgPathName.remove("file:///"));
#endif
    if(isPng(imgPathName)){
        this->imgPathName = imgPathName;
        return imgPathName;
    }
    else {
        return "";
    }
}

bool PngCompress::isPng(QString imgPathName)
{
#ifdef Q_OS_UNIX
    QFile file(imgPathName.remove("file://"));
#else
    QFile file(imgPathName.remove("file:///"));
#endif
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return false;
    }
    QDataStream in(&file);
    quint32 n;
    in >> n;
    file.close();
    if(!(n==PNG_TITLE))
        return false;
    else
        return true;
}

bool PngCompress::judgePath(QString &imgPathName)
{
    if(imgPathName.isEmpty()){
        if(this->imgPathName.isEmpty()){
            qDebug()<<"PngCompress: the image's pathName is empty, return false.";
            return false;
        }
        else {
            qDebug()<<"PngCompress: the image's pathName is empty, but class's is not empty, return true.";
            imgPathName = this->imgPathName;
        }
    }
    return true;
}
