#ifndef PNGCOMPRESS_H
#define PNGCOMPRESS_H

#include <QObject>

const quint32 PNG_TITLE = 0X89504e47;

class PngCompress : public QObject
{
    Q_OBJECT
public:
    explicit PngCompress();
    PngCompress(QString imgPathName);

public:
    static bool compress(QString imgPathName);
    bool compress();

    // 获取处理后的pathName,并复制到class内的imgPathName，为空则不是png，或者名字有错
    // 参数为空时，获取class内的imgPathName
    QString getPathName(QString imgPathName = nullptr);

    static bool isPng(QString imgPathName);

signals:

public slots:

private:
    QString imgPathName;

    bool judgePath(QString &imgPathName);
};

#endif // PNGCOMPRESS_H
