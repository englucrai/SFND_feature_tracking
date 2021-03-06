/* INCLUDES FOR THIS PROJECT */
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <limits>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>

#include "dataStructures.h"
#include "matching2D.hpp"

using namespace std;

/* MAIN PROGRAM */
int main(int argc, const char *argv[])
{

    /* INIT VARIABLES AND DATA STRUCTURES */

    int keypointsvehicle = 0;
    int matches_total = 0;
    double total_time_desc = 0;
    double total_time_det = 0;

    // data location
    string dataPath = "../";

    // camera
    string imgBasePath = dataPath + "images/";
    string imgPrefix = "KITTI/2011_09_26/image_00/data/000000"; // left camera, color
    string imgFileType = ".png";
    int imgStartIndex = 0; // first file index to load (assumes Lidar and camera names have identical naming convention)
    int imgEndIndex = 9;   // last file index to load
    int imgFillWidth = 4;  // no. of digits which make up the file index (e.g. img-0001.png)

    // misc
    int dataBufferSize = 2;       // no. of images which are held in memory (ring buffer) at the same time
    vector<DataFrame> dataBuffer; // list of data frames which are held in memory at the same time
    bool bVis = false;            // visualize results

    string detectorType;
    string descriptorType;

    // DETECTOR TYPES
    cout << "DETECTORS\n";
    cout << "1) AKAZE\n";
    cout << "2) BRISK\n";
    cout << "3) FAST\n";
    cout << "4) HARRIS\n";
    cout << "5) ORB\n";
    cout << "6) SHITOMASI\n";
    cout << "7) SIFT\n";

    do
    {
        cout << "CHOOSE DESIRED DETECOR TYPE\n";
        cin >> detectorType;

    } while (detectorType != "AKAZE" && detectorType != "BRISK" && detectorType != "FAST" &&
             detectorType != "HARRIS" && detectorType != "ORB" && detectorType != "SHITOMASI" &&
             detectorType != "SIFT" );
    // DESCRIPTORS TYPES
    cout << "1) AKAZE\n";
    cout << "2) BRISK\n";
    cout << "3) BRIEF\n";
    cout << "4) FREAK\n";
    cout << "5) ORB\n";

    do
    {
        cout << "CHOOSE DESIRED DESCRIPTOR TYPE\n";
        cin >> descriptorType;

    } while (descriptorType != "AKAZE" && descriptorType != "BRISK" && descriptorType != "BRIEF" &&
             descriptorType != "FREAK" && descriptorType != "ORB" );

    /* MAIN LOOP OVER ALL IMAGES */

    for (size_t imgIndex = 0; imgIndex <= imgEndIndex - imgStartIndex; imgIndex++)
    {
        /* LOAD IMAGE INTO BUFFER */

        // assemble filenames for current index
        ostringstream imgNumber;
        imgNumber << setfill('0') << setw(imgFillWidth) << imgStartIndex + imgIndex;
        string imgFullFilename = imgBasePath + imgPrefix + imgNumber.str() + imgFileType;

        // load image from file and convert to grayscale
        cv::Mat img, imgGray;
        img = cv::imread(imgFullFilename);
        cv::cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);

        /* RING BUFFER */

        // push image into data frame buffer
        DataFrame frame;
        frame.cameraImg = imgGray;
        dataBuffer.push_back(frame);

        cout << "#1 : LOAD IMAGE INTO BUFFER done" << endl;

        /* DETECT IMAGE KEYPOINTS */

        // extract 2D keypoints from current image
        vector<cv::KeyPoint> keypoints; // create empty feature list for current image

        if (detectorType.compare("SHITOMASI") == 0)
        {
            detKeypointsShiTomasi(keypoints, imgGray, total_time_det, false);
        }
        else
        {
            detKeypointsModern(keypoints, imgGray, detectorType, total_time_det, false);
        }

        // only keep keypoints on the preceding vehicle
        bool bFocusOnVehicle = true;
        cv::Rect vehicleRect(535, 180, 180, 150); 
        if (bFocusOnVehicle)
        {
            
            //cv::rectangle(img, vehicleRect, cv::Scalar(0,0,255), 1, 8, 0);
            //imshow ("Rectangle detecting the car in front",img);
            
           
            // Aply keypoint mask
            auto kPit = keypoints.begin();
            while (kPit != keypoints.end())
            {
                // Check if the point is in the predefined rectangle or not
                if (vehicleRect.contains(kPit->pt))
                {
                    kPit++;
                    continue;
                }
                kPit = keypoints.erase(kPit);           
            }
        }

        // optional : limit number of keypoints (helpful for debugging and learning)
        bool bLimitKpts = false;
        if (bLimitKpts)
        {
            int maxKeypoints = 50;

            if (detectorType.compare("SHITOMASI") == 0)
            { // there is no response info, so keep the first 50 as they are sorted in descending quality order
                keypoints.erase(keypoints.begin() + maxKeypoints, keypoints.end());
            }
            cv::KeyPointsFilter::retainBest(keypoints, maxKeypoints);
            cout << " NOTE: Keypoints have been limited!" << endl;
        }

        // push keypoints and descriptor for current frame to end of data buffer
        (dataBuffer.end() - 1)->keypoints = keypoints;
        cout << "#2 : DETECT KEYPOINTS done" << endl;
        //cout << "Keypoints in the vehicle ahead: " << keypoints.size() << " keypoints" << endl;
        keypointsvehicle += keypoints.size();

        /* EXTRACT KEYPOINT DESCRIPTORS */

        cv::Mat descriptors;
        descKeypoints((dataBuffer.end() - 1)->keypoints, (dataBuffer.end() - 1)->cameraImg, descriptors, descriptorType, total_time_desc);

        // push descriptors for current frame to end of data buffer
        (dataBuffer.end() - 1)->descriptors = descriptors;

        cout << "#3 : EXTRACT DESCRIPTORS done" << endl;

        if (dataBuffer.size() > 1) // wait until at least two images have been processed
        {

            /* MATCH KEYPOINT DESCRIPTORS */

            vector<cv::DMatch> matches;
            string matcherType = "MAT_BF";        // MAT_BF, MAT_FLANN
            string descriptorType = "DES_BINARY"; // DES_BINARY, DES_HOG
            string selectorType = "SEL_KNN";       // SEL_NN, SEL_KNN

            matchDescriptors((dataBuffer.end() - 2)->keypoints, (dataBuffer.end() - 1)->keypoints,
                             (dataBuffer.end() - 2)->descriptors, (dataBuffer.end() - 1)->descriptors,
                             matches, descriptorType, matcherType, selectorType, matches_total);

            //// EOF STUDENT ASSIGNMENT

            // store matches in current data frame
            (dataBuffer.end() - 1)->kptMatches = matches;

            cout << "#4 : MATCH KEYPOINT DESCRIPTORS done" << endl;

            // visualize matches between current and previous image
            bVis = false;
            if (bVis)
            {
                cv::Mat matchImg = ((dataBuffer.end() - 1)->cameraImg).clone();
                cv::drawMatches((dataBuffer.end() - 2)->cameraImg, (dataBuffer.end() - 2)->keypoints,
                                (dataBuffer.end() - 1)->cameraImg, (dataBuffer.end() - 1)->keypoints,
                                matches, matchImg,
                                cv::Scalar::all(-1), cv::Scalar::all(-1),
                                vector<char>(), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

                string windowName = "Matching keypoints between two camera images";
                cv::namedWindow(windowName, 7);
                cv::imshow(windowName, matchImg);
                cout << "Press key to continue to next image" << endl;
                cv::waitKey(0); // wait for key to be pressed
            }
            bVis = false;
        }

    } // eof loop over all images

    cout << "SELECTED DETECTOR: " << detectorType << endl;
    cout << "SELECTED DESCRIPTOR: " << descriptorType << endl;
    cout << "Total points in the vehicle ahead over all images: " << keypointsvehicle << " keypoints" << endl;
    cout << "Total matches: " << matches_total << endl; 
    cout << "Total time for detection: " << total_time_det << " ms" << endl;
    cout << "Total time for description: " << total_time_desc << " ms" << endl; 
    cout << "Total time for detection and description: " << total_time_det + total_time_desc << " ms" << endl;

    return 0;
}
