#include <iostream>

#include <musicreader.h>

MusicReader::MusicReader(QObject *parent) : QObject(parent)
{

}

void MusicReader::readSong() {
    Song song {
        Chord { Note::quarter(6) },
        Chord { Note::quarter(5) },
        Chord { Note::half(4) },

        Chord { Note::quarter(6) },
        Chord { Note::quarter(5) },
        Chord { Note::half(4) },

        Chord { Note::eigth(4) },
        Chord { Note::eigth(4) },
        Chord { Note::eigth(4) },
        Chord { Note::eigth(4) },
        Chord { Note::eigth(5) },
        Chord { Note::eigth(5) },
        Chord { Note::eigth(5) },
        Chord { Note::eigth(5) },

        Chord { Note::quarter(6) },
        Chord { Note::quarter(5) },
        Chord { Note::half(4) },
    };
    std::cout << "Song read!" << std::endl;
    emit songRead(song);
}
