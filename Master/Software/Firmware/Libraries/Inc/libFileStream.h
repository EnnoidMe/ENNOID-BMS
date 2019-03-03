#include <stdio.h>

struct __FILE {
	char (*outputFunctionPointer)(char, FILE *);
	int(*inputFunctionPointer)(FILE *);
};
