#include "QtLog.h"
#include <QDebug>
#include <qglobal.h>
#include "LogMgr.h"

QFile *gFileLog = NULL;

char *msgHead[] = {"Debug   ", "Warning ", "Critical", "Fatal   ", "Info    "};

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg          = msg.toUtf8();
    QString    current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");

    QString msgText = "%1 | %6 | %2:%3, %4 | %5\n";
    msgText         = msgText.arg(msgHead[type]).arg(context.file).arg(context.line).arg(context.function).arg(msg.constData()).arg(current_date_time);

    if (gFileLog)
    {
        QTextStream tWrite(gFileLog);
        tWrite << msgText;
    }

    LogMgr::getInst()->sendLogMsg(localMsg);
}

void logSysInit(QString filePath)
{
    gFileLog = new QFile(filePath);
    
    if (!gFileLog->open(QIODevice::WriteOnly | QIODevice::Text)) //| QIODevice::Truncate
    {
        qDebug() << "LOG文件创建失败";
    }

    qInstallMessageHandler(myMessageOutput);
}
