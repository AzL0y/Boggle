#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include "utils.h"

#if defined(PLATFORM_WINDOWS)
	#include <windows.h>
#else
	#include <sys/types.h>
	#include <unistd.h>
#endif

static Bool g_seed_initialized = B_FALSE;


/* Initializes or resets the pseudo-random generator. */
void set_seed( int seed )
{
	unsigned int effective_seed;


	/* A negative seed selects a process-dependent seed; otherwise it is fixed. */
	#if defined(PLATFORM_WINDOWS)
		effective_seed = (unsigned int)(seed<0 ? GetCurrentProcessId()*(unsigned int)time(NULL) : seed);
	#else
		effective_seed = (unsigned int)(seed<0 ? getpid()*(unsigned int)time(NULL) : seed);
	#endif

	srand(effective_seed);
	g_seed_initialized = B_TRUE;
}

/* Returns a pseudo-random integer in [min,max]. */
int random_int( int min, int max )
{
	if( !g_seed_initialized )
		set_seed(-1);

	/* Degenerate intervals are returned unchanged. */
	if( min==max )
		return min;

	return min + rand() % (max+1 - min);
}

/* Returns a pseudo-random floating-point value in [min,max]. */
float random_float( float min, float max )
{
	if( !g_seed_initialized )
		set_seed(-1);

	/* Degenerate intervals are returned unchanged. */
	if( min==max )
		return min;

	return min + ((float)rand() / (float)RAND_MAX) * (max - min);
}

/* Returns a newly allocated copy of a string followed by one character. */
char* append_character( char *str, char c )
{
	char *t = NULL;
	size_t length;


	if( !str )
		return NULL;

	/* Allocate one extra character plus the final null terminator. */
	length = strlen(str);

	if( !(t = (char*)calloc(length+2, sizeof(*str))) )
		return NULL;

	strcpy(t, str);
	t[length] = c;

	return t;
}

/* Returns a newly allocated duplicate of a C string. */
char* duplicate_string( const char *str )
{
	char *t = NULL;
	size_t length;


	if( !str )
		return NULL;

	/* strdup is not ISO C/C++ portable, so we duplicate explicitly. */
	length = strlen(str);

	if( !(t = (char*)malloc((length+1)*sizeof(*t))) )
		return NULL;

	memcpy(t, str, length+1);

	return t;
}

/* Converts each letter of a string to uppercase. */
void to_uppercase( char *str )
{
	if( !str )
		return;

	for( int i=0; str[i]!='\0'; i++ )
		str[i] = (char)toupper((int)str[i]);
}

/* Return some statistics on an array of floats. */
void stats_float_array( float *array, unsigned int size, float *sum, float *mean, float *min, float *max )
{
	if( !array )
		return;

	if( sum!=NULL  ) *sum  = 0.0f;
	if( sum!=NULL  ) *mean = 0.0f;
	if( min!=NULL  ) *min  = array[0];
	if( max!=NULL  ) *max  = array[0];

	for( unsigned int i=0; i<size; i++ )
	{
		if( sum!=NULL  ) *sum  = *sum+array[i];
		if( mean!=NULL ) *mean = *mean+array[i];
		if( min!=NULL  ) *min  = MIN(*min,array[i]);
		if( max!=NULL  ) *max  = MAX(*max,array[i]);
	}

	if( mean!=NULL )
		*mean = *mean / (float)size;
}

/* Compares two integers and returns 0 if a==b, -1 if a<b and +1 if a>b. */
int compare_integers( const void *a, const void *b )
{
	int valA = *(const int*)a;
	int valB = *(const int*)b;
	return (valA>valB)-(valA<valB);
}


/* Compares two floats and returns 0 if a==b, -1 if a<b and +1 if a>b. */
int compare_floats( const void *a, const void *b )
{
	float valA = *(const float*)a;
	float valB = *(const float*)b;
	return (valA>valB)-(valA<valB);
}
