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
	//initial image
    cv::Mat image;
    image = cv::imread(argv[1]);
  	
	//scales big phone pictures.
	cv::pyrDown(image, image, cv::Size(image.cols /2, image.rows / 2));


	//generates the initial binary of the sheet
    cv::Mat binary;
    cv::inRange(image, cv::Scalar(0, 0, 0), cv::Scalar(225, 225, 225), binary); 
	//binary mat for holding the horizontals
     cv::Mat horizontal = binary.clone();



     //morphology operations to remove everything but the staffs

    int horizontal_size = horizontal.cols / 30;
    cv::Mat horizontalStructure = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(horizontal_size, 1));
	    
    // Apply morphology operations
    erode(horizontal, horizontal, horizontalStructure, cv::Point(-1, -1));
    dilate(horizontal, horizontal, horizontalStructure, cv::Point(-1, -1));


cv::Mat notes = binary.clone();

 //morphology operations to remove everything but the notes
    int vertical_size = 5;
    cv::Mat verticalStructure = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1, vertical_size));

    // Apply morphology operations
    erode(notes, notes, verticalStructure, cv::Point(-1, -1));
    dilate(notes, notes, verticalStructure, cv::Point(-1, -1));








	//first hough transform this begins to isolate the staff lines out by finding chucks of them

    std::vector<cv::Vec4i> houghOut;
    cv::HoughLinesP(horizontal, houghOut, 1, CV_PI /180, 150, 100 , 80); 
	double slopeSum = 0;

	for(int j = 0; j < houghOut.size(); j++)
        {
               
	       	double slope = ((double)houghOut[j][1] - (double)houghOut[j][3]) / (houghOut[j][0] - houghOut[j][2]);
		
		slopeSum += slope;
        }

	//slope filtering to rat out noice and eighth note bars this will purify the lines a little sstaffs are easier to pick out
	
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





//galaxy brain strat use the hough again!!! links staff line segments

std::vector<cv::Vec4i> houghOut2;
    cv::HoughLinesP(staffLines, houghOut2, 1, CV_PI /180, 200, 400 , 400);


//slope filter agian to pull any other inconsistent lines

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



//cluster really close lines to make potential staff lines

   std::vector<int> lineCluster;

	lineCluster.push_back(houghOut2[0][1] + (houghOut2[0][1] - houghOut2[0][3])/2);

        for(int j = 1; j < houghOut2.size(); j++)
        {
		bool fit = false;
		int midpoint = houghOut2[j][1] + (houghOut2[j][1] - houghOut2[j][3])/2;
		for(int curclust = 0; curclust < lineCluster.size(); curclust++)
		{
			if(abs(midpoint - lineCluster[curclust]) <= 3)
			{
				fit = true;
			}
		}
		if(!fit)
		{
			lineCluster.push_back(midpoint);
		}
        }

//sort the staff lines

	  std::sort(lineCluster.begin(), lineCluster.end());


	  std::vector<std::vector<int>> staffs;
	std::vector<int> potStaff;
	
	
//bunch up the staff lines
	while(!lineCluster.empty())
	{
	if(potStaff.empty())
	{
		potStaff.push_back(lineCluster.back());
                        lineCluster.pop_back();

		if(lineCluster.back() > potStaff[0] - .035*image.cols && lineCluster.size() > 0)
		{
			potStaff.push_back(lineCluster.back());
			lineCluster.pop_back();
		}
		else
		{
		potStaff.clear();
		}

	}
	else
	{
		if(lineCluster.back() >= potStaff.back() - (potStaff[potStaff.size() - 2] - potStaff.back()) * 2 )
		{

			potStaff.push_back(lineCluster.back());
                        lineCluster.pop_back();
		}
		else
		{
			if(potStaff.size() >= 5)
			{
				staffs.push_back(std::vector<int>(potStaff));
				potStaff.clear();
			}	
			else if(potStaff.size() < 5)
			{
				potStaff.clear();
			}
		}
	}
	}
//pull extrunious lines till only 5 remain
	for(int staff = 0; staff < staffs.size(); staff++)
	{
		while(staffs[staff].size() > 5)
		{
		int sum = 0;
			for(int i = 0; i < staffs[staff].size(); i++)
			{
				sum += staffs[staff][i];
			}
			int staffavg = sum/staffs[staff].size();
			auto max = staffs[staff].begin();
			for(auto j = staffs[staff].begin(); j!=staffs[staff].end(); j++)
			{
				if(abs(*j - staffavg) > *max)
				{
					max = j;
				}
			}
			staffs[staff].erase(max);
		}
	}

//display lines
for(int i = 0; i < staffs.size(); i++)
{
	for(int j = 0; j < staffs[i].size(); j++)
	{
		cv::line( image, cv::Point(0, staffs[i][j]), cv::Point(2000, staffs[i][j]), cv::Scalar(50*j,20*i,0), 1, cv::LINE_AA);
	}
}

std::cout << staffs.size() << std::endl;


    
    //display steps
    /*
   cv::namedWindow("Step 0", cv::WINDOW_AUTOSIZE);
   cv::imshow("Step 0", image);
   
   cv::namedWindow("Step 1", cv::WINDOW_AUTOSIZE);
   cv::imshow("Step 1", binary);

   cv::namedWindow("Step 2.5", cv::WINDOW_AUTOSIZE);
   cv::imshow("Step 2.5", horizontal);

cv::namedWindow("big brain time", cv::WINDOW_AUTOSIZE);
cv::imshow("big brain time", staffLines);
   */

cv::namedWindow("bigger brain time", cv::WINDOW_AUTOSIZE);
cv::imshow("bigger brain time", notes);

   cv::waitKey();

    return 0;
}
