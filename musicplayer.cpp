#include <algorithm>
#include <iostream>
#include <memory>
#include <thread>

#include <fluidsynth.h>
#include <QFile>

#include <musicplayer.h>
#include <data.h>

// wrapper functions for fluidsynth to load the soundfont file from memory
// not actually being used rn, and they don't completely work
// but I don't want to delete them because they took too much of my time :(
namespace sf {
    void *open(const char *filename);
    int read(void *buf, int count, void *handle);
    int seek(void *handle, long offset, int origin);
    long tell(void *handle);
    int close(void *handle);
}

MusicPlayer::MusicPlayer(QObject *parent) : QObject(parent),
    timer(new QTimer()),
    song_events(),
    synth_settings(new_fluid_settings()),
    song_index(0),
    sixteenths_timestamp(0)
{
    this->timer->setSingleShot(false);
    this->timer->setInterval(150);
    connect(this->timer, &QTimer::timeout, this, &MusicPlayer::timeout);

    fluid_settings_setstr(this->synth_settings, "audio.driver", "pulseaudio");
    this->synth = new_fluid_synth(this->synth_settings);
    fluid_synth_sfload(this->synth, "assets/Grand Piano.sf2", true);
    this->audio_driver = new_fluid_audio_driver(this->synth_settings, this->synth);
}

MusicPlayer::~MusicPlayer() {
    delete_fluid_synth(this->synth);
    delete_fluid_settings(this->synth_settings);
    delete_fluid_audio_driver(this->audio_driver);
}

void MusicPlayer::setSong(Song song) {
    int sixteenth_count = 0;

    for (const Chord& chord : song) {
        int min_duration = std::numeric_limits<int>::max();

        for (const Note& note : chord) {
            int pitch = note.getPitch();
            int note_duration = note.getSixteenthsDuration();

            this->song_events.push_back(MidiEvent {
                .start = true,
                .sixteenths_timestamp = sixteenth_count,
                .pitch = pitch
            });
            this->song_events.push_back(MidiEvent {
                .start = false,
                .sixteenths_timestamp = sixteenth_count + note_duration,
                .pitch = pitch
            });

            if (note_duration < min_duration) {
                min_duration = note_duration;
            }
        }

        sixteenth_count += min_duration;
    }

    std::sort(this->song_events.begin(), this->song_events.end(),
        [](const MidiEvent& left, const MidiEvent& right) {
            return left.sixteenths_timestamp < right.sixteenths_timestamp;
        }
    );
    this->song_index = this->song_events.size();
}

void MusicPlayer::playSong() {
    this->song_index = 0;
    this->sixteenths_timestamp = 0;
    this->timer->start();
    this->sixteenthStep();
}

void MusicPlayer::timeout() {
    this->sixteenthStep();
}

void MusicPlayer::sixteenthStep() {
    for (; this->song_index < this->song_events.size(); ++this->song_index) {
        const MidiEvent& event = this->song_events[this->song_index];
        if (event.sixteenths_timestamp <= this->sixteenths_timestamp) {
            if (event.start) {
                std::cout << "Playing note " << event.pitch << std::endl;
                fluid_synth_noteon(this->synth, 0, event.pitch, 100);
            }
            else {
                fluid_synth_noteoff(this->synth, 0, event.pitch);
            }
        }
        else {
            this->sixteenths_timestamp++;
            return;
        }
    }

    // we return early if we hit an event after our timestamp into the song.
    // this means that if we've reached this point, the song is over
    this->timer->stop();
}

void MusicPlayer::stopSong() {
    for (; this->song_index < this->song_events.size(); ++this->song_index) {
        const MidiEvent& event = this->song_events[this->song_index];
        if (!event.start) {
            fluid_synth_noteoff(this->synth, 0, event.pitch);
        }
    }
    this->timer->stop();
}

namespace sf {
    void *open(const char *filename) {
        std::cout << "opening file" << std::endl;
        QFile *qresource = new QFile(filename);
        if (!qresource->open(QIODevice::ReadOnly)) {
            std::cout << "open failed" << std::endl;
        }
        return qresource;
    }

    int read(void *buf, int count, void *handle) {
        std::cout << "entering read (" << count << ") = ";
        std::vector<char> tmp_buf(count);
        QFile *qresource = reinterpret_cast<QFile *>(handle);
        auto c = qresource->read(tmp_buf.data(), count);
        if (c == count) {
            char *cbuf = reinterpret_cast<char *>(buf);
            for (int i = 0; i < tmp_buf.size(); ++i) {
                cbuf[i] = tmp_buf[i];
            }
            std::cout << std::endl;
            return FLUID_OK;
        }
        std::cout << c << std::endl;
        return FLUID_FAILED;
    }

    int seek(void *handle, long offset, int origin) {
        std::cout << "entering seek (" << offset << ", " << origin << ")" << std::endl;
        QFile *qresource = reinterpret_cast<QFile *>(handle);
        qresource->seek((long)origin + offset);
        return FLUID_OK;
    }

    long tell(void *handle) {
        std::cout << "entering tell = ";
        QFile *qresource = reinterpret_cast<QFile *>(handle);
        auto p = qresource->pos();
        std::cout << p << std::endl;
        return p;
    }

    int close(void *handle) {
        std::cout << "entering close" << std::endl;
        QFile *qresource = reinterpret_cast<QFile *>(handle);
        qresource->close();
        delete qresource;
        return FLUID_OK;
    }
}
