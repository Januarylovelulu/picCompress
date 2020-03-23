#ifndef IMGCONTROLBASE_H
#define IMGCONTROLBASE_H

#include <QObject>
#include <QThread>

class ImgControlBase : public QThread
{
    Q_OBJECT
public:
    explicit ImgControlBase(){}

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
};

#endif // IMGCONTROLBASE_H
