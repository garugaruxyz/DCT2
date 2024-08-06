//
// Created by Davide Costantini on 02/07/24.
//

#include "DiscreteCosineTransform.h"

#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <chrono>

using namespace DiscreteCosineTransform;

DCT2::DCT2(const QString& imagePath)
{
    loadImage(imagePath);
}

DCT2::~DCT2()
{
    blocks.clear();
}

void DCT2::loadImage(const QString& imagePath)
{
    originalImage = imread(imagePath.toStdString(), cv::IMREAD_GRAYSCALE);
    if (originalImage.empty())
    {
        std::cerr << "Could not load image." << std::endl;
        originalHeight = 0;
        originalWidth = 0;
        return;
    }

    originalHeight = originalImage.rows;
    originalWidth = originalImage.cols;

#ifdef QT_DEBUG
    QFileInfo file(imagePath);
    outputPath = file.absoluteDir();
    if (!outputPath.exists("output") && !outputPath.mkdir("output"))
        qDebug("Could not create output folder");
#endif
}

const cv::Mat& DCT2::getOriginalImage() const
{
    return originalImage;
}

const cv::Mat& DCT2::getResultImage() const
{
    return resultImage;
}

int DCT2::getOriginalHeight() const
{
    return originalHeight;
}

int DCT2::getOriginalWidth() const
{
    return originalWidth;
}

void DCT2::setBlockSize(const int blockSize)
{
    this->blockSize = blockSize;
}

int DCT2::getBlockSize() const
{
    return blockSize;
}

std::string DCT2::getResultFilePath()
{
    return resultFilePath;
}

void DCT2::setThreshold(int threshold)
{
    this->threshold = threshold;
}

int DCT2::getThreshold() const
{
    return threshold;
}

void DCT2::performDCT2()
{
    originalImage.convertTo(resultImage, CV_32FC1);

    int x = 0, y = 0;
    while (y < resultImage.rows)
    {
        if (x + blockSize - 1 < originalWidth && y + blockSize - 1 < originalHeight)
        {
            cv::Mat submatrix = cv::Mat(blockSize, blockSize, CV_32FC1);
            resultImage(cv::Rect(x, y, blockSize, blockSize)).copyTo(submatrix);

#ifdef QT_DEBUG
            std::string filename = std::to_string(x) + std::to_string(y) + "_before" + ".bmp";
            std::string test = outputPath.path().toStdString() + "/output/" + filename;
            imwrite(outputPath.path().toStdString() + "/output/" + filename, submatrix);
#endif

            if (submatrix.rows % 2 != 0)
            {
                cv::Mat newRow = cv::Mat::zeros(1, submatrix.cols, CV_32FC1);
                vconcat(submatrix, newRow, submatrix);
            }
            if (submatrix.cols % 2 != 0)
            {
                cv::Mat newColumn = cv::Mat::zeros(submatrix.rows, 1, CV_32FC1);
                hconcat(submatrix, newColumn, submatrix);
            }

            dct(submatrix, submatrix);

            for (int i = 0; i < submatrix.rows; ++i)
                for (int j = 0; j < submatrix.cols; ++j)
                    if (i + j > threshold)
                        submatrix.at<float>(i, j) = 0.0f;

            block b = { submatrix, x, y };
            blocks.push_back(b);
        }

        x += blockSize;
        if (x >= resultImage.cols)
        {
            x = 0;
            y += blockSize;
        }
    }
}

void DCT2::performIDCT2()
{
    int width = originalWidth;
    if (width % blockSize != 0)
        width = width - (width % blockSize);
    int height = originalHeight;
    if (height % blockSize != 0)
        height = height - (height % blockSize);
    resultImage = cv::Mat(height, width, CV_32FC1);

    for (block& b : blocks)
    {
        dct(b.data, b.data, cv::DCT_INVERSE);

        cv::Mat tmp(blockSize,blockSize, CV_32FC1);

        if (b.data.rows != blockSize || b.data.cols != blockSize)
            b.data(cv::Rect(0, 0, blockSize, blockSize)).copyTo(tmp);
        else
            b.data.copyTo(tmp);

        for (int i = 0; i < tmp.rows; ++i)
            for (int j = 0; j < tmp.cols; ++j)
            {
                if (tmp.at<float>(i, j) < 0.0f)
                    tmp.at<float>(i, j) = 0.0f;
                else if (tmp.at<float>(i, j) > 255.0f)
                    tmp.at<float>(i, j) = 255.0f;
            }

#ifdef QT_DEBUG
        std::string filename = std::to_string(b.originalX) + std::to_string(b.originalY) + "_after" + ".bmp";
        imwrite(outputPath.path().toStdString() + "/output/" + filename, tmp);
#endif

        tmp.copyTo(resultImage(cv::Rect(b.originalX, b.originalY, blockSize, blockSize)));
    }

    resultImage.convertTo(resultImage, CV_8U);
    uint64_t ms = duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    resultFilePath = outputPath.path().toStdString() + std::to_string(ms) + ".bmp";
    imwrite(resultFilePath, resultImage);
}

void DCT2::reset()
{
    blocks.clear();
    resultImage = cv::Mat();
}
