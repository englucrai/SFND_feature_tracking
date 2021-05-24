# 2D Feature Tracking

<img src="images/keypoints.png" width="820" height="248" />

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

---

# Methodology

1.  Data Buffer Optimization

It was implemented one vector for a data buffer object whose size does not exceed a limit (e.g. 2 elements). 

The data Buffer approach is based on FIFO Logic (First In First Out). FIFO is an algorithm for queue-type data structures, which presents the following criteria: the first element to be removed is the first that has been inserted.

This implementation avoids memory problems since the memory allocated is predefined.

2. Keypoint Detection

The modern detectors were implemented: HARRIS, FAST, BRISK, ORB, AKAZE, and SIFT in a function that receives the selected detector accordingly to the user's choice.

3. Keypoint Removal

After detecting the key points over the whole image, it's time to set the region that contains the proceeding vehicle over all the sequence of images. A rectangle is defined and all the key points outside it are erased.

Since the final result is to keep track of the time to collision within the vehicle ahead, the key points of interest are only on the vehicle ahead.

4. Keypoint Descriptors

The  BRIEF, ORB, FREAK, AKAZE, and SIFT descriptors were implemented in a function that receives the selected detector accordingly to the user's choice, such as in the detection function.

5. Descriptor Matching

FLANN matching and k-nearest neighbor selection are done. Both methods are selectable using the respective strings in the main function.

6. Descriptor Distance Ratio

The K-Nearest-Neighbor matching is used to implement the descriptor distance ratio test, which looks at the ratio of best vs. second-best match to decide whether to keep an associated pair of key points.

# Results criteria

The results are analyzed accordingly to the following criteria:

1. Performance Evaluation 1

The total number of key points on the preceding vehicle for all 10 images.

2. Performance Evaluation 2

The total number of matched key points for all 10 images using all possible combinations of detectors and descriptors. 

In the matching step, the BF approach is used with the descriptor distance ratio set to 0.8.

2. Performance Evaluation 3

The total time it takes for keypoint detection and descriptor extraction.

The results are displayed in the following spreadsheet.

The analyses of the results, such as total time for detection and description, neighborhood size and distribution of key points detected, percentage of matched keypoints allows to indicate the TOP 3 detector-descriptor combinations:

1. FAST detector and BRIEF descriptor 

2. FAST detector and ORB descriptor

3. FAST detectro and BRISK descriptor

The tests' results are displayed at the spreadsheet above:

| DESCRIPTOR | TIME [ms] | KEYPOINTS     | DETECTOR | TIME [ms]  | KEYPOINT MATCHES | %           | TOTAL TIME [ms] |
|------------|-----------|---------------|----------|------------|------------------|-------------|-----------------|
| AKAZE      | 1,23618   | 1670          | AKAZE    | 1,08427    | 1491             | 0,892814371 | 2,32045         |
| AKAZE      | 1,28936   | 1670          | BRIEF    | 0,0163074  | 1491             | 0,892814371 | 1,30027         |
| AKAZE      | 1,26301   | 1670          | BRISK    | 0,0228609  | 1491             | 0,892814371 | 1,28588         |
| AKAZE      | 1,20042   | 1670          | FREAK    | 0,524131   | 1491             | 0,892814371 | 1,72455         |
| AKAZE      | 1,26187   | 1670          | ORB      | 0,0512111  | 1491             | 0,892814371 | 1,31308         |
| BRISK      | -         | (core dumped) | AKAZE    | -          | -                | -           | -               |
| BRISK      | 0,414716  | 2762          | BRIEF    | 0,0149099  | 2508             | 0,908037654 | 0,429626        |
| BRISK      | 0,409279  | 2762          | BRISK    | 0,0313825  | 2508             | 0,908037654 | 0,440661        |
| BRISK      | 0,402615  | 2762          | FREAK    | 0,478113   | 2326             | 0,842143374 | 0,880727        |
| BRISK      | 0,405453  | 2762          | ORB      | 0,0719303  | 2508             | 0,908037654 | 0,477384        |
| FAST       | -         | (core dumped) | AKAZE    | -          | -                | -           | -               |
| FAST       | 0,0126443 | 1491          | BRIEF    | 0,015125   | 1348             | 0,904091214 | 0,0277693       |
| FAST       | 0,0140664 | 1491          | BRISK    | 0,0219895  | 1348             | 0,904091214 | 0,0360559       |
| FAST       | 0,0129023 | 1491          | FREAK    | 0,459974   | 1348             | 0,904091214 | 0,472876        |
| FAST       | 0,0128658 | 1491          | ORB      | 0,0154348  | 1348             | 0,904091214 | 0,0283006       |
| ORB        | -         | (core dumped) | AKAZE    | -          | -                | -           | -               |
| ORB        | 0,121711  | 1161          | BRIEF    | 0,0121743  | 1033             | 0,889750215 | 0,133885        |
| ORB        | 0,124598  | 1161          | BRISK    | 0,0137764  | 1033             | 0,889750215 | 0,138375        |
| ORB        | 0,109374  | 1161          | FREAK    | 0,491872   | 549              | 0,472868217 | 0,601246        |
| ORB        | 0,132035  | 1161          | ORB      | 0,0783353  | 1033             | 0,889750215 | 0,210371        |
| SHITOMASI  | -         | (core dumped) | AKAZE    | -          | -                | -           | -               |
| SHITOMASI  | 0,245383  | 1179          | BRIEF    | 0,0197685  | 1067             | 0,905004241 | 0,265151        |
| SHITOMASI  | 0,241795  | 1179          | BRISK    | 0,0220539  | 1067             | 0,905004241 | 0,263848        |
| SHITOMASI  | 0,18837   | 1179          | FREAK    | 0,458696   | 1067             | 0,905004241 | 0,647066        |
| SHITOMASI  | 0,253789  | 1179          | ORB      | 0,0145907  | 1067             | 0,905004241 | 0,268379        |
| SIFT       | -         | (core dumped) | AKAZE    | -          | -                | -           | -               |
| SIFT       | 1,78551   | 1386          | BRIEF    | 0,00898051 | 1249             | 0,901154401 | 1,79449         |
| SIFT       | 1,54109   | 1386          | BRISK    | 0,0164391  | 1248             | 0,9004329   | 1,55753         |
| SIFT       | 1,79885   | 1386          | FREAK    | 0,4651     | 1239             | 0,893939394 | 2,26395         |
| SIFT       | -         | (core dumped) | ORB      | -          | -                | -           | -               |
