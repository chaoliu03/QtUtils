#include "Utils.h"

#ifdef IS_WINDOWS
#include <Windows.h>
#endif

#include <QApplication>
#include <QColor>
#include <QCryptographicHash>
#include <QDateTime>
#include <QDir>
#include <QFileInfo>
#include <QGuiApplication>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkInterface>
#include <QNetworkReply>
#include <QRegularExpression>
#include <QScreen>
#include <QSettings>
#include <QTextCodec>
#include <QTimer>
#include <cmath>
#include <iomanip>
#include <mutex>
#include <random>
#include <sstream>
#include "qrandom.h"

#ifdef IS_WINDOWS

#pragma comment(lib, "Iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib") //这是链接API相关连的Ws2_32.lib静态库

#endif

QtUtils::QtUtils(QObject *parent) : QObject{parent}
{
}

QString QtUtils::getAppDirPath()
{
    return qApp->applicationDirPath();
}

bool QtUtils::IsFileExist(QString fullFilePath)
{
    QFileInfo fileInfo(fullFilePath);
    if (fileInfo.exists())
        return true;
    return false;
}

QString QtUtils::getHostMacAddress()
{
    QList<QNetworkInterface> nets       = QNetworkInterface::allInterfaces(); // 获取所有网络接口列表
    int                      nCnt       = nets.count();
    QString                  strMacAddr = "";
    for (int i = 0; i < nCnt; i++)
    {
        // 如果此网络接口被激活并且正在运行并且不是回环地址，则就是我们需要找的Mac地址
        if (nets[i].flags().testFlag(QNetworkInterface::IsUp) && nets[i].flags().testFlag(QNetworkInterface::IsRunning) && !nets[i].flags().testFlag(QNetworkInterface::IsLoopBack))
        {
            strMacAddr = nets[i].hardwareAddress();
            break;
        }
    }
    return strMacAddr;
}

QString QtUtils::getHostMacAddressWithoutColon()
{
    QList<QNetworkInterface> nets         = QNetworkInterface::allInterfaces(); // 获取所有网络接口列表
    int                      nCnt         = nets.count();
    QString                  str_mac_addr = "";
    for (int i = 0; i < nCnt; i++)
    {
        // 如果此网络接口被激活并且正在运行并且不是回环地址，则就是我们需要找的Mac地址
        if (nets[i].flags().testFlag(QNetworkInterface::IsUp) && nets[i].flags().testFlag(QNetworkInterface::IsRunning) && !nets[i].flags().testFlag(QNetworkInterface::IsLoopBack))
        {
            str_mac_addr = nets[i].hardwareAddress();
            break;
        }
    }
    str_mac_addr = str_mac_addr.replace(QRegExp(":"), "");
    return str_mac_addr;
}

void QtUtils::setIsAutoRun(QString app_name, bool is_auto_run)
{
    QSettings reg("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);

    if (is_auto_run)
    {
        QString strAppPath = QDir::toNativeSeparators(QCoreApplication::applicationFilePath());
        reg.setValue(app_name, strAppPath);
    }
    else
    {
        reg.setValue(app_name, "");
    }
}

bool QtUtils::isAutoRun(QString app_name)
{
    QSettings reg("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    return reg.value(app_name).toString() != "";
}

QString QtUtils::getInteger(double value)
{
    auto total     = value;
    auto total_str = QString::number(total, 'f', 3);
    auto total_vec = total_str.split('.');
    return total_vec[0];
}

QString QtUtils::getDecimal(double value)
{
    auto total     = value;
    auto total_str = QString::number(total, 'f', 3);
    auto total_vec = total_str.split('.');
    if (total_vec[1] == "000")
        return "";

    total_vec[1].chop(1);
    return "." + total_vec[1];
}

QString QtUtils::floorDecimalStr(const double &value, int decimal_places)
{
    std::stringstream sstream1;
    sstream1 << std::fixed << std::setprecision(decimal_places) << value;
    auto final_str = sstream1.str();
    return QString::fromStdString(final_str);
}

double QtUtils::floorDecimal(const double &value, int decimal_places)
{
    std::stringstream sstream1;
    sstream1 << std::fixed << std::setprecision(decimal_places + 1) << value;
    auto final_str = sstream1.str();
    return std::stod(final_str.substr(0, final_str.size() - 1));
}

double QtUtils::floorDecimal2(const double &value, int decimal_places)
{
    int  multiplier = std::pow(10, decimal_places);
    auto tmp1       = (value + 0.000001) * multiplier;
    auto val        = std::floor(tmp1);
    auto result     = (double)val / multiplier;
    return result;
}

QString QtUtils::generateRandomNum(int length)
{
    return QString::number(QRandomGenerator::global()->generate() % (int)pow(10, length));
}

int QtUtils::getNumLength(unsigned long long num_in)
{
    int len = 0;              // 初始长度为0
    for (; num_in > 0; ++len) // 判断num是否大于0，否则长度+1
        num_in /= 10;         // 使用除法进行运算，直到num小于1
    return len;               // 返回长度的值
}

bool QtUtils::isValidPriceNum(QString num_str)
{
    QRegularExpression regex(QRegularExpression::anchoredPattern(QLatin1String(R"(^[0-9]{0,4}(?:\.[0-9]{1,2})?$)")));

    auto match_type = regex.match(num_str);
    return match_type.hasMatch();
}

QString QtUtils::mkMutiDir(const QString path)
{
    if (path.isEmpty())
    {
        return "";
    }

    QDir dir(path);

    if (dir.exists(path))
    {
        return path;
    }

    QString parentDir = mkMutiDir(path.mid(0, path.lastIndexOf('/')));

    QString dirname = path.mid(path.lastIndexOf('/') + 1);

    QDir parentPath(parentDir);

    if (!dirname.isEmpty())

        parentPath.mkpath(dirname);

    return parentDir + "/" + dirname;
}

void QtUtils::setCursorState(bool state)
{
    state ? QApplication::setOverrideCursor(Qt::ArrowCursor) : QApplication::setOverrideCursor(Qt::BlankCursor);
}

QString QtUtils::getColorWithOpacity(QString color, float opacity)
{
    QColor tmp_color(color);
    tmp_color.setAlphaF(opacity);

    QString hexColor = QString("#%1%2%3%4").arg(tmp_color.alpha(), 2, 16, QLatin1Char('0')).arg(tmp_color.red(), 2, 16, QLatin1Char('0')).arg(tmp_color.green(), 2, 16, QLatin1Char('0')).arg(tmp_color.blue(), 2, 16, QLatin1Char('0'));

    return hexColor;
}

std::string QtUtils::generateRandStr(int length)
{
    using std::default_random_engine;
    using std::random_device;
    using std::string;

    char   tmp;    // tmp: 暂存一个随机数
    string buffer; // buffer: 保存返回值

    // 下面这两行比较重要:
    random_device         rd;           // 产生一个 std::random_device 对象 rd
    default_random_engine random(rd()); // 用 rd 初始化一个随机数发生器 random

    for (int i = 0; i < length; i++)
    {
        tmp = random() % 36; // 随机一个小于 36 的整数，0-9、A-Z 共 36 种字符
        if (tmp < 10)
        { // 如果随机数小于 10，变换成一个阿拉伯数字的 ASCII
            tmp += '0';
        }
        else
        { // 否则，变换成一个大写字母的 ASCII
            tmp -= 10;
            tmp += 'A';
        }
        buffer += tmp;
    }
    return buffer;
}

bool QtUtils::isGoodsBarcode(QString in_str)
{
    bool is_numeric = false;
    in_str.toULongLong(&is_numeric);

    if (!is_numeric || in_str.length() < 8)
        return false;

    //缩短码
    {
        if (in_str.length() == 8)
        {
            return true;
        }
    }

    //无码?
    {
        if (in_str.length() == 12)
        {
            return true;
        }
    }

    //中国
    {
        std::vector<QString> prefix_vec;
        prefix_vec.push_back("69");
        if (QtUtils::String::isHavePrefixs(in_str, prefix_vec))
        {
            if (in_str.length() == 13)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }


    //日本
    {
        std::vector<QString> prefix_vec;

        for (int i = 45; i <= 49; ++i)
        {
            prefix_vec.push_back(QString::number(i));
        }

        if (QtUtils::String::isHavePrefixs(in_str, prefix_vec))
        {
            if (in_str.length() == 13)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }

    //英国
    {
        std::vector<QString> prefix_vec;

        for (int i = 50; i <= 59; ++i)
        {
            prefix_vec.push_back(QString::number(i));
        }

        if (QtUtils::String::isHavePrefixs(in_str, prefix_vec))
        {
            if (in_str.length() == 13)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }

    //德国
    {
        std::vector<QString> prefix_vec;

        for (int i = 400; i <= 440; ++i)
        {
            prefix_vec.push_back(QString::number(i));
        }

        if (QtUtils::String::isHavePrefixs(in_str, prefix_vec))
        {
            if (in_str.length() == 13)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }

    //美国
    {
        if (in_str.length() == 12)
        {
            return true;
        }
    }

    //欧洲
    {
        std::vector<QString> prefix_vec;

        for (int i = 0; i <= 19; ++i)
        {
            std::stringstream string_stream;
            string_stream << std::setfill('0') << std::setw(2) << i;
            prefix_vec.push_back(QString::fromStdString(string_stream.str()));
        }

        if (QtUtils::String::isHavePrefixs(in_str, prefix_vec))
        {
            if (in_str.length() == 13)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }

    return false;
}

void QtUtils::truncateFile(QString file_name)
{
    QFile file(file_name);
    file.open(QFile::WriteOnly | QFile::Truncate);
    file.close();
}

double QtUtils::roundDecimal(const double &value, int decimal_places)
{
    const int multiplier = std::pow(10.0, decimal_places);
    auto      round_num  = (double)(int)std::round(value * multiplier) / multiplier;
    return round_num;
}

double QtUtils::ceilDecimal(const double &value, int decimal_places)
{
    const int multiplier = std::pow(10.0, decimal_places);
    auto      round_num  = (double)(int)std::ceil(value * multiplier) / multiplier;
    return round_num;
}

QtUtils::Screen *QtUtils::Screen::singleton_{nullptr};
std::mutex       QtUtils::Screen::mutex_;

QtUtils::Screen *QtUtils::Screen::GetInstance()
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (singleton_ == nullptr)
        singleton_ = new QtUtils::Screen();
    return singleton_;
}

int QtUtils::Screen::GetScreenNum()
{
    return list_screen_.size();
}

double QtUtils::Screen::GetScreenWidth(int index)
{
    double width = index < list_screen_.size() ? list_screen_.at(index)->geometry().width() : -1;
    return width;
}

int QtUtils::Screen::GetScreenHeight(int index)
{
    return index < list_screen_.size() ? list_screen_.at(index)->geometry().height() : -1;
}

QtUtils::Screen::Screen()
{
    list_screen_ = QGuiApplication::screens();
}

// QtUtils::DateTime *Utils::DateTime::singleton_{nullptr};
// std::mutex     QtUtils::DateTime::mutex_;

// QtUtils::DateTime *QtUtils::DateTime::GetInstance()
//{
//     std::lock_guard<std::mutex> lock(mutex_);
//     if (singleton_ == nullptr)
//         singleton_ = new QtUtils::DateTime();
//     return singleton_;
// }

QString QtUtils::DateTime::getCurYearDateStr()
{
    QDateTime dateTime(QDateTime::currentDateTime());
    return dateTime.toString("yyyy-MM-dd");
}

QString QtUtils::DateTime::getOffsetYearDateTimeStr(int year, int month, int day, int hour, int minute)
{
    QDateTime dateTime(QDateTime::currentDateTime());
    dateTime = dateTime.addYears(year);
    dateTime = dateTime.addMonths(month);
    dateTime = dateTime.addDays(day);
    dateTime = dateTime.addSecs(hour * 60 * 60 + minute * 60);
    return dateTime.toString("yyyy-MM-dd hh:mm:ss");
}

QString QtUtils::DateTime::getOffsetYearDateStr(int year, int month, int day)
{
    QDateTime dateTime(QDateTime::currentDateTime());
    dateTime = dateTime.addYears(year);
    dateTime = dateTime.addMonths(month);
    dateTime = dateTime.addDays(day);
    return dateTime.toString("yyyy-MM-dd");
}

QString QtUtils::DateTime::getCurDateStr()
{
    QDateTime dateTime(QDateTime::currentDateTime());
    return dateTime.toString("yyyy-MM-dd");
}

QString QtUtils::DateTime::getCurTimeStr()
{
    QDateTime dateTime(QDateTime::currentDateTime());
    return dateTime.toString("hh:mm:ss");
}

QString QtUtils::DateTime::getCurDateTimeStr()
{
    QDateTime dateTime(QDateTime::currentDateTime());
    return dateTime.toString("yyyy-MM-dd hh:mm:ss");
}

QString QtUtils::DateTime::getTimestampMSecsStr()
{
    return QString::number(QDateTime::currentMSecsSinceEpoch());
}

long long QtUtils::DateTime::getTimestampMSecs()
{
    return QDateTime::currentMSecsSinceEpoch();
}

int QtUtils::DateTime::getDaysByYearMonth(int year, int month)
{
    if (year < 0 || month > 12 || month < 0)
    {
        return -1;
    }
    int days = 0;
    if (month != 2)
    {
        switch (month)
        {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            days = 31;
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            days = 30;
        }
    }
    else
    {
        // 闰年
        if (year % 4 == 0 && year % 100 != 0 || year % 400 == 0)
        {
            days = 29;
        }
        else
        {
            days = 28;
        }
    }
    return days;
}

QString QtUtils::DateTime::sec2Day(int n)
{
    QString result;

    int day = n / (24 * 3600);
    if (day > 0)
    {
        result += QString::number(day) + "天";
    }
    n        = n % (24 * 3600);
    int hour = n / 3600;
    if (hour > 0)
    {
        result += QString::number(hour) + "时";
    }

    n %= 3600;
    int minutes = n / 60;
    if (minutes > 0)
    {
        result += QString::number(minutes) + "分";
    }

    n %= 60;
    int seconds = n;
    if (seconds > 0)
    {
        result += QString::number(seconds) + "秒";
    }

    return result;
}

QString QtUtils::DateTime::getWeek()
{
    QString result;

    QLocale locale = QLocale::Chinese; //指定中文显示

    result = (locale.toString(QDateTime::currentDateTime(), QString("dddd"))); //这样获得的星期为指定的语言格式下的。此处为星期一 - 星期日

    return result;
}

QString QtUtils::Encrypt::str2Md5(QString str)
{
    return QCryptographicHash::hash(str.toUtf8(), QCryptographicHash::Md5).toHex();
}

QString QtUtils::Encrypt::fileMd5(const QString &source_file_path)
{
    QFile        source_file(source_file_path);
    qint64       file_size   = source_file.size();
    const qint64 buffer_size = 10240;

    if (source_file.open(QIODevice::ReadOnly))
    {
        char buffer[buffer_size];
        int  bytes_read;
        int  read_size = qMin(file_size, buffer_size);

        QCryptographicHash hash(QCryptographicHash::Md5);

        while (read_size > 0 && (bytes_read = source_file.read(buffer, read_size)) > 0)
        {
            file_size -= bytes_read;
            hash.addData(buffer, bytes_read);
            read_size = qMin(file_size, buffer_size);
        }

        source_file.close();
        return QString(hash.result().toHex()).toUpper();
    }
    return QString();
}

bool QtUtils::Barcode::isEan13Valid(const QString barcode)
{
    int sum = 0;
    for (int i = 1; i < 12; i += 2)
    {
        sum += barcode.at(i).digitValue();
    }

    int sum2 = 0;
    for (int i = 0; i < 12; i += 2)
    {
        sum2 += barcode.at(i).digitValue();
    }
    int count = sum * 3 + sum2;
    if ((10 - count % 10) % 10 == barcode.at(12).digitValue())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool QtUtils::Barcode::isEan18Valid(const QString barcode)
{
    int sum = 0;

    for (int i = 1; i < 17; i += 2)
    {
        sum += barcode.at(i).digitValue();
    }

    int sum2 = 0;
    for (int i = 0; i < 17; i += 2)
    {
        sum2 += barcode.at(i).digitValue();
    }

    int count = sum * 3 + sum2;
    if ((10 - count % 10) % 10 == barcode.at(17).digitValue())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool QtUtils::Network::isValidIp(const QString ip)
{
    QRegExp pattern(R"(^(\d{1,3}\.){3}\d{1,3}$)");
    return pattern.exactMatch(ip);
}

bool QtUtils::Network::initWinSocket()
{
#ifdef IS_WINDOWS

    WSADATA wsaData;

    // Initialize Winsock
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        printf("WSAStartup failed: %d\n", iResult);
        return false;
    }

    return true;
#endif
    return false;
}

int QtUtils::Network::getFileInfoSize(QString url)
{
    qDebug() << "getFileInfoSize url:" << url;

    qint64 size = -1;

    QNetworkAccessManager manager;
    // 事件循环，等待请求文件头信息结束;
    QEventLoop loop;
    // 超时，结束事件循环;
    QTimer timer;

    // 发出 Get 请求（这个地方看你的 url 需要 get 还是 post，自己改）
    QNetworkReply *reply = manager.get(QNetworkRequest(url));
    // 忽略 SSL 错误
    reply->ignoreSslErrors();
    if (!reply)
    {
        qDebug() << "replay error!";
        return -1;
    }

    // 有数据则退出 loop
    QObject::connect(reply, SIGNAL(readyRead()), &loop, SLOT(quit()));
    // 请求完成则退出 loop
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    // 超时则退出 loop
    QObject::connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));

    // 超时 5s
    timer.start(1000 * 5);
    loop.exec();

    if (!timer.isActive())
    {
        // 请求超时超时,未获取到文件信息;
        qDebug() << "Request Timeout";
        return -1;
    }
    timer.stop();

    QVariant var = reply->header(QNetworkRequest::ContentLengthHeader);
    size         = var.toLongLong();
    if (size == 0)
    {
        if (reply->hasRawHeader(QString("Content-Length").toUtf8()))
        {
            QString fileSize = reply->rawHeader(QString("Content-Length").toUtf8());
            size             = fileSize.toLongLong();
        }
    }
    qDebug() << "size:" << size << " bytes";

    reply->deleteLater();

    return size;
}

bool QtUtils::Network::downloadFileFromURL(const QString &url, const QString &file_path, int timeout)
{
    timeout *= 1000;

    QFile file(file_path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        return false;
    }

    QNetworkAccessManager m;
    QNetworkRequest       req;

    req.setUrl(QUrl(url));

    QNetworkReply *reply = m.get(req);
    reply->ignoreSslErrors();

    QEventLoop loop;
    QTimer     timer;

    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    QObject::connect(reply, &QNetworkReply::downloadProgress,
                     [=, &file, &timer]()
                     {
                         auto data = reply->readAll();

                         file.write(data);
                         if (timer.isActive())
                         {
                             timer.start(timeout);
                         }
                     });

    if (timeout > 0)
    {
        QObject::connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
        QObject::connect(&timer, &QTimer::timeout,
                         [&]()
                         {
                             reply->abort();
                         });
        timer.start(timeout);
    }

    loop.exec();

    file.write(reply->readAll());

    file.close();

    if (reply->error() != QNetworkReply::NoError)
    {
        return false;
    }

    delete reply;
    return true;
}

void QtUtils::System::setAutoRun(QString tag_str, bool is_auto_run)
{
    QSettings reg("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);

    if (is_auto_run)
    {
        QString strAppPath = QDir::toNativeSeparators(QCoreApplication::applicationFilePath());
        reg.setValue(tag_str, strAppPath);
    }
    else
    {
        reg.setValue(tag_str, "");
    }
}

bool QtUtils::System::isAutoRun(QString tag_str)
{
    QSettings reg("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    return reg.value(tag_str).toString() != "";
}

void QtUtils::System::shutdown()
{
    system("shutdown -s -t 00");
}

double QtUtils::Printer::dpi2Mm(double dpi)
{
    const auto result = dpi / 25.4;
    return result;
}

double QtUtils::Printer::mm2Dpi(double mm)
{
    const auto result = 25.4 * mm;
    return result;
}

QString QtUtils::String::fillZeroFront(QString in_str, unsigned int num)
{
    std::stringstream str_stream;
    str_stream << std::setw(num) << std::setfill('0') << in_str.toStdString(); // 输出：0009
    return QString::fromStdString(str_stream.str());
}

bool QtUtils::String::isHavePrefixs(QString in_str, std::vector<QString> prefix_vec)
{
    if (in_str.length() != 13)
    {
        return false;
    }

    for (auto prefix_item : prefix_vec)
    {
        if (in_str.left(prefix_item.length()) == prefix_item)
        {
            return true;
        }
    }
    return false;
}

QString QtUtils::String::getClearNumStr(double num_in, int digital)
{
    auto tmp    = FLOOR_DECIMAL(num_in, digital);
    auto result = QString::number(tmp, 'f', QLocale::FloatingPointShortest);
    return result;
}

std::string QtUtils::String::getClearNumStr2(std::string str_in)
{
    //删除尾部多余的0，如果尾部以点结束，也删除小数点
    std::string tmps = str_in;
    if (tmps.find(".") > 0)
    {
        size_t fp = tmps.rfind(".");
        size_t f  = tmps.rfind("0");
        while (f > fp)
        {

            if (f != -1)
            {
                tmps = tmps.erase(f);
            }
            f = tmps.rfind("0");
        }
        fp = tmps.rfind(".");
        if (fp == tmps.size() - 1)
        {
            tmps = tmps.erase(fp);
        }
        return (tmps);
    }

    return str_in;
}

bool QtUtils::String::isDigital(QString s)
{
    for (auto c : s)
    {
        if (!c.isDigit())
        {
            return false;
        }
    }
    return true;
}
