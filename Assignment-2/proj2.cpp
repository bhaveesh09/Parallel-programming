#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <stdio.h>

// #define CSV

int	NowYear;		// 2023-2028
int	NowMonth;		// 0 - 11

float	NowPrecip;		// inches of rain per month
float	NowTemp;		// temperature this month
float	NowHeight;		// rye grass height in inches
int	NowNumRabbits;		// number of rabbits in the current population
int permMonth=1;


const float RYEGRASS_GROWS_PER_MONTH =		20.0;
const float ONE_RABBITS_EATS_PER_MONTH =	 1.0;

const float AVG_PRECIP_PER_MONTH =		12.0;	// average
const float AMP_PRECIP_PER_MONTH =		4.0;	// plus or minus
const float RANDOM_PRECIP =			2.0;	// plus or minus noise

const float AVG_TEMP =				60.0;	// average
const float AMP_TEMP =				20.0;	// plus or minus
const float RANDOM_TEMP =			10.0;	// plus or minus noise

const float MIDTEMP =				60.0;
const float MIDPRECIP =				14.0;



float		Ranf(unsigned int *seedp, float, float );
int			Ranf(unsigned int *seedp, int, int );

float       SQR( float x );
void        Rabbits();
void        RyeGrass();
void        Watcher();





int main ()
{
;
    // starting date and time:
    NowMonth =    0;
    NowYear  = 2023;

    // starting state (feel free to change this if you want):
    NowNumRabbits = 1;
    NowHeight = 5.;

    printf( "Month\tTemp(C)\tPrecipitation(cm)\tRyeGrass Height\tRabbits\n");



    omp_set_num_threads( 3);	// same as # of sections
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            Rabbits( );
        }

        #pragma omp section
        {
            RyeGrass( );
        }

        #pragma omp section
        {
            Watcher( );
        }

        // #pragma omp section
        // {
        //     MyAgent();	// your own
        // }
    }
    // implied barrier -- all functions must return in order
     // to allow any of them to get past here

    return 0;
}

void
Rabbits( )
{
    int TempNumRabbits;
    while( NowYear < 2029 )
    {
        TempNumRabbits = NowNumRabbits;
        if (NowNumRabbits < NowHeight)
        {
            TempNumRabbits++;
        }
        else
        {
            TempNumRabbits--;
        }

    // DoneComputing barrier:
	#pragma omp barrier

    NowNumRabbits = TempNumRabbits;


    // DoneAssigning barrier:
    #pragma omp barrier

    // DonePrinting barrier:
	#pragma omp barrier

    }
}

void
RyeGrass( )
{

    float nextHeight;
    while( NowYear < 2029 )
    {
        nextHeight = NowHeight;

        float tempFactor = exp(   -SQR(  ( NowTemp - MIDTEMP ) / 10.  )   );

        float precipFactor = exp(   -SQR(  ( NowPrecip - MIDPRECIP ) / 10.  )   );

        nextHeight += tempFactor * precipFactor * RYEGRASS_GROWS_PER_MONTH;
        nextHeight -= (float)NowNumRabbits * ONE_RABBITS_EATS_PER_MONTH;


        if (nextHeight < 0)
        {
            nextHeight = 0.;
        }

     // DoneComputing barrier:
	#pragma omp barrier

        NowHeight = nextHeight;

    // DoneAssigning barrier:
    #pragma omp barrier

    // DonePrinting barrier:
	#pragma omp barrier

    }

}

void
Watcher( )
{
    int tempMonth;
    int tempYear;
    float tempTemp;
    float tempPrecip;
    unsigned int seed = 0;

    while( NowYear < 2029 )
    {

        // DoneComputing barrier:
        #pragma omp barrier

        // DoneAssigning barrier:
        #pragma omp barrier

        #ifdef CSV
        fprintf(stderr, "%.2f,%.2f,%.2f,%d,%.2f\n",
        (5./9.)*(NowTemp-32),NowPrecip*2.54, NowHeight, NowNumRabbits);
        #else
        printf( "%d\t%.2f\t%.2f\t%.2f\t%d\n",
        NowMonth, (5./9.)*(NowTemp-32),NowPrecip*2.54, NowHeight, NowNumRabbits);
        #endif

        tempMonth = NowMonth + 1;
        tempYear = NowYear;


        if (tempMonth == 12)
        {
            tempYear++;
            tempMonth = 0;
        }

        NowMonth = tempMonth;
        NowYear = tempYear;

        //The temperature and precipitation are a function of the particular month:
        float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );

        float temp = AVG_TEMP - AMP_TEMP * cos( ang );
        NowTemp = temp + Ranf( &seed, -RANDOM_TEMP, RANDOM_TEMP );

        float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
        NowPrecip = precip + Ranf( &seed,  -RANDOM_PRECIP, RANDOM_PRECIP );
        if( NowPrecip < 0. )
            NowPrecip = 0.;

        // DonePrinting barrier:
        #pragma omp barrier
        // . . .
    }

}

//Helper Functions
float
Ranf( unsigned int *seedp,  float low, float high )
{
        float r = (float) rand_r( seedp );              // 0 - RAND_MAX

        return(   low  +  r * ( high - low ) / (float)RAND_MAX   );
}

float
SQR( float x )
{
        return x*x;
}
