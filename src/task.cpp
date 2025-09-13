#include "task.hpp"
#include <iostream>

using namespace cv;
using namespace std;

namespace Correlation {

Object::Object(Mat scene, Mat image) {

    if (scene.empty() || image.empty()) {
        cerr << "Error: Could not load one or more images." << endl;
        exit(1);
    }

    this->image = image;
    this->scene = scene;
}

Object::~Object() {

}

double Object::calculateMean() {
    double sum = 0.0;
    for (int i = 0; i < this->image.rows; ++i) {
        for (int j = 0; j < this->image.cols; ++j) {
            sum += static_cast<double>(image.at<uchar>(i, j));
        }
    }
    return sum / (image.rows * image.cols);
}

void Object::centerImage(Mat& image) {
    double mean = calculateMean();
    for (int i = 0; i < image.rows; ++i) {
        for (int j = 0; j < image.cols; ++j) {
            int centered_val = static_cast<int>(image.at<uchar>(i, j)) - static_cast<int>(mean);
            // Ограничиваем значение, чтобы избежать переполнения при сохранении в uchar
            centered_val = max(0, min(255, centered_val));
            image.at<uchar>(i, j) = static_cast<uchar>(centered_val);
        }
    }
}

void Object::centerImages() {
    centerImage(this->scene);
    centerImage(this->image);
}

void Object::computeCrossCorrelation() {
    int corrRows = scene.rows - image.rows + 1;
    int corrCols = scene.cols - image.cols + 1;
    Mat correlation(corrRows, corrCols, CV_64F, Scalar(0));

    // Перебираем все возможные позиции шаблона на большом изображении
    for (int y = 0; y < corrRows; ++y) {
        for (int x = 0; x < corrCols; ++x) {
            double sum = 0.0;
            // Перебираем все пиксели шаблона
            for (int ty = 0; ty < image.rows; ++ty) {
                for (int tx = 0; tx < image.cols; ++tx) {
                    // Умножаем яркости соответствующих пикселей
                    double largeVal = static_cast<double>(scene.at<uchar>(y + ty, x + tx));
                    double templVal = static_cast<double>(image.at<uchar>(ty, tx));
                    sum += largeVal * templVal;
                }
            }
            correlation.at<double>(y, x) = sum;
        }
    }

    this->correlation = correlation;
}

void Object::findMaxLocation() {
    minMaxLoc(correlation, nullptr, &this->maxVal, nullptr, &this->maxLoc);
}

void Object::normalizeCorrelationForDisplay() {
    double minVal, maxVal;
    minMaxLoc(this->correlation, &minVal, &maxVal);
    this->correlation.convertTo(this->forDisplay, CV_8U, 255.0 / (maxVal - minVal), -minVal * 255.0 / (maxVal - minVal));
}

void Object::saveCorrelation(string path) {
    this->normalizeCorrelationForDisplay();
    imwrite(path, this->forDisplay);
}

Point Object::get_maxLoc() {
    return this->maxLoc;
}

double Object::get_maxVal() {
    return this->maxVal;
}

}