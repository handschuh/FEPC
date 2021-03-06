/*
 * FEPC
 * Copyright (C) 2010 Stefan Handschuh (handschu@mis.mpg.de)
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
 
#include "interval.h"


interval_p 
interval_new(int dimension) {
    interval_p result = (interval_p) malloc(sizeof(interval_t));
    
    interval_init(result, dimension);
    return result;    
}

void
interval_init(interval_p interval, int dimension) {
	interval->dimension = dimension;
	interval->start = (fepc_real_t*) malloc(sizeof(fepc_real_t)*dimension);
	interval->end = (fepc_real_t*) malloc(sizeof(fepc_real_t)*dimension);

	int n;

	for (n = 0; n < dimension; n++) {
		interval->start[n] = 0.;
	}
}

void interval_del(interval_p interval) {
    interval_clear(interval);
    free(interval);
}

void
interval_clear(interval_t * interval) {
	free(interval->start);
	free(interval->end);
}

void intervals_del(interval_p* intervals, int interval_count) {
    int n;
    
    for (n = 0; n < interval_count; n++) {
        interval_del(intervals[n]);
    }
    free(intervals);
}

void intervals_del_type(interval_t * intervals, int interval_count) {
    int n;

    for (n = 0; n < interval_count; n++) {
        interval_clear(&intervals[n]);
    }
    free(intervals);
}

void print_interval(interval_p interval) {
    int n;
    
    for (n = 0; n < interval->dimension; n++) {
        printf("%f\t<\t%f\n", interval->start[n], interval->end[n]);
    } 
}

void print_intervals(interval_p * intervals, int count) {
    int n;
    
    for (n = 0; n < count; n++) {
        printf("\nInterval %i\n", n+1);
        print_interval(intervals[n]);
    }
}

interval_p interval_clone(interval_p interval) {
    int n, dimension;
    
    dimension = interval->dimension;
    interval_p result = interval_new(dimension);
    for (n = 0; n < dimension; n++) {
        result->start[n] = interval->start[n];
        result->end[n] = interval->end[n];
    }
    return result;
}

interval_p * intervals_clone(interval_p* intervals, int count) {
    interval_p* result = (interval_p*) malloc(sizeof(interval_p)*count);
    
    int n;
    
    for (n = 0; n < count; n++) {
        result[n] = interval_clone(intervals[n]);
    }
    return result;
}


