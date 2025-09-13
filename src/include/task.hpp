#include <opencv2/opencv.hpp>
#include <string>

using namespace cv;
using namespace std;

namespace Correlation {

class Object {
private:
    Mat scene;
    Mat image;
    Mat correlation;
    Mat forDisplay;
    Point maxLoc;
    double maxVal;

    double calculateMean();
    void centerImage(Mat& image);
    void normalizeCorrelationForDisplay();

public:
    Object(Mat scene, Mat image);
    ~Object();

    void centerImages();
    void computeCrossCorrelation();
    void findMaxLocation();
    Point get_maxLoc();
    double get_maxVal();
    void saveCorrelation(string path);
};

}