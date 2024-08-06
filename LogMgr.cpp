#include "LogMgr.h"
#include <qchar.h>

using namespace std;

std::unique_ptr<LogMgr> LogMgr::singleton_;
std::mutex              LogMgr::mutex_;

LogMgr *LogMgr::getInst()
{
    std::lock_guard<std::mutex> lock(mutex_);

    if (singleton_ == nullptr)
    {
        singleton_.reset(new LogMgr());
    }
    return singleton_.get();
}

void LogMgr::sendLogMsg(QString msg)
{
    emit sigSendLogMsg(QString("> ") + msg.left(500));
}

LogMgr::LogMgr(QObject *parent) : QObject{parent}
{
}
