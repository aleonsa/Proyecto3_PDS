#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define M           40000
#define PI          3.1415926536
#define SNR         1
#define Fs          8000
#define N           298

void convolucion(float *x, int len_x, float *h, int len_h, float *y) {
    for (int n = 0; n < len_x + len_h - 1; n++) {
        y[n] = 0;
        for (int k = 0; k < len_h; k++) {
            if (n - k >= 0 && n - k < len_x) {
                y[n] += x[n - k] * h[k];
            }
        }
    }
}

void Goertzel(float *x, int len_x, double *y, int klim){
	double vk0,vk1,vk2,AI, AR;
	int k,n;
	for(k=0; k<klim; k++){
		vk0=0;
		vk1=0;
		vk2=0;
		for(n=0;n<len_x;n++){	
			vk0=x[n]+2*cos(2*PI*k/len_x)*vk1-vk2;
			vk2=vk1;
			vk1=vk0;
		}
		AR=vk0+vk1*cos(2*PI*k/len_x);
		AI=vk1*sin(2*PI*k/len_x);
		y[k]= sqrt(AR*AR+AI*AI);
	}
}

double signalPower(double* signal, int length) {
    double power = 0.0;

    for (int i = 0; i < length; i++) {
        power += signal[i] * signal[i];
    }

    power /= length;

    return power;
}

void firFSPW(float* hFSBW, int fl, int fh){

    float wl = (2 * PI * fl) / Fs;
    float wh = (2 * PI * fh) / Fs;

    float hFPB[N], hann[N];
    float w0 = 2 * PI * (fh-fl) / Fs;
    float wc = (wh - wl) / 2;

    hFPB[0] = (1 / PI * 0.00001) * (sin(wc * 0.00001));
    hFSBW[0] = 2 * hFPB[0];

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
    }
    
}

int main()
{   
    float v[M], vt[M], vf[M];
    float T1[M], T2[M];
    int n = 0;
    /* *********************Define File Pointers for plotting in GNUPlot*****************/
    FILE *fv, *fvt, *fvf, *fv_spec, *fvt_spec, *fvf_spec;
        fv = fopen("v.dat", "r");
        fvt = fopen("vt.dat","w");
        fvf = fopen("vf.dat","w");
        fv_spec = fopen("v_spec.dat","w");
        fvt_spec = fopen("vt_spec.dat","w");
        fvf_spec = fopen("vf_spec.dat","w");

        if (fv == NULL) {
            printf("Error opening voice file.");
            return 1;
        }

    /*************************Filtro FIR 300Hz*************************/
    float hF1[N],hF2[N], hFT[N];
    firFSPW(hF1, 290, 310);
    firFSPW(hF2, 1190, 1210);

    convolucion(hF1,N,hF2,N,hFT);


    // int fl = 290;
    // int fh = 310;
    // float wl = (2 * PI * fl) / Fs;
    // float wh = (2 * PI * fh) / Fs;

    // float hFSBW[N], hFPB[N], hann[N];
    // float w0 = 2 * PI * 300 / Fs;
    // float wc = (wh - wl) / 2;

    // hFPB[0] = (1 / PI * 0.00001) * (sin(wc * 0.00001));
    // hFSBW[0] = 2 * hFPB[0];

    // for (int n = 1; n < N; n++) {
    //     hFPB[n] = sin(wc * n) / (PI * n);
    //     hFSBW[n] = 2 * cos(n * w0) * hFPB[n]; // NO CAUSAL
    // }

    // // Generamos ventana de Hanning
    // for (int i = 0; i < N; i++) {
    //     hann[i] = 0.5 * (1 - cos(2 * PI * i / N));
    // }

    // // Multiplicamos hFSBW por la ventana y guardamos los resultados en el archivo filtro.dat
    // for (int i = 0; i < N; i++) {
    //     hFSBW[i] = hann[i] * hFSBW[i];
    // }
   
    /*************************************************************************************/
    // Read the voice file recorded with Matlab
        while (fscanf(fv, "%f", &v[n]) == 1){
        n++;
        if (n >= M) {
            printf("Maximum array size reached. Some data may have been truncated.\n");
            break;
        }
    }

    float power = 0;

    for (int i = 0; i < M; i++) {
        power += v[i] * v[i];
    }
    power /= M;
    float powerdB = 10 * log10(power);
    printf("Power of the voice signal: %.4f dB\n",powerdB);

    float A = sqrt(pow(10,(powerdB/10))); // amp de los tonos
    float w1 = (300*2*PI)/M; // 300Hz
    float w2 = (1200*2*PI)/M; // 1200 Hz     frecuencias para tonos

    /*Generate the tones to add to signal*/

    float a11, a12 , a2;
    a11 = -2*cos(w1);
    a12 = -2*cos(w2);
    a2 = 1;
    
    T1[0] = 0; T1[1] = sin(w1); 
    T2[0] = 0; T2[1] = sin(w2);

    vt[0] = v[0] + T1[0] + T2[0];  vt[1] = v[1] + T1[1] + T2[1];
    fprintf(fvt, "%f\n", vt[0]); fprintf(fvt, "%f\n", vt[1]);

	
	for (int n = 2; n < M; n++)
    {
        T1[n] = -a11*T1[n-1] - a2*T1[n-2];
        T2[n] = -a12*T2[n-1] - a2*T2[n-2];
        vt[n] = v[n] + T1[n] + T2[n];
        fprintf(fvt, "%f\n", vt[n]);
    }


    convolucion(hFT, N, v, M, vf);
    for (int i = 0; i < M; i++) {
        fprintf(fvf, "%f\n", 100*vf[i]);
    }


    /***************************************************************************************/
    // Close the file pointers
    fclose(fv);
    fclose(fvt);
    fclose(fvf);
    fclose(fv_spec);
    fclose(fvt_spec);
    fclose(fvf_spec);

    system("gnuplot -p main.gp");

    return 0;
}