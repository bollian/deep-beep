#include <vector>

enum class Naturality {
    Natural,
    Sharp,
    Flat
};

class Note {
private:
    Note(int pitch, int sixteenths, Naturality modifier);

    int pitch;
    int sixteenths;
    Naturality modifier;

public:
    static Note whole_note(int e_distance, Naturality modifier);
    static Note half_note(int e_distance, Naturality modifier);
    static Note quarter_note(int e_distacne, Naturality modifier);
    static Note eigth_note(int e_distance, Naturality modifier);
    static Note sixteenth_note(int e_distance, Naturality modifier);

    int getPitch() const;
    int getSixteenthsDuration() const;
    Naturality getModifier() const;

};

using Chord = std::vector<Note>;
using Song = std::vector<Chord>;

