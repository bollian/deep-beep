#include <iostream>

#include <opencv/cv.hpp>

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Missing filepath to image" << std::endl;
        std::cerr << "Usage: imgproc <fpath>" << std::endl;
    }

    cv::Mat image;
    image = cv::imread(argv[1]);
    
    return 0;
}
