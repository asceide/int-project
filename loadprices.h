#ifndef LOADPRICES_H_
#define LOADPRICES_H_

void get_monthly_prices(double* m_prices, char* filename);
void get_discounts(double* d_prices, char* filename);
void get_ride_prices(double* r_prices, char* filename);
void get_price_per_stop(double* s_prices, char* filename);
int get_num_lines(char* filename);
#endif