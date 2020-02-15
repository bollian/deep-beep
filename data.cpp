#include <data.h>

Note::Note(int e_distance, int sixteenths, Naturality modifier) :
    pitch(e_distance),
    sixteenths(sixteenths),
    modifier(modifier)
{

}

Note Note::whole_note(int e_distance, Naturality modifier) {
    return Note(e_distance, 16, modifier);
}

Note Note::half_note(int e_distance, Naturality modifier) {
    return Note(e_distance, 8, modifier);
}

Note Note::quarter_note(int e_distance, Naturality modifier) {
    return Note(e_distance, 4, modifier);
}

Note Note::eigth_note(int e_distance, Naturality modifier) {
    return Note(e_distance, 2, modifier);
}

Note Note::sixteenth_note(int e_distance, Naturality modifier) {
    return Note(e_distance, 1, modifier);
}

int Note::getPitch() const {
    return this->pitch;
}

int Note::getSixteenthsDuration() const {
    return this->sixteenths;
}

Naturality Note::getModifier() const {
    return this->modifier;
}
