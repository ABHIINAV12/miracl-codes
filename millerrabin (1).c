#include <stdio.h>
#include <stdlib.h>
#include "miracl.h"
#include <time.h>
#include <stdbool.h>
int iter;


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
		iter++;
		return true;
	}

	while(mr_compare(d2,b)){
		iter++;
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

big findPrime(){
	miracl *mip=mirsys(5000,10);
	big a;
	long seed = time(0);
	irand(seed);
	while(1){

		a=mirvar(0);
		bigbits(512,a);
		iter=0;
		if(checkPrimality(a,20)){
			printf("\nFound Probable Prime = ");
			otnum(a,stdout);
			return a;
		}
		else{
			printf("\nIn %d iterations,Found Composite Number = ",iter);
			otnum(a,stdout);			
		}
		mr_free(a);
	}
}

int main(){
	findPrime();
}