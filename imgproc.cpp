#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <algorithm>

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Missing filepath to image" << std::endl;
        std::cerr << "Usage: imgproc <fpath>" << std::endl;
    }
	//proccess pipeline
    cv::Mat image;
    image = cv::imread(argv[1]);
  	
	//cv::GaussianBlur( image, image, cv::Size( 13, 0 ), 0, 0.01 );	

	cv::pyrDown(image, image, cv::Size(image.cols /2, image.rows / 2));














    cv::Mat binary;
    cv::inRange(image, cv::Scalar(0, 0, 0), cv::Scalar(225, 225, 225), binary); 
	





     cv::Mat horizontal = binary.clone();




            // Specify size on horizontal axis
    int horizontal_size = horizontal.cols / 30;
    // Create structure element for extracting horizontal lines through morphology operations
    cv::Mat horizontalStructure = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(horizontal_size, 1));
    // Apply morphology operations
    erode(horizontal, horizontal, horizontalStructure, cv::Point(-1, -1));
    dilate(horizontal, horizontal, horizontalStructure, cv::Point(-1, -1));














    std::vector<cv::Vec4i> houghOut;
    cv::HoughLinesP(horizontal, houghOut, 1, CV_PI /180, 150, 100 , 80); 
	double slopeSum = 0;

	for(int j = 0; j < houghOut.size(); j++)
        {
               
	       	double slope = ((double)houghOut[j][1] - (double)houghOut[j][3]) / (houghOut[j][0] - houghOut[j][2]);
		
		slopeSum += slope;
        }
	
double slopeAvg = slopeSum/houghOut.size();
	
	cv::Mat staffLines(image.rows, image.cols, binary.type());
	staffLines = 0;
	
	for(int j = 0; j < houghOut.size(); j++)
        {
                double slope = ((double)houghOut[j][1] - (double)houghOut[j][3]) / (houghOut[j][0] - houghOut[j][2]);
		if(abs(slopeAvg - slope) < .02)
		{
                cv::line( staffLines, cv::Point(houghOut[j][0], houghOut[j][1]), cv::Point(houghOut[j][2], houghOut[j][3]), cv::Scalar(255,0,0), 1, cv::LINE_AA);
		}
	}






std::vector<cv::Vec4i> houghOut2;
    cv::HoughLinesP(staffLines, houghOut2, 1, CV_PI /180, 200, 400 , 400);



double slopeSum2 = 0;

        for(int j = 0; j < houghOut2.size(); j++)
        {

                double slope = ((double)houghOut2[j][1] - (double)houghOut2[j][3]) / (houghOut2[j][0] - houghOut2[j][2]);

                slopeSum2 += slope;
        }

double slopeAvg2 = slopeSum2/houghOut2.size();

        for(auto j = houghOut2.begin(); j != houghOut2.end(); j++)
        {
                double slope = ((double)(*j)[1] - (double)(*j)[3]) / ((*j)[0] - (*j)[2]);
                if(abs(slopeAvg2 - slope) > .015)
                {
                	houghOut2.erase(j);
                }
        }





   std::vector<int> lineCluster;

	lineCluster.push_back(houghOut2[0][1] + (houghOut2[0][1] - houghOut2[0][3])/2);

        for(int j = 1; j < houghOut2.size(); j++)
        {
                cv::line(image, cv::Point(houghOut2[j][0], houghOut2[j][1]), cv::Point(houghOut2[j][2], houghOut2[j][3]), cv::Scalar(0,255,0), 1, cv::LINE_AA);
		bool fit = false;
		int midpoint = houghOut2[j][1] + (houghOut2[j][1] - houghOut2[j][3])/2;
		for(int curclust = 0; curclust < lineCluster.size(); curclust++)
		{
			if(abs(midpoint - lineCluster[curclust]) <= 3)
			{
				fit = true;
				/*if(midpoint < lineCluster[curclust])
				{
					lineCluster[curclust]--;
				}
				else
				{
					lineCluster[curclust]++;
				}*/
			}
		}
		if(!fit)
		{
			lineCluster.push_back(midpoint);
		}
        }


	  for(int curclust = 0; curclust < lineCluster.size(); curclust++)
                {
			cv::line( image, cv::Point(0, lineCluster[curclust]), cv::Point(2000, lineCluster[curclust]), cv::Scalar(255,0,0), 1, cv::LINE_AA);
		}

	  std::sort(lineCluster.begin(), lineCluster.end());


	  std::vector<std::vector<int>> staffs;

	for(int i = 0; i < lineCluster.size(); i++)
	{	
		std::cout << lineCluster[i]<< std::endl;
	}	







    
    //display steps
   cv::namedWindow("Step 0", cv::WINDOW_AUTOSIZE);
   cv::imshow("Step 0", image);
   
   cv::namedWindow("Step 1", cv::WINDOW_AUTOSIZE);
   cv::imshow("Step 1", binary);

   cv::namedWindow("Step 2.5", cv::WINDOW_AUTOSIZE);
   cv::imshow("Step 2.5", horizontal);

cv::namedWindow("big brain time", cv::WINDOW_AUTOSIZE);
cv::imshow("big brain time", staffLines);
   
   cv::waitKey();

    return 0;
}
