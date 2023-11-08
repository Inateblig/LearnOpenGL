/* util.h */

#define NELM(X) (sizeof (X) / sizeof (X)[0])

#define DO2(A, B)\
	do {\
		A;\
		B;\
	} while(0)
