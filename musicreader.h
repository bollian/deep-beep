#ifndef DEEPBEEP_MUSICREADER_H_
#define DEEPBEEP_MUSICREADER_H_

#include <QImage>
#include <QObject>

#include <QQuickItem>
#include <QUrl>

#include <data.h>

class MusicReader : public QObject {
    Q_OBJECT
public:
    explicit MusicReader(QObject *parent = nullptr);

public slots:
    void readSong(QUrl url);

signals:
    void songRead(Song song);

};

#endif // DEEPBEEP_MUSICREADER_H_
