#include <stdio.h>
#include <stdlib.h>
#include "miracl.h"
#include <time.h>
#include <stdbool.h>

/*
    setup phase
    encryption phase
    decryption phase
*/



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
    if(mr_compare(c,zero)==0){
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
    bigbits(1024,rand);
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
        if(checkPrimality(a,5)){
            return a;
        }
        mr_free(a);
    }
}

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
    miracl *mip=mirsys(5000,10);
    big r1,ZERO=mirvar(0),n, e, b, a, c ;
    
    e=mirvar(30);
    n=mirvar(29);

    bigdig(180,10,e);
    bigdig(512,2,n);
    
    big prime_no=mirvar(0);
    prime_no = findPrime();

    big exponent = modexp(prime_no,e,n);
    printf("Number being Encrypted : ");
    otnum(exponent,stdout);
    
    c=mirvar(0);
    a=mirvar(0);
    b=mirvar(0);
    copy(prime_no,a);
    copy(exponent,b);
    
    printf("\nThe prime number is :");
    otnum(a,stdout);
    
    
    big x = mirvar(0),y=mirvar(0);
    c=extendedEuclidean(a,b,x,y);
    
    big p = mirvar(0),q = mirvar(0);
    copy(a,p);
    copy(b,q);
    
    big one = mirvar(1);
    if (mr_compare(c,one) != 0){
        printf("\nNo Inverse\n");
        return 0;
    }
    big X = mirvar(0);
    copy(x,X);
    divide(X,b,a);
    add(X,b,X);
    divide(X,b,a);
    printf("\nMultiplicative Inverse : ");
    otnum(X,stdout);
    divide(p,q,c);
    printf("\nFinal Answer : ");
    otnum(q,stdout);

    return 0;
}