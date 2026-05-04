#ifndef __UTILS_H__
#define __UTILS_H__

/**
 * \enum Bool
 * \brief Definition of Bool type.
 */
typedef enum
{
	B_TRUE = 1,
	B_FALSE = 0
} Bool;

/**
 * \def EPSILON
 * \brief Tolerance threshold for numerical comparisons.
 */
#define EPSILON 0.000001f

/**
 * \def ALPHABET_SIZE
 * \brief Number of uppercase letters handled by the solver.
 */
#define ALPHABET_SIZE 26

/**
 * \def MAX_GRID_NEIGHBORS
 * \brief Maximum number of Boggle neighbors for one grid cell.
 */
#define MAX_GRID_NEIGHBORS 8

/**
 * \def FIRST_UPPERCASE_LETTER
 * \brief First accepted uppercase letter.
 */
#define FIRST_UPPERCASE_LETTER 'A'

/**
 * \def LAST_UPPERCASE_LETTER
 * \brief Last accepted uppercase letter.
 */
#define LAST_UPPERCASE_LETTER 'Z'


/**
 * \def PLATFORM_WINDOWS
 * \brief Defined when the target platform is Microsoft Windows.
 */
/**
 * \def PLATFORM_APPLE
 * \brief Defined when the target platform is Apple macOS.
 */
/**
 * \def PLATFORM_LINUX
 * \brief Defined when the target platform is GNU/Linux.
 */
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
	#define PLATFORM_WINDOWS
#elif defined(__APPLE__) && defined(__MACH__)
	#define PLATFORM_APPLE
#elif defined(__linux__) || defined(linux)
	#define PLATFORM_LINUX
#else
	#error "unsupported platform"
#endif


/**
 * \def MIN(a,b)
 * \brief Returns the minimum of two values.
 *
 * \param[in] a  First value
 * \param[in] b  Second value
 * \return Minimum of a and b
 */
#define MIN(a,b) ((a)<(b)?(a):(b))


/**
 * \def MAX(a,b)
 * \brief Returns the maximum of two values.
 *
 * \param[in] a  First value
 * \param[in] b  Second value
 * \return Maximum of a and b
 */
#define MAX(a,b) ((a)>(b)?(a):(b))


/**
 * \fn void set_seed( int seed )
 * \brief Sets or resets the pseudo-random generator seed.
 *
 * If \p seed is negative, the generator is initialized from a process-dependent
 * value. Otherwise, it is deterministically initialized with the provided seed.
 * Each call reinitializes the generator state.
 *
 * \param[in] seed  Seed value
 * \return void
 */
void set_seed( int seed );


/**
 * \fn int random_int( int min, int max )
 * \brief Returns a pseudo-random integer in the inclusive range [min,max].
 *
 * \param[in] min  Minimum value.
 * \param[in] max  Maximum value.
 * \return int
 */
int random_int( int min, int max );


/**
 * \fn float random_float( float min, float max )
 * \brief Returns a pseudo-random floating-point value in the range [min,max].
 *
 * \param[in] min  Minimum value.
 * \param[in] max  Maximum value.
 * \return float
 */
float random_float( float min, float max );


/**
 * \fn char* append_character( char *s, char c )
 * \brief Returns a newly allocated string equal to \p s followed by \p c.
 *
 * The caller is responsible for freeing the returned string.
 *
 * \param[in] str  Source string.
 * \param[in] c    Character to append.
 * \return char*
 */
char* append_character( char *str, char c );


/**
 * \fn char* duplicate_string( const char *str )
 * \brief Returns a newly allocated duplicate of a C string.
 *
 * The caller is responsible for freeing the returned string.
 *
 * \param[in] str  Source string.
 * \return char*
 */
char* duplicate_string( const char *str );


/**
 * \fn void to_uppercase( char *str )
 * \brief Converts each letter of a string to uppercase.
 *
 * \param[in,out] str  Source string.
 * \return void
 */
void to_uppercase( char *str );


/**
 * \fn float stats_float_array( float *array, unsigned int, float *sum, float *mean, float *min, float *max )
 * \brief Return some statistics on an array of floats.
 *
 * \param[in] array  Array of floats.
 * \param[in] size   Size of the array.
 * \param[out] sum of elements.
 * \param[out] mean of elements.
 * \param[out] minimum of elements.
 * \param[out] maximum of elements.
 * \return float
 */
void stats_float_array( float *array, unsigned int size, float *sum, float *mean, float *min, float *max );


/**
 * \fn int compare_integers( const void *a, const void *b )
 * \brief Compares two integers and returns 0 if a==b, -1 if a<b and +1 if a>b.
 *
 * When one want to sort integers in an array, it can be used as follows:
 * 
 * qsort(array, array_size, sizeof(int), compare_integers);
 *
 * \param[in] a  First integer.
 * \param[in] b  Second integer.
 * \return int
 */
int compare_integers( const void *a, const void *b );


/**
 * \fn int compare_floats( const void *a, const void *b )
 * \brief Compares two floats and returns 0 if a==b, -1 if a<b and +1 if a>b.
 *
 * When one want to sort floats in an array, it can be used as follows:
 * 
 * qsort(array, array_size, sizeof(float), compare_floats);
 *
 * \param[in] a  First float.
 * \param[in] b  Second float.
 * \return int
 */
int compare_floats( const void *a, const void *b );

#endif //__UTILS_H__
