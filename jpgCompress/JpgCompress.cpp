#include "JpgCompress.h"

#include <QDebug>

JpgCompress::JpgCompress(QString imgPathName):
    imgPathName(imgPathName)
{

}

JpgCompress::~JpgCompress()
{

}

bool JpgCompress::isJPG(QString imgPathName)
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
    quint32 n;
    in >> n;
    file.close();
    if(!(n==JPG_TITLE))
        return false;
    else
        return true;
}

bool JpgCompress::compress(QString imgPathName)
{
    if(!judgePath(imgPathName)){
        return false;
    }
}

bool JpgCompress::checkImage(QString imgPathName)
{
    if(isJPG(imgPathName))
        return true;
    else
        return false;
}

void JpgCompress::initialData()
{

}

void JpgCompress::run()
{
    initialData();
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
