#include<opencv2/opencv.hpp>
#include<iostream>


using namespace cv;
using namespace std;


Mat src;
int main(int argc, char** argv)
{
    src = imread("C:\\Users\\jc\\Desktop\\Assignments_DigitalMediaComputing\\seamcarving\\1.jpg");
    if (!src.data)
    {
        cout << "no" << endl;
        return -1;
    }
    imshow("input title", src);
    int numfeature = 300;
    Ptr<SIFT>detector = SIFT::create(numfeature);
    vector<KeyPoint> keypoints;
    detector->detect(src, keypoints, Mat());
    Mat resultImg;
    drawKeypoints(src, keypoints, resultImg, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    imshow("SIFT keypoint", resultImg);
    waitKey(0);

    return 0;



}
