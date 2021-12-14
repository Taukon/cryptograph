#include<gmp.h>
#include<stdio.h>
#include<time.h>

#define aaa 10   //入力する文字数
#define Number 1024 //乱数の上限を決める

int main(void){
    mpz_t p,q;
    mpz_init(p);
    mpz_init(q);

    mpz_t rop;
    // 乱数のステート 
    gmp_randstate_t state;

    // 初期化して値を 0 にする
    mpz_init(rop);

    // 速度とランダムさのバランスが良い乱数アルゴリズム 
    gmp_randinit_default(state);

    // 現在の時刻をシードとして使う 
    gmp_randseed_ui(state, (unsigned long)time(NULL));

    // 0 から 2^Number-1 の範囲で乱数を生成 
    mpz_urandomb(rop, state, Number);

    mpz_nextprime(p,rop);   //rop以上の素数pを生成

    mpz_urandomb(rop, state, Number);
    mpz_nextprime(q,rop);    //rop以上の素数qを生成

    gmp_printf ("P: %Zd\n\n", p);
    gmp_printf ("Q: %Zd\n\n", q);


    mpz_t n,pq,d,l,p_1,q_1;

    //公開鍵N
    mpz_init(n);
    mpz_init(pq);
    mpz_mul(pq,p,q);
    mpz_mul(n,pq,p);
    gmp_printf ("N: %Zd\n\n", n);



    //p-1とq-1の最小公倍数
    mpz_init(p_1);
    mpz_init(q_1);
    mpz_init(l);
    mpz_sub_ui(p_1,p,1);
    gmp_printf("p_1: %Zd\n", p_1);
    mpz_sub_ui(q_1,q,1);
    gmp_printf("q_1: %Zd\n",q_1);
    mpz_lcm(l,p_1,q_1);

    gmp_printf ("l [lcm((p-1)(q-1))]: %Zd\n\n", l);

    //秘密鍵d
    mpz_init(d);
    mpz_invert(d,n,l);
    gmp_printf ("D: %Zd\n\n", d);


    //平文

    char hirabun[aaa+1]; 
    printf("plain（平文) : ");
    scanf("%s", hirabun);

    mpz_t m[aaa];
    printf("plain_num: {");
    for(int i=0; i<aaa; i++){
      mpz_init_set_si(m[i],hirabun[i]);
      gmp_printf("%Zd  ",m[i]);
    }
    printf("}\n\n");


    //暗号化

    mpz_t encrypted[aaa];

    printf("encrypted_num: {");
    for(int i=0; i<aaa; i++){
        mpz_init(encrypted[i]);
        mpz_powm(encrypted[i],m[i],n,n);
        gmp_printf("%Zd  ",encrypted[i]);
        mpz_clear(m[i]);    //m[i]のメモリ開放

    }
    printf("}\n\n");

    //復号化

    mpz_t decrypted[aaa];
    printf("decrypted_num: {");
    for(int i=0; i<aaa; i++){
        mpz_init(decrypted[i]);
        mpz_powm(decrypted[i],encrypted[i],d,pq);
        gmp_printf("%Zd  ",decrypted[i]);
        mpz_clear(encrypted[i]);    //crypetd[i]のメモリ開放
    }
    printf("}\n\n");

    printf("復号文: ");
    for (int i=0; i<aaa; i++){
        unsigned int ans = mpz_get_ui(decrypted[i]);
        printf("%c",ans);
        mpz_clear(decrypted[i]);    //derypetd[i]のメモリ開放
    }
    printf("\n");

    //p,q,rop,n,pp,d,l,p_1,q_1,stateのメモリ開放
    mpz_clears(p,q,rop,n,pq,d,l,p_1,q_1,NULL);
    gmp_randclear(state);

    return 0;
}