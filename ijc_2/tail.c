// tail.c
   // Řešení IJC-DU2, příklad a), 24.4.2025
   // Autor: Ondrej Cagalinec, FIT
   // Přeloženo: gcc Red Hat 14.2.1-7
   // Program pro zobrazení posledních n řádků ze souboru
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define limit 4095

typedef struct{
    char **lines;
    int read;
    int write;
    int size;
}circular_buffer;

circular_buffer* cbuf_create(unsigned n){
    circular_buffer *cB=(circular_buffer *)malloc(sizeof(circular_buffer));

    if (cB==NULL) {
        fprintf(stderr,"Memmory allocation failed in cbuf_create\n");
        free(cB);
        return NULL;
    }

    cB->lines = (char**)malloc(sizeof(char*)*n);

    if (cB->lines==NULL) {
        fprintf(stderr,"Memory allocation failed in cbuf_create, lines\n");
        for (unsigned i=0; i<n; i++) {
            free(cB->lines[i]);
        }
        free(cB->lines);
        return NULL;
    }

    for(unsigned i=0; i<n;i++){
        cB->lines[i]=NULL;
    }
    cB->read=0;
    cB->write=0;
    cB->size=n;
    return cB;
}
void cbuf_put(circular_buffer* cb, char *line){
    if(cb->read>=cb->write && cb->lines[cb->write]!=NULL){
        free(cb->lines[cb->write]);
        cb->lines[cb->write]=line;
        cb->write=(cb->write+1)%cb->size;
        cb->read=(cb->read+1)%cb->size;
        return;
    }
    cb->lines[cb->write]=line;
    cb->write=(cb->write+1)%cb->size;

    return;
}

char* cbuf_get(circular_buffer* cb){
    char* line=cb->lines[cb->read];
    cb->read=(cb->read+1)%cb->size;
    return line;
}
void cbuf_free(circular_buffer *cb){
    for (int i=0; i<cb->size; i++) {
        free(cb->lines[i]);
    }
    free(cb->lines);
    free(cb);
}

char *duplicate(const char *src) {
    char *dst = (char* ) malloc(strlen(src)+1);
    if (dst==NULL){
        return NULL;
    }
    strcpy(dst, src);
    return dst;
}
int isValidNum(char* num){
    for (unsigned long i=0; i<strlen(num); i++) {
        if (!isdigit(num[i])) {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char* argv[]){
    int n=10;
    FILE *input = stdin;

    if(argc>1){
        for(int i=1; i<argc; i++){
            if ((strcmp(argv[i],"-n"))==0) {
                if(i+1<argc){
                    if(isValidNum(argv[i+1])){
                        n=atoi(argv[i+1]);
                        i++;
                    }
                    else{
                        fprintf(stderr,"-n argument is not a valid number\n");
                        fclose(input);
                        return 1;
                    }
                }
                else{
                    fprintf(stderr, "-n was used but no file path provided\n");
                    fclose(input);
                    return 1;
                }
            }
            else{
                input = fopen(argv[i],"r");
                if (!input) {
                    fprintf(stderr,"Error opening file\n");
                    return 1;
                }
            }
        }
    }
    if (n<=0) {
        return 0;
    }

    circular_buffer *cb=cbuf_create(n);
    if(cb==NULL){
        fclose(input);
        return 1;
    }
    char buffer[limit+2];
    while(fgets(buffer, limit+2, input)!=NULL){
        if (strlen(buffer)>limit) {
            if(buffer[limit]!='\n'){
                buffer[limit]='\n';
                while (fgetc(input) != '\n' && !feof(input)){}
            }
            buffer[limit+1]='\0';
        }
        char* temp = duplicate(buffer);
        if (temp==NULL) {
            fprintf(stderr,"Duplication failed\n");
            cbuf_free(cb);
            fclose(input);
            return 1;
        }
        cbuf_put(cb, temp);
    }
    for (int i=0; i<n; i++) {
        char* line=cbuf_get(cb);

        if (line!=NULL) {
            printf("%s",line);
        }
    }
    cbuf_free(cb);
    fclose(input);

    return 0;
}
