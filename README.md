# SFND 2D Feature Tracking

<img src="images/keypoints.png" width="820" height="248" />

The idea of the camera course is to build a collision detection system - that's the overall goal for the Final Project. As a preparation for this, you will now build the feature tracking part and test various detector / descriptor combinations to see which ones perform best. This mid-term project consists of four parts:

* Loading images, setting up data structures and putting everything into a ring buffer to optimize memory load. 
* Integrate several keypoint detectors such as HARRIS, FAST, BRISK and SIFT and compare them with regard to number of keypoints and speed. 
* Focus on descriptor extraction and matching using brute force and also the FLANN approach we discussed in the previous lesson. 
* Test the various algorithms in different combinations and compare them with regard to some performance measures. 


## Dependencies for Running Locally
* cmake >= 2.8
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* OpenCV >= 4.1
  * This must be compiled from source using the `-D OPENCV_ENABLE_NONFREE=ON` cmake flag for testing the SIFT and SURF detectors.
  * The OpenCV 4.1.0 source code can be found [here](https://github.com/opencv/opencv/tree/4.1.0)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./2D_feature_tracking`.


## Results

| DESCRIPTOR | TIME [ms]     | KEYPOINTS     | DETECTOR | TIME [ms]     | KEYPOINT MATCHES | %             | TOTAL TIME [ms] |
|------------|---------------|---------------|----------|---------------|------------------|---------------|-----------------|
| AKAZE      | 1,23618       | 1670          | AKAZE    | 1,08427       | 1491             | 0,892814371   | 2,32045         |
| AKAZE      | 1,28936       | 1670          | BRIEF    | 0,0163074     | 1491             | 0,892814371   | 1,30027         |
| AKAZE      | 1,26301       | 1670          | BRISK    | 0,0228609     | 1491             | 0,892814371   | 1,28588         |
| AKAZE      | 1,20042       | 1670          | FREAK    | 0,524131      | 1491             | 0,892814371   | 1,72455         |
| AKAZE      | 1,26187       | 1670          | ORB      | 0,0512111     | 1491             | 0,892814371   | 1,31308         |
| BRISK      | (core dumped) | (core dumped) | AKAZE    | (core dumped) | (core dumped)    | (core dumped) | (core dumped)   |
| BRISK      | 0,414716      | 2762          | BRIEF    | 0,0149099     | 2508             | 0,908037654   | 0,429626        |
| BRISK      | 0,409279      | 2762          | BRISK    | 0,0313825     | 2508             | 0,908037654   | 0,440661        |
| BRISK      | 0,402615      | 2762          | FREAK    | 0,478113      | 2326             | 0,842143374   | 0,880727        |
| BRISK      | 0,405453      | 2762          | ORB      | 0,0719303     | 2508             | 0,908037654   | 0,477384        |
| FAST       | (core dumped) | (core dumped) | AKAZE    | (core dumped) | (core dumped)    | (core dumped) | (core dumped)   |
| FAST       | 0,0126443     | 1491          | BRIEF    | 0,015125      | 1348             | 0,904091214   | 0,0277693       |
| FAST       | 0,0140664     | 1491          | BRISK    | 0,0219895     | 1348             | 0,904091214   | 0,0360559       |
| FAST       | 0,0129023     | 1491          | FREAK    | 0,459974      | 1348             | 0,904091214   | 0,472876        |
| FAST       | 0,0128658     | 1491          | ORB      | 0,0154348     | 1348             | 0,904091214   | 0,0283006       |
| ORB        | (core dumped) | (core dumped) | AKAZE    | (core dumped) | (core dumped)    | (core dumped) | (core dumped)   |
| ORB        | 0,121711      | 1161          | BRIEF    | 0,0121743     | 1033             | 0,889750215   | 0,133885        |
| ORB        | 0,124598      | 1161          | BRISK    | 0,0137764     | 1033             | 0,889750215   | 0,138375        |
| ORB        | 0,109374      | 1161          | FREAK    | 0,491872      | 549              | 0,472868217   | 0,601246        |
| ORB        | 0,132035      | 1161          | ORB      | 0,0783353     | 1033             | 0,889750215   | 0,210371        |
| SHITOMASI  | (core dumped) | (core dumped) | AKAZE    | (core dumped) | (core dumped)    | (core dumped) | (core dumped)   |
| SHITOMASI  | 0,245383      | 1179          | BRIEF    | 0,0197685     | 1067             | 0,905004241   | 0,265151        |
| SHITOMASI  | 0,241795      | 1179          | BRISK    | 0,0220539     | 1067             | 0,905004241   | 0,263848        |
| SHITOMASI  | 0,18837       | 1179          | FREAK    | 0,458696      | 1067             | 0,905004241   | 0,647066        |
| SHITOMASI  | 0,253789      | 1179          | ORB      | 0,0145907     | 1067             | 0,905004241   | 0,268379        |
| SIFT       | (core dumped) | (core dumped) | AKAZE    | (core dumped) | (core dumped)    | (core dumped) | (core dumped)   |
| SIFT       | 1,78551       | 1386          | BRIEF    | 0,00898051    | 1249             | 0,901154401   | 1,79449         |
| SIFT       | 1,54109       | 1386          | BRISK    | 0,0164391     | 1248             | 0,9004329     | 1,55753         |
| SIFT       | 1,79885       | 1386          | FREAK    | 0,4651        | 1239             | 0,893939394   | 2,26395         |
| SIFT       | (core dumped) | (core dumped) | ORB      | (core dumped) | (core dumped)    | (core dumped) | (core dumped)   |
