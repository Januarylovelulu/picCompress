#ifndef IMGCONTROLBASE_H
#define IMGCONTROLBASE_H

#include <QObject>
#include <QThread>
#include <QDebug>

class ImgControlBase : public QThread
{
    Q_OBJECT

public:
    enum ImgType {
            PNG = 1,
            JPG = 2,
            UNDEFINE
        };
        Q_ENUM(ImgType)

    explicit ImgControlBase(){}
    virtual ~ImgControlBase(){}

    virtual bool compress(QString imgPathName) = 0;
    virtual bool checkImage(QString imgPathName) = 0;

protected:
    virtual void initialData() = 0;
    virtual void run() = 0;

signals:

public:
    // 将绝对路径取名字，hideSuffix判断是否删除后缀名
    static QString pathNameToName(QString pathName, bool hideSuffix = false){
        if(pathName.isEmpty())
            return "";
        pathName = pathName.split("/").last();
        if(hideSuffix == true){
            int find = pathName.lastIndexOf('.');
            pathName.remove(find,pathName.size()-find);
        }
        return pathName;
    }

    // 获取图片类型，映射枚举
    static ImgType getImgType(QString pathName){
        if(pathName.isEmpty() || !pathName.contains('.')){
            return ImgType::UNDEFINE;
        }
        pathName = pathName.split('.').back().toLower();
        if(pathName == "png")
            return ImgControlBase::PNG;
        else if(pathName == "jpg" || pathName == "jpeg")
            return ImgControlBase::JPG;
        else
            return ImgControlBase::UNDEFINE;
    }
};

#endif // IMGCONTROLBASE_H
