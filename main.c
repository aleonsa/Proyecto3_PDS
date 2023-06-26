#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define M           40000
#define PI          3.1415926536
#define SNR         1

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

int main()
{   
    float v[M], vt[M], T1[M], T2[M];
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

    /***************************************************************************************/
    // Close the file pointers
    fclose(fv);
    fclose(fvt);
    fclose(fvf);
    fclose(fv_spec);
    fclose(fvt_spec);
    fclose(fvf_spec);

    //system("gnuplot -p main.gp");

    return 0;
}