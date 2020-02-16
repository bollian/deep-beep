#include <algorithm>
#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

#include <pipeline.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Missing filepath to image" << std::endl;
        std::cerr << "Usage: imgproc <fpath>" << std::endl;
        return 1;
    }
    //proccess pipeline
    //initial image
    cv::Mat image;
    image = cv::imread(argv[1]);

    Song song = readImage(image);

    return 0;
}
