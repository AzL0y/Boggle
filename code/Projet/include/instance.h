#ifndef __INSTANCE_H__
#define __INSTANCE_H__

#include "grid.h"


/**
 * \struct _Instance
 * \brief Definition of an optimization instance.
 *
 * An instance stores all data required to run one R-Boogle optimization.
 */
typedef struct _Instance Instance;

struct _Instance
{
	int seed;                         /**< Random seed */
	unsigned int max_nb_iterations;   /**< Maximum number of iterations */
	Grid *g;                          /**< Current grid */
};


/**
 * \fn Instance *new_instance( unsigned int grid_size, int seed, unsigned int max_nb_iterations, char *letters_freq_filename )
 * \brief Creates a new random instance.
 *
 * The random seed is initialized from \p seed. If \p seed is negative, the
 * pseudo-random generator is initialized randomly. Otherwise, it is initialized
 * deterministically with the provided value. The grid letters are generated
 * according to the letter frequency file.
 *
 * \param[in] grid_size              Grid size (must be >= 2)
 * \param[in] seed                   Random seed
 * \param[in] max_nb_iterations      Maximum number of optimization iterations
 * \param[in] letters_freq_filename  Letter frequencies filename
 * \return Instance*
 */
Instance* new_instance( unsigned int grid_size, int seed, unsigned int max_nb_iterations, char *letters_freq_filename );


/**
 * \fn Instance *load_instance( char *filename )
 * \brief Loads an instance from an ASCII text file.
 *
 * File format:
 *   - first line: seed
 *   - second line: maximum number of iterations
 *   - third line: grid size
 *   - next 'size' lines: 'size' letters separated by spaces
 *
 * CAUTION: grid size must be >= 2.
 * 
 * \param[in] filename  Input filename
 * \return Instance*
 */
Instance* load_instance( char *filename );


/**
 * \fn void save_instance( Instance *instance, char *filename )
 * \brief Saves an instance to an ASCII text file.
 *
 * File format:
 *   - first line: seed
 *   - second line: maximum number of iterations
 *   - third line: grid size
 *   - next 'size' lines: 'size' letters separated by spaces
 *
 * \param[in] instance  Instance structure
 * \param[in] filename  Output filename
 * \return void
 */
void save_instance( Instance *inst, char *filename );


/**
 * \fn void delete_instance( Instance **instance )
 * \brief Deletes an Instance structure.
 *
 * \param[in,out] instance  Instance structure
 * \return void
 */
void delete_instance( Instance **instance );


/**
 * \fn void __create_instance( char *type, char *difficulty, unsigned int grid_size, unsigned int max_nb_iterations, int seed, char *letters_freq_filename )
 * \brief Creates and saves a random instance file (private helper).
 *
 * The generated instance is initialized from the given grid size, seed, maximum
 * number of iterations and letter frequency file. The \p type and
 * \p difficulty strings are used to build the output filename and/or identify
 * the generated instance category.
 *
 * If \p seed is negative, the pseudo-random generator is initialized randomly.
 * Otherwise, it is initialized deterministically with the provided value.
 *
 * \param[in] type                 Instance type, e.g. "public" or "private"
 * \param[in] difficulty           Difficulty level, e.g. "easy", "medium" or "hard"
 * \param[in] grid_size            Grid size
 * \param[in] max_nb_iterations    Maximum number of optimization iterations
 * \param[in] seed                 Random seed
 * \param[in] letters_freq_filename Letter frequencies filename
 * \return void
 */
void __create_instance( char *type, char *difficulty, unsigned int grid_size, unsigned int max_nb_iterations, int seed, char *letters_freq_filename );

#endif //__INSTANCE_H__
