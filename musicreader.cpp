#include <cmath>
#include <iostream>

#include <opencv2/opencv.hpp>

#include <QQuickItem>
#include <QQuickItemGrabResult>
#include <QUrl>

#include <musicreader.h>
#include <pipeline.h>

MusicReader::MusicReader(QObject *parent) : QObject(parent)
{

}

void MusicReader::readSong(QUrl url) {
    std::cout << "entered MusicReader::readSong" << std::endl;
    QString s = url.toString();
    std::string stdstring = s.toStdString();
    const char *cstr = stdstring.c_str();
    cstr += 7;
    Song song = readImage(cstr);
    emit songRead(song);
    // auto grab_result = image_view->grabToImage();
    // connect(grab_result.data(), &QQuickItemGrabResult::ready, [=]() {
    //     QImage image = grab_result->image();
    //     cv::Mat cv_image(image.height(), image.width(), CV_8UC3);
    //     std::cout << image.height() << ", " << image.width() << std::endl;
    //     std::cout << cv_image.rows << ", " << cv_image.cols << std::endl;
    //
    //     for (int x = 0; x < image.width(); ++x)
    //     for (int y = 0; y < image.height(); ++y) {
    //         QRgb qcolor = image.pixel(x, y);
    //         int blue = qcolor & 0xFF;
    //         int green = (qcolor >> 8) & 0xFF;
    //         int red = (qcolor >> 16) & 0xFF;
    //         cv::Vec3b cv_color(red, green, blue);
    //         int new_x = std::max(x - 180, 0);
    //         cv_image.at<cv::Vec3b>(y, (int)((float)new_x)) = cv_color;
    //     }
    //
    //     Song song = readImage(cv_image);
    //     std::cout << "Song read!" << std::endl;
    //     emit songRead(song);
    // });
    // Song song {
    //     Chord { Note::quarter(6) },
    //     Chord { Note::quarter(5) },
    //     Chord { Note::half(4) },
    //
    //     Chord { Note::quarter(6) },
    //     Chord { Note::quarter(5) },
    //     Chord { Note::half(4) },
    //
    //     Chord { Note::eigth(4) },
    //     Chord { Note::eigth(4) },
    //     Chord { Note::eigth(4) },
    //     Chord { Note::eigth(4) },
    //     Chord { Note::eigth(5) },
    //     Chord { Note::eigth(5) },
    //     Chord { Note::eigth(5) },
    //     Chord { Note::eigth(5) },
    //
    //     Chord { Note::quarter(6) },
    //     Chord { Note::quarter(5) },
    //     Chord { Note::half(4) },
    // };
    // std::cout << "Song read!" << std::endl;
    // emit songRead(song);
}
