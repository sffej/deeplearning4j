#include <scalar.h>

__device__ double op(double d1,double d2,double *params) {
   return d2 - d1;
}

extern "C"
__global__ void sub_scalar_double(int n, int idx,double dx,double *dy,int incx,double *params,double *result) {
       transform(n,idx,dx,dy,incx,params,result);
 }


