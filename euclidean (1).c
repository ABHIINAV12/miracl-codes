#include<stdio.h>
#include "miracl.h"
#include<time.h>
big gcd(big c,big d)
{
    big zero=mirvar(0);
    if(mr_compare(d,zero)==0)
    {
        return c;
    }
    divide(c,d,d);
    return gcd(d,c);
}
int main()
{
    big a,b,g;
    miracl *mip=mirsys(500,10);
    long seed = time(0);
	irand(seed);
    a=mirvar(0);
	b=mirvar(0);
	g=mirvar(0);
    bigbits(512,a);
    bigbits(512,b);
    printf("A = ");
    otnum(a,stdout);
    printf("\nB = ");
    otnum(b,stdout);
    big c,d;
    c=mirvar(0);
	d=mirvar(0);
	copy(a,c);
    copy(b,d);
    g=gcd(c,d);
    printf("\ngcd(A,B) = ");
    otnum(g,stdout);
    return 0;
}