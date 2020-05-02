#include "QmlCompressControl.h"

#include <QDebug>
#include <QFile>
#include <QImage>

QmlCompressControl::QmlCompressControl()
{
    quality = 30;
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

QStringList QmlCompressControl::getImgInDirectory(QString directoryPath)
{
    QStringList result,dir,file;
    dir << ImgControlBase::qmlPath_to_QtPath(directoryPath);
    while(!dir.empty()){
        QStringList dirTmp;
        for(QString path : dir){
            file = myFile.fileInDirectory(path,true);
            dirTmp += myFile.directoryInDirectory(path,true);
            for(QString tmp : file){
                QString s = tmp.split(".").last().toLower();
                if(s == "jpg" || s == "jpeg" || s == "png"){
                    tmp = "file://" + tmp;
                    result << tmp;
                }
            }
            file.clear();
        }
        dir = dirTmp;
    }
    return result;
}

void QmlCompressControl::setQuality(int quality)
{
    this->quality = quality;
}

void QmlCompressControl::push(QString imgPathName)
{
    imgPathNameList.push_back(imgPathName);
}

void QmlCompressControl::clear()
{
    imgPathNameList.clear();
}

bool QmlCompressControl::isDirectory(QString path)
{
    QFileInfo file(ImgControlBase::qmlPath_to_QtPath(path));
    if(file.isDir())
        return true;
    else
        return false;
}

bool QmlCompressControl::checkImage(QString imgPathName)
{
    // 如果名称为空或者不是png文件
    if(ImgControlBase::getImgType(imgPathName)==ImgControlBase::PNG && PngCompress::isPng(ImgControlBase::qmlPath_to_QtPath(imgPathName))){
        return true;
    }
    else if(ImgControlBase::getImgType(imgPathName)==ImgControlBase::JPG && JpgCompress::isJPG(ImgControlBase::qmlPath_to_QtPath(imgPathName))){
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
    for(QString imgPathName : imgPathNameList){
        ImgControlBase *imgCompressBase;
        switch (ImgControlBase::getImgType(imgPathName)) {
            case ImgControlBase::PNG : imgCompressBase = new PngCompress(ImgControlBase::qmlPath_to_QtPath(imgPathName));break;
            case ImgControlBase::JPG : imgCompressBase = new JpgCompress(ImgControlBase::qmlPath_to_QtPath(imgPathName),quality);;break;
            default: returnIsRuning(false, 0, imgPathNameList.size());return false;
        }
        connect(imgCompressBase,&ImgControlBase::finished,this,&QmlCompressControl::on_deleteImgControl);
        vImgCompress.push_back(imgCompressBase);
        vImgCompress.last()->start();
    }
    return true;
}

void QmlCompressControl::on_deleteImgControl()
{
    ImgControlBase *imgCompressBase = qobject_cast<ImgControlBase*>(sender());
    disconnect(imgCompressBase,&ImgControlBase::finished,this,&QmlCompressControl::on_deleteImgControl);
    delete imgCompressBase;
    for(auto &p : vImgCompress){
        if(p == imgCompressBase)
            p = nullptr;
    }
    int now = 0;
    for (int x=0;x<vImgCompress.size();x++) {
        if(!vImgCompress[x]) // 如果压缩完成
            now++;
    }

    if(now == imgPathNameList.size()){
        returnIsRuning(false, now, imgPathNameList.size());
        vImgCompress.clear();
    }
    else{
        returnIsRuning(true, now, imgPathNameList.size());
    }
}
