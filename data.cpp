#include <cmath>

#include <data.h>

// the frequency of A4 (center A)
static const float A4_FREQ = 440.0;

// the number of letters in an octave (A-G)
static const int NOTE_LETTER_COUNT = 7;

// the number of notes in an octave (A-G plus sharps and flats)
static const int OCTAVE_SIZE = 12;

Note::Note(int a4_distance, int sixteenths, Naturality modifier) :
    a4_distance(0),
    sixteenths(sixteenths),
    modifier(modifier)
{

}

Note Note::whole(int a4_distance, Naturality modifier) {
    return Note(a4_distance, 16, modifier);
}

Note Note::half(int a4_distance, Naturality modifier) {
    return Note(a4_distance, 8, modifier);
}

Note Note::quarter(int a4_distance, Naturality modifier) {
    return Note(a4_distance, 4, modifier);
}

Note Note::eigth(int a4_distance, Naturality modifier) {
    return Note(a4_distance, 2, modifier);
}

Note Note::sixteenth(int a4_distance, Naturality modifier) {
    return Note(a4_distance, 1, modifier);
}

int Note::getPitch() const {
    return this->a4_distance;
}

int Note::getSixteenthsDuration() const {
    return this->sixteenths;
}

Naturality Note::getModifier() const {
    return this->modifier;
}

float Note::getFrequency() const {
    return 0.0;
    // int octave_height = this->a4_distance % NOTE_LETTER_COUNT;
    // int octave_num = (this->a4_distance - octave_height) / NOTE_LETTER_COUNT;
    // int note_distance = OCTAVE_SIZE * octave_num this->a4_distance;
    // switch (this->modifier) {
    // case Naturality::Flat:
    //     note_distance--;
    //     break;
    // case Naturality::Natural:
    //     // do nothing
    //     break;
    // case Naturality::Sharp:
    //     note_distance++;
    //     break;
    // }
    //
    // return A4_FREQ * std::pow(2.0, (float)note_distance / 12.0);
}

