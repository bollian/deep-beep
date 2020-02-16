#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <memory>
#include <vector>

#include <QAudioFormat>
#include <QAudioOutput>
#include <QBuffer>
#include <QByteArray>
#include <QObject>
#include <QTimer>

#include <fluidsynth.h>

#include <data.h>

typedef struct {
    bool start;
    int sixteenths_timestamp;
    int pitch;
} MidiEvent;

class MusicPlayer : public QObject {
    Q_OBJECT

private:
    QTimer *timer;

    std::vector<MidiEvent> song_events;

    fluid_settings_t *synth_settings;
    fluid_synth_t *synth;
    fluid_audio_driver_t *audio_driver;

    /*
     * Stores the next index in song_events to look at when the timer comes back.
     * If this is equal to the length of song_events, we aren't playing anything.
     * */
    int song_index;

    /*
     * Stores the last timestamp
     * */
    int sixteenths_timestamp;

    /*
     * Play the next events in our song and advance our index and timestamp
     * */
    void sixteenthStep();

    /*
     * Looks for any remaining stop note events in the song_events list
     * and tells the audio device to stop playing them.
     * */
    void stopSong();

private slots:
    void timeout();

public:
    explicit MusicPlayer(QObject *parent = nullptr);
    virtual ~MusicPlayer();

public slots:
    void setSong(Song song);
    void playSong();

};

#endif // MUSICPLAYER_H
