#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>
#include "task.hpp"

int main(int argc, char** argv) {

    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <large_image_path> <template1_path> <path/to/save/correlation> <fast/slow>" << std::endl;
        return -1;
    }

    Correlation::Object wObject1(imread(argv[1], IMREAD_GRAYSCALE), imread(argv[2], IMREAD_GRAYSCALE));

    wObject1.centerImages();
    auto start = std::chrono::high_resolution_clock::now();
    wObject1.computingCorrelation(argv[4]);
    auto end = std::chrono::high_resolution_clock::now();
    wObject1.findMaxLocation();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Image potentially found at: (" << wObject1.get_maxLoc().x << ", " << wObject1.get_maxLoc().y 
        << ") with correlation value: " << wObject1.get_maxVal() << std::endl;

    std::cout << "Cross Correlation computation time: " << duration.count() << " milliseconds" << std::endl;

    wObject1.saveCorrelation(argv[3]);

    std::cout << "Done!" << std::endl;

    return 0;
}