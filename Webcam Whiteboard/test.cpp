#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

#define TEST_IMAGE_PATH "Resources/test.png"
#define TEST_VIDEO_PATH "Resources/test_video.mp4"

using namespace cv;
using namespace std;

// Image/Video Reading methods:
void readImage(string path, string imgName = "Image", int delay = 0);
void readVideo(string path, string videoName = "Video", int delay = 20);
void readWebCam(int path = 0, string webCamName = "Webcam", bool mirror = false);

// Image Manipulation methods:
void imageManipulation(string path, string imgName = "Image", int delay = 0);
void imageResize(string path, double x, double y, bool scale, int delay = 0);
void imageCrop(string path, int x, int y, int w, int h, int delay = 0);

// Drawing methods:
void drawImages(int w, int h, int b, int g, int r, string imgName = "Blank Image", int delay = 0);

int main() {

    //readImage(TEST_IMAGE_PATH);
    //readVideo(TEST_VIDEO_PATH);
    //readWebCam(0, "Webcam", true);
    //imageManipulation(TEST_VIDEO_PATH);
    //imageResize(TEST_IMAGE_PATH, .5, .5, true); // scales the image to half its size.
    //imageResize(TEST_IMAGE_PATH, 800, 800, false); // creates exact pixel size.
    //imageCrop(TEST_IMAGE_PATH, 0, 0, 300, 300);
    drawImages(512, 512, 255, 0, 255);

    return 0;

}

void readImage(string path, string imgName, int delay) {

    // Creates the image from the file path.
    Mat img = imread(path);
    imshow(imgName, img);

    // Timer for how long the image will be opened.
    waitKey(delay);

}

void readVideo(string path, string videoName, int delay) {

    // Creates the Video from the .mp4.
    VideoCapture cap(path);
    Mat img;

    // Displays the VideoCapture.
    while (true) {

        // Displays the Images.
        cap.read(img);
        imshow(videoName, img);
        waitKey(delay); // Speed of the video.
    }

}

void readWebCam(int path, string webCamName, bool mirror) {

    // Chooses which WebCam to stream the video from.
    VideoCapture cap(path);
    Mat img;
    
    // Displays the video.
    while (true) {
        cap.read(img);

        // Mirrors image
        if (mirror)
            flip(img, img, 1);

        imshow(webCamName, img);
        waitKey(1);
    }
}

void imageManipulation(string path, string imgName, int delay) {
    
   // Images:
   Mat img = imread(path); // Origional image.
   Mat imgGray, imgBlur, imgCanny, imgDil, imgErode; // Modified images.

   // Image mofifiers:
   cvtColor(img, imgGray, COLOR_BGR2GRAY); // Converts the img to Grayscale and stores it in imgGray.
   GaussianBlur(imgGray, imgBlur, Size(3, 3), 5, 1); // Blurs the img and stores it in imgBlur.
   Canny(imgBlur, imgCanny, 50, 100); // Uses blurred image to get the edges. Lower values get more edges.
   
   Mat kernel = getStructuringElement(MORPH_RECT, Size(2, 2)); // Creates Kernel to dilate the image. Larger size is more dilation.
   dilate(imgCanny, imgDil, kernel); // Uses the edge image to dilate (Increase thinkess of lines) the image.
   erode(imgDil, imgErode, kernel); // Uses diluted image to erode (Decrease thickness of lines) the image.

   // Image display:
   imshow(imgName, img);
   imshow("Gray", imgGray);
   imshow("Blur", imgBlur);
   imshow("Canny/Edges", imgCanny);
   imshow("Dilation", imgDil);
   imshow("Erode", imgErode);
   waitKey(delay);

}

void imageResize(string path, double x, double y, bool scale, int delay) {

    Mat img = imread(path); // Origional image.
    Mat imgResize;  // Resized image.

    if (scale)
        resize(img, imgResize, Size(), x, y); // scales the image
    else
        resize(img, imgResize, Size(x, y)); // resizes using exact pixels

    imshow("Resized Image", imgResize);
    waitKey(delay);
}

void imageCrop(string path, int x, int y, int w, int h, int delay) {

    Mat img = imread(path); // Origional image.
    Mat imgCrop;  // Resized image.

    Rect roi(x, y, w, h); // Creates the region of interest rectangle (Rectangle of the part of the image we're cropping).
    imgCrop = img(roi); // Sets imgCrop to that region from the origional image.

    imshow("Cropped Image", imgCrop);
    waitKey(delay);

}

void drawImages(int w, int h, int b, int g, int r, string imgName, int delay) {

    Mat img(w, h, CV_8UC3, Scalar(b, g, r)); // Building the image. CV_8UC3 is the bit channel for bgr values.

    circle(img)

    imshow(imgName, img);
    waitKey(delay);

}