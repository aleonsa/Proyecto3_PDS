#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>

#define PI 3.14159265358979323846

typedef struct {
    double a[3];
    double b[3];
} IIRCoefficients;

void calculateNotchCoefficients(double Fs, double F0, double Q, IIRCoefficients* coeffs) {
    double w0 = 2.0 * PI * F0 / Fs;
    double alpha = sin(w0) / (2.0 * Q);
    double cosw0 = cos(w0);

    coeffs->b[0] = 1.0;
    coeffs->b[1] = -2.0 * cosw0;
    coeffs->b[2] = 1.0;

    coeffs->a[0] = 1.0 + alpha;
    coeffs->a[1] = -2.0 * cosw0;
    coeffs->a[2] = 1.0 - alpha;
}

int main() {
    // Parámetros del filtro 1 (300 Hz)
    double Fs = 8000.0;  // Frecuencia de muestreo en Hz
    double F0_1 = 300.0; // Frecuencia a suprimir en Hz
    double Q_1 = 10;   // Factor de calidad del filtro

    // Coeficientes del filtro 1
    IIRCoefficients coeffs_1;
    calculateNotchCoefficients(Fs, F0_1, Q_1, &coeffs_1);

    // Parámetros del filtro 2 (1200 Hz)
    double F0_2 = 1200.0; // Frecuencia a suprimir en Hz
    double Q_2 = 10.0;    // Factor de calidad del filtro

    // Coeficientes del filtro 2
    IIRCoefficients coeffs_2;
    calculateNotchCoefficients(Fs, F0_2, Q_2, &coeffs_2);

    // Calcular la respuesta en frecuencia de los filtros
    const int numPoints = 1000;
    double frequency[numPoints];
    double magnitude_1[numPoints];
    double magnitude_2[numPoints];

    for (int i = 0; i < numPoints; i++) {
        double f = Fs * i / numPoints;
        double omega = 2.0 * PI * f / Fs;
        double complex z = cos(omega) + I * sin(omega);

        double complex H_1 = (coeffs_1.b[0] + coeffs_1.b[1] * z + coeffs_1.b[2] * z * z) /
                             (coeffs_1.a[0] + coeffs_1.a[1] * z + coeffs_1.a[2] * z * z);
        double complex H_2 = (coeffs_2.b[0] + coeffs_2.b[1] * z + coeffs_2.b[2] * z * z) /
                             (coeffs_2.a[0] + coeffs_2.a[1] * z + coeffs_2.a[2] * z * z);

        frequency[i] = f;
        magnitude_1[i] = cabs(H_1);
        magnitude_2[i] = cabs(H_2);
    }

    // Guardar los datos en un archivo
    FILE* file = fopen("response.dat", "w");
    for (int i = 0; i < numPoints; i++) {
        fprintf(file, "%f %f %f\n", frequency[i], magnitude_1[i], magnitude_2[i]);
    }
    fclose(file);

    // Generar el gráfico utilizando Gnuplot
    system("gnuplot -p test.gp");


    return 0;
}