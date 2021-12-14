#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

#define aaa 13 //入力する文字数

//拡張ユークリッド互除法で逆元を求める
long long extEuqlid(long long a,long long b,long long* x){

    long long a_mod_b, t, q;
    a_mod_b = a%b;

    if (a_mod_b != 0){
        q=a/b;
        t=x[0]-q*x[1];
        x[0]=x[1];
        x[1]=t;

        return extEuqlid(b,a_mod_b,x);
    }
    return x[1];

}

//最大公約数
long long gcd(long long a,long long b){
    if(b==0){
        return a;
    }
    return gcd(b,a%b);
}

//最小公倍数
long long lcm(long long a,long long b){
    return a*b / gcd(a,b);
}

//素数判定
int primeNum(){
    int num,i,flg=0;
    while(flg == 0){
        num=rand() % 100000;
        for(i =2; i<=sqrt(num);i++){
            if(num % i == 0){
                flg = 0;
                break;
            }
            flg = 1;
        }
    }
    return num;
}

//べき剰余計算
long long powmod(long long x,long long k,long long m){
    if(k == 0){
        return 1;
    }
    if( k%2 == 0){
        return powmod(x*x %m, k/2, m);
    }
    return x*powmod(x, k-1, m) % m;
}

int main(void){
    srand((unsigned)time(NULL));

    long long x[2]={1,0};

    long long p,q;
    do {
        p = primeNum();
        q = primeNum();
    } while( p*q>3037000500 || p==q); //p*q=N>3037000500(N^2<9223372036854775807=long long型)のときはオーバーフロー発生

    if(p < q) {
        long long tmp = p;
        p = q;
        q = tmp;
    }

    printf("P : %lld   Q : %lld\n",p,q);

     //公開鍵 n
    long long n = p * q;

    //最小公倍数 l
    long long l = lcm(p-1,q-1);
    //long long l=(p-1)*(q-1);

    //公開鍵 e
    long long e= 65537;
    while(gcd(l, e) != 1) {
        e++;
    }

    //秘密鍵 d (e^-1 mod l)

    long long d=extEuqlid(e,l,x);

    printf("D first: %lld\n",d);

    if(d<0){
        d+=l;
    }


    printf("N : %lld   L : %lld   E : %lld   D : %lld\n",n,l,e,d);

    //平文

    char hirabun[aaa+1]; 
    printf("plain（平文) : ");
    scanf("%s", hirabun);

    printf("plain_num : ");
    for(int i=0 ;i<aaa ;i++ ){
        printf("%d ",hirabun[i]);
    }
    printf("\n\n");


    //暗号化

    long long encrypted[aaa];
    for (int i=0; i<aaa; i++){
        encrypted[i]=powmod(hirabun[i],e,n);
    }

    printf("encrypted_num: ");
    for (int i=0; i<aaa; i++){

        printf("%lld ",encrypted[i]);

    }
    printf("\n");

    printf("encrypted(暗号文): ");
    for (int i=0; i<aaa; i++){
        unsigned int encrypted_1=encrypted[i];
        printf("%c ",encrypted_1);
    }
    printf("\n\n");



    //復号化

    long long decrypted[aaa];
    for(int i=0; i<aaa; i++){
        decrypted[i]=powmod(encrypted[i],d,n); 
    }

    printf("decrypted_num: ");

    for(int i=0; i<aaa; i++){

        printf("%lld ",decrypted[i]);
    }
    printf("\n");

    printf("decrypted(復号文): ");

    for(int i=0; i<aaa; i++){
        unsigned int decrypted_1=decrypted[i];
        printf("%c",decrypted_1);
    }
    printf("\n");

    return 0;

}