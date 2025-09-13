#include <opencv2/opencv.hpp>
#include <iostream>
#include "task.hpp"

int main(int argc, char** argv) {

    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <large_image_path> <template1_path> <template2_path>" << std::endl;
        return -1;
    }

    Correlation::Object wObject1(imread(argv[1], IMREAD_GRAYSCALE), imread(argv[2], IMREAD_GRAYSCALE));

    wObject1.centerImages();
    wObject1.computeCrossCorrelation();
    wObject1.findMaxLocation();

    std::cout << "Template 1 found at: (" << wObject1.get_maxLoc().x << ", " << wObject1.get_maxLoc().y 
        << ") with correlation value: " << wObject1.get_maxVal() << std::endl;

    wObject1.saveCorrelation(argv[3]);

    std::cout << "Done!" << std::endl;

    return 0;
}