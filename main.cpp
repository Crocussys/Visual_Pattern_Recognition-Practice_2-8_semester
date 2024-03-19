#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

Mat img, new_img, lines_img, circles_img;
int blure = 1;

int rho = 74;
int theta = 65;
int threshold_slider = 0;

int dp = 1;
int minDist = 1;
int param1 = 145;
int param2 = 12;
int minRadius = 0;
int maxRadius = 11;

static void blure_on_trackbar(int, void*){
    if(blure == 0){
        blure = 1;
    }
    GaussianBlur(img, new_img, Size(0, 0), blure);
    cvtColor(new_img, new_img, COLOR_BGR2GRAY);
    imshow("Processed", new_img);
}

static void lines_on_trackbar(int, void*){
    if(rho == 0) rho = 1;
    if(theta == 0) theta = 360;
    lines_img = img.clone();
    vector<Vec2f> lines;
    HoughLines(new_img, lines, rho, CV_PI/theta, threshold_slider);
    for(size_t i = 0; i < lines.size(); i++)
    {
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        line(lines_img, pt1, pt2, Scalar(0,0,255), 3, LINE_AA);
    }
    imshow("Lines", lines_img);
}

static void circles_on_trackbar(int, void*){
    if(dp == 0) dp = 1;
    if(minDist == 0) minDist = 1;
    if(param1 == 0) param1 = 1;
    if(param2 == 0) param2 = 1;
    circles_img = img.clone();
    vector<Vec3f> circles;
    HoughCircles(new_img, circles, HOUGH_GRADIENT, dp, minDist, param1, param2, minRadius, maxRadius);
    for(size_t i = 0; i < circles.size(); i++)
    {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        circle( circles_img, center, 3, Scalar(0,255,0), -1, 8, 0 );
        circle( circles_img, center, radius, Scalar(0,0,255), 2, 8, 0 );
    }
    imshow("Circles", circles_img);
}

int main()
{
    img = imread("../Visual_Pattern_Recognition-Practice_2-8_semester/image.jpg", IMREAD_COLOR);
    if (img.empty()){
        cout << "Изображение не загружено" << endl;
        return -1;
    }
    namedWindow("Original", WINDOW_NORMAL);
    imshow("Original", img);

    namedWindow("Processed", WINDOW_NORMAL);
    blure_on_trackbar(blure, 0);
    createTrackbar("Blure", "Processed", &blure, 20, blure_on_trackbar);

    namedWindow("Lines", WINDOW_NORMAL);
    lines_on_trackbar(rho, 0);
    createTrackbar("rho", "Lines", &rho, 100, lines_on_trackbar);
    createTrackbar("theta", "Lines", &theta, 360, lines_on_trackbar);
    createTrackbar("threshold_slider", "Lines", &threshold_slider, 1000, lines_on_trackbar);

    namedWindow("Circles", WINDOW_NORMAL);
    circles_on_trackbar(rho, 0);
    createTrackbar("dp", "Circles", &dp, 10, circles_on_trackbar);
    createTrackbar("minDist", "Circles", &minDist, 1000, circles_on_trackbar);
    createTrackbar("param1", "Circles", &param1, 1000, circles_on_trackbar);
    createTrackbar("param2", "Circles", &param2, 1000, circles_on_trackbar);
    createTrackbar("minRadius", "Circles", &minRadius, 1000, circles_on_trackbar);
    createTrackbar("maxRadius", "Circles", &maxRadius, 1000, circles_on_trackbar);

    waitKey(0);
    return 0;
}
