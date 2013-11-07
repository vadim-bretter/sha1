#include "sha.h"
#include <QString>
#include <QDataStream>
#include <fstream>
using namespace std;

#define size_max 1024
QString Sha1( const QString filename_i )
{
    return sha1(filename_i.toUtf8().data());
}
