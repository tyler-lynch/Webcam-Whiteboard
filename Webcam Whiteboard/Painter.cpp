#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

// Webcam image.
Mat img;

// Holds the mask values for each detectable color. {hmin, smin, vmin, hmax, smax, vmax}
vector<vector<int>> colors{ //{0, 104, 255, 74, 255, 255},    // red
                            {61, 7, 101, 149, 118, 255},   // blue
                            {39, 0, 0, 91, 215, 255} };  // green

// Holds the bgr color values for each detectable color. {b, g, r}
vector<Scalar> colorValues{ //{0, 0, 255},    // red
                            {255, 0, 0},    // blue
                            {0, 255, 0} };  // green

// Holds vectors that represent the location of the marker and its color. {point.x, point.y, index for colorValues}
vector<vector<int>> newPoints;

// Pre-processing methods
void findColor();   // Detetcts the colored markers in the webcam video and creates the color mask.
Point getContours(Mat imgDil);  // Gets the contour of the markers from the color mask.
void draw();    // Draws The points from the contour of the markers on to the screen.

int main() {

    // Chooses which WebCam to stream the video from.
    VideoCapture cap(0);

    // Displays the video.
    while (true) {

        // Reads the input from the webcam and mirrors the video feed.
        cap.read(img);
        flip(img, img, 1);

        // Finds the colors and its position on the screen, then draws it to the webcam video.
        findColor();
        draw();

        // Shows the webcam video.
        imshow("Canvas", img);
        waitKey(1);
        
    }

    return 0;

}


void findColor() {
   
    // Creates the HSV image from the webcam video.
    Mat imgHSV;
    cvtColor(img, imgHSV, COLOR_BGR2HSV);

    // Loops through the color vector's vectors to create the mask for each color.
    for (int i = 0; i < colors.size(); i++) {

        // Takes the HSV values from colors and creates the mask.
        Scalar lower(colors[i][0], colors[i][1], colors[i][2]); // Takes the min HSV values.
        Scalar upper(colors[i][3], colors[i][4], colors[i][5]); // Takes the max HSV values.
        Mat mask;   // Mask for the corresponding color.
        inRange(imgHSV, lower, upper, mask);    // Creates the mask using the HSV values.

        // Creates the point to draw on to the webcam video.
        Point cursor = getContours(mask); // Gets the point of the marker from the mask.
        if (cursor.x != 0 && cursor.y != 0) // Makes sure it only adds the the point vector if the marker is detected.
            newPoints.push_back({ cursor.x, cursor.y, i });
    }

}


Point getContours(Mat imgDil) {

    // Creates the contour vectors using the color mask.
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    vector<vector<Point>> conPoly(contours.size());
    vector<Rect> boundRect(contours.size());

    // Point that will be set to the location of the marker.
    Point cursor(0, 0);

    // Goes through all of the contours on screen.
    for (int i = 0; i < contours.size(); i++)
    {

        // Finds the area of the contours to filter noise.
        int area = contourArea(contours[i]);
        if (area > 1000)
        {
            // Creates the bounding rectangle
            float peri = arcLength(contours[i], true);
            approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
            boundRect[i] = boundingRect(conPoly[i]);

            // Gets the middle-top point of the rectangle and sets the cursor point to that x and y coordinate.
            cursor.x = boundRect[i].x + boundRect[i].width / 2;
            cursor.y = boundRect[i].y + boundRect[i].width / 2;

            // Draws the contour and the bounding rectangle.
            //drawContours(img, conPoly, i, Scalar(255, 0, 255), 2);
            //rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 5);
        }
    }
    return cursor;  // Returns the location of the marker as a Point
}


void draw() {
    
    // Draws Circles on to the screen using the newPoints Vector.
    for (int i = 0; i < newPoints.size(); i++) {
        circle(img, Point(newPoints[i][0], newPoints[i][1]), 10, colorValues[newPoints[i][2]], FILLED);
    }
}