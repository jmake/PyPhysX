%module swig_example

%include "std_vector.i"
%include "std_string.i"

%template(vectori) std::vector<int>;
%template(vectorc) std::vector<std::string>;


%{
    #define SWIG_FILE_WITH_INIT
    #include "physx.hpp"
%}

## ShowArray(int* array, int narray)
%include "carrays.i"
%array_class(int, iArray)

## 
%include "numpy.i"
%init 
%{
    import_array();
%}
 
## GetArray(std::string key, FLOAT* array, int narray);
%apply (double* IN_ARRAY1, int DIM1) {(double* array, int narray)}
%apply (char *STRING, double* IN_ARRAY1, int DIM1) {(char *s, double* array, int narray)}

## GetKeys 
##%apply (char* IN_ARRAY1, int DIM1) {(char* array, int narray)}


%include "physx.hpp"
