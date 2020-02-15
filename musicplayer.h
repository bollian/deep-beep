#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <memory>

#include <QAudioFormat>
#include <QAudioOutput>
#include <QBuffer>
#include <QByteArray>
#include <QObject>

#include <data.h>

class MusicPlayer : public QObject {
    Q_OBJECT
private:
    std::unique_ptr<QAudioOutput> audio_output;
    QAudioFormat audio_format;

    QByteArray buffer_bytes;
    QBuffer audio_buffer;

    Song current_song;

public:
    explicit MusicPlayer(QObject *parent = nullptr);

public slots:
    void setSong(Song song);
    void playSong();

};

#endif // MUSICPLAYER_H
