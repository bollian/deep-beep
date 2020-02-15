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
  	
	cv::GaussianBlur( image, image, cv::Size( 13, 0 ), 0, 0.01 );	

	cv::pyrDown(image, image, cv::Size(image.cols /2, image.rows / 2));
	

    cv::Mat binary;
    cv::inRange(image, cv::Scalar(0, 0, 0), cv::Scalar(185, 185, 185), binary); 
	
    std::vector<cv::Vec4i> houghOut;
    cv::HoughLinesP(binary, houghOut, 1, CV_PI /180, 150, 150 ,40); 

	for(int j = 0; j < houghOut.size(); j++)
        {
		std::cout << houghOut[j][0] << "\t" << houghOut[j][1] << "\t" << houghOut[j][2] << "\t" <<  houghOut[j][3] << std::endl;
                cv::line( image, cv::Point(houghOut[j][0], houghOut[j][1]), cv::Point(houghOut[j][2], houghOut[j][3]), cv::Scalar(255,0,0), 3, cv::LINE_AA);
        }
    
    //display steps
   cv::namedWindow("Step 0", cv::WINDOW_AUTOSIZE);
   cv::imshow("Step 0", image);
   
   cv::namedWindow("Step 1", cv::WINDOW_AUTOSIZE);
   cv::imshow("Step 1", binary);

  /* std::vector<std::vector<cv::Vec2f>> staves;
   std::vector<cv::Vec2f> potentialStaff;
   double staffAngle;
   int staffCount = 5;

   for(auto i = houghOut.begin(); i != houghOut.end(); i++)
   {
	double angle = (*i)[1];
	if(!(angle > 1.5 && angle < 1.7))
	{
		houghOut.erase(i);
		i--;
	}
   }

 for(int j = 0; j < houghOut.size(); j++)
        {
                std::cout << houghOut[j][0] << "\t" << houghOut[j][1] << std::endl;

                float rho = houghOut[j][0], theta = houghOut[j][1];
                cv::Point pt1, pt2;
                double a = cos(theta), b = sin(theta);
                double x0 = a*rho, y0 = b*rho;
                pt1.x = cvRound(x0 + 1000*(-b));
                pt1.y = cvRound(y0 + 1000*(a));
                pt2.x = cvRound(x0 - 1000*(-b));
                pt2.y = cvRound(y0 - 1000*(a));
                cv::line( image, pt1, pt2, cv::Scalar(0,255,0), 3, cv::LINE_AA);
        }


   for(int i = 0; i < houghOut.size() - 2; i++)
   {
	potentialStaff.push_back(houghOut[i]);
	for(int j = i + 1; j < houghOut.size() - 1; j++)
	{
		if( abs(houghOut[j][1] - potentialStaff[0][1]) < .05)
		{
			potentialStaff.push_back(houghOut[j]);
			int staffStep = potentialStaff[1][0] - potentialStaff[0][0];
			for(int k = j + 1; k < houghOut.size(); k++)
			{
				if(abs(houghOut[k][0] - potentialStaff[potentialStaff.size() - 1][0] - staffStep) < 1 && abs(houghOut[k][1] - potentialStaff[0][1]) < .05)
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
		pt1.x = cvRound(x0 + 2000*(-b));
		pt1.y = cvRound(y0 + 2000*(a));
		pt2.x = cvRound(x0 - 2000*(-b));
		pt2.y = cvRound(y0 - 2000*(a));
		cv::line( image, pt1, pt2, cv::Scalar(0,0,255), 3, cv::LINE_AA);
	}
   }


   cv::namedWindow("Step 3*", cv::WINDOW_AUTOSIZE);
   cv::imshow("Step 3*", image);

*/
   
   cv::waitKey();

    return 0;
}
