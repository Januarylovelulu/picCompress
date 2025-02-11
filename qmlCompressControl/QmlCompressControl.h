#ifndef QMLCOMPRESSCONTROL_H
#define QMLCOMPRESSCONTROL_H

#include <QObject>
#include <QVector>
#include "ImgControlBase.h"
#include "PngCompress.h"
#include "JpgCompress.h"
#include "MyFile.h"

class QmlCompressControl : public QObject
{
    Q_OBJECT

public:
    explicit QmlCompressControl();

    Q_INVOKABLE QString getImgName(int index);
    Q_INVOKABLE QString getImgPathName(int index);
    Q_INVOKABLE QStringList getImgPathNameList();

    Q_INVOKABLE QStringList getImgInDirectory(QString directoryPath);

    Q_INVOKABLE void setQuality(int quality);

    Q_INVOKABLE void push(QString imgPathName);
    Q_INVOKABLE void clear();

    Q_INVOKABLE bool isDirectory(QString path);

    // 判断文件是否是正常的图片文件，如果不是则直接返回false
    Q_INVOKABLE bool checkImage(QString imgPathName);

    // 全路径转图片名，如果不是png返回空，是png返回图片名称
    Q_INVOKABLE QString pathNameToName(QString imgPathName);

    Q_INVOKABLE bool compress();

signals:
    void returnIsRuning(const bool &isRuning, const int now, const int total,
                        QString time="" ,QString orignalSize="", QString resultSize="", QString ratio="");

protected:
    void on_deleteImgControl();

private:
    QStringList imgPathNameList;
    QVector<ImgControlBase*> vImgCompress;
    MyFile myFile;
    int quality;

    QTime time;
};

#endif // QMLCOMPRESSCONTROL_H
