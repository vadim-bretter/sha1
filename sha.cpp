#include "sha.h"

unsigned int shl(unsigned int a, unsigned int s)
{
    return ((a<<s)|(a>>(32-s)));
}

void hash_one(unsigned int *W, unsigned int &H0, unsigned int &H1,
                unsigned int &H2, unsigned int &H3, unsigned int &H4)
{
    //��������������� ����������
    unsigned int a, b, c, d, e, f, k, temp;

    //���������
    for(int i = 16; i < 80; i++) {
        W[i] = shl((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]), 1);
    }

    a = H0;
    b = H1;
    c = H2;
    d = H3;
    e = H4;

    for(int i = 0; i < 80; i++) {
        if (i >= 0 && i < 20) {
            f = (b & c) | ((~ b) & d);
            k = 0x5A827999;
        }
        else if (i >= 20 && i < 40) {
            f = b ^ c ^ d;
            k = 0x6ED9EBA1;
        }
        else if (i >= 40 && i < 60) {
            f = (b & c) | (b & d) | (c & d);
            k = 0x8F1BBCDC;
        }
        else if (i >= 60 && i < 80) {
            f = b ^ c ^ d;
            k = 0xCA62C1D6;
        }

        temp = shl(a, 5) + f + e + k + W[i];
        e = d;
        d = c;
        c = shl(b, 30);
        b = a;
        a = temp;
    }
    H0 += a;
    H1 += b;
    H2 += c;
    H3 += d;
    H4 += e;
}

QString sha1(char *name)
{
    //��������� ����
    QFile file(name);
    file.open(QIODevice::ReadWrite);

    //���������� ������ �����
    qint64 file_size = file.size();
    file.close();

    //������� ������� ������� ���� ��������
    int last_size = file_size % SIZE_OF_BLOCK;

    int zero_size;
    if (last_size < 56) zero_size = 56 - last_size;
    else zero_size = SIZE_OF_BLOCK - last_size + 56;

   //������� ���������� ������
    qint64 num_of_blocks = (file_size + zero_size + 8) / SIZE_OF_BLOCK;

    //������� ��������� ������ ������ ���� ��������������
    int blocks_mod = 1;
    if (last_size >= 56)
        blocks_mod++;

    //�������� ��������������

    //��������� �������� ��������
    unsigned int H0 = 0x67452301;
    unsigned int H1 = 0xefcdab89;
    unsigned int H2 = 0x98badcfe;
    unsigned int H3 = 0x10325476;
    unsigned int H4 = 0xc3d2e1f0;

    //������ �� ����� ������� �� 64 �����
    FILE *ff = fopen(name, "rb");
    unsigned int R[80];
    unsigned char b[64];
    for(qint64 i = 0; i < num_of_blocks - blocks_mod; i++)
    {
        //��������� 16 4-� �������� ����� � ������ 16 ��������� ������� R
        fread(b, 1, 64, ff);
        for(int j = 0; j < 16; j++)
        {
            R[j] = 256*256*256*b[4*j] + 256*256*b[4*j + 1] + 256*b[4*j + 2] + b[4*j + 3];
        }
        hash_one(R, H0, H1, H2, H3, H4);
    }
    //���������� ����� �� ��������
    if (last_size < 56)
    {
        //��������� ���� ��������������
        unsigned char buf[64];
        //������ ������� ����� � ��������� ���
        fread(buf, 1, last_size, ff);
        fclose(ff);

        //���������� ���� �����������
        int it = last_size;
        buf[it++] = 0x80;
        for(int i = 0; i < zero_size - 1; buf[it++] = 0, i++);

        //���������� ����� � �����
        quint64 bit_file_size = qToBigEndian(file_size*8);
        for(int i = 0; i < 8; i++) {
            buf[it++] = bit_file_size & 0xFF;
            bit_file_size /= 256;
        }

        //�������� ��������� ���� - ������������ ��� ��� ������ int ��������
        for(int i = 0; i < 16; i++) {
            R[i] = 256*256*256*buf[4*i] + 256*256*buf[4*i + 1] + 256*buf[4*i + 2] + buf[4*i + 3];
        }
        hash_one(R, H0, H1, H2, H3, H4);
    }
    else
    {
        //��������� 2 ����� ��������������
        unsigned char buf[128];
        //������ ������� ����� � ��������� ���
        fread(buf, 1, last_size, ff);
        fclose(ff);

        //���������� ���� �����������
        int it = last_size;
        buf[it++] = 0x80;
        for(int i = 0; i < zero_size - 1; buf[it++] = 0, i++);

        //���������� ����� � �����
        quint64 bit_file_size = qToBigEndian(file_size*8);
        for(int i = 0; i < 8; i++) {
            buf[it++] = bit_file_size & 0xFF;
            bit_file_size /= 256;
        }

        //�������� 1-� �� ��������� ������ - ������������ ��� ��� ������ int ��������
        for(int i = 0; i < 16; i++) {
            R[i] = 256*256*256*buf[4*i] + 256*256*buf[4*i + 1] + 256*buf[4*i + 2] + buf[4*i + 3];
        }
        hash_one(R, H0, H1, H2, H3, H4);

        //�������� 2-� �� ��������� ������ - ������������ ��� ��� ������ int ��������
        for(int i = 16; i < 32; i++) {
            R[i - 16] = 256*256*256*buf[4*i] + 256*256*buf[4*i + 1] + 256*buf[4*i + 2] + buf[4*i + 3];
        }
        hash_one(R, H0, H1, H2, H3, H4);
    }
    //������� ���
    QString str = QString ("%1%2%3%4%5").arg(H0,-1,16).arg(H1,-1,16).arg(H2,-1,16).arg(H3,-1,16).arg(H4,-1,16);
    return str;
}
