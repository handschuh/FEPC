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
 
#include "fepc_easy_helper.h"
#include "fepc_easy.h"


func_p
func_multi(func_p f, func_p g, fepc_real_t stepping) {
	func_p result = func_new(f->maxlevel > g->maxlevel ? f->maxlevel : g->maxlevel, f->dim);

	func_multi_overwrite(result, f, g, stepping);
    return result;
}

void
func_multi_overwrite(func_t * result, func_p f, func_p g, fepc_real_t stepping) {
	ASSERT(f != NULL && g != NULL);
	ASSERT(f->dim == g->dim);

	folgen_vektor_p  temp;

	int n, k, l, m, steps;

	steps = (f->maxlevel > g->maxlevel ? f->maxlevel : g->maxlevel)+1;

	ASSERT(result->maxlevel == steps -1);
	ASSERT(result->dim == f->dim);

	for (n = 0; n < steps; n++) {
	    temp = folgen_vektor_multi( f->maxlevel < n ? NULL : f->hierarchie[n], g->maxlevel < n ? NULL : g->hierarchie[n], n, stepping );
	    folgen_vektor_del(result->hierarchie[n]);
		result->hierarchie[n] = temp;
	}
}

func_p
func_div(func_p f, func_p g, fepc_real_t stepping) {
    ASSERT(f != NULL && g != NULL);
    ASSERT(f->dim == g->dim);
    
    folgen_vektor_p  temp;
    
    int n, k, l, m, steps;
    
    steps = (f->maxlevel > g->maxlevel ? f->maxlevel : g->maxlevel)+1;
    
    func_p result = func_new(steps-1, f->dim);
    
    for (n = 0; n < steps; n++) {
        temp = folgen_vektor_div( f->maxlevel < n ? NULL : f->hierarchie[n], g->maxlevel < n ? NULL : g->hierarchie[n], n, stepping );
		folgen_vektor_del(result->hierarchie[n]);
		result->hierarchie[n] = temp;
    }
    return result;
}


folgen_vektor_p
folgen_vektor_multi(folgen_vektor_p f, folgen_vektor_p g, int step, fepc_real_t stepping) {
    folgen_vektor_p  back;
	int  k, d, size, dim, a, b, test_f, test_g;
	vec_p  max, vec_1, n, n_f, n_g, r;

	ASSERT( f->grad->dim == g->grad->dim );
	dim = f->grad->dim;
	max = vec_max( f->grad, g->grad );


	vec_1 = vec_one( dim );
	n = vec_add( vec_1, max );
	n_f = vec_add( vec_1, f->grad );
	n_g = vec_add( vec_1, g->grad );
	size = vec_size( n );

	back = folgen_vektor_new( max );
	for(k=0;k<size;k++) {
		r = entry_one2d( k, n );
		test_f = 0;
		test_g = 0;
		for(d=0;d<dim;d++) {
			if( r->array[d] > f->grad->array[d] ) {
				test_f = test_f + 1;
			}
			if( r->array[d] > g->grad->array[d] ) {
				test_g = test_g + 1;
			}
		}
		if( (test_f == 0) && (test_g == 0) ) {
			a = entry_d2one( r, n_f );
			b = entry_d2one( r, n_g );
			folge_del( back->vektor[k] );
			back->vektor[k] = folge_multi( f->vektor[a], g->vektor[b], step, stepping );
		}
		if( (test_f != 0) && (test_g == 0) ) {
			b = entry_d2one( r, n_g );
			folge_del( back->vektor[k] );
			back->vektor[k] = folge_copy( g->vektor[b] );
		}
		if( (test_f == 0) && (test_g != 0) ) {
			a = entry_d2one( r, n_f );
			folge_del( back->vektor[k] );
			back->vektor[k] = folge_copy( f->vektor[a] );
		}
		vec_del( r );
	}
	vec_del( n );
	vec_del( n_f );
	vec_del( n_g );
	vec_del( vec_1 );

	return back;
}


folgen_vektor_p
folgen_vektor_div(folgen_vektor_p f, folgen_vektor_p g, int step, fepc_real_t stepping) {
    folgen_vektor_p  back;
	int  k, d, size, dim, a, b, test_f, test_g;
	vec_p  max, vec_1, n, n_f, n_g, r;

	ASSERT( f->grad->dim == g->grad->dim );
	dim = f->grad->dim;
	max = vec_max( f->grad, g->grad );


	vec_1 = vec_one( dim );
	n = vec_add( vec_1, max );
	n_f = vec_add( vec_1, f->grad );
	n_g = vec_add( vec_1, g->grad );
	size = vec_size( n );

	back = folgen_vektor_new( max );
	for(k=0;k<size;k++) {
		r = entry_one2d( k, n );
		test_f = 0;
		test_g = 0;
		for(d=0;d<dim;d++) {
			if( r->array[d] > f->grad->array[d] ) {
				test_f = test_f + 1;
			}
			if( r->array[d] > g->grad->array[d] ) {
				test_g = test_g + 1;
			}
		}
		if( (test_f == 0) && (test_g == 0) ) {
			a = entry_d2one( r, n_f );
			b = entry_d2one( r, n_g );
			folge_del( back->vektor[k] );
			back->vektor[k] = folge_div( f->vektor[a], g->vektor[b], step, stepping );
		}
		if( (test_f != 0) && (test_g == 0) ) {
			b = entry_d2one( r, n_g );
			folge_del( back->vektor[k] );
			back->vektor[k] = folge_copy( g->vektor[b] );
		}
		if( (test_f == 0) && (test_g != 0) ) {
			a = entry_d2one( r, n_f );
			folge_del( back->vektor[k] );
			back->vektor[k] = folge_copy( f->vektor[a] );
		}
		vec_del( r );
	}
	vec_del( n );
	vec_del( n_f );
	vec_del( n_g );
	vec_del( vec_1 );

	return back;
}


fepc_real_t folge_norm2(folge_p folge, int step, fepc_real_t stepping) {
    int n, interval_element_count, position, k;
    
    vec_p v, p;
    
    vec_real_p x = vec_real_new(folge->start->dim);
    
    p = vec_new(folge->start->dim);
    
    fepc_real_t result, h_l;
    
    result = 0.;
    h_l = get_h_l(step, stepping);
    
    for (n = 0, interval_element_count = get_interval_element_count(folge); n < interval_element_count; n++) {
        v = generate_folgenvector(folge, n);
        position = entry_d2one(v, folge->lang);
        vec_add2(v, folge->start);
        for (k = 0; k < v->dim; k++) {
            x->array[k] = v->array[k]*h_l; // dummy values because only p = 0 is supported
        }
        result = phi_l(step, v, p, x, stepping);
        //printf("Result %f\n", result);
        vec_del(v);
    }
    vec_del(p);
    vec_real_del(x);
    return result;
}


folge_p
folge_multi(folge_p f, folge_p g, int step, fepc_real_t stepping) {
	folge_p  back;
	int  size, k, size_g, size_f;
	fepc_real_t  x, y;
	vec_p  temp1, temp2, max, lang, min, r;


	size_f = vec_size( f->lang );
	size_g = vec_size( g->lang );
	if(size_f == 0) {
		back = folge_copy( g );
		return back;
	}

	if(size_g == 0) {
		back = folge_copy( f );
		return back;
	}

	if( (size_g!=0) && (size_f!=0) ) {
		min = vec_min( f->start, f->start );
		temp1 = vec_add( f->start, f->lang );
		temp2 = vec_add( f->start, f->lang );
		max = vec_max( temp1, temp2 );
		vec_del( temp1 );
		vec_del( temp2 );
		lang = vec_op( 1, max, -1, min);
		vec_del( max );
		back = folge_new( min, lang );
		size = vec_size( lang );
		for(k=0;k<size;k++) {
			temp1 = entry_one2d( k, lang );
			r = vec_add( min, temp1 );
			vec_del( temp1 );
			x = folge_glied( r, f );
			y = folge_glied( r, g );
			//print_vec(r);
			vec_del( r );
			back->glied[k] = x*y*folge_norm2(g, step, stepping);
			//printf("%i\t%f\t%f\n", step, x, y);
		}
		return back;
	}
}

folge_p
folge_div(folge_p f, folge_p g, int step, fepc_real_t stepping) {
	folge_p  back;
	int  size, k, size_g, size_f;
	fepc_real_t  x, y;
	vec_p  temp1, temp2, max, lang, min, r;


	size_f = vec_size( f->lang );
	size_g = vec_size( g->lang );
	if(size_f == 0) {
		back = folge_copy( g );
		return back;
	}

	if(size_g == 0) {
		back = folge_copy( f );
		return back;
	}

	if( (size_g!=0) && (size_f!=0) ) {
		min = vec_min( f->start, g->start );
		temp1 = vec_add( f->start, f->lang );
		temp2 = vec_add( f->start, f->lang );
		max = vec_max( temp1, temp2 );
		vec_del( temp1 );
		vec_del( temp2 );
		lang = vec_op( 1, max, -1, min);
		vec_del( max );
		back = folge_new( min, lang );
		size = vec_size( lang );
		for(k=0;k<size;k++) {
			temp1 = entry_one2d( k, lang );
			r = vec_add( min, temp1 );
			vec_del( temp1 );
			x = folge_glied( r, f );
			y = folge_glied( r, g );
			vec_del( r );
			back->glied[k] = x == 0. ? 0. : x/(y*folge_norm2(g, step, stepping));
		}
		return back;
	}
}


vec_real_p get_mean_points(vec_p v, vec_p grad, int step, fepc_real_t stepping) {
    fepc_real_t h_l = get_h_l(step, stepping);
    
    vec_real_p result = vec_real_new(v->dim);

    int n;
    
    for (n = 0; n < v->dim; n++) {
        result->array[n] = (v->array[n] + .5)*h_l;
    }
    return result;
}

folgen_vektor_p
folgen_vektor_reflect(folgen_vektor_p f, int step) {
    folgen_vektor_p  result = folgen_vektor_copy_structure(f);
	
	fepc_real_t temp;
	
	vec_p r;
	
	int n, k, degree_count, interval_element_count, position;
	
	degree_count = get_degree_count(f->grad);
    interval_element_count = get_interval_element_count(f->vektor[0]); // since they have all the same element-count
    
    for (n = 0; n < interval_element_count / 2; n++) {
        for (k = 0; k < degree_count; k++) {
            r = generate_folgenvector(f->vektor[k], n);
            position = entry_d2one(r, f->vektor[k]->lang);
            result->vektor[k]->glied[position] = f->vektor[k]->glied[interval_element_count - position - 1];
            result->vektor[k]->glied[interval_element_count - position - 1] = f->vektor[k]->glied[position];
            vec_del(r);
        }
    }
    return result;
}


folgen_vektor_p
folgen_vektor_multi_factor(folgen_vektor_p f, fepc_real_t factor, int step) {
    folgen_vektor_p  result = folgen_vektor_copy_structure(f);
	
	
	vec_p r;
	
	int n, k, degree_count, interval_element_count, position;
	
	degree_count = get_degree_count(f->grad);
    interval_element_count = get_interval_element_count(f->vektor[0]); // since they have all the same element-count
    
    for (n = 0; n < interval_element_count; n++) {
        for (k = 0; k < degree_count; k++) {
            r = generate_folgenvector(f->vektor[k], n);
            position = entry_d2one(r, f->vektor[k]->lang);
            result->vektor[k]->glied[position] = factor*f->vektor[k]->glied[position]; // TODO maybe this factor needs to be adjusted
            vec_del(r);
        }
    }
    return result;
}

folgen_vektor_p
folgen_vektor_reflect_x(folgen_vektor_p f, int step) {
    return folgen_vektor_multi_factor(f, -1., step);
}




func_p
func_reflect(func_p f) {
    func_p result = func_new(f->maxlevel, f->dim);
    
    int n;
    
    folgen_vektor_p temp;
    
    for (n = 0; n <= result->maxlevel; n++) {
        temp = folgen_vektor_reflect(f->hierarchie[n], n);
		folgen_vektor_del(result->hierarchie[n]);
		result->hierarchie[n] = temp;
    }
    return result;
}

func_p
func_reflect_x(func_p f) {
    func_p result = func_new(f->maxlevel, f->dim);
    
    int n;
    
    folgen_vektor_p temp;
    
    for (n = 0; n <= result->maxlevel; n++) {
        temp = folgen_vektor_reflect_x(f->hierarchie[n], n);
		folgen_vektor_del(result->hierarchie[n]);
		result->hierarchie[n] = temp;
    }
    return result;
}

void
func_add2(func_p f, func_p g) {
	int  k, maxlevel, dim;
	folgen_vektor_p  temp;

	ASSERT( f->dim == g->dim );

	if (f->maxlevel > g->maxlevel) {
		maxlevel = g->maxlevel;
	}
	else {
		maxlevel = f->maxlevel;
	}

	for (k=0;k<=maxlevel;k++) {
		temp = folgen_vektor_add( f->hierarchie[k], g->hierarchie[k] );
		folgen_vektor_del( f->hierarchie[k] );
		f->hierarchie[k] = temp;
	}
}

void
func_add3(func_p f, fepc_real_t factor, func_p g) {
	int  k, maxlevel, dim;
	folgen_vektor_p  temp, temp2;

	ASSERT( f->dim == g->dim );

	if (f->maxlevel > g->maxlevel) {
		maxlevel = g->maxlevel;
	}
	else {
		maxlevel = f->maxlevel;
	}

	for (k=0;k<=maxlevel;k++) {
	    temp = folgen_vektor_multi_factor(g->hierarchie[k], factor, k);
		temp2 = folgen_vektor_add( f->hierarchie[k], temp);
		folgen_vektor_del(temp);
		folgen_vektor_del( f->hierarchie[k] );
		f->hierarchie[k] = temp2;
	}
}

func_p
func_subtract(func_p f, func_p g) {
	func_p  back;
	int  k, maxlevel, dim;
	folgen_vektor_p  temp, temp2;

	ASSERT( f->dim == g->dim );
	dim = f->dim;
	if (f->maxlevel > g->maxlevel) {
		maxlevel = g->maxlevel;
	}
	else {
		maxlevel = f->maxlevel;
	}


	back = func_new( maxlevel, dim );
	for (k=0;k<=maxlevel;k++) {
	    temp = folgen_vektor_multi_factor(g->hierarchie[k], -1., k);
		temp2 = folgen_vektor_add( f->hierarchie[k], temp);
		folgen_vektor_del(temp);
		folgen_vektor_del( f->hierarchie[k] );
		back->hierarchie[k] = temp2;
	}

	return back;
}


void
func_subtract2(func_p f, func_p g) {
    int  k, maxlevel, dim;
	folgen_vektor_p  temp;

	ASSERT( f->dim == g->dim );

	if (f->maxlevel > g->maxlevel) {
		maxlevel = g->maxlevel;
	}
	else {
		maxlevel = f->maxlevel;
	}

	for (k=0;k<=maxlevel;k++) {
		temp = folgen_vektor_subtract( f->hierarchie[k], g->hierarchie[k] );
		folgen_vektor_del( f->hierarchie[k] );
		f->hierarchie[k] = temp;
	}
}

fepc_real_t func_norm_l2_sqr(func_p function, fepc_real_t stepping) {
    func_p temp = func_multi(function, function, stepping);
    
    fepc_real_t result = func_integrate(temp, stepping);
    func_del(temp);

    return result;
}



