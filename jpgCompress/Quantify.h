#ifndef QUANTIFY_H
#define QUANTIFY_H

#include <QObject>
#include <QVector>
#include <QDebug>

#define NUM 8

// 亮度量化表
const int Y[NUM][NUM] =
{
    {16,11,10,16,24,40,51,61},
    {12,12,14,19,26,58,60,55},
    {14,13,16,24,40,57,69,56},
    {14,17,22,29,51,87,80,62},
    {18,22,37,56,68,109,103,77},
    {24,35,55,64,81,104,113,92},
    {49,64,78,87,103,121,120,101},
    {72,92,95,98,112,100,103,99}
};
// 色差量化表
const int C[NUM][NUM] =
{
    {17,18,24,47,99,99,99,99},
    {18,21,26,66,99,99,99,99},
    {24,26,56,99,99,99,99,99},
    {47,66,99,99,99,99,99,99},
    {99,99,99,99,99,99,99,99},
    {99,99,99,99,99,99,99,99},
    {99,99,99,99,99,99,99,99},
    {99,99,99,99,99,99,99,99}
};

class Quantify{
public:
    // res = round(G/Y) 或 res = round(G/C)
    static void quantify(QVector<QVector<QVector<int>>> &color){
        for (int a=0;a<color.size();a+=NUM) {
            for (int b=0;b<color[0].size();b+=NUM) {
                for (int x=0;x<NUM;x++) {
                    for (int y=0;y<NUM;y++) {
                        double t = color[x+a][y+b][0]/Y[x][y];
                        color[x+a][y+b][0] = t >= 0 ? int(t+0.5) : int(t-0.5);
                        t = color[x+a][y+b][1]/C[x][y];
                        color[x+a][y+b][1] = t >= 0 ? int(t+0.5) : int(t-0.5);
                        t = color[x+a][y+b][2]/C[x][y];
                        color[x+a][y+b][2] = t >= 0 ? int(t+0.5) : int(t-0.5);
                    }
                }
            }
        }
    }
private:

};

#endif // QUANTIFY_H
