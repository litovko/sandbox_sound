#include <QApplication>
#include <QQmlApplicationEngine>
//#include <SDL.h>
#include <c_wavfile.h>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    qmlRegisterType<c_wavfile>("litovko", 1, 0, "WavFile");
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
