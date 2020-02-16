#ifndef DEEPBEEP_DATA_H_
#define DEEPBEEP_DATA_H_

#include <exception>
#include <vector>

enum class Naturality {
    Natural,
    Sharp,
    Flat
};

class Note {
private:
    Note(int c4_distance, int sixteenths, Naturality modifier);

    int c4_distance;
    int sixteenths;
    Naturality modifier;

public:
    static Note whole(int c4_distance, Naturality modifier = Naturality::Natural);
    static Note half(int c4_distance, Naturality modifier = Naturality::Natural);
    static Note quarter(int c4_distance, Naturality modifier = Naturality::Natural);
    static Note eigth(int c4_distance, Naturality modifier = Naturality::Natural);
    static Note sixteenth(int c4_distance, Naturality modifier = Naturality::Natural);

    int getPitch() const;
    int getSixteenthsDuration() const;
    Naturality getModifier() const;

    float getFrequency() const;
    float getUSecDuration() const;

};

using Chord = std::vector<Note>;
using Song = std::vector<Chord>;

#endif // DEEPBEEP_DATA_H_
