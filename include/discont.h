/*
 * FEPC
 * Copyright (C) 2010, 2011 Stefan Handschuh (handschu@mis.mpg.de)
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#ifndef _DISCONT
#define _DISCONT

#define ONE_THIRD 0.333333333333333333333333333333333333333333333333333333333333333333333
#define TWO_THIRD 0.666666666666666666666666666666666666666666666666666666666666666666666
#define SQRT_12 3.464101615137754587054892683011744733885610507620761256111613958903866

#include "fepc_easy.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    fepc_real_t y_0;
    
    fepc_real_t slope;

} linear_function_t;

typedef linear_function_t * linear_function_p;


typedef struct {
    int count;
    
    linear_function_p * functions;
} linear_function_set_t; 

typedef linear_function_set_t * linear_function_set_p;

typedef struct {    
    int stepcount;
    
    interval_p * intervals;
    
    linear_function_set_p * function_sets;

} discont_function_t;

typedef discont_function_t * discont_function_p;


linear_function_p
linear_function_new(fepc_real_t x_0, fepc_real_t slope);

linear_function_p
linear_function_new_points(fepc_real_t y_0, fepc_real_t y_1, fepc_real_t x_0, fepc_real_t x_1);

void
discont_function_del(discont_function_p function);

void
linear_function_set_del(linear_function_set_p function_set);

linear_function_set_p
linear_function_set_new(int count);

discont_function_p
discont_function_new(int stepcount);

func_p 
convert_discont_function(discont_function_p function, fepc_real_t stepping);

discont_function_p
convert_func(func_p function, interval_p * intervals, fepc_real_t stepping);

void
discont_function_print(discont_function_p function);

void
linear_function_set_print(linear_function_set_p function_set);

fepc_real_t 
integrate_coeff_discont(discont_function_p function, int v, int position, int p, int step, fepc_real_t stepping);

void 
add_folgenentries_discont(func_p function, discont_function_p discont_function, fepc_real_t stepping);

void
discont_function_setup(discont_function_p function, int step, fepc_real_t start, fepc_real_t end, linear_function_set_p function_set);

void
discont_function_setup_points(discont_function_p function, int step, fepc_real_t start, fepc_real_t end, fepc_real_t * y1, fepc_real_t * y2, fepc_real_t stepping);

#ifdef __cplusplus
}
#endif

#endif
