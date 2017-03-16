#ifndef TRANSFORM
#define TRANSFORM

#include<QString>
//QString转换为char*
static char* T_QString2Char(QString str)
{
	QByteArray b = str.toLatin1();
	char* strChar = b.data();
	return strChar;
}

/*字符串等的转换操作*/
//这么做的原因是因为需要显示中文
static char* T_Char2Char(char* str)
{
	QString err = QString::fromLocal8Bit(str);
	return T_QString2Char(err);
}


static QString TrimEnd(QString str)
{
	str = str.left(str.length() - 1);
	
	return str;
}

static QString T_char2QString(char* str)
{
	QString str1 = QString::fromLocal8Bit(str);
	return str1;
}

static int RoundUpToTheNextHighestPowerOf2(int v)
{
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v++;
	return v;
}

#endif // !TRANSFORM
