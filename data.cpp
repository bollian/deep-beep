#include <cmath>

#include <data.h>

// the frequency of A4 (center A)
static const float A4_FREQ = 440.0;

// the number of letters in an octave (A-G)
static const int NOTE_LETTER_COUNT = 7;

// the number of notes in an octave (A-G plus sharps and flats)
static const int OCTAVE_SIZE = 12;

Note::Note(int c4_distance, int sixteenths, Naturality modifier) :
    c4_distance(c4_distance),
    sixteenths(sixteenths),
    modifier(modifier)
{

}

Note Note::whole(int c4_distance, Naturality modifier) {
    return Note(c4_distance, 16, modifier);
}

Note Note::half(int c4_distance, Naturality modifier) {
    return Note(c4_distance, 8, modifier);
}

Note Note::quarter(int c4_distance, Naturality modifier) {
    return Note(c4_distance, 4, modifier);
}

Note Note::eigth(int c4_distance, Naturality modifier) {
    return Note(c4_distance, 2, modifier);
}

Note Note::sixteenth(int c4_distance, Naturality modifier) {
    return Note(c4_distance, 1, modifier);
}

int Note::getPitch() const {
    int letter_num = this->c4_distance % NOTE_LETTER_COUNT;

    // we need to map the letter numbers to note numbers with this weird
    // lookup since not all letters have sharps/flats
    int octave_height = 0;
    switch (letter_num) {
    case 0:
        octave_height = 0;
        break;
    case 1:
        octave_height = 2;
        break;
    case 2:
        octave_height = 4;
        break;
    case 3:
        octave_height = 5;
        break;
    case 4:
        octave_height = 7;
        break;
    case 5:
        octave_height = 9;
        break;
    case 6:
        octave_height = 11;
        break;
    }
    if (this->modifier == Naturality::Sharp) {
        ++octave_height;
    }
    else if (this->modifier == Naturality::Flat) {
        --octave_height;
    }

    int octave_num = (this->c4_distance - letter_num) / NOTE_LETTER_COUNT;
    int note_number = OCTAVE_SIZE * octave_num + octave_height;
    note_number += 60; // since we're doing everything relative to C4
    return note_number;
}

int Note::getSixteenthsDuration() const {
    return this->sixteenths;
}

Naturality Note::getModifier() const {
    return this->modifier;
}

