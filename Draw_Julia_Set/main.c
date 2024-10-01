#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

#define FRAME_WIDTH 640
#define FRAME_HEIGHT 480
#define FRAME_BUFFER_DEVICE "/dev/fb0"

extern teamNo, name1, name2, name3;
extern idSum, id1, id2, id3;
extern void name();
extern void id();
extern void drawJuliaSet( int cY, int16_t (*frame)[640] );

int main() {
    int16_t frame[FRAME_HEIGHT][FRAME_WIDTH];
    int max_cX = -700, cX = -700;
    int min_cY = 270, cY_step = -5, cY;
    int fd;

    printf( "Function1: Name\n" );
    name();
    printf( "Function2: ID\n" );
    id();

    printf( "Main Function:\n" );
    printf( "*****Print All*****\n" );
    printf( "%s", &teamNo );
    printf( "%d, %s", id1, &name1 );
    printf( "%d, %s", id2, &name2 );
    printf( "%d, %s", id3, &name3 );
    printf( "ID Summation = %d\n", idSum );
    printf( "*****End Print*****\n" );

    printf( "\n***** Please enter p to draw Julia Set animation *****\n" );
    while ( getchar() != 'p' ) {}

    system("clear");
    fd = open( FRAME_BUFFER_DEVICE, (O_RDWR | O_SYNC) );

    if ( fd < 0 ) printf( "Frame Buffer Device Open Error!!\n" );
    else {
        for ( cY = 400 ; cY >= min_cY ; cY += cY_step ) {
            drawJuliaSet( cY, frame );
            write( fd, frame, sizeof(int16_t)*FRAME_HEIGHT*FRAME_WIDTH );
            lseek( fd, 0, SEEK_SET );
        } // for

        printf( ".*.*.*.<:: Happy New Year ::>.*.*.*.\n" );
        printf( "%s\n", &teamNo );
        printf( "%d     %s\n", id1, &name1 );
        printf( "%d     %s\n", id2, &name2 );
        printf( "%d     %s\n", id3, &name3 );

        close( fd );
    } // else

    while ( getchar() != 'p' ) {}

    return 0;
} // main()

