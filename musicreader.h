#ifndef DEEPBEEP_MUSICREADER_H_
#define DEEPBEEP_MUSICREADER_H_

#include <QObject>

#include <data.h>

class MusicReader : public QObject {
    Q_OBJECT
public:
    explicit MusicReader(QObject *parent = nullptr);

signals:
    void songRead(Song song);

};

#endif // DEEPBEEP_MUSICREADER_H_
