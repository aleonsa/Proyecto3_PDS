#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>


#define N_V        40000


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

int main()
{
    /* *********************Define File Pointers for plotting in GNUPlot*****************/
    FILE *fv, *fvt, *fvf, *fv_spec, *fvt_spec, *fvf_spec;
        fv = fopen("v.dat", "r");
        fvt = fopen("vt.dat","w");
        fvf = fopen("vf.dat","w");
        fv_spec = fopen("v_spec.dat","w");
        fvt_spec = fopen("vt_spec.dat","w");
        fvf_spec = fopen("vf_spec.dat","w");
    /*************************************************************************************/


    // ... code to be written ...



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