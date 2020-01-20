#include "QmlCompressControl.h"

#include <QDebug>
#include <QFile>
#include <QImage>

QmlCompressControl::QmlCompressControl()
{

}

QString QmlCompressControl::getImgName(int index)
{
    return pathNameToName(getImgPathName(index));
}

QString QmlCompressControl::getImgPathName(int index)
{
    if(this->imgPathNameList.size()>index)
        return this->imgPathNameList.at(index);
    else
        return "";
}

QStringList QmlCompressControl::getImgPathNameList()
{
    return this->imgPathNameList;
}

void QmlCompressControl::push(QString imgPathName)
{
    imgPathNameList.push_back(imgPathName);
}

void QmlCompressControl::clear()
{
    imgPathNameList.clear();
}

bool QmlCompressControl::checkImage(QString imgPathName)
{
    // 如果名称为空或者不是png文件
    if(PngCompress::isPng(imgPathName)){
        PngCompress::compress(imgPathName);
        return true;
    }
    else{
        return false;
    }
}

QString QmlCompressControl::pathNameToName(QString imgPathName)
{
    if(imgPathName.isEmpty())
        return "";
    imgPathName = imgPathName.split("/").at(imgPathName.split("/").size()-1);
    if(imgPathName.contains(".png",Qt::CaseInsensitive))
        return imgPathName;
    else
        return "";
}
