#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

int com (const void *p, const void *q){
    return ( *(int*)p - *(int*)q);
}

int main(int arg, char *argv[]){
    if(argv[1]){
        FILE *main = fopen(argv[1], "r");
        int size = 0;
        char ch;
        while((ch=fgetc(main))!=EOF){
            if(ch=='\n')
                size++;
        }        
        fclose(main);
        main = fopen(argv[1], "r");
        float sum_temp = 0;
        float sum_weight = 0;
        float sum_height = 0;
        float sum_press1 = 0;
        float sum_press2 = 0;
        float temp_temp = 0;
        float temp_weight = 0;
        float temp_height = 0;
        float temp_press1 = 0;
        float temp_press2 = 0;
        int i = 0;
        float array_temp[size];
        float array_weight[size];
        float array_height[size];
        float array_press1[size];
        float array_press2[size];
        while((fscanf(main,"%f,%f,%f,%f,%f\n",&temp_temp,&temp_weight,&temp_height,&temp_press1,&temp_press2)) != EOF){
            sum_temp += temp_temp;
            sum_weight += temp_weight;
            sum_height += temp_height;
            sum_press1 += temp_press1;
            sum_press2 += temp_press2;
            array_temp[i] = temp_temp;
            array_height[i] = temp_height;
            array_weight[i] = temp_weight;
            array_press1[i] = temp_press1;
            array_press2[i] = temp_press2;
            i++;
        }
        fclose(main);
        qsort(array_temp,size,sizeof(float),com);
        qsort(array_weight,size,sizeof(float),com);
        qsort(array_height,size,sizeof(float),com);
        qsort(array_press1,size,sizeof(float),com);
        qsort(array_press2,size,sizeof(float),com);
        float mid_temp = 0;
        float mid_weight = 0;
        float mid_height = 0;
        float mid_press1 = 0;
        float mid_press2 = 0;
        if(size % 2 == 0){
            mid_temp = array_temp[size/2];
            mid_weight = array_weight[(size/2)-1];
            mid_height = array_height[(size/2)-1];
            mid_press1 = array_press1[(size/2)-1];
            mid_press2 = array_press2[(size/2)-1];
        }else{
            mid_temp = array_temp[(size/2)+1];
            mid_weight = array_weight[size/2];
            mid_height = array_height[size/2];
            mid_press1 = array_press1[size/2];
            mid_press2 = array_press2[size/2];
        }
        FILE *out_csv = fopen(argv[2],"w");
        FILE *out_txt = fopen(argv[3],"w");
        fprintf(out_csv,"type,temperatur,weight,height,pressure1,pressure2\n");
        fprintf(out_csv,"avrage,%f,%f,%f,%f,%f\n",sum_temp/size,sum_weight/size,sum_height/size,sum_press1/size,sum_press2/size);
        fprintf(out_csv,"median,%f,%f,%f,%f,%f\n",mid_temp,mid_weight,mid_height,mid_press1,mid_press2);
        fprintf(out_csv,"min,%f,%f,%f,%f,%f\n",array_temp[0],array_weight[0],array_height[0],array_press1[0],array_press2[0]);
        fprintf(out_csv,"max,%f,%f,%f,%f,%f\n",array_temp[size-1],array_weight[size-1],array_height[size-1],array_press1[size-1],array_press2[size-1]);
        fclose(out_csv);
        fprintf(out_txt,"temp:\n\tavr: %f\n\tmed: %f\n\tmin: %f\n\tmax: %f\n\n",sum_temp/size,mid_temp,array_temp[0],array_temp[size-1]);
        fprintf(out_txt,"weight:\n\tavr: %f\n\tmed: %f\n\tmin: %f\n\tmax: %f\n\n",sum_weight/size,mid_weight,array_weight[0],array_weight[size-1]);
        fprintf(out_txt,"height:\n\tavr: %f\n\tmed: %f\n\tmin: %f\n\tmax: %f\n\n",sum_height/size,mid_height,array_height[0],array_height[size-1]);
        fprintf(out_txt,"pressure1:\n\tavr: %f\n\tmed: %f\n\tmin: %f\n\tmax: %f\n\n",sum_press1/size,mid_press1,array_press1[0],array_press1[size-1]);
        fprintf(out_txt,"pressure2:\n\tavr: %f\n\tmed: %f\n\tmin: %f\n\tmax: %f\n\n",sum_press2/size,mid_press2,array_press2[0],array_press2[size-1]);
        fclose(out_txt);
    }
}