#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <musicplayer.h>
#include <musicreader.h>

// These are taken ownership of by the QML engine when they're returned from
// the below singleton provider functions
MusicReader *music_reader = new MusicReader();
MusicPlayer *music_player = new MusicPlayer();

QObject *musicReaderProvider(QQmlEngine *qml, QJSEngine *js) {
    return music_reader;
}

QObject *musicPlayerProvider(QQmlEngine *qml, QJSEngine *js) {
    return music_player;
}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QObject::connect(music_reader, &MusicReader::songRead, music_player, &MusicPlayer::setSong);

    qmlRegisterSingletonType<MusicReader>("MusicReader", 1, 0, "MusicReader", &musicReaderProvider);
    qmlRegisterSingletonType<MusicPlayer>("MusicPlayer", 1, 0, "MusicPlayer", &musicPlayerProvider);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
