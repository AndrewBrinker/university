#include <cstdio>

struct {
    unsigned char dc : 11;
    bool overflow : 1, less : 1, equal : 1, greater : 1, carry : 1;
} sr;

int main()
{
    printf( "Size of sr: %d bytes\n", sizeof( sr ) );

    return 0;
}
