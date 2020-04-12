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
    JpgCompress(QString imgPathName);
    ~JpgCompress();

    bool compress(QString imgPathName);
    bool checkImage(QString imgPathName);

    static bool isJPG(QString imgPathName);

    /** 清理数据 */
    void clean(void);

    /** 读取jpg图片，并 */
    bool readJpg(QString imgPathName);

    /** 压缩到jpg文件中，quality_scale表示质量，取值范围(0,100), 数字越大压缩比例越高*/
    bool encodeToJPG(QString imgPathName, int quality_scale);

protected:
    bool initialData();
    void run();

private:
    QString imgPathName;

    bool judgePath(QString &imgPathName);

private:
    int				m_width;
    int				m_height;
    unsigned char*	m_rgbBuffer;

    unsigned char	m_YTable[64];
    unsigned char	m_CbCrTable[64];

    struct BitString
    {
        int length;
        int value;
    };

    BitString m_Y_DC_Huffman_Table[12];
    BitString m_Y_AC_Huffman_Table[256];

    BitString m_CbCr_DC_Huffman_Table[12];
    BitString m_CbCr_AC_Huffman_Table[256];

private:
    void _initHuffmanTables(void);
    void _initCategoryAndBitcode(void);
    void _initQualityTables(int quality);
    void _computeHuffmanTable(const char* nr_codes, const unsigned char* std_table, BitString* huffman_table);
    BitString _getBitCode(int value);

    void _convertColorSpace(int xPos, int yPos, char* yData, char* cbData, char* crData);
    void _foword_FDC(const char* channel_data, short* fdc_data);
    void _doHuffmanEncoding(const short* DU, short& prevDC, const BitString* HTDC, const BitString* HTAC,
        BitString* outputBitString, int& bitStringCounts);

private:
    void _write_jpeg_header(FILE* fp);
    void _write_byte_(unsigned char value, FILE* fp);
    void _write_word_(unsigned short value, FILE* fp);
    void _write_bitstring_(const BitString* bs, int counts, int& newByte, int& newBytePos, FILE* fp);
    void _write_(const void* p, int byteSize, FILE* fp);
};

#endif // JPGCOMPRESS_H
