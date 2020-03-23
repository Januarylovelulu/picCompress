#include "PngCompress.h"

#include <QImage>
#include <QFile>
#include <QMap>
#include "JQZopfli.h"
#include <QDebug>

PngCompress::PngCompress(QString imgPathName):imgPathName(imgPathName)
{
//    getPathName(imgPathName);
//    LzCompress ls;
//    ls.Compress(imgPathName.toStdString());
//    std::string filename = imgPathName.toStdString();
//    filename.append(".fzip");
//    ls.UnCompress(filename);
}

PngCompress::~PngCompress()
{

}

bool PngCompress::compress(QString imgPathName)
{
    if(!judgePath(imgPathName))
        return false;

//    bool isWrite = false;
//    if(str == "IHDR" || str == "PLTE" || str == "IDAT" || str == "IEND" || str=="tRNS"){
//        isWrite = true;
//    }

    JQZopfli::optimize(imgPathName,imgPathName.replace(".png","_Compressed.png",Qt::CaseInsensitive));

    return true;
}

QString PngCompress::getPathName()
{
    if(!judgePath(imgPathName))
        return "";
    else
        return imgPathName;
}

void PngCompress::setPathName(QString imgPathName)
{
    if(isPng(imgPathName)){
        this->imgPathName = imgPathName;
    }
    else {
        qDebug()<<"PngCompress："<<imgPathName<<"不是一个有效的Png图片,setPathName失败。";
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

bool PngCompress::savePng(QString imgPathName)
{
    QFile file(imgPathName);
    if (!file.open(QIODevice::WriteOnly)){
        return false;
    }
    QDataStream out(&file);

    return true;
}

bool PngCompress::checkImage(QString imgPathName)
{
    if(isPng(imgPathName)){
        return true;
    }
    else
        return false;
}

bool PngCompress::readPng(QString imgPathName)
{
    QFile file(imgPathName);
    if (!file.open(QIODevice::ReadOnly)){
        return false;
    }
    QDataStream in(&file);
    QString str;
    quint64 s64;
    in>>s64;
    str+=QString::number(s64)+" ";
//    qDebug()<<"";
//    qDebug()<<"文件署名 : "<<str;

    quint8 s;
    while(!in.atEnd()){
        qint32 tmp,name,crc;
        in>>tmp>>name;
        QString str;
        while(name!=0){
            QChar s=name%256;
            name/=256;
            str=s+str;
        }
        bool isIdat = false;
        QVector<quint8> idat;
        if(str == "IDAT"){
            isIdat = true;
            idat.resize(tmp);
        }
        str+=" : ";
        while(tmp!=0){
            in>>s;
            if(isIdat)
                idat[idat.size()-tmp]=s;
            str+=QString::number(s)+" ";
            tmp--;
        }
        if(isIdat)
            idatList.push_back(idat);
//        qDebug()<<"";
//        qDebug()<<str;
        in>>crc;
    }
    qDebug()<<idatList.size()<<" "<<idatList[0].size();

    file.close();
    return true;
}

void PngCompress::initialData()
{

}

void PngCompress::run()
{
    initialData();
    compress(imgPathName);
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
    else {
        if(!isPng(imgPathName)){
            qDebug()<<"PngCompress: the path("<<imgPathName<<")is not a legal Png file, return false";
            return false;
        }
    }
    return true;
}
