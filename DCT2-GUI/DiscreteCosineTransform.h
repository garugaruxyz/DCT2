//
// Created by Davide Costantini on 02/07/24.
//

#ifndef DCT2_H
#define DCT2_H

#include <QString>
#include <opencv2/core.hpp>
#include <QDir>

namespace DiscreteCosineTransform
{
    struct block
    {
        cv::Mat data;
        int originalX;
        int originalY;
    };

    class DCT2
    {
        private:
            cv::Mat originalImage;

            cv::Mat resultImage;

            int originalHeight = 0;

            int originalWidth = 0;

            int blockSize = 0;

            int threshold = 0;

            std::vector<block> blocks;

            std::string resultFilePath;

            QDir outputPath;

        public:
            DCT2() = default;

            explicit DCT2(const QString& imagePath);

            ~DCT2();

            void loadImage(const QString& imagePath);

            const cv::Mat& getOriginalImage() const;

            const cv::Mat& getResultImage() const;

            int getOriginalHeight() const;

            int getOriginalWidth() const;

            void setBlockSize(int blockSize);

            int getBlockSize() const;

            void setThreshold(int threshold);

            int getThreshold() const;

            std::string getResultFilePath();

            void performDCT2();

            void performIDCT2();

            void reset();
    };
}

#endif //DCT2_H
