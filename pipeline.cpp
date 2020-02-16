#include <opencv2/opencv.hpp>

#include <data.h>
#include <pipeline.h>

cv::Mat initialBinarize(cv::Mat image);
cv::Mat isolateStaffs(cv::Mat bin_image);
cv::Mat isolateNotes(cv::Mat bin_image);

cv::Mat horizontalHoughSegments(cv::Mat horiz_line_image);
std::vector<cv::Vec4i> completeHorizontalLines(cv::Mat horiz_line_sements);
std::vector<int> sortedYPositions(std::vector<cv::Vec4i> lines);
std::vector<std::vector<int> > clusterStaves(std::vector<int> line_ys, int img_columns);

std::vector<cv::Rect> noteRectangles(cv::Mat binary_notes);

Song readImage(cv::Mat image) {
    Song song;

    //scales big phone pictures.
    cv::resize(image, image, cv::Size(770, 988));
    cv::Mat binary = initialBinarize(image);
    cv::Mat binary_staves = isolateStaffs(binary);
    cv::Mat binary_notes = isolateNotes(image);

    cv::Mat horizontal_segments = horizontalHoughSegments(binary_staves);
    std::vector<cv::Vec4i> full_lines = completeHorizontalLines(horizontal_segments);
    std::vector<int> valid_line_ys = sortedYPositions(full_lines);
    std::vector<std::vector<int> > staves = clusterStaves(valid_line_ys, image.cols);
    std::cout << "Found " << staves.size() << " staves" << std::endl;

    std::vector<cv::Rect> note_rects = noteRectangles(binary_notes);

    // draw the rectangles
    cv::Mat rect_img = cv::Mat::zeros(image.size(), image.type());
    for (int i = 0; i < note_rects.size(); ++i) {
        auto color = cv::Scalar(255, 255, 255);
        cv::rectangle(rect_img, note_rects[i].tl(), note_rects[i].br(), color, 2);
    }

    //display steps
    for (int i = 0; i < staves.size(); i++) {
        for (int j = 0; j < staves[i].size(); j++) {
            int y = staves[i][j];
            cv::line(image, cv::Point(0, y), cv::Point(2000, y), cv::Scalar(50*j,20*i,0), 1, cv::LINE_AA);
        }
    }
    cv::namedWindow("Initial Image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Initial Image", image);

    cv::namedWindow("First Thresholding", cv::WINDOW_AUTOSIZE);
    cv::imshow("First Thresholding", binary);

    cv::imwrite("binary-image.png", binary);

    // cv::namedWindow("Morphised Staff Lines", cv::WINDOW_AUTOSIZE);
    // cv::imshow("Morphised Staff Lines", binary_staves);
    //
    // cv::namedWindow("big brain time", cv::WINDOW_AUTOSIZE);
    // cv::imshow("big brain time", horizontal_segments);

    cv::namedWindow("Morphised Notes", cv::WINDOW_AUTOSIZE);
    cv::imshow("Morphised Notes", binary_notes);

    cv::namedWindow("Bounding Rectangles", cv::WINDOW_AUTOSIZE);
    cv::imshow("Bounding Rectangles", rect_img);

    cv::waitKey();

    return song;
}

cv::Mat initialBinarize(cv::Mat image) {
    std::cout << "initialBinarize" << std::endl;
    cv::Mat binary;
    cv:cvtColor(image, binary, cv::COLOR_RGB2GRAY);
    cv::adaptiveThreshold(binary, binary, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 11, 4); 
    return binary;
}

cv::Mat isolateStaffs(cv::Mat bin_image) {
    std::cout << "isolateStaffs" << std::endl;
    int horizontal_size = bin_image.cols / 30;
    cv::Mat horiz_structure = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(horizontal_size, 1));

    int horizontal_size2 = 9;
    cv::Mat horiz_structure2 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(horizontal_size2, 1));

    // apply morphology operations
    cv::Mat staves;
    dilate(bin_image, staves, horiz_structure2, cv::Point(-1, -1));
    erode(staves, staves, horiz_structure2, cv::Point(-1, -1));


    erode(staves, staves, horiz_structure, cv::Point(-1, -1));
    dilate(staves, staves, horiz_structure, cv::Point(-1, -1));

    return staves;
}

cv::Mat isolateNotes(cv::Mat image) {
    std::cout << "isolateNotes" << std::endl;

    int vertical_size = 5;
    cv::Mat verticalStructure = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(vertical_size, vertical_size));

    cv::Mat binary;
    cv::inRange(image, cv::Scalar(0, 0, 0), cv::Scalar(225, 225, 225), binary); 

    // Apply morphology operations
    erode(binary, binary, verticalStructure, cv::Point(-1, -1));
    dilate(binary, binary, verticalStructure, cv::Point(-1, -1));

    return binary;
}

cv::Mat horizontalHoughSegments(cv::Mat horiz_line_image) {
    std::cout << "horizontalHoughSegments" << std::endl;
    //first hough transform this begins to isolate the staff lines out by finding chucks of them
    std::vector<cv::Vec4i> hough_out;
    cv::HoughLinesP(horiz_line_image, hough_out, 1, CV_PI / 180, 150, 100, 80); 
	std::cout << hough_out.size() << std::endl;
    double slope_sum = 0.0;
    for (const cv::Vec4i& hough_seg : hough_out) {
        double slope =
            ((double)hough_seg[1] - (double)hough_seg[3])
            / (hough_seg[0] - hough_seg[2]);
        slope_sum += slope;
    }


    // slope filtering to rat out noice and eighth note bars
    // this will purify the lines a little so staffs are easier to pick out
    double slope_avg = slope_sum / hough_out.size();

    cv::Mat staff_lines(horiz_line_image.rows, horiz_line_image.cols, CV_8UC1);
    staff_lines = 0;
    std::cout << "fuck" << std::endl;

    for (const cv::Vec4i& hough_seg : hough_out)
    {
        double slope = ((double)hough_seg[1] - (double)hough_seg[3]) / (hough_seg[0] - hough_seg[2]);
        if(abs(slope_avg - slope) < .02)
        {
            cv::line(staff_lines, cv::Point(hough_seg[0], hough_seg[1]), cv::Point(hough_seg[2], hough_seg[3]), cv::Scalar(255,0,0), 2, cv::LINE_AA);
        }
    }

    return staff_lines;
}

std::vector<cv::Vec4i> completeHorizontalLines(cv::Mat horiz_line_sements) {
    std::cout << "completeHorizontalLines" << std::endl;
    //galaxy brain strat use the hough again!!! links staff line segments
    std::vector<cv::Vec4i> hough_out;
    cv::HoughLinesP(horiz_line_sements, hough_out, 1, CV_PI / 180, 200, 400, 400);


    //slope filter agian to pull any other inconsistent lines
    double slope_sum = 0.0;
 std::cout << "hi" << std::endl;

    for (const cv::Vec4i& hough_seg : hough_out) {
        double slope = ((double)hough_seg[1] - (double)hough_seg[3]) / (hough_seg[0] - hough_seg[2]);
        slope_sum += slope;
    }

 std::cout << "hi" << std::endl;

    double slope_avg = slope_sum / hough_out.size();

    for(int j = 0; j < hough_out.size(); j++) {
        double slope = ((double)hough_out[j][1] - (double)hough_out[j][3]) / (hough_out[j][0] - hough_out[j][2]);
        if(abs(slope_avg - slope) > .02)
        {
            auto at_point = hough_out.begin() + j;
	    hough_out.erase(at_point);
	    j--;
        }
    }

    return hough_out;
}

std::vector<int> sortedYPositions(std::vector<cv::Vec4i> lines) {
    std::cout << "sortedYPositions" << std::endl;
    //cluster really close lines to make potential staff lines
    if (lines.empty()) {
        std::cerr << "Didn't receive any hough lines to sort" << std::endl;
        return std::vector<int>();
    }

    std::vector<int> line_ys;

    line_ys.push_back((lines[0][1] + lines[0][3]) / 2);

    for (const cv::Vec4i& line : lines) {
        bool fit = false;
        int midpoint = (line[1] + line[3]) / 2;

        for (const int clustered_y : line_ys) {
            if(abs(midpoint - clustered_y) <= 3) {
                fit = true;
            }
        }

        if(!fit) {
            line_ys.push_back(midpoint);
        }
    }

    // sort the staff lines
    std::sort(line_ys.begin(), line_ys.end());
    return line_ys;
}

std::vector<std::vector<int> > clusterStaves(std::vector<int> line_ys, int img_columns) {
    std::cout << "clusterStaves" << std::endl;
    std::vector<std::vector<int> > staves;
    std::vector<int> pot_staff;

    //bunch up the staff lines
    while (!line_ys.empty()) {
        if (pot_staff.empty()) {
            pot_staff.push_back(line_ys.back());
            line_ys.pop_back();

            if (line_ys.back() > pot_staff[0] - .035 * img_columns && line_ys.size() > 0) {
                pot_staff.push_back(line_ys.back());
                line_ys.pop_back();
            }
            else {
                pot_staff.clear();
            }
        }
        else {
            if (line_ys.back() >= pot_staff.back() - (pot_staff[pot_staff.size() - 2] - pot_staff.back()) * 2) {
                pot_staff.push_back(line_ys.back());
                line_ys.pop_back();
            }
            else {
                if(pot_staff.size() >= 5) {
                    staves.push_back(std::vector<int>(pot_staff));
                    pot_staff.clear();
                }	
                else if(pot_staff.size() < 5) {
                    pot_staff.clear();
                }
            }
        }
    }

    //pull extrunious lines till only 5 remain
    for (std::vector<int>& staff : staves) {
        while(staff.size() > 5) {
            int sum = 0;
            for(int i = 0; i < staff.size(); i++) {
                sum += staff[i];
            }

            int staffavg = sum / staff.size();
            auto max = staff.begin();
            for(auto j = staff.begin(); j != staff.end(); j++) {
                if(abs(*j - staffavg) > *max) {
                    max = j;
                }
            }
            staff.erase(max);
        }
    }

    return staves;
}

std::vector<cv::Rect> noteRectangles(cv::Mat binary_notes) {
    std::cout << "noteRectangles" << std::endl;

    cv::Mat canny;
    cv::Canny(binary_notes, canny, 128, 255);
    std::vector<std::vector<cv::Point> > contours;
    cv::findContours(canny, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
    std::vector<std::vector<cv::Point> > contours_poly(contours.size());
    std::vector<cv::Rect> bounding_rects(contours.size());

    for (int i = 0; i < contours.size(); ++i) {
        cv::approxPolyDP(contours[i], contours_poly[i], 3, true);
        bounding_rects[i] = cv::boundingRect(contours_poly[i]);
    }
    // remove objects with extra wide aspect ratios to filter out
    // the ties between eigth and sixteenth notes
    for (int i = 0; i < bounding_rects.size(); ++i) {
        if (bounding_rects[i].width > (2 * bounding_rects[i].height)) {
            bounding_rects.erase(bounding_rects.begin() + i);
            contours_poly.erase(contours_poly.begin() + i);
            --i;
        }
    }
    // remove extraordinarily large objects to filter out punch holes
    int total_width = 0;
    for (const cv::Rect& rect : bounding_rects) {
        total_width += rect.width;
    }
    int max_width = 2 * total_width / bounding_rects.size();
    for (int i = 0; i < bounding_rects.size(); ++i) {
        if (bounding_rects[i].width > max_width) {
            bounding_rects.erase(bounding_rects.begin() + i);
            contours_poly.erase(contours_poly.begin() + i);
            --i;
        }
    }

    return bounding_rects;
}
