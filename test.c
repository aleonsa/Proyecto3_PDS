#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define PI          3.1415926536
#define Fs          8000
#define N           298



void Goertzel(float *x, int len_x, double *y, int klim) {
    double vk0, vk1, vk2, AI, AR;
    int k, n;
    for (k = 0; k < klim; k++) {
        vk0 = 0;
        vk1 = 0;
        vk2 = 0;
        for (n = 0; n < len_x; n++) {
            vk0 = x[n] + 2 * cos(2 * PI * k / len_x) * vk1 - vk2;
            vk2 = vk1;
            vk1 = vk0;
        }
        AR = vk0 + vk1 * cos(2 * PI * k / len_x);
        AI = vk1 * sin(2 * PI * k / len_x);
        y[k] = sqrt(AR * AR + AI * AI);
    }
}

int main() {
    FILE *file, *especFile;
        file = fopen("filtro.dat", "w");
        especFile = fopen("espec.dat", "w");

    int fl = 290;
    int fh = 310;
    float wl = (2 * PI * fl) / Fs;
    float wh = (2 * PI * fh) / Fs;

    float hFSBW[N], hFPB[N], hann[N];
    float w0 = 2 * PI * 300 / Fs;
    float wc = (wh - wl) / 2;

    hFPB[0] = (1 / PI * 0.00001) * (sin(wc * 0.00001));
    hFSBW[0] = 2 * hFPB[0];
    fprintf(file, "%f\n", hFSBW[0]);

    for (int n = 1; n < N; n++) {
        hFPB[n] = sin(wc * n) / (PI * n);
        hFSBW[n] = 2 * cos(n * w0) * hFPB[n]; // NO CAUSAL
    }

    // Generamos ventana de Hanning
    for (int i = 0; i < N; i++) {
        hann[i] = 0.5 * (1 - cos(2 * PI * i / N));
    }

    // Multiplicamos hFSBW por la ventana y guardamos los resultados en el archivo filtro.dat
    for (int i = 0; i < N; i++) {
        hFSBW[i] = hann[i] * hFSBW[i];
        fprintf(file, "%6.4f\n", hFSBW[i]);
    }

    fclose(file);

    // Calcular el espectro de hFSBW utilizando el algoritmo de Goertzel
    double espectro[N / 2];
    Goertzel(hFSBW, N, espectro, N / 2);

    // Guardar el espectro en el archivo espec.dat
    for (int i = 0; i < N / 2; i++) {
        fprintf(especFile, "%f\n", -espectro[i]);
    }

    fclose(especFile);
    system("gnuplot -p test.gp");

    return 0;
}
