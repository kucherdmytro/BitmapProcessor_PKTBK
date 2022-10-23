#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QCommandLineParser>
#include <QDebug>
#include <QQmlContext>

#include "appengine.h"
#include "filelistmodel.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);

    QCommandLineOption pathOption(QStringList {"p", "path"}, "folder path", "full path to target folder");
    parser.addOption(pathOption);
    parser.process(app);

    QDir rootDir;
    QString dirPath;
    if(parser.isSet(pathOption))
    {
        rootDir.setPath(parser.value(pathOption));
        if(!rootDir.exists())
        {
            dirPath = qApp->applicationDirPath();
        }
        else
        {
            dirPath = parser.value(pathOption);
        }
    }
    else
    {
        dirPath = qApp->applicationDirPath();
    }

    AppEngine appEngine(dirPath);
    qmlRegisterSingletonInstance<AppEngine>("AppEngine", 1, 0, "AppEngine", &appEngine);
    qmlRegisterSingletonInstance<FileListFilterModel>("AppEngine.Models", 1, 0, "FileListFilterModel", FileListFilterModel::instance());

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/src/gui/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, [url](QObject *obj, const QUrl &objUrl)
    {
        if(!obj && url == objUrl) QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.load(url);
    return app.exec();
}
