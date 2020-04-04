#ifndef DCT_H
#define DCT_H

#include <QObject>
#include <QtMath>
#include <QVector>

#define NUM 8
#define PI 3.1415926

class Dct{
public:
    static void dct(QVector<QVector<QVector<int>>> &color){
        // 图片的大小
        int width = color.size();
        if(width<=0)
            return;
        int height = color[0].size();
        if(height<=0)
            return;

        int x=0,y=0,fx=0,fy=0;

        // 无法凑齐NUM×NUM，则补齐格子
        if(width%NUM!=0)
            fx = NUM - width%NUM;
        if(height%NUM!=0)
            fy = NUM - height%NUM;
        if(fx||fy){
            for (int a=width;a<fx+width;a++) {
                color.push_back(QVector<QVector<int>>());
                for (int b=0;b<height;b++) {
                    color[a].push_back(QVector<int>());
                    color[a][b].push_back(0);
                    color[a][b].push_back(0);
                    color[a][b].push_back(0);
                }
            }
            for (int a=0;a<fx+width;a++) {
                for (int b=height;b<fy+height;b++) {
                    color[a].push_back(QVector<int>());
                    color[a][b].push_back(0);
                    color[a][b].push_back(0);
                    color[a][b].push_back(0);
                }
            }
            width+=fx;
            height+=fy;
        }

        // DCT算法开始，重复8×8的算数
        double alpha,beta;//C(k)  C(l)
        int m=0,n=0,k=0,l=0;

        int tmpArr[NUM][NUM][3];

        for (x=0;x<width;x+=NUM) {
            for (y=0;y<height;y+=NUM) {
                // DCT核心算法，8×8格算
                for(k = 0;k < NUM;k++) {
                    for(l = 0;l < NUM;l++) {
                        if(k == 0)
                            alpha = sqrt(1.0/NUM);
                        else
                            alpha = sqrt(2.0/NUM);
                        if(l == 0)
                            beta = sqrt(1.0/NUM);
                        else
                            beta = sqrt(2.0/NUM);
                        double temp[3] = {0.0 , 0.0 , 0.0};
                        for(m = 0;m < NUM;m++) {
                            for(n = 0;n < NUM;n++) {
                                temp[0] += color[m+x][n+y][0] * cos((2*m+1)*k*PI/(2.0*NUM)) * cos((2*n+1)*l*PI/(2.0*NUM));
                                temp[1] += color[m+x][n+y][1] * cos((2*m+1)*k*PI/(2.0*NUM)) * cos((2*n+1)*l*PI/(2.0*NUM));
                                temp[2] += color[m+x][n+y][2] * cos((2*m+1)*k*PI/(2.0*NUM)) * cos((2*n+1)*l*PI/(2.0*NUM));
                            }
                        }
                        double t = alpha * beta * temp[0];
                        tmpArr[k][l][0] = t >= 0 ? int(t+0.5) : int(t-0.5);
                        t = alpha * beta * temp[1];
                        tmpArr[k][l][1] = t >= 0 ? int(t+0.5) : int(t-0.5);
                        t = alpha * beta * temp[2];
                        tmpArr[k][l][2] = t >= 0 ? int(t+0.5) : int(t-0.5);
                    }
                }
                for(int a=0;a<NUM;a++){
                    for (int b=0;b<NUM;b++) {
                        color[a+x][b+y][0] = tmpArr[a][b][0];
                        color[a+x][b+y][1] = tmpArr[a][b][1];
                        color[a+x][b+y][2] = tmpArr[a][b][2];
                    }
                }
            }
        }
    }
};

#endif
