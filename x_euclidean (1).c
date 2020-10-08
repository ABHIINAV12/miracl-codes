#include <stdio.h>
#include "miracl.h"
#include <time.h>

big gcd(big a, big b){
	big c=mirvar(0);

	if(!mr_compare(a,c))
		return b;

	divide(b,a,c);

	return gcd(b,a);
}

big extendedEuclidean(big a,big b,big x,big y){
	big c=mirvar(0);

	if(!mr_compare(a,c)){
		copy(mirvar(0),x);
		copy(mirvar(1),y);
		return b;
	}

	big x1=mirvar(0),y1=mirvar(0);
	big b2=mirvar(0);

	copy(b,b2);
	divide(b2,a,c);

	big g=extendedEuclidean(b2,a,x1,y1);

	multiply(c,x1,c);
	subtract(y1,c,x);

	copy(x1,y);

	return g;
}

int main(){

	big a,b,c,d;
	
	miracl *mip=mirsys(5000,10);

	a=mirvar(0);
	b=mirvar(0);
	c=mirvar(0);
	d=mirvar(1);

	long seed = time(0);
	irand(seed);

	bigbits(512,a);
	bigbits(512,b);

	printf("a = ");
	otnum(a,stdout);
	printf("b = ");
	otnum(b,stdout);

	big a2=mirvar(0),b2=mirvar(0);
	copy(a,a2);
	copy(b,b2);
	c=gcd(a2,b2);

	printf("gcd = ");
	otnum(c,stdout);
	printf("\n");

	if(mr_compare(d,c)){
		printf("gcd is not 1\n");
		return 0;
	}

	big x=mirvar(0),y=mirvar(0);

	c=extendedEuclidean(a,b,x,y);

	otnum(a,stdout);printf("*");otnum(x,stdout);printf(" + ");otnum(b,stdout);printf("*");otnum(y,stdout);printf(" = ");otnum(c,stdout);printf("\n");


	divide(x,b,c);

	add(x,b,x);
	divide(x,b,c);

	printf("(1/a)mod b = \n" );
	otnum(x,stdout);

}