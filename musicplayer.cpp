#include <iostream>
#include <memory>

#include <musicplayer.h>
#include <data.h>

MusicPlayer::MusicPlayer(QObject *parent) : QObject(parent),
    audio_output(),
    audio_format(),
    buffer_bytes(),
    audio_buffer(),
    current_song()
{
    this->audio_format.setCodec("audio/pcm");
    this->audio_format.setSampleRate(44'100);
    this->audio_format.setSampleSize(8);
    this->audio_format.setChannelCount(1);
    this->audio_format.setByteOrder(QAudioFormat::Endian::LittleEndian);
    this->audio_format.setSampleType(QAudioFormat::SampleType::UnSignedInt);

    this->audio_output = std::make_unique<QAudioOutput>(this->audio_format);
}

void MusicPlayer::setSong(Song song) {
    this->current_song = std::move(song);
}

void MusicPlayer::playSong() {
    if (audio_buffer.isOpen()) {
        this->buffer_bytes.clear();
        audio_buffer.seek(0);
    }
    std::cout << "Song " << this->current_song.size() << " played!" << std::endl;
}
