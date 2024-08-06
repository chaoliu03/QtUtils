#ifndef QT_UTILS_H
#define QT_UTILS_H

#include <QChar>
#include <QDateTime>
#include <QObject>
#include <mutex>

#define TO_STR(x) (#x)

class QScreen;
class QSystemSemaphore;
class QtUtils : public QObject
{
    Q_OBJECT
public:
    explicit QtUtils(QObject *parent = nullptr);

    static QString getAppDirPath();
    static bool    IsFileExist(QString fullFilePath);

    static QString getHostMacAddress();
    static QString getHostMacAddressWithoutColon();
    static void    setIsAutoRun(QString app_name, bool is_auto_run);
    static bool    isAutoRun(QString app_name);

    static QString getInteger(double value);
    static QString getDecimal(double value);

    static QString floorDecimalStr(const double &value, int decimal_places = 2);
    static double  floorDecimal(const double &value, int decimal_places = 2);
    static double  roundDecimal(const double &value, int decimal_places = 2);
    static double  ceilDecimal(const double &value, int decimal_places = 2);

    static double floorDecimal2(const double &value, int decimal_places);

    static QString generateRandomNum(int length = 1);

    static int getNumLength(unsigned long long num_in);

    static bool isValidPriceNum(QString num_str);

    static QString mkMutiDir(const QString path);

    static void setCursorState(bool state);

    static QString getColorWithOpacity(QString color, float opacity = 1);

    static std::string generateRandStr(int length = 10);

    static bool isGoodsBarcode(QString in_str);
    static void truncateFile(QString file_name);

    class String;
    class Screen;
    class DateTime;
    class Encrypt;
    class Barcode;
    class Network;
    class System;
    class Printer;
signals:
};


class QtUtils::String : public QObject
{
public:
    static QString fillZeroFront(QString in_str, unsigned int num);

    static bool isHavePrefixs(QString in_str, std::vector<QString> prefix_vec);

    static QString     getClearNumStr(double num_in, int digital = 3);
    static std::string getClearNumStr2(std::string str_in);

    static bool isDigital(QString s);
};

class QtUtils::Screen
{
public:
    static QtUtils::Screen *GetInstance();

    int    GetScreenNum();
    double GetScreenWidth(int index = 0);
    int    GetScreenHeight(int index = 0);
    //    Singleton(Singleton &other)       = delete;
    //    void operator=(const Singleton &) = delete;

private:
    Screen();
    static std::mutex mutex_;

private:
    static QtUtils::Screen *singleton_;

    QList<QScreen *> list_screen_;
};
class QtUtils::DateTime
{
public:
    //    static QtUtils::DateTime *GetInstance();
    static QString   getCurYearDateStr();
    static QString   getOffsetYearDateTimeStr(int year = 0, int month = 0, int day = 0, int hour = 0, int minute = 0);
    static QString   getOffsetYearDateStr(int year = 0, int month = 0, int day = 0);
    static QString   getCurDateStr();
    static QString   getCurTimeStr();
    static QString   getCurDateTimeStr();
    static QString   getTimestampMSecsStr();
    static long long getTimestampMSecs();
    static int       getDaysByYearMonth(int year, int month);
    static QString   sec2Day(int n);
    static QString   getWeek(); //获取日期
};

class QtUtils::Encrypt
{
public:
    static QString str2Md5(QString str);

    static QString fileMd5(const QString &source_file_path);
};

class QtUtils::Barcode
{
public:
    static bool isEan13Valid(const QString barcode);

    static bool isEan18Valid(const QString barcode);
};

class QtUtils::Network : public QObject
{
public:
    static bool isValidIp(const QString ip);

    static bool initWinSocket();

    static int getFileInfoSize(QString url);

    static bool downloadFileFromURL(const QString &url, const QString &filePath, int timeout = -1);
};

class QtUtils::System
{
public:
    static void setAutoRun(QString tag_str, bool is_auto_run = true);

    static bool isAutoRun(QString tag_str);

    static void shutdown();
};

class QtUtils::Printer
{
public:
    static double dpi2Mm(double dpi);

    static double mm2Dpi(double mm);
};


#define CLEAR_DECIMAL QtUtils::String::getClearNumStr

#define FLOOR_DECIMAL QtUtils::floorDecimal
#define ROUND_DECIMAL QtUtils::roundDecimal
#define CEIL_DECIMAL QtUtils::ceilDecimal

#endif // QT_UTILS_H
