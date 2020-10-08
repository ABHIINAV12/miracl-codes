#include <stdio.h>
#include <stdbool.h>
#include "miracl.h"
#include <time.h>

void modmult(big a, big b,big mod){
	big c=mirvar(0);
	big d=mirvar(0);
	multiply(a,b,c);
	divide(c,mod,d);
	copy(c,a);
	mr_free(d);
	mr_free(c);
}

big modexp(big x,big y,big n)
{
    big zero=mirvar(0);
    big ans=mirvar(1);
    big c=mirvar(0),d=mirvar(0);
    copy(x,c);
    copy(y,d);
    divide(c,n,n);
    if(mr_compare(c,zero)==0)
    {
        return ans;
    }
    while(mr_compare(d,zero)>0)
    {
        big temp=mirvar(0);
        copy(d,temp);
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
    mr_free(c);
    mr_free(d);
    mr_free(zero);
    return ans;
}


big gcd_with_sideEffects(big a, big b){
	big mir_ZERO = mirvar(0);

	if(!mr_compare(a,mir_ZERO))
		return b;

	divide(b,a,a);

	return gcd_with_sideEffects(b,a);
}


big gcd(big a, big b){

	big a1 = mirvar(0); copy(a, a1);
	big b1 = mirvar(0); copy(b, b1);

	return gcd_with_sideEffects(a1,b1);
}




big extendedEuclidean(big a,big b,big x,big y){
	big c = mirvar(0);

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



big get_inverse(big x, big mod)
{
	big DISCARD = mirvar(0);
	big inv_x = mirvar(0);

	extendedEuclidean(x, mod, inv_x,  DISCARD);

	divide(inv_x, mod, mod);// bring inv_x in range 
	add(inv_x, mod, inv_x); // if inv_x is negative integer convert it to positive integer
	divide(inv_x, mod, mod);// bring inv_x in range

	return inv_x;
}


bool are_coPrime(big a, big b)
{
	big gcd_val = mirvar(0);

	gcd_val = gcd(a, b);

	if(mr_compare(gcd_val , mirvar(1)) == 0)
		return true;
	else
		return false;
}

big get_toient(big p, big q)
{
	big p_minus_1 = mirvar(0);	subtract(p, mirvar(1), p_minus_1);
	big q_minus_1 = mirvar(0);	subtract(q, mirvar(1), q_minus_1);

	big toient = mirvar(0);

	multiply(p_minus_1, q_minus_1, toient);

	return toient;
}

bool millerTest(big n,big d){
	big c=mirvar(0);
	big b=mirvar(0);
	big one=mirvar(1);
	big temp;

	subtract(n,one,b); //b=n-1

	big n2=mirvar(0),d2=mirvar(0);
	copy(n,n2); //n2=n
	copy(d,d2); //d2=d

    //Finding rand number between [2,n-2]
	big rand=mirvar(0);
	bigbits(512,rand);
	temp=mirvar(5);
	subtract(n2,temp,n2); 
	mr_free(temp);
	divide(rand,n2,c);         
	temp=mirvar(2);
	add(rand,temp,rand);  
	mr_free(temp);
    //x= (rand^d)%n
	big x=mirvar(0);
	x=modexp(rand,d2,n);   

	if(!mr_compare(x,one) || !mr_compare(x,b)){ // ans==1 or n-1
		mr_free(c);
		mr_free(b);
		mr_free(one);
		mr_free(rand);
		mr_free(x);
		mr_free(n2);
		mr_free(d2);
		//iter++;
		return true;
	}

	while(mr_compare(d2,b)){
		//iter++;
		modmult(x,x,n);
		temp=mirvar(2);
		multiply(d2,temp,d2);
		mr_free(temp);

		if(!mr_compare(x,one)){
			mr_free(c);
			mr_free(b);
			mr_free(one);
			mr_free(rand);
			mr_free(x);
			mr_free(n2);
			mr_free(d2);
			return false;
		}
		if(!mr_compare(x,b)){
			mr_free(c);
			mr_free(b);
			mr_free(one);
			mr_free(rand);
			mr_free(x);
			mr_free(n2);
			mr_free(d2);
			return true;
		}
	}

	mr_free(c);
	mr_free(b);
	mr_free(one);
	mr_free(rand);
	mr_free(x);
	mr_free(n2);
	mr_free(d2);

	return false;
}

bool checkPrimality(big n,int k){
	big temp=mirvar(0);
	temp=mirvar(0);
	if(!subdiv(n,2,temp)) {   //n is even
		mr_free(temp);
		return false;
	}
	mr_free(temp);
	big d=mirvar(0),a=mirvar(0);
	temp=mirvar(1);
	subtract(n,temp,d);     
	mr_free(temp);
    //Finding d=(n-1)/(2^r)
	while(!subdiv(d,2,a)){
		copy(a,d);
	}						        
	for(int i=0;i<k;++i){
		if(millerTest(n,d)==0){
			mr_free(d);
			mr_free(a);
			return false;
		}
	}
	mr_free(d);
	mr_free(a);
	return true;
}

void get_two_primes(big p, big q)
{
	copy(mirvar(0), p);
	copy(mirvar(0), q);

	long seed = time(0);
	irand(seed);

	big temp = mirvar(0);

	do  { 
		bigbits(512, temp); 
	} 
	while(!checkPrimality(temp,5));

	copy(temp, p);

	seed = time(0);
	irand(seed);

	do  { 
		bigbits(512, temp); 
	} 
	while(!checkPrimality(temp,5) || (mr_compare(p,temp) == 0) );

	copy(temp, q);
}

int main(){

	
	
	miracl *mip=mirsys(5000,10);

	big p = mirvar(0);
	big q = mirvar(0);

	get_two_primes(p,q);

	printf("\n\n");
	printf("Private key p = ");
	otnum(p,stdout);
	if(checkPrimality(p,5))
		printf("\np is prime\n");


	printf("\n\n");
	printf("Private key q = ");
	otnum(q,stdout);
	if(checkPrimality(q,5))
		printf("\nq is prime\n");

	
	big n = mirvar(0);
	multiply(p, q, n);

	printf("\n\n");
	printf("Public key n = ");
	otnum(n,stdout);

	big x = mirvar(0);
	int public_key_in_int = (1<<16) + 1;
	convert(public_key_in_int, x);

	
	big mod = mirvar(0); mod = get_toient(p, q);

	

	printf("\n\n");
	printf("Public key x = ");
	otnum(x,stdout);

	printf("\n\n");
	printf("toient = ");
	otnum(mod,stdout);

	

	big gcd_val = mirvar(0);

	gcd_val = gcd(x, mod);

	printf("\n\n");
	printf("gcd of x and toient = ");
	otnum(gcd_val,stdout);

	if(!are_coPrime(x, mod))
	{
		printf("\nx amd toient are not coprimes \n");
		return 1;
	}

	big inv_x = mirvar(0);

	inv_x = get_inverse(x, mod);
	printf("\n\n");
	printf("inv_x = ");
	otnum(inv_x, stdout);


	big product = mirvar(0);

	multiply(x, inv_x, product);
	divide(product, mod, mod); //product = product % mod

	printf("\n\n");
	printf("product = ");
	otnum(product, stdout);

	


}