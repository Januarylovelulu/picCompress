#ifndef CODING_H
#define CODING_H

#include <QObject>
#include <QVector>
#include <QImage>
#include <QMap>

#define NUM 8

const int zTable[NUM][NUM] =
{
    {0, 1, 5, 6, 14, 15, 27, 28},
    {2, 4, 7, 13, 16, 26, 29, 42},
    {3, 8, 12, 17, 25, 30, 41, 43},
    {9, 11, 18, 24, 31, 40, 44, 53},
    {10, 19, 23, 32, 39, 45, 52, 54},
    {20, 22, 33, 38, 46, 51, 55, 60},
    {21, 34, 37, 47, 50, 56, 59, 61},
    {35, 36, 48, 49, 57, 58, 62, 63}
};

class Coding
{
public:
    Coding(QVector<QVector<QVector<int> > > color, QImage *jpg);
    void loopColorList();

protected:
    QVector<QString> Rle_Huffman(QVector<QVector<int>> z);

    QString rleToBinary(int num);
    QString dcYToBinary(QString num);
    QString acYToBinary(int num);
    QString dcCToBinary(int num);
    QString acCToBinary(int num);

private:
    const QVector<QVector<QVector<int>>> color;
    QImage *jpg;

    QStringList dcYTable;
};

#endif // CODING_H
