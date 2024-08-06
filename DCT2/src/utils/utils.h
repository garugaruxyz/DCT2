#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <opencv2/core.hpp>

namespace utils {
	//creazione matrice
	std::vector<std::vector<double>> creaMatrice(int size);
	//creazione matrice per OpenCV
	cv::Mat creaMatriceCV(int size);
	//funzione per dct2
	std::vector<double> DCT1(const std::vector<double>& vettore);
	//funzione per dct2
	std::vector<std::vector<double>> DCT2(const std::vector<std::vector<double>>& matrice);
}



#endif //UTILS_H
