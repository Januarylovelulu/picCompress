#ifndef JPGCOMPRESS_H
#define JPGCOMPRESS_H

#include <QObject>
#include <QFile>
#include <QDataStream>
#include <QImage>
#include "ImgControlBase.h"
#include <memory.h>
#include <math.h>

const quint32 JPG_TITLE = 0x00FFD8FF;

class JpgCompress : public ImgControlBase
{
public:
    JpgCompress(QString imgPathName, int quality = 60);
    ~JpgCompress();

    bool compress(QString imgPathName);
    bool checkImage(QString imgPathName);

    static bool isJPG(QString imgPathName);

    void clean(void);

    void setQuality(int quality);

protected:
    bool readImg(QString imgPathName);
    bool readImg_New(QString imgPathName);

    bool initialData();
    void run();

private:
    // 图片虚拟大小
    int				m_width;
    int				m_height;
    // 图片真实大小
    int				m_width_real;
    int				m_height_real;
    unsigned char*	m_rgbBuffer;

    // 量化表
    unsigned char	m_YTable[64];
    unsigned char	m_CbCrTable[64];

    struct BitString
    {
        int length;
        int value;
    };

    // 编码哈弗曼树
    BitString m_Y_DC_Huffman_Table[12];
    BitString m_Y_AC_Huffman_Table[256];

    BitString m_CbCr_DC_Huffman_Table[12];
    BitString m_CbCr_AC_Huffman_Table[256];

    // 解码哈弗曼树
    BitString d_Y_DC_Huffman_Table[12];
    BitString d_Y_AC_Huffman_Table[256];

    BitString d_CbCr_DC_Huffman_Table[12];
    BitString d_CbCr_AC_Huffman_Table[256];

private:
    void initHuffmanTables(void);
    void initQualityTables(int quality);
    void computeHuffmanTable(const char* nr_codes, const unsigned char* std_table, BitString* huffman_table);
    BitString getBitCode(int value);

    void convertColorSpace(int xPos, int yPos, char* yData, char* cbData, char* crData);
    void foword_FDC(const char* channel_data, short* fdc_data);
    void doHuffmanEncoding(const short* DU, short& prevDC, const BitString* HTDC, const BitString* HTAC,
        BitString* outputBitString, int& bitStringCounts);

    int getValue(BitString bitCode);
    void doHuffManDecoding(const short* DU, short& prevDC, const BitString* HTDC, const BitString* HTAC,
                           BitString* outputBitString, int& bitStringCounts);

private:
    void _write_jpeg_header(QFile* fp);
    void _write_byte_(unsigned char value, QFile* fp);
    void _write_word_(unsigned short value, QFile* fp);
    void _write_bitstring_(const BitString* bs, int counts, int& newByte, int& newBytePos, QFile* fp);
    void _write_(const void* p, int byteSize, QFile* fp);

private:
    QString imgPathName;
    int quality;

    bool judgePath(QString &imgPathName);
};

#endif // JPGCOMPRESS_H
