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
        return true;
    }
    else{
        return false;
    }
}

QString QmlCompressControl::pathNameToName(QString imgPathName)
{
    return ImgControlBase::pathNameToName(imgPathName);
}

bool QmlCompressControl::compress()
{
    if(imgPathNameList.empty())
        return false;

    returnIsRuning(true, 0, imgPathNameList.size());
    for(QString imgNamePath : imgPathNameList){
        ImgControlBase *pngCompress = new PngCompress(imgNamePath);
        connect(pngCompress,&ImgControlBase::finished,this,&QmlCompressControl::on_deleteImgControl);
        vPngCompress.push_back(pngCompress);
        vPngCompress.last()->start();
    }
    return true;
}

void QmlCompressControl::on_deleteImgControl()
{
    ImgControlBase *pngCompress=qobject_cast<ImgControlBase*>(sender());
    delete pngCompress;
    for(auto &p : vPngCompress){
        if(p == pngCompress)
            p = nullptr;
    }
    int now = 0;
    for (int x=0;x<vPngCompress.size();x++) {
        if(!vPngCompress[x]) // 如果压缩完成
            now++;
    }

    if(now == imgPathNameList.size()){
        returnIsRuning(false, now, imgPathNameList.size());
        vPngCompress.clear();
    }
    else{
        returnIsRuning(true, now, imgPathNameList.size());
    }
}
