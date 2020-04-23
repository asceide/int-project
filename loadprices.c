/*
    Author: Patrick Alarcon
    This program is used to load and get prices and discounts located within various files
*/


#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

void get_monthly_prices(double* m_prices, char* filename);
void get_discounts(double* d_prices, char* filename);
void get_ride_prices(double* r_prices, char* filename);
void get_price_per_stop(double* s_prices, char* filename);
int get_num_lines(char* filename);

void get_discounts(double* d_prices, char* filename){
    char* d_reader=NULL;
    FILE *d_file=fopen(filename, "r");
    int counter=0;
    size_t d_buffer=100;
    char d_type[60];
    double price=0.0;
    if(d_file==NULL){
        fprintf(stderr, "Cannot open file: %s\n", filename);
        return;
    }
    d_reader=malloc(d_buffer*sizeof(char));
    while(getline(&d_reader, &d_buffer, d_file)!=-1){
        sscanf(d_reader, "%s %lf", d_type, &price);
        if(strcmp(d_type,"Student")==0){
            d_prices[counter]=price;
        }else if(strcmp(d_type, "Elderly")==0){
            d_prices[counter]=price;
        }
        counter++;
    }
    fclose(d_file);
    free(d_reader);
}

void get_monthly_prices(double* m_prices, char* filename){
    char* m_reader=NULL;
    FILE *m_file=fopen(filename, "r");
    size_t m_buffer=100;
    char m_type[30];
    double price=0.0;
    int counter=0;
    if(m_file==NULL){
        fprintf(stderr, "Cannot open file: %s\n", filename);
        return;
    }
    m_reader=malloc(m_buffer*sizeof(char));
    while(getline(&m_reader, &m_buffer, m_file)!=-1){
        sscanf(m_reader, "%s %lf", m_type, &price);
        if(strcmp(m_type,"Bus")==0){
            m_prices[counter]=price;
        }else if(strcmp(m_type, "Subway")==0){
            m_prices[counter]=price;
        }else if(strcmp(m_type, "Commuter")==0){
            m_prices[counter]=price;
        }else if(strcmp(m_type, "Special")==0){
            m_prices[counter]=price;
        }
        counter++;
    }
    fclose(m_file);
    free(m_reader);
}

void get_ride_prices(double* r_prices, char* filename){
    char* r_reader=NULL;
    FILE *r_file=fopen(filename, "r");
    size_t r_buffer=100;
    char r_type[50];
    double price=0.0;
    int counter=0;
    if(r_file==NULL){
        fprintf(stderr, "Cannot open file: %s\n", filename);
        return;
    }
    r_reader=malloc(r_buffer*sizeof(char));
    while(getline(&r_reader, &r_buffer, r_file)!=-1){
        sscanf(r_reader, "%s %lf", r_type, &price);
        if(strcmp(r_type,"Bus")==0){
            r_prices[counter]=price;
        }else if(strcmp(r_type, "Subway")==0){
            r_prices[counter]=price;
        }else if(strcmp(r_type, "Commuter")==0){
            r_prices[counter]=price;
        }else if(strcmp(r_type, "Special")==0){
            r_prices[counter]=price;
        }
      //  printf("%f This is price", r_prices[counter]);
        counter++;
    }
    fclose(r_file);
    free(r_reader);
}
void get_price_per_stop(double* s_prices, char* filename){
    char* s_reader=NULL;
    FILE *s_file=fopen(filename, "r");
    size_t s_buffer=100;
    double price=0.0;
    int temp=0;
    int counter=0;
    if(s_file==NULL){
        fprintf(stderr, "Cannot open file: %s\n", filename);
        return;
    }
    s_reader=malloc(s_buffer*sizeof(char));
    while(getline(&s_reader, &s_buffer, s_file)!=-1){
        sscanf(s_reader, "%d %lf", &temp, &price);
        s_prices[counter]=price;
        counter++;
    }
    fclose(s_file);
    free(s_reader);

}
int get_num_lines(char* filename){
    int num_lines=0;
    char* l_reader=NULL;
    size_t l_buffer=100;
    FILE *l_file=fopen(filename, "r");
    if(l_file==NULL){
        fprintf(stderr, "Cannot open file: %s\n", filename);
        return 0;
    }
    l_reader=malloc(l_buffer*sizeof(char));
    while(getline(&l_reader, &l_buffer, l_file)!=-1){
        num_lines++;
    }

    fclose(l_file);
    free(l_reader);
    return num_lines;
}
