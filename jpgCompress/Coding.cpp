#include "Coding.h"

#include <QDebug>

Coding::Coding(QVector<QVector<QVector<int> > > color, QImage *jpg):
    color(color),jpg(jpg)
{
    dcYTable<<"00"<<"010"<<"011"<<"100"<<"101"<<"110"<<"1110"
              <<"11110"<<"111110"<<"1111110"<<"11111110"<<"111111110";
}

void Coding::loopColorList()
{
    QMap<int,QString> map;
    for(int i = 0;i < NUM;i++)
        for(int j = 0;j < NUM;j++)
            map[zTable[i][j]] = QString::number(i)+QString::number(j);


    for (int a=0;a<color.size();a+=8) {
        for (int b=0;b<color[0].size();b+=8) {
            QVector<QVector<int>> z;
            z.push_back(QVector<int>());
            z.push_back(QVector<int>());
            z.push_back(QVector<int>());
            for (auto i = map.begin();i!=map.end();i++) {
                int num = i.value().toInt();
                z[0].push_back(color[a+num/10][b+num%10][0]);
                z[1].push_back(color[a+num/10][b+num%10][1]);
                z[2].push_back(color[a+num/10][b+num%10][2]);
            }
            // RLE编码
            Rle_Huffman(z);
        }
    }
}

QVector<QString> Coding::Rle_Huffman(QVector<QVector<int> > z)
{
    QVector<QString> result;
    int symbol[3] = {0,0,0};
    QVector<QVector<int>> num;
    QVector<QVector<int>> len;
    for (int a=0;a<3;a++) {
        num.push_back(QVector<int>());
        len.push_back(QVector<int>());
        result.push_back(QString());
    }
    for (int a=0;a<z[0].size();a++) {
        for (int b=0;b<3;b++) {
            if(z[b][a]!=0){
                num[b].push_back(z[b][a]);
                len[b].push_back(symbol[b]);
                symbol[b]=0;
            }
            else {
                if(symbol[b]==15){
                    num[b].push_back(0);
                    len[b].push_back(15);
                    symbol[b]=0;
                }
                else{
                    symbol[b]++;
                }
            }
        }
    }
    for (int a=0;a<3;a++) {
        if(symbol[a]!=0){
            num[a].push_back(0);
            len[a].push_back(symbol[a]);
        }
        bool signal = false; // 如果最后有0
        for (int b=num[a].size()-1;b>=0;b--) {
            if(num[a][b]==0){
                num[a].pop_back();
                len[a].pop_back();
                signal = true;
            }
        }
        if(signal){
            num[a].push_back(0);
            len[a].push_back(0); // 0,0代表EOB
        }
    }
//    qDebug()<<"Y: "<<num[0].size();
//    for (int a=0;a<num[0].size();a++) {
//        qDebug()<<len[0][a]<<" "<<num[0][a];
//    }
//    qDebug()<<"Cb: "<<num[1].size();
//    for (int a=0;a<num[1].size();a++) {
//        qDebug()<<len[1][a]<<" "<<num[1][a];
//    }
//    qDebug()<<"Cr: "<<num[2].size();
//    for (int a=0;a<num[2].size();a++) {
//        qDebug()<<len[2][a]<<" "<<num[2][a];
//    }
    qDebug()<<rleToBinary(42);

    return result;
}

QString Coding::rleToBinary(int num)
{
    QString result;
    if(num>=0){
        result = QString::number(num,2);
    }
    else {
        QString tmp = QString::number(-num,2);
        for (QChar c : tmp) {
            result += c=='0' ? '1' : '0';
        }
    }

    return result;
}

QString Coding::dcYToBinary(QString num)
{
    QString result;
    int len = num.size();
    result = dcYTable[len];
    return result;
}

QString Coding::acYToBinary(int num)
{

}

QString Coding::dcCToBinary(int num)
{

}

QString Coding::acCToBinary(int num)
{

}
