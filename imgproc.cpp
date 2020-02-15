#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Missing filepath to image" << std::endl;
        std::cerr << "Usage: imgproc <fpath>" << std::endl;
    }
	//proccess pipeline
    cv::Mat image;
    image = cv::imread(argv[1]);
  
    cv::Mat binary;
    cv::inRange(image, cv::Scalar(0, 0, 0), cv::Scalar(128, 128, 128), binary); 
	
    std::vector<cv::Vec2f> houghOut;
    cv::HoughLines(binary, houghOut, 1, CV_PI /180, 150, 0, CV_PI/180); 
	//display steps
   cv::namedWindow("Step 0", cv::WINDOW_AUTOSIZE);
   cv::imshow("Step 0", image);
   
   cv::namedWindow("Step 1", cv::WINDOW_AUTOSIZE);
   cv::imshow("Step 1", binary);
   std::vector<std::vector<cv::Vec2f>> staves;
   std::vector<cv::Vec2f> potentialStaff;
   double staffAngle;
   int staffCount = 5;

   for(auto i = houghOut.begin(); i != houghOut.end(); i++)
   {
	double angle = (*i)[1];
	if(!(angle > 1.56 && angle < 1.58))
	{
		houghOut.erase(i);
		i--;
	}
   }

   for(int i = 0; i < houghOut.size() - 2; i++)
   {
	potentialStaff.push_back(houghOut[i]);
	for(int j = i + 1; j < houghOut.size() - 1; j++)
	{
		potentialStaff.push_back(houghOut[j]);
		int staffStep = potentialStaff[1][0] - potentialStaff[0][0];
		for(int k = j + 1; k < houghOut.size(); k++)
		{
			if(abs(houghOut[k][0] - potentialStaff[potentialStaff.size() - 1][0] - staffStep) < 1)
					{
						potentialStaff.push_back(houghOut[k]);
					}
		}

		if(potentialStaff.size() == 5)
		{
			staves.push_back(std::vector<cv::Vec2f>(potentialStaff));
			break;
		}
		while(potentialStaff.size() > 1)
		{
			potentialStaff.pop_back();
		}
		
	}
	potentialStaff.clear();
   }

   for(int i = 0; i < staves.size(); i++)
   {
	   std::vector<cv::Vec2f> currStaff = staves[i];
	for(int j = 0; j < currStaff.size(); j++)
	{
		float rho = currStaff[j][0], theta = currStaff[j][1];
		cv::Point pt1, pt2;
        	double a = cos(theta), b = sin(theta);
        	double x0 = a*rho, y0 = b*rho;
		pt1.x = cvRound(x0 + 1000*(-b));
		pt1.y = cvRound(y0 + 1000*(a));
		pt2.x = cvRound(x0 - 1000*(-b));
		pt2.y = cvRound(y0 - 1000*(a));
		cv::line( image, pt1, pt2, cv::Scalar(0,0,255), 3, cv::LINE_AA);
	}
   }


   cv::namedWindow("Step 3*", cv::WINDOW_AUTOSIZE);
   cv::imshow("Step 3*", image);


   
   cv::waitKey();

    return 0;
}
