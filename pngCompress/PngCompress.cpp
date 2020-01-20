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
    if(!isPng(imgPathName))
        return false;
    QFile file(imgPathName);
    if (!file.open(QIODevice::ReadOnly)){
        return false;
    }
    QDataStream in(&file);
    quint8 s;
    QString str;
    for (int x=0;x<8;x++) {
        in>>s;
        str+=QString::number(s)+" ";
    }
    qDebug()<<"";
    qDebug()<<"文件署名 : "<<str;

    while(!in.atEnd()){
        qint32 tmp,useless;
        in>>tmp>>useless;
        QString str;
        while(useless!=0){
            QChar s=useless%256;
            useless/=256;
            str=s+str;
        }
        str+=" : ";
        while(tmp!=0){
            in>>s;
            str+=QString::number(s)+" ";
            tmp--;
        }
        qDebug()<<"";
        qDebug()<<str;
        in>>useless;
    }

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

    if(isPng(imgPathName)){
        this->imgPathName = imgPathName;
        return imgPathName;
    }
    else {
        return "";
    }
}

bool PngCompress::isPng(QString &imgPathName)
{
#ifdef Q_OS_UNIX
    QFile file(imgPathName.remove("file://"));
#else
    QFile file(imgPathName.remove("file:///"));
#endif
    if (!file.open(QIODevice::ReadOnly)){
        return false;
    }
    QDataStream in(&file);
    quint64 n;
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
