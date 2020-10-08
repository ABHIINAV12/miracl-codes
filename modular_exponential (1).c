#include<stdio.h>
#include "miracl.h"
#include<time.h>
big modexp(big c,big d,big n)
{
    big zero=mirvar(0);
    big ans=mirvar(1);
    divide(c,n,n);
    if(mr_compare(c,zero)==0)
    {
        return ans;
    }
    while(mr_compare(d,zero)>0)
    {
        big temp=d;
        divide(temp,mirvar(2),mirvar(2));
        if(mr_compare(temp,zero)==0)
        {
            divide(d,mirvar(2),d);
            multiply(c,c,c);
            divide(c,n,n);
        }
        else
        {
            subtract(d,mirvar(1),d);
            multiply(ans,c,ans);
            divide(ans,n,n);
        }
    }
    divide(ans,n,n);
    return ans;
}
int main()
{
    big a,b,n;
    miracl *mip=mirsys(500,10);
    long seed = time(0);
	irand(seed);
    a=mirvar(0);
	b=mirvar(0);
	n=mirvar(0);
    bigbits(512,a);
    bigbits(512,b);
    bigbits(512,n);
    printf("A = ");
    otnum(a,stdout);
    printf("\nB = ");
    otnum(b,stdout);
    printf("\nN = ");
    otnum(n,stdout);
    big c,d;
    c=mirvar(0);
	d=mirvar(0);
	copy(a,c);
    copy(b,d);
    big ans=mirvar(0);
    ans=modexp(c,d,n);
    printf("\n(A ^ B) % N = ");
    otnum(ans,stdout);
    return 0;
}