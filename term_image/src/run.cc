#include <chrono>
#include <filesystem>
#include <iostream>
#include <thread>
#include <vector>

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/video.hpp"
#include "opencv2/videoio.hpp"
#include "term/term_image.h"

void Clear() { std::cout << "\x1b[H"; }

void Video() {
  using namespace term;
  namespace fs = std::filesystem;

  TermImage term_image{};
  std::vector<std::string> res = {};

  // From https://gist.github.com/itsrifat/66b253db2736b23f247c.
  try {
    // open the video file
    cv::VideoCapture
        cap("D:\\workflow\\videos\\term_image\\pr\\素材\\视频\\one more "
            "time.mp4");  // open the video
                          // file
    if (!cap.isOpened())  // check if we succeeded
      CV_Error(CV_StsError, "Can not open Video file");

    // cap.get(CV_CAP_PROP_FRAME_COUNT) contains the number of frames in the
    // video;
    for (int frameNum = 0; frameNum < cap.get(CV_CAP_PROP_FRAME_COUNT);
         frameNum++) {
      cv::Mat frame;
      cap >> frame;  // get the next frame from video
      std::string item = term_image.ShowFrame(frame);
      res.push_back(item);
      /*std::cout << "Progress: " << frameNum << "|"
                << cap.get(CV_CAP_PROP_FRAME_COUNT) << '\n';*/
    }

    for (auto& item : res) {
      Clear();
      std::cout << item;
    }
  } catch (cv::Exception& e) {
    std::cerr << e.msg << std::endl;
    exit(1);
  }
}

void Image() {
  using namespace term;
  TermImage term_image{};
  term_image.ShowImage("lucy2.jpg");
}
int main() {
  Image();
  return 0;
}