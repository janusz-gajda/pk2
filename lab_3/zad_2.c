#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

int main(int arg, char *argv[]){
    srand(time(0));
    float proto = 36 + rand() % 5 + (float)rand()/(float)RAND_MAX;
    FILE *file = fopen(argv[1],"w");
    FILE *prop = fopen("propertis.da", "wb");
    FILE *json = fopen("json.json", "w");
    if(argv[1]){
        for(int i = 0; i < 10; i++){
            fprintf(file, "%f,%f,%f,%f,%f\n",36 + rand() % 5 + (float)rand()/(float)RAND_MAX, 50 + rand() % 70 + (float)rand()/(float)RAND_MAX, 120 + rand() % 100 + (float)rand()/(float)RAND_MAX, 80 + rand() % 60 + (float)rand()/(float)RAND_MAX, 60 + rand() % 40 + (float)rand()/(float)RAND_MAX);
        }
        fwrite(&argv[1],sizeof(argv[1]),1,prop);
        fprintf(json, "{\n\t\"param\" : \"%s\"\n}", argv[1]);
        fclose(file);
        fclose(prop);
        fclose(json);
    }
}