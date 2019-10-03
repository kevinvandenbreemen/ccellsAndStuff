//  Math lib wrapper for simple linalg etc calculations.  Any imports etc for mathematical functions
//  should be carried out in this file and nowhere else!

#ifndef KMATH_IN
#define KMATH_IN

double * kmath_linalg_scalar_times_vec(double * vector, int size, double scalar);

double * kmath_linalg_scalar_times_vec(double * vector, int size, double scalar) {

    double * ret = malloc(sizeof(double) * size);
    int i;
    for(i=0; i<size; i++) {
        ret[i] = vector[i] * scalar;
    }

    return ret;

}

#endif