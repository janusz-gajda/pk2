/** @file */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "cJSON/cJSON.h"
#include "generic.h"
#include "param.h"
#include "json.h"

/**Create empty JSON objects*/
char *json_create_missing(void){
    char *string = NULL;
    cJSON *main = cJSON_CreateObject();
    cJSON *history = cJSON_CreateArray();
    cJSON *hashes = cJSON_CreateArray();

    cJSON_AddItemToObject(main, "history", history);
    cJSON_AddItemToObject(main, "hashes", hashes);

    string = cJSON_Print(main);
    cJSON_Delete(main);
    return string;
}

/**Add entry for puzlle into JSON
 * @param array_before array before solving
 * @param array_after array after solving
 * @param hash hash
 * @param json_string contents of JSON
 * @param size size of JSON string 
*/
char *json_create_object(int *array_before, int *array_after, uint32_t hash, char *json_string, long size){
    char *string = NULL;
    cJSON *json_main = cJSON_ParseWithLength(json_string,size);
    cJSON *object = cJSON_CreateObject();
    cJSON *hash_hashes = cJSON_CreateNumber(hash);
    cJSON *hash_history = cJSON_CreateNumber(hash);

    cJSON *before_solving = NULL;
    cJSON *after_solving = NULL;
    cJSON *before_element = NULL;
    cJSON *after_element = NULL;

    cJSON *history = cJSON_GetObjectItemCaseSensitive(json_main, "history");
    cJSON *hashes = cJSON_GetObjectItemCaseSensitive(json_main, "hashes");

    before_solving = cJSON_CreateArray();
    after_solving = cJSON_CreateArray();

    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            before_element = cJSON_CreateNumber(*(array_before + (i*9) + j));
            after_element = cJSON_CreateNumber(*(array_after + (i*9) + j));
            cJSON_AddItemToArray(before_solving,before_element);
            cJSON_AddItemToArray(after_solving,after_element);
        }
    }

    cJSON_AddItemToObject(object, "hash", hash_history);
    cJSON_AddItemToObject(object, "before_solving", before_solving);
    cJSON_AddItemToObject(object, "after_solving", after_solving);
    cJSON_AddItemToArray(history, object);
    cJSON_AddItemToArray(hashes, hash_hashes);

    string = cJSON_Print(json_main);
    cJSON_Delete(json_main);
    return string;

}

/**Check, if entry already exists
 * @param string contents of JSON file
 * @param len size of JSON file
 * @param hash hash
*/
int json_compare_hash(char *string, long len, uint32_t hash){
    cJSON *main = cJSON_ParseWithLength(string, len);
    cJSON *hashes = cJSON_GetObjectItemCaseSensitive(main, "hashes");
    cJSON *item = NULL;

    cJSON_ArrayForEach(item, hashes){
        if(item->valuedouble == hash){
            cJSON_Delete(main);
            return 1;
        }
    }
    cJSON_Delete(main);
    return 0;
}

/**Get object form JSON
 * @param string contents of JSON file
 * @param len size of JSON file
 * @param hash hash
 * @param grid_after array after_solving 
*/
int json_find_existing(char *string, long len, uint32_t hash, int *grid_after){
    cJSON *main = cJSON_ParseWithLength(string, len);
    cJSON *history = cJSON_GetObjectItemCaseSensitive(main, "history");
    cJSON *object = NULL;
    cJSON *hash_json = NULL;
    cJSON *grid_from_json = NULL;
    cJSON *element_from_grid = NULL;
    
    cJSON_ArrayForEach(object, history){
        hash_json = cJSON_GetObjectItemCaseSensitive(object, "hash");
        if(hash_json->valuedouble == hash){
            int i = 0;
            grid_from_json = cJSON_GetObjectItemCaseSensitive(object, "after_solving");
            cJSON_ArrayForEach(element_from_grid, grid_from_json){
                *(grid_after + i) = element_from_grid->valuedouble;
                i++;
            }
            cJSON_Delete(main);
            return 1;
        }
    }
    cJSON_Delete(main);
    return 0;

}

/**Called, when problem occured with JSON file*/
int json_found_problem(void){
    printf("There was a problem with history file! What to do:\n");
    printf("c - continue without history\n");
    printf("n - create new history file\n");
    char ch = getchar();
    if(ch == 'n' || ch == 'N'){
        return 0;
    } else{
        return 2;
    }
}

/**Check, if JSON file is valid
 * @param string contents of JSON file
 * @param len size of JSON file 
*/
int json_check_integrity(char *string, long len){
    cJSON *main = cJSON_ParseWithLength(string, len);
    if(main == NULL){
        return json_found_problem();
    } else{
        cJSON *history = cJSON_GetObjectItemCaseSensitive(main, "history");
        cJSON *hashes = cJSON_GetObjectItemCaseSensitive(main, "hashes");
        if(hashes == NULL || history == NULL){
            cJSON_Delete(main);
            return json_found_problem();
        } else{
            cJSON *object = NULL;
            cJSON *grid_before = NULL;
            cJSON *grid_after = NULL;
            cJSON *hash = NULL;
            int i = 0;
            cJSON_ArrayForEach(object, history){
                i++;
                grid_before = cJSON_GetObjectItemCaseSensitive(object, "before_solving");
                grid_after = cJSON_GetObjectItemCaseSensitive(object, "after_solving");
                hash = cJSON_GetObjectItemCaseSensitive(object, "hash");
                if(grid_before == NULL || grid_after == NULL || hash == NULL){
                    cJSON_Delete(main);
                    return json_found_problem();
                } if(!cJSON_IsNumber(hash) || !cJSON_IsArray(grid_before) || !cJSON_IsArray(grid_after) || cJSON_GetArraySize(grid_before) != cJSON_GetArraySize(grid_after) || cJSON_GetArraySize(grid_before) != 81){
                    cJSON_Delete(main);
                    return json_found_problem();
                }
            }
            if(i == 0){
                cJSON_Delete(main);
                return 1;
            }
            cJSON_ArrayForEach(object,hashes){
                if(!cJSON_IsNumber(object)){
                    cJSON_Delete(main);
                    return json_found_problem();
                }
            }
        }
        cJSON_Delete(main);
        return 1;
    }
}

/**Iterate through JSON file
 * @param string contents of JSON file
 * @param len size of JSON file 
*/
int json_history(char *string, long len){
    cJSON *main = cJSON_ParseWithLength(string,len);
    cJSON *history = cJSON_GetObjectItemCaseSensitive(main, "history");
    cJSON *list = history->child;
    cJSON *grid_before_json = NULL;
    cJSON *grid_after_json = NULL;
    cJSON *element = NULL;
    int next_avaliable = 1;
    int previous_avaliable = 0;
    int grid_before[9][9], grid_after[9][9];
    int *grid_before_p = &grid_before[0][0];
    int *grid_after_p = &grid_after[0][0];
    clrscr();
    while(1){
        if(list->prev == NULL){
            previous_avaliable = 0;
        } else{
            previous_avaliable = 1;
        }

        if(list->next == NULL){
            next_avaliable = 0;
        } else{
            next_avaliable = 1;
        }
        grid_before_json = cJSON_GetObjectItemCaseSensitive(list, "before_solving");
        grid_after_json = cJSON_GetObjectItemCaseSensitive(list, "after_solving");
        int i = 0;
        cJSON_ArrayForEach(element,grid_before_json){
            if(i < 81)
                *(grid_before_p + i) = element->valuedouble;
            i++;
        }
        i = 0;
        cJSON_ArrayForEach(element,grid_after_json){
            if(i < 81)
                *(grid_after_p + i) = element->valuedouble;
            i++;    
        }
        printf("Puzzle provided:\n\n\t");
        for(i = 0; i < 9; i++){
            for(int j = 0; j < 9; j++){
                if(grid_before[i][j] == 0){
                    printf(". ");
                } else{
                    printf("%d ", grid_before[i][j]);
                }

                if(j == 2 || j == 5){
                    printf("  ");
                }
            }
            if(i == 2 || i == 5){
                printf("\n");
            } 
            printf("\n\t");
        }
        printf("\n\nPuzzle solved:\n\n\t");
        for(i = 0; i < 9; i++){
            for(int j = 0; j < 9; j++){
                printf("%d ", grid_after[i][j]);
                if(j == 2 || j == 5){
                    printf("  ");
                }
            }
            if(i == 2 || i == 5){
                printf("\n");
            } 
            printf("\n\t");
        }
        if(next_avaliable == 0){
            printf("\n\n e - exit  p - previous\n");
        } else if(previous_avaliable == 0){
            printf("\n\n e - exit  n - next\n");
        } else{
            printf("\n\n e - exit  n - next  p - previous\n");
        }
        i = 0;
        while(!i){
            char ch = getchar();
            if(ch == 'e' || ch == 'E'){
                i = -1;
                break;
            } else if((ch == 'n' || ch == 'N') && next_avaliable){
                i = 1;
                break;
            } else if((ch == 'p' || ch == 'P') && previous_avaliable){
                i = 2;
                break;
            } else if(ch == 'n' || ch == 'N'){
                printf("\nNext is not available\n");
            } else if(ch == 'p' || ch == 'P'){
                printf("\nPrevious is not available\n");
            }
        }
        if(i == 1){
            list = list->next;
            clrscr();
        } else if(i == 2){
            list = list->prev;
            clrscr();
        } else{
            cJSON_Delete(main);
            return 1;
        }

    }
        
    cJSON_Delete(main);
    return 1;
}

/**Initialize JSON
 * @param head head of struct
 * @param size size of JSON file
*/
char *start_history(arg_t *head, long *size){
    char *string_from_file = NULL;
    FILE *json = fopen("history.json", "r+");
    if(json == NULL){
        printf("History file does not exist, attempting to create...\n");
        json = fopen("history.json", "w+");
        if(!json){
            printf("Failed to create history file, ommiting it...\n");
            param_disable_history(head);
        }    
    }

    if(param_status_history(head)){
        long fsize = file_size(json);
        rewind(json);
        if(is_empty(json)){
            string_from_file = json_create_missing();
            fsize = strlen(string_from_file);
        } else{
            string_from_file = malloc(fsize + 1);
            int read = fread(string_from_file, 1, fsize, json);
        }
        fclose(json);

        int json_check = json_check_integrity(string_from_file, fsize+1);

        if(!json_check){
            free(string_from_file);
            string_from_file = json_create_missing();
            fsize = strlen(string_from_file);
        } else if(json_check != 1){
            param_disable_history(head);
        }

        *size = fsize;
    }

    return string_from_file;
}

/**Previous function
 * @param head head of struct
 * @param string contents of JSON file 
 * @param fsize size of JSON file
*/
void start_previous(arg_t *head, char *string, long fsize){
    if(!param_status_history(head)){
            printf("There was a problem with history file, therefore previous results are unavailable\n");
        } else{
            json_history(string, fsize+1);
        }
}