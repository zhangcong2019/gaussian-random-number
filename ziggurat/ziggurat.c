#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <random>

#define N (256)



struct ziggurat_generator
{
    float* x;
    unsigned char lfsr_0;
    unsigned short lfsr_1;
};


/* 
 * uniform random number generator lfsr
 */
void lfsr_init(struct ziggurat_generator* ptr_zig)
{
    // ptr_zig->lfsr_0 = 0xACE1u;
    // ptr_zig->lfsr_1 = 0xACE1u + 0xACE1u;
    
    ptr_zig->lfsr_0 = 0xb8u;
    ptr_zig->lfsr_1 = 0xd008u;
    // ptr_zig->lfsr_0 = 0xe7e5u;
    // ptr_zig->lfsr_1 = 0xf8b0u;
}

void lfsr_deinit(struct ziggurat_generator* ptr_zig)
{
    return;
}

unsigned short lfsr_generate_0(struct ziggurat_generator* ptr_zig)
{
    unsigned char bit = ((ptr_zig->lfsr_0 >> 0) ^ (ptr_zig->lfsr_0 >> 2) ^ (ptr_zig->lfsr_0 >> 3) ^ (ptr_zig->lfsr_0 >> 4)) & 1u;
    ptr_zig->lfsr_0 = (ptr_zig->lfsr_0 >> 1) | (bit << 7);
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
    ptr_zig->lfsr_1 = (ptr_zig->lfsr_0 << 8) + 0xACE1u;
    ptr_zig->lfsr_0 = temp - 0xACE1u;
    return;
}



/* 
 * gaussian random number generator ziggurat
 */
void ziggurat_init(struct ziggurat_generator* ptr_zig)
{
    ptr_zig->x = (float*) malloc(N * sizeof(float));
    
    FILE* fp = fopen("x.bin", "rb");

    for (int i = 0; i < N; i++)
    {
        fread(ptr_zig->x + i, 1, sizeof(float), fp);
        printf("x[%d] is %f\n", i, ptr_zig->x[i]);
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

        int u1 = lfsr_generate_0(ptr_zig);
        int i = u1;
        if (0 == i) {
            continue;
        }

        float u2 = lfsr_generate_1(ptr_zig)/65535.0f * 2 - 1;

        // float u1 = random() / (float)(RAND_MAX);
        // float u2 = random() / (float)(RAND_MAX) * 2 - 1;

        // static std::random_device rd;
        // static std::default_random_engine eng(rd());
        // static std::uniform_real_distribution<> distr(0.0f, 1.0f);
        // float u1 = distr(eng);
        // float u2 = distr(eng) * 2 - 1;

        // printf("%f\n", u1);

        float x = ptr_zig->x[i] * u2;
        
        if (fabs(x) < ptr_zig->x[i - 1])
        {
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