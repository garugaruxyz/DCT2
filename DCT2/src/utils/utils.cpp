#include "utils.h"

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>

//creazione matrice
std::vector<std::vector<double> > utils::creaMatrice(int size) {
    std::vector<std::vector<double> > matrice(size, std::vector<double>(size));
    unsigned seed = time(0);
    srand(seed);

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            matrice[i][j] = rand() % 256;
        }
    }

    return matrice;
}

//creazione matrice
cv::Mat utils::creaMatriceCV(int size) {
    cv::Mat matrice = cv::Mat(size, size, CV_32F);
    unsigned seed = time(0);
    srand(seed);

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            matrice.at<float>(i, j) = rand() % 256;
        }
    }

    return matrice;
}

//funzione dct
std::vector<double> utils::DCT1(const std::vector<double>  &vettore) {
    double N = vettore.size();
    std::vector<double> result(N, 0.0);
    double somma = 0;
    /*
     *  Non è necessario calcolare esplicitamente tutte le volte il prodotto scalare di wk
     *  con se stesso per capire quale valore mettere. È possibile essere più efficienti
     *  semplicemente controllando l’indice k.
     *  se k = 0, w0·w0 = N, altrimenti wk· wk = N/2
    */

    // N: Il numero totale degli elementi nella sequenza di input
    // k: indice della dct (preso da index (mi dice a che vettore siamo))

    for (int k = 0; k < N; k++) {
        somma = 0;
        for (int j = 0; j < N; j++) {
            //applico sommatoria cos;
            somma += cos((M_PI * k) * (((2 * j) + 1) / (2 * N))) * vettore[j];
        }
        if (k == 0) {
            result[k] = sqrt(1 / N) * somma;
        } else {
            result[k] = sqrt(2 / N) * somma;
        }
    }
    return result;
}

//funzione per dct2
std::vector<std::vector<double> > utils::DCT2(const std::vector<std::vector<double> > &matrice) {
    int N = matrice.size();
    std::vector<std::vector<double> > new_matrix(N, std::vector<double>(N));

    //applico dct1 per righe
    for (int i = 0; i < N; ++i) {
        std::vector<double> row_dct = DCT1(matrice[i]);
        for (int j = 0; j < N; ++j) {
            new_matrix[i][j] = row_dct[j];
        }
    }
    //inverto righe e colonne. necessario per poter lavorare come fatto sopra
    std::vector<std::vector<double> > transposed(N, std::vector<double>(N, 0.0));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            transposed[j][i] = new_matrix[i][j];
        }
    }

    //applico dct1 per colonne
    for (int i = 0; i < N; ++i) {
        std::vector<double> col_dct = DCT1(transposed[i]);
        for (int j = 0; j < N; ++j) {
            new_matrix[j][i] = col_dct[j];
        }
    }

    return new_matrix;
}
