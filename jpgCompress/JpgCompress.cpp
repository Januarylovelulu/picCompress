#include "JpgCompress.h"

#include <QDebug>
#include <QTime>

namespace {
    const unsigned char Luminance_Quantization_Table[64] =
    {
        16,  11,  10,  16,  24,  40,  51,  61,
        12,  12,  14,  19,  26,  58,  60,  55,
        14,  13,  16,  24,  40,  57,  69,  56,
        14,  17,  22,  29,  51,  87,  80,  62,
        18,  22,  37,  56,  68, 109, 103,  77,
        24,  35,  55,  64,  81, 104, 113,  92,
        49,  64,  78,  87, 103, 121, 120, 101,
        72,  92,  95,  98, 112, 100, 103,  99
    };

    const unsigned char Chrominance_Quantization_Table[64] =
    {
        17,  18,  24,  47,  99,  99,  99,  99,
        18,  21,  26,  66,  99,  99,  99,  99,
        24,  26,  56,  99,  99,  99,  99,  99,
        47,  66,  99,  99,  99,  99,  99,  99,
        99,  99,  99,  99,  99,  99,  99,  99,
        99,  99,  99,  99,  99,  99,  99,  99,
        99,  99,  99,  99,  99,  99,  99,  99,
        99,  99,  99,  99,  99,  99,  99,  99
    };

    const char ZigZag[64] =
    {
        0, 1, 5, 6,14,15,27,28,
        2, 4, 7,13,16,26,29,42,
        3, 8,12,17,25,30,41,43,
        9,11,18,24,31,40,44,53,
        10,19,23,32,39,45,52,54,
        20,22,33,38,46,51,55,60,
        21,34,37,47,50,56,59,61,
        35,36,48,49,57,58,62,63
    };

    const char Standard_DC_Luminance_NRCodes[] = { 0, 0, 7, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
    const unsigned char Standard_DC_Luminance_Values[] = { 4, 5, 3, 2, 6, 1, 0, 7, 8, 9, 10, 11 };

    const char Standard_DC_Chrominance_NRCodes[] = { 0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 };
    const unsigned char Standard_DC_Chrominance_Values[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };

    const char Standard_AC_Luminance_NRCodes[] = { 0, 2, 1, 3, 3, 2, 4, 3, 5, 5, 4, 4, 0, 0, 1, 0x7d };
    const unsigned char Standard_AC_Luminance_Values[] =
    {
        0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12,
        0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07,
        0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xa1, 0x08,
        0x23, 0x42, 0xb1, 0xc1, 0x15, 0x52, 0xd1, 0xf0,
        0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0a, 0x16,
        0x17, 0x18, 0x19, 0x1a, 0x25, 0x26, 0x27, 0x28,
        0x29, 0x2a, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
        0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
        0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
        0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
        0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
        0x7a, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
        0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98,
        0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
        0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6,
        0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3, 0xc4, 0xc5,
        0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4,
        0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xe1, 0xe2,
        0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea,
        0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
        0xf9, 0xfa
    };

    const char Standard_AC_Chrominance_NRCodes[] = { 0, 2, 1, 2, 4, 4, 3, 4, 7, 5, 4, 4, 0, 1, 2, 0x77 };
    const unsigned char Standard_AC_Chrominance_Values[] =
    {
        0x00, 0x01, 0x02, 0x03, 0x11, 0x04, 0x05, 0x21,
        0x31, 0x06, 0x12, 0x41, 0x51, 0x07, 0x61, 0x71,
        0x13, 0x22, 0x32, 0x81, 0x08, 0x14, 0x42, 0x91,
        0xa1, 0xb1, 0xc1, 0x09, 0x23, 0x33, 0x52, 0xf0,
        0x15, 0x62, 0x72, 0xd1, 0x0a, 0x16, 0x24, 0x34,
        0xe1, 0x25, 0xf1, 0x17, 0x18, 0x19, 0x1a, 0x26,
        0x27, 0x28, 0x29, 0x2a, 0x35, 0x36, 0x37, 0x38,
        0x39, 0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
        0x49, 0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
        0x59, 0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
        0x69, 0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
        0x79, 0x7a, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
        0x88, 0x89, 0x8a, 0x92, 0x93, 0x94, 0x95, 0x96,
        0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5,
        0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4,
        0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3,
        0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2,
        0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda,
        0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9,
        0xea, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
        0xf9, 0xfa
    };
}

JpgCompress::JpgCompress(QString imgPathName, int quality):
    m_width(0)
    , m_height(0)
    , m_rgbBuffer(nullptr)
    , imgPathName(imgPathName)
    , quality(quality)
{
    // 初始化哈夫曼
    initHuffmanTables();
}

JpgCompress::~JpgCompress()
{
    clean();
}

bool JpgCompress::isJPG(QString imgPathName)
{
    QFile file(imgPathName);
    if (!file.open(QIODevice::ReadOnly)){
        return false;
    }
    QDataStream in(&file);
    quint32 n;
    in >> n;
    file.close();
    if(!(n>>8==JPG_TITLE))
        return false;
    else
        return true;
}

bool JpgCompress::compress(QString imgPathName)
{
    // 图片初始化正常
    if(m_rgbBuffer==0 || m_width==0 || m_height==0) return false;

    // 初始化写出
    QFile* fp = new QFile(imgPathName);
    if(!fp->open(QIODevice::ReadWrite)) {
        qDebug()<<"JPEG::错误: "<<imgPathName<<" QFile无法打开，导致错误";
        return false;
    }

    // 定义量化表
    initQualityTables(quality);

    // 写入文件头
    _write_jpeg_header(fp);

    short prev_DC_Y = 0, prev_DC_Cb = 0, prev_DC_Cr = 0;
    int newByte=0, newBytePos=7;

    for(int yPos=0; yPos<m_height; yPos+=8)
    {
        for (int xPos=0; xPos<m_width; xPos+=8)
        {
            char yData[64], cbData[64], crData[64];
            short yQuant[64], cbQuant[64], crQuant[64];

            // 转换颜色空间
            convertColorSpace(xPos, yPos, yData, cbData, crData);

            BitString outputBitString[128];
            int bitStringCounts;

            // Y分量压缩
            foword_FDC(yData, yQuant);
            doHuffmanEncoding(yQuant, prev_DC_Y, m_Y_DC_Huffman_Table, m_Y_AC_Huffman_Table, outputBitString, bitStringCounts);
            _write_bitstring_(outputBitString, bitStringCounts, newByte, newBytePos, fp);

            // Cb分量压缩
            foword_FDC(cbData, cbQuant);
            doHuffmanEncoding(cbQuant, prev_DC_Cb, m_CbCr_DC_Huffman_Table, m_CbCr_AC_Huffman_Table, outputBitString, bitStringCounts);
            _write_bitstring_(outputBitString, bitStringCounts, newByte, newBytePos, fp);

            // Cr分量压缩
            foword_FDC(crData, crQuant);
            doHuffmanEncoding(crQuant, prev_DC_Cr, m_CbCr_DC_Huffman_Table, m_CbCr_AC_Huffman_Table, outputBitString, bitStringCounts);
            _write_bitstring_(outputBitString, bitStringCounts, newByte, newBytePos, fp);
        }
    }

    _write_word_(0xFFD9, fp); //Write End of Image Marker

    fp->close();
    delete fp;

    return true;
}

bool JpgCompress::checkImage(QString imgPathName)
{
    if(isJPG(imgPathName))
        return true;
    else
        return false;
}

bool JpgCompress::initialData()
{
    QFileInfo finfo(imgPathName);
    originalSize = finfo.size();
    return true;
}

void JpgCompress::run()
{
    if(!initialData())
        return;

    QTime time;
    time.start();

//    readImg_New(imgPathName);
    readImg(imgPathName);
    qDebug()<<"读取 运行时间:"<<time.elapsed()/1000.0<<"s";

    time.restart();

    QString name = imgPathName;
    name.replace(".jpg","_Compress.jpg",Qt::CaseInsensitive).replace(".jpeg","_Compress.jpeg",Qt::CaseInsensitive);

    compress(name);

    qDebug()<<"压缩 运行时间:"<<time.elapsed()/1000.0<<"s";
}

bool JpgCompress::judgePath(QString &imgPathName)
{
    if(imgPathName.isEmpty()){
        if(this->imgPathName.isEmpty()){
            qDebug()<<"JpgCompress: the image's pathName is empty, return false.";
            return false;
        }
        else {
            qDebug()<<"JpgCompress: the image's pathName is empty, but class's is not empty, return true.";
            imgPathName = this->imgPathName;
        }
    }
    else {
        if(!isJPG(imgPathName)){
            qDebug()<<"JpgCompress: the path("<<imgPathName<<")is not a legal Png QFile, return false";
            return false;
        }
    }
    return true;
}

void JpgCompress::clean(void)
{
    if(m_rgbBuffer) delete[] m_rgbBuffer;
    m_rgbBuffer=nullptr;

    m_width=0;
    m_height=0;
}

bool JpgCompress::readImg(QString imgPathName)
{
    bool successed = false;
    clean();

    QImage jpg;
    jpg.load(imgPathName);
    int width = jpg.width(), height = jpg.height();
    m_width_real = width;
    m_height_real = height;

    // 无法凑齐8×8，则补齐格子
    int fx=0,fy=0;
    if(width%8!=0)
        fx = 8 - width%8;
    if(height%8!=0)
        fy = 8 - height%8;
    width+=fx;
    height+=fy;

    QVector<QVector<QVector<int> > > color(width,QVector<QVector<int> >(height,QVector<int>(3,0)));

    // 获取颜色
    for(int y=0;y<jpg.height();y++){
        for(int x=0;x<jpg.width();x++){
            QColor c = jpg.pixelColor(x,y);
            color[x][y][0] = c.red();
            color[x][y][1] = c.green();
            color[x][y][2] = c.blue();
        }
    }
    unsigned long long total = (unsigned long long)width*(unsigned long long)height*(unsigned long long)3;

    unsigned char* buffer = new unsigned char[total];
    if(buffer){
        for(int y=0;y<height;y++){
            for(int x=0;x<width;x++){
                buffer[(x+y*width)*3] = (unsigned char)color[x][y][2];
                buffer[(x+y*width)*3+1] = (unsigned char)color[x][y][1];
                buffer[(x+y*width)*3+2] = (unsigned char)color[x][y][0];
            }
        }
        m_rgbBuffer = buffer;
        m_width = width;
        m_height = height;
        successed = true;
    }

    return successed;
}

bool JpgCompress::readImg_New(QString imgPathName)
{
    QFile file(imgPathName);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"JPEG::错误 打开图片文件出错";
        return false;
    }
    QDataStream in(&file);
    quint16 SOS = 0xFFDA, DHT = 0xFFC4, DQT = 0xFFDB, APP0 = 0xFFE0,
            SOF0 = 0xFFC0, COM = 0xFFFE, Skip;
    in>>Skip;
    // 哈夫曼编码后的data
    QVector<quint8> data;
    // 哈夫曼树的构造
    char Standard_DC_Luminance_NRCodes[16];
    unsigned char* Standard_DC_Luminance_Values=nullptr;
    char Standard_AC_Luminance_NRCodes[16];
    unsigned char* Standard_AC_Luminance_Values=nullptr;
    char Standard_DC_Chrominance_NRCodes[16];
    unsigned char* Standard_DC_Chrominance_Values=nullptr;
    char Standard_AC_Chrominance_NRCodes[16];
    unsigned char* Standard_AC_Chrominance_Values=nullptr;
    int countList[4]={-1,-1,-1,-1};
    // 量化表
    unsigned char	d_YTable[64];
    unsigned char	d_CbCrTable[64];
    // 组件ID，采样系数，量化表号
    int idQ[3];
    int hv[3];
    int qTable[3];
    // 哈夫曼表映射
    int huffmanTable[3];
    int idHuffman[3];

    while(!in.atEnd()){
        quint16 head,len;
        in>>head;

        in>>len;
        len-=2;

        switch (head) {
        case 0xFFDA:{
            quint8 tmp;
            in>>tmp;
            if(tmp!=0x3){
                qDebug()<<"JPEG::错误  FFDA扫描行组件数不等于3的暂时无法处理";
                return false;
            }
            for (int x=0;x<3;x++) {
                in>>tmp;
                idHuffman[x] = tmp;
                in>>tmp;
                huffmanTable[x] = tmp;
            }
            in>>tmp>>tmp>>tmp;
            while (!in.atEnd()) {
                in>>tmp;
                data.push_back(tmp);
            }
            // 删除0xFFD9
            data.pop_back();data.pop_back();
            break;
        };
        case 0xFFC4:{
            for (int x=0;x<4;x++) {
                quint8 tmp;
                in>>tmp;
                switch (tmp) {
                case 0x0:{
                    int times=0,count=0;
                    while (times!=16) {
                        in>>tmp;
                        Standard_DC_Luminance_NRCodes[times] = tmp;
                        count+=tmp;
                        times++;
                    }
                    Standard_DC_Luminance_Values = new unsigned char[count];
                    for (int x=0;x<count;x++) {
                        in>>tmp;
                        Standard_DC_Luminance_Values[x] = tmp;
                    }
                    countList[0]=count;
                    break;
                }
                case 0x10:{
                    int times=0,count=0;
                    while (times!=16) {
                        in>>tmp;
                        Standard_AC_Luminance_NRCodes[times] = tmp;
                        count+=tmp;
                        times++;
                    }
                    Standard_AC_Luminance_Values = new unsigned char[count];
                    for (int x=0;x<count;x++) {
                        in>>tmp;
                        Standard_AC_Luminance_Values[x] = tmp;
                    }
                    countList[1]=count;
                    break;
                }
                case 0x01:{
                    int times=0,count=0;
                    while (times!=16) {
                        in>>tmp;
                        Standard_DC_Chrominance_NRCodes[times] = tmp;
                        count+=tmp;
                        times++;
                    }
                    Standard_DC_Chrominance_Values = new unsigned char[count];
                    for (int x=0;x<count;x++) {
                        in>>tmp;
                        Standard_DC_Chrominance_Values[x] = tmp;
                    }
                    countList[2]=count;
                    break;
                }
                case 0x11:{
                    int times=0,count=0;
                    while (times!=16) {
                        in>>tmp;
                        Standard_AC_Chrominance_NRCodes[times] = tmp;
                        count+=tmp;
                        times++;
                    }
                    Standard_AC_Chrominance_Values = new unsigned char[count];
                    for (int x=0;x<count;x++) {
                        in>>tmp;
                        Standard_AC_Chrominance_Values[x] = tmp;
                    }
                    countList[3]=count;
                    break;
                }
                default:{
                    qDebug()<<"JPEG::错误  DHT读取时遇到了未知表序号";
                    return false;
                }
                }
            }

            memset(&d_Y_DC_Huffman_Table, 0, sizeof(d_Y_DC_Huffman_Table));
            computeHuffmanTable(Standard_DC_Luminance_NRCodes, Standard_DC_Luminance_Values, d_Y_DC_Huffman_Table);

            memset(&d_Y_AC_Huffman_Table, 0, sizeof(d_Y_AC_Huffman_Table));
            computeHuffmanTable(Standard_AC_Luminance_NRCodes, Standard_AC_Luminance_Values, d_Y_AC_Huffman_Table);

            memset(&d_CbCr_DC_Huffman_Table, 0, sizeof(d_CbCr_DC_Huffman_Table));
            computeHuffmanTable(Standard_DC_Chrominance_NRCodes, Standard_DC_Chrominance_Values, d_CbCr_DC_Huffman_Table);

            memset(&d_CbCr_AC_Huffman_Table, 0, sizeof(d_CbCr_AC_Huffman_Table));
            computeHuffmanTable(Standard_AC_Chrominance_NRCodes, Standard_AC_Chrominance_Values, d_CbCr_AC_Huffman_Table);


            break;
        };
        case 0xFFDB:{
            quint8 tmp;
            in>>tmp; // QT号
            if(len<80){
                int times=0;
                if(tmp==0x00){
                    while (times!=64) {
                        in>>tmp;
                        d_YTable[times++] = tmp;
                    }
                }
                else {
                    while (times!=64) {
                        in>>tmp;
                        d_CbCrTable[times++] = tmp;
                    }
                }
            }
            else {
                if(tmp==0x00){
                    int times=0;
                    while (times!=64) {
                        in>>tmp;
                        d_YTable[times++] = tmp;
                    }
                    in>>tmp;
                    times = 0;
                    while (times!=64) {
                        in>>tmp;
                        d_CbCrTable[times++] = tmp;
                    }
                }
                else {
                    int times=0;
                    while (times!=64) {
                        in>>tmp;
                        d_CbCrTable[times++] = tmp;
                    }
                    in>>tmp;
                    times = 0;
                    while (times!=64) {
                        in>>tmp;
                        d_YTable[times++] = tmp;
                    }
                }
            }

            break;
        };
        case 0xFFE0:{
            // 暂不知晓英寸和厘米有什么差别，直接跳过，主次版本号也直接默认为1
            len-=4;
            quint32 format;
            in>>format;
            if(format!=0x4A464946){
                qDebug()<<"JPEG::错误  此图片格式不为JFIF，暂时认定为错误，并终止运行";
                return false;
            }
            quint8 tmp;
            while (len!=0) {
                in>>tmp;
                len--;
            }
            break;
        };
        case 0xFFC0:{
            if(len!=15){
                qDebug()<<"JPEG::错误 FFC0无法处理长度不为17的情况";
                return false;
            }
            quint16 height,width;
            quint8 tmp;
            in>>tmp; // 丢掉
            in>>height>>width;
            m_width_real=width;
            m_height_real=height;
            in>>tmp; // 组件数，一般为3
            for (int x=0;x<3;x++) {
                in>>tmp;
                idQ[x]=tmp;
                in>>tmp;
                hv[x]=tmp;
                in>>tmp;
                qTable[x]=tmp;
            }
            break;
        };
        default:{
            qDebug()<<"JPEG::警告  跳过段 "+QString::number(head,16)+" 长度为："<<len;
            quint8 tmp;
            while (len!=0) {
                in>>tmp;
                len--;
            }
        };
        }
    }
    QString str1,str2;
    for (int x=0;x<12;x++) {
        str1+=QString::number(d_Y_DC_Huffman_Table[x].length,16)+" ";
        str2+=QString::number(d_Y_DC_Huffman_Table[x].value,16)+" ";
    }
    qDebug()<<str1;
    qDebug()<<str2;
    file.close();

    return true;
}

void JpgCompress::setQuality(int quality)
{
    this->quality = quality;
}

void JpgCompress::initHuffmanTables(void)
{
    memset(&m_Y_DC_Huffman_Table, 0, sizeof(m_Y_DC_Huffman_Table));
    computeHuffmanTable(Standard_DC_Luminance_NRCodes, Standard_DC_Luminance_Values, m_Y_DC_Huffman_Table);

    memset(&m_Y_AC_Huffman_Table, 0, sizeof(m_Y_AC_Huffman_Table));
    computeHuffmanTable(Standard_AC_Luminance_NRCodes, Standard_AC_Luminance_Values, m_Y_AC_Huffman_Table);

    memset(&m_CbCr_DC_Huffman_Table, 0, sizeof(m_CbCr_DC_Huffman_Table));
    computeHuffmanTable(Standard_DC_Chrominance_NRCodes, Standard_DC_Chrominance_Values, m_CbCr_DC_Huffman_Table);

    memset(&m_CbCr_AC_Huffman_Table, 0, sizeof(m_CbCr_AC_Huffman_Table));
    computeHuffmanTable(Standard_AC_Chrominance_NRCodes, Standard_AC_Chrominance_Values, m_CbCr_AC_Huffman_Table);
}

JpgCompress::BitString JpgCompress::getBitCode(int value)
{
    BitString ret;
    int v = (value>0) ? value : -value;

    // bit 长度
    int length = 0;
    for(length=0; v; v>>=1) length++;

    // -6 l=3 8-6-1=1  5 l=3 5
    ret.value = value>0 ? value : ((1<<length)+value-1);
    ret.length = length;

    return ret;
};

void JpgCompress::initQualityTables(int quality_scale)
{
    if(quality_scale<=0) quality_scale=1;
    if(quality_scale>=100) quality_scale=100;

    for(int i=0; i<64; i++) {
        int temp = ((int)(Luminance_Quantization_Table[i] * quality_scale + 50) / 100);
        if (temp<=0) temp = 1;
        if (temp>0xFF) temp = 0xFF;
        m_YTable[ZigZag[i]] = (unsigned char)temp;

        temp = ((int)(Chrominance_Quantization_Table[i] * quality_scale + 50) / 100);
        if (temp<=0) 	temp = 1;
        if (temp>0xFF) temp = 0xFF;
        m_CbCrTable[ZigZag[i]] = (unsigned char)temp;
    }
}

void JpgCompress::computeHuffmanTable(const char* nr_codes, const unsigned char* std_table, BitString* huffman_table)
{
    unsigned char pos_in_table = 0;
    unsigned short code_value = 0;

    for(int k = 1; k <= 16; k++) {
        for(int j = 1; j <= nr_codes[k-1]; j++) {
            huffman_table[std_table[pos_in_table]].value = code_value;
            huffman_table[std_table[pos_in_table]].length = k;
            pos_in_table++;
            code_value++;
        }
        code_value <<= 1;
    }
}

void JpgCompress::_write_byte_(unsigned char value, QFile* fp)
{
    _write_(&value, 1, fp);
}

void JpgCompress::_write_word_(unsigned short value, QFile* fp)
{
    unsigned short _value = ((value>>8)&0xFF) | ((value&0xFF)<<8);
    _write_(&_value, 2, fp);
}

void JpgCompress::_write_(const void* p, int byteSize, QFile* fp)
{
//    fwrite(p, 1, byteSize, fp);

    fp->write((char*)p,byteSize);
    resultSize += byteSize;
}

void JpgCompress::doHuffmanEncoding(const short* DU,short& prevDC,const BitString* HTDC
                                     ,const BitString* HTAC,BitString* outputBitString,int& bitStringCounts)
{
    BitString EOB = HTAC[0x00];
    BitString SIXTEEN_ZEROS = HTAC[0xF0];

    int index=0;

    // 编码 DC
    int dcDiff = (int)(DU[0] - prevDC);
    prevDC = DU[0];

    if (dcDiff == 0)
        outputBitString[index++] = HTDC[0];
    else{
        BitString bs = getBitCode(dcDiff);

        outputBitString[index++] = HTDC[bs.length];
        outputBitString[index++] = bs;
    }

    // 编码 ACs
    int endPos=63; //end0pos = first element in reverse order != 0
    while((endPos > 0) && (DU[endPos] == 0)) endPos--;

    for(int i=1; i<=endPos; )
    {
        int startPos = i;
        while((DU[i] == 0) && (i <= endPos)) i++;

        int zeroCounts = i - startPos;
        if (zeroCounts >= 16)
        {
            for (int j=1; j<=zeroCounts/16; j++)
                outputBitString[index++] = SIXTEEN_ZEROS;
            zeroCounts = zeroCounts%16;
        }

        BitString bs = getBitCode(DU[i]);

        outputBitString[index++] = HTAC[(zeroCounts << 4) | bs.length];
        outputBitString[index++] = bs;
        i++;
    }

    if (endPos != 63)
        outputBitString[index++] = EOB;

    bitStringCounts = index;
}

int JpgCompress::getValue(JpgCompress::BitString bitCode)
{
    int result;


    return result;
}

void JpgCompress::doHuffManDecoding(const short *DU, short &prevDC, const JpgCompress::BitString *HTDC, const JpgCompress::BitString *HTAC, JpgCompress::BitString *outputBitString, int &bitStringCounts)
{

}

void JpgCompress::_write_bitstring_(const BitString* bs, int counts, int& newByte, int& newBytePos, QFile* fp)
{
    unsigned short mask[] = {1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768};

    for(int i=0; i<counts; i++)
    {
        int value = bs[i].value;
        int posval = bs[i].length - 1;

        while (posval >= 0)
        {
            if ((value & mask[posval]) != 0)
            {
                newByte = newByte  | mask[newBytePos];
            }
            posval--;
            newBytePos--;
            if (newBytePos < 0)
            {
                // Write to stream
                _write_byte_((unsigned char)(newByte), fp);
                if (newByte == 0xFF)
                {
                    // Handle special case
                    _write_byte_((unsigned char)(0x00), fp);
                }

                // Reinitialize
                newBytePos = 7;
                newByte = 0;
            }
        }
    }
}

void JpgCompress::convertColorSpace(int xPos, int yPos, char* yData, char* cbData, char* crData)
{
    for (int y=0; y<8; y++)
    {
        unsigned char* p = m_rgbBuffer + (y+yPos)*m_width*3 + xPos*3;
        for (int x=0; x<8; x++)
        {
            unsigned char B = *p++;
            unsigned char G = *p++;
            unsigned char R = *p++;

            yData[y*8+x] = (char)(0.299f * R + 0.587f * G + 0.114f * B - 128);
            cbData[y*8+x] = (char)(-0.1687f * R - 0.3313f * G + 0.5f * B );
            crData[y*8+x] = (char)(0.5f * R - 0.4187f * G - 0.0813f * B);
        }
    }
}

void JpgCompress::foword_FDC(const char* channel_data, short* fdc_data)
{
    const float PI = 3.1415926f;
    for(int v=0; v<8; v++)
    {
        for(int u=0; u<8; u++)
        {
            float alpha_u = (u==0) ? 1/sqrt(8.0f) : 0.5f;
            float alpha_v = (v==0) ? 1/sqrt(8.0f) : 0.5f;

            float temp = 0.f;
            for(int x=0; x<8; x++)
            {
                for(int y=0; y<8; y++)
                {
                    float data = channel_data[y*8+x];

                    data *= cos((2*x+1)*u*PI/16.0f);
                    data *= cos((2*y+1)*v*PI/16.0f);

                    temp += data;
                }
            }

            temp *= alpha_u*alpha_v/m_YTable[ZigZag[v*8+u]];
            fdc_data[ZigZag[v*8+u]] = (short) ((short)(temp + 16384.5) - 16384);
        }
    }
}

void JpgCompress::_write_jpeg_header(QFile* fp)
{
    //SOI
    _write_word_(0xFFD8, fp);		// marker = 0xFFD8

    //APPO
    _write_word_(0xFFE0,fp);		// marker = 0xFFE0
    _write_word_(16, fp);			// length = 16 for usual JPEG, no thumbnail
    _write_("JFIF", 5, fp);			// 'JFIF\0'
    _write_byte_(1, fp);			// version_hi
    _write_byte_(1, fp);			// version_low
    _write_byte_(0, fp);			// xyunits = 0 no units, normal density
    _write_word_(1, fp);			// xdensity
    _write_word_(1, fp);			// ydensity
    _write_byte_(0, fp);			// thumbWidth
    _write_byte_(0, fp);			// thumbHeight

    //DQT
    _write_word_(0xFFDB, fp);		//marker = 0xFFDB
    _write_word_(132, fp);			//size=132
    _write_byte_(0, fp);			//QTYinfo== 0:  bit 0..3: 8ber of QT = 0 (table for Y)
                                    //				bit 4..7: precision of QT
                                    //				bit 8	: 0
    _write_(m_YTable, 64, fp);		//YTable
    _write_byte_(1, fp);			//QTCbinfo = 1 (quantization table for Cb,Cr)
    _write_(m_CbCrTable, 64, fp);	//CbCrTable

    //SOFO
    _write_word_(0xFFC0, fp);			//marker = 0xFFC0
    _write_word_(17, fp);				//length = 17 for a truecolor YCbCr JPG
    _write_byte_(8, fp);				//precision = 8: 8 bits/sample
    _write_word_(m_height_real&0xFFFF, fp);	//height
    _write_word_(m_width_real&0xFFFF, fp);	//width
    _write_byte_(3, fp);				//nrofcomponents = 3: We encode a truecolor JPG

    _write_byte_(1, fp);				//IdY = 1
    _write_byte_(0x11, fp);				//HVY sampling factors for Y (bit 0-3 vert., 4-7 hor.)(SubSamp 1x1)
    _write_byte_(0, fp);				//QTY  Quantization Table 8ber for Y = 0

    _write_byte_(2, fp);				//IdCb = 2
    _write_byte_(0x11, fp);				//HVCb = 0x11(SubSamp 1x1)
    _write_byte_(1, fp);				//QTCb = 1

    _write_byte_(3, fp);				//IdCr = 3
    _write_byte_(0x11, fp);				//HVCr = 0x11 (SubSamp 1x1)
    _write_byte_(1, fp);				//QTCr Normally equal to QTCb = 1

    //DHT
    _write_word_(0xFFC4, fp);		//marker = 0xFFC4
    _write_word_(0x01A2, fp);		//length = 0x01A2
    _write_byte_(0, fp);			//HTYDCinfo bit 0..3	: 8ber of HT (0..3), for Y =0
                                    //			bit 4		: type of HT, 0 = DC table,1 = AC table
                                    //			bit 5..7	: not used, must be 0
    _write_(Standard_DC_Luminance_NRCodes, sizeof(Standard_DC_Luminance_NRCodes), fp);	//DC_L_NRC
    _write_(Standard_DC_Luminance_Values, sizeof(Standard_DC_Luminance_Values), fp);		//DC_L_VALUE
    _write_byte_(0x10, fp);			//HTYACinfo
    _write_(Standard_AC_Luminance_NRCodes, sizeof(Standard_AC_Luminance_NRCodes), fp);
    _write_(Standard_AC_Luminance_Values, sizeof(Standard_AC_Luminance_Values), fp); //we'll use the standard Huffman tables
    _write_byte_(0x01, fp);			//HTCbDCinfo
    _write_(Standard_DC_Chrominance_NRCodes, sizeof(Standard_DC_Chrominance_NRCodes), fp);
    _write_(Standard_DC_Chrominance_Values, sizeof(Standard_DC_Chrominance_Values), fp);
    _write_byte_(0x11, fp);			//HTCbACinfo
    _write_(Standard_AC_Chrominance_NRCodes, sizeof(Standard_AC_Chrominance_NRCodes), fp);
    _write_(Standard_AC_Chrominance_Values, sizeof(Standard_AC_Chrominance_Values), fp);

    //SOS
    _write_word_(0xFFDA, fp);		//marker = 0xFFC4
    _write_word_(12, fp);			//length = 12
    _write_byte_(3, fp);			//nrofcomponents, Should be 3: truecolor JPG

    _write_byte_(1, fp);			//Idy=1
    _write_byte_(0, fp);			//HTY	bits 0..3: AC table (0..3)
                                    //		bits 4..7: DC table (0..3)
    _write_byte_(2, fp);			//IdCb
    _write_byte_(0x11, fp);			//HTCb

    _write_byte_(3, fp);			//IdCr
    _write_byte_(0x11, fp);			//HTCr

    _write_byte_(0, fp);			//Ss not interesting, they should be 0,63,0
    _write_byte_(0x3F, fp);			//Se
    _write_byte_(0, fp);			//Bf
}
