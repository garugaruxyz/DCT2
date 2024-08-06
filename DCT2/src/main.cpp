#include <chrono>
#include <iostream>

#include "utils/utils.h"

void demo();

int main() {
    std::chrono::time_point<std::chrono::system_clock> start, end;

    for (int size = 100; size <= 2000; size += 100)
    {
        std::cout << "Dimensione: " << size << std::endl;

        //CREO LA MATRICE
        std::vector<std::vector<double> > matrice = utils::creaMatrice(size);

        //INIZIO CRONOMETRO
        start = std::chrono::system_clock::now();

        //ESEGUO MIO DCT2
        std::vector<std::vector<double> > result = utils::DCT2(matrice);

        //FINE CRONOMETRO
        end = std::chrono::system_clock::now();

        std::chrono::duration<double> elapsed_seconds = end - start;

        std::cout << "Tempo trascorso mio DCT: " << elapsed_seconds.count() << std::endl;

        cv::Mat mat = cv::Mat(size, size, CV_32FC1);
        for (int i = 0; i < mat.rows; ++i)
            for (int j = 0; j < mat.cols; ++j)
                mat.at<float>(i, j) = matrice[i][j];

        // //INIZIO CRONOMETRO
        start = std::chrono::system_clock::now();
        cv::Mat resultCV = cv::Mat(mat.cols, mat.rows, mat.type());
        dct(mat, resultCV);

        //FINE CRONOMETRO
        end = std::chrono::system_clock::now();

        elapsed_seconds = end - start;

        std::cout << "Tempo trascorso DCT OpenCV: " << elapsed_seconds.count() << std::endl;
        std::cout << std::endl;


        std::cout << "_________________________________" << std::endl << std::endl;
    }

    // demo();

    return 0;
}

void demo()
{
    std::vector<std::vector<double> > data{
        {231, 32, 233, 161, 24, 71, 140, 245},
        {247, 40, 248, 245, 124, 204, 36, 107},
        {234, 202, 245, 167, 9, 217, 239, 173},
        {193, 190, 100, 167, 43, 180, 8, 70},
        {11, 24, 210, 177, 81, 243, 8, 112},
        {97, 195, 203, 47, 125, 114, 165, 181},
        {193, 70, 174, 167, 41, 30, 127, 245},
        {87, 149, 57, 192, 65, 129, 178, 228}
    };

    cv::Mat mat = cv::Mat(8, 8, CV_32FC1);
    for (int i = 0; i < mat.rows; ++i)
        for (int j = 0; j < mat.cols; ++j)
            mat.at<float>(i, j) = data[i][j];

    std::vector<std::vector<double>> result = utils::DCT2(data);

    cv::Mat resultCV = cv::Mat(mat.rows, mat.cols, mat.type());
    dct(mat, resultCV);

    std::cout << "Applico DCT2 su:" << std::endl;
    for (int i = 0; i < data.size(); ++i) {
        for (int j = 0; j < data[i].size(); ++j)
            std::cout << std::to_string(data[i][j]) << " ";
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Risultato mio DCT2: " << std::endl;
    for (int i = 0; i < result.size(); ++i) {
        for (int j = 0; j < result[i].size(); ++j) {
            std::cout << result[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Risultato DCT2 OpenCV: " << std::endl;
    for (int i = 0; i < resultCV.rows; ++i) {
        std::cout << resultCV.row(i) << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Applico DCT1 su:" << std::endl;
    for (int i = 0; i < data.size(); ++i) {
        std::cout << std::to_string(data[0][i]) << " ";
    }
    std::cout << std::endl << std::endl;

    std::vector<double> result_v = utils::DCT1(data[0]);

    std::cout << "Risultato mio DCT1:" << std::endl;
    for (int i = 0; i < result_v.size(); ++i) {
        std::cout << std::to_string(result_v[i]) << " ";
    }
    std::cout << std::endl << std::endl;

    cv::Mat vec = cv::Mat(1, 8, CV_32F);
    for (int i = 0; i < vec.cols; ++i)
        vec.at<float>(0, i) = data[0][i];

    cv::Mat resultCV2 = cv::Mat(vec.rows, vec.cols, vec.type());
    dct(vec, resultCV2);

    std::cout << "Risultato DCT1 OpenCV:" << std::endl;
    std::cout << resultCV2 << std::endl;
}