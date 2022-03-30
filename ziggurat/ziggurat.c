#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <random>

#define N (256)



struct ziggurat_generator
{
    float* x;
    short* x_short;
    unsigned short* x_ratio_short;
    unsigned short lfsr_0;
    unsigned short lfsr_1;
};


/* 
 * uniform random number generator lfsr
 */
void lfsr_init(struct ziggurat_generator* ptr_zig)
{
    // ptr_zig->lfsr_0 = 0xACE1u;
    // ptr_zig->lfsr_1 = 0xACE1u + 0xACE1u;
    
    ptr_zig->lfsr_0 = 0x3edfu;
    ptr_zig->lfsr_1 = 0x5d79u;
    // ptr_zig->lfsr_0 = 0xe7e5u;
    // ptr_zig->lfsr_1 = 0xf8b0u;
}

void lfsr_deinit(struct ziggurat_generator* ptr_zig)
{
    return;
}

unsigned short lfsr_generate_0(struct ziggurat_generator* ptr_zig)
{
    unsigned char bit = ((ptr_zig->lfsr_0 >> 0) ^ (ptr_zig->lfsr_0 >> 2) ^ (ptr_zig->lfsr_0 >> 3) ^ (ptr_zig->lfsr_0 >> 5)) & 1u;
    ptr_zig->lfsr_0 = (ptr_zig->lfsr_0 >> 1) | (bit << 15);
    return ptr_zig->lfsr_0;
}
unsigned short lfsr_generate_1(struct ziggurat_generator* ptr_zig)
{
    unsigned short bit = ((ptr_zig->lfsr_1 >> 0) ^ (ptr_zig->lfsr_1 >> 1) ^ (ptr_zig->lfsr_1 >> 3) ^ (ptr_zig->lfsr_1 >> 11)) & 1u;
    ptr_zig->lfsr_1 = (ptr_zig->lfsr_1 >> 1) | (bit << 15);
    return ptr_zig->lfsr_1;
}
void lfsr_shuffle(struct ziggurat_generator* ptr_zig)
{
    unsigned short temp = ptr_zig->lfsr_1;
    ptr_zig->lfsr_1 = (ptr_zig->lfsr_0) + ptr_zig->lfsr_0;
    ptr_zig->lfsr_0 = temp - ptr_zig->lfsr_0;
    return;
}



/* 
 * gaussian random number generator ziggurat
 */
void ziggurat_init(struct ziggurat_generator* ptr_zig)
{
    ptr_zig->x = (float*) malloc(N * sizeof(float));
    ptr_zig->x_short = (short*) malloc(N * sizeof(short));
    ptr_zig->x_ratio_short = (unsigned short*) malloc(N * sizeof(unsigned short));
    
    FILE* fp = fopen("x.bin", "rb");

    for (int i = 0; i < N; i++)
    {
        fread(ptr_zig->x + i, 1, sizeof(float), fp);
        printf("x[%d] is %f\n", i, ptr_zig->x[i]);
        ptr_zig->x_short[i] = ptr_zig->x[i] / 8 * 32768;
    }
    for (int i = 1; i < N; i++)
    {
        ptr_zig->x_ratio_short[i] = (ptr_zig->x[i - 1] / ptr_zig->x[i]) * 65536;
    }

    lfsr_init(ptr_zig);
}

void ziggurat_deinit(struct ziggurat_generator* ptr_zig)
{
    lfsr_deinit(ptr_zig);
    free(ptr_zig->x);
}

float ziggurat_generate(struct ziggurat_generator* ptr_zig)
{
    static int cnt = 0;
    while(1)
    {
        cnt++;
        if (cnt > 32768)
        {
            lfsr_shuffle(ptr_zig);
            cnt = 0;
        }

        unsigned short u1 = lfsr_generate_0(ptr_zig);
        unsigned char i = u1 & 0x00ff;
        // printf("i is %d\n", i);
        if (0 == i) {
            continue;
        }

        float u2 = lfsr_generate_1(ptr_zig);

        // float u1 = random() / (float)(RAND_MAX);
        // float u2 = random() / (float)(RAND_MAX) * 2 - 1;

        // static std::random_device rd;
        // static std::default_random_engine eng(rd());
        // static std::uniform_real_distribution<> distr(0.0f, 1.0f);
        // float u1 = distr(eng);
        // float u2 = distr(eng) * 2 - 1;

        // printf("%f\n", u1);

        // float x = ptr_zig->x[i] * u2;
        
        // if (fabs(u2) < ptr_zig->x[i - 1] / ptr_zig->x[i])
        // {
        //     if (((u1>>14) & 1u))
        //     {
        //         x = -x;
        //     }
        //     return x;
        // }

        float x = ptr_zig->x[i] * u2 / 65536.0;
        
        if (u2 < ptr_zig->x_ratio_short[i])
        {
            if (((u1>>14) & 1u))
            {
                x = -x;
            }
            return x;
            return x;
        }
    }
}



/* 
 * sample code
 */
int main()
{
    srandom(0);
    // printf("hello_world\n");

    struct ziggurat_generator* ptr_zig = 0;

    ptr_zig = (struct ziggurat_generator*) malloc(sizeof(struct ziggurat_generator));

    ziggurat_init(ptr_zig);


    FILE* fp = fopen("random.bin", "wb");

    for (int i = 0; i < 0; i++)
    {
        float grng = ziggurat_generate(ptr_zig);
        // fwrite(&grng, sizeof(float), 1, fp);
        // printf("grng is %f\n", grng);
    }

    printf("lfsr1 is %x, lfsr2 is %x\n", ptr_zig->lfsr_0, ptr_zig->lfsr_1);

    for (int i = 0; i < 3000000; i++)
    {
        float grng = ziggurat_generate(ptr_zig);
        fwrite(&grng, sizeof(float), 1, fp);
        // printf("grng is %f\n", grng);
    }

    fclose(fp);

    
    
    
    ziggurat_deinit(ptr_zig);

    free(ptr_zig);
}