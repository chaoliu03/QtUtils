#ifndef LOGMGR_H
#define LOGMGR_H

#include <QObject>
#include <mutex>

class LogMgr : public QObject
{
    Q_OBJECT
public:
    static LogMgr *getInst();
    
    void sendLogMsg(QString msg);

private:
    explicit LogMgr(QObject *parent = nullptr);
    static std::unique_ptr<LogMgr> singleton_;
    static std::mutex              mutex_;

signals:
    void sigSendLogMsg(QString msg);
};

#endif // LOGMGR_H
