#include "LogMgr.h"
#include "QtLog.h"
#include "Utils.h"

#include <QApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <qglobal.h>
#include <qtimer.h>

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QApplication app(argc, argv);

    auto log_path    = QtUtils::getAppDirPath();
    auto date__y_m_d = QtUtils::DateTime::getCurDateStr();

    log_path += "/log/" + date__y_m_d + "/";
    
    QtUtils::mkMutiDir(log_path);

    logSysInit(log_path + "qt.log");

    QQmlApplicationEngine engine;

    const QUrl url(QStringLiteral("qrc:/main.qml"));

    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url](QObject *obj, const QUrl &objUrl)
        {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    
    engine.rootContext()->setContextProperty("logMgr", LogMgr::getInst());

    engine.load(url);

    QTimer::singleShot(1000,
                       []()
                       {
                           qDebug() << u8"CPP测试LOG";
                       });

    return app.exec();
}
