#ifndef __GRID_H__
#define __GRID_H__

#include "utils.h"


/**
 * \struct _Grid
 * \brief Definition of Grid structure.
 */
typedef struct _Grid Grid;
struct _Grid
{
	char *letters;               /**< Letters stored in row-major order. */
	unsigned int size;           /**< Grid size. */
	Bool *visited;               /**< Visited flags stored in row-major order. */
	unsigned int *neighbors_i;   /**< Neighbor row indices: at most MAX_GRID_NEIGHBORS values per cell. */
	unsigned int *neighbors_j;   /**< Neighbor column indices: at most MAX_GRID_NEIGHBORS values per cell. */
	unsigned int *nb_neighbors; /**< Number of neighbors for each cell. */
};

/**
 * \fn Grid* new_grid( unsigned int size, char letter )
 * \brief Returns a new Grid structure.
 *
 * The grid is square and initialized with the same letter in all cells.
 *
 * \param[in] size    Grid size (must be >= 2).
 * \param[in] letter  Letter assigned to all cells.
 * \return Grid*
 */
Grid *new_grid( unsigned int size, char letter );


/**
 * \fn Grid* copy_grid( Grid *g )
 * \brief Returns a deep copy of a Grid structure.
 *
 * All internal arrays, including visited flags and neighbor coordinate tables, are duplicated.
 *
 * \param[in] g  Grid structure to copy.
 * \return Grid*
 */
Grid *copy_grid( Grid *g );


/**
 * \fn void uniform_init_grid( Grid *g )
 * \brief Initializes a grid with a uniform letter distribution.
 *
 * \param[in,out] g  Grid structure.
 * \return void
 */
void uniform_init_grid( Grid *g );


/**
 * \fn Bool non_uniform_init_grid( Grid *g, char *filename )
 * \brief Initializes a grid with a non-uniform letter distribution.
 *
 * The probability distribution is read from a text file. The latter is assumed to be ordered in A..Z with 
 * 26 lines. Each frequency lies in [0,100] and the sum of frequencies must be approximately equal to 100.
 * 
 * File format:
 *   A frequency
 *   B frequency
 *   ...
 *   Z frequency
 *
 * \param[in,out] g         Grid structure.
 * \param[in]     filename  Filename containing letter frequencies.
 * \return Bool
 */
Bool non_uniform_init_grid( Grid *g, char *filename );


/**
 * \fn char get_letter_grid( Grid *g, int i, int j )
 * \brief Returns the letter stored at cell (i,j).
 *
 * \param[in] g  Grid structure.
 * \param[in] i  Row index.
 * \param[in] j  Column index.
 * \return char
 */
char get_letter_grid( Grid *g, int i, int j );


/**
 * \fn void set_letter_grid( Grid *g, int i, int j, char letter )
 * \brief Assigns a letter to cell (i,j).
 *
 * \param[in,out] g       Grid structure.
 * \param[in]     i       Row index.
 * \param[in]     j       Column index.
 * \param[in]     letter  Letter to assign.
 * \return void
 */
void set_letter_grid( Grid *g, int i, int j, char letter );


/**
 * \fn Bool get_visited_grid( Grid *g, int i, int j )
 * \brief Returns the visited flag stored at cell (i,j).
 *
 * \param[in] g  Grid structure.
 * \param[in] i  Row index.
 * \param[in] j  Column index.
 * \return Bool
 */
Bool get_visited_grid( Grid *g, int i, int j );


/**
 * \fn void set_visited_grid( Grid *g, int i, int j, Bool flag )
 * \brief Assigns a visited flag to cell (i,j).
 *
 * \param[in,out] g     Grid structure.
 * \param[in]     i     Row index.
 * \param[in]     j     Column index.
 * \param[in]     flag  Flag to assign.
 * \return void
 */
void set_visited_grid( Grid *g, int i, int j, Bool flag );


/**
 * \fn void print_grid( Grid *g )
 * \brief Prints a grid to the standard output.
 *
 * \param[in] g  Grid structure.
 * \return void
 */
void print_grid( Grid *g );


/**
 * \fn void get_neighbor_grid( Grid *g, int i, int j, unsigned int k, int *ni, int *nj )
 * \brief Returns the k-th neighbor of cell (i,j).
 *
 * \param[in]  g   Grid structure.
 * \param[in]  i   Row index.
 * \param[in]  j   Column index.
 * \param[in]  k   Neighbor index.
 * \param[out] ni  Neighbor row index.
 * \param[out] nj  Neighbor column index.
 * \return void
 */
void get_neighbor_grid( Grid *g, int i, int j, unsigned int k, int *ni, int *nj );


/**
 * \fn void set_neighbor_grid( Grid *g, int i, int j, unsigned int k, int ni, int nj )
 * \brief Assigns the k-th neighbor of cell (i,j).
 *
 * \param[in,out] g   Grid structure.
 * \param[in]     i   Row index.
 * \param[in]     j   Column index.
 * \param[in]     k   Neighbor index.
 * \param[in]     ni  Neighbor row index.
 * \param[in]     nj  Neighbor column index.
 * \return void
 */
void set_neighbor_grid( Grid *g, int i, int j, unsigned int k, int ni, int nj );


/**
 * \fn unsigned int get_nb_neighbors_grid( Grid *g, int i, int j )
 * \brief Returns the number of neighbors of cell (i,j).
 *
 * \param[in] g  Grid structure.
 * \param[in] i  Row index.
 * \param[in] j  Column index.
 * \return unsigned int
 */
unsigned int get_nb_neighbors_grid( Grid *g, int i, int j );


/**
 * \fn void set_nb_neighbors_grid( Grid *g, int i, int j, unsigned int nb_neighbors )
 * \brief Assigns the number of neighbors of cell (i,j).
 *
 * \param[in,out] g             Grid structure.
 * \param[in]     i             Row index.
 * \param[in]     j             Column index.
 * \param[in]     nb_neighbors  Number of neighbors.
 * \return void
 */
void set_nb_neighbors_grid( Grid *g, int i, int j, unsigned int nb_neighbors );


/**
 * \fn void swap_letters_grid( Grid *g, int i0, int j0, int i1, int j1 )
 * \brief Swap the letter of two cells (i0,j0) and (i1,j1).
 *
 * \param[in,out] g    Grid structure.
 * \param[in] (i0,j0)  Row and column index of first cell.
 * \param[in] (i1,j1)  Row and column index of second cell.
 * \return void
 */
void swap_letters_grid( Grid *g, int i0, int j0, int i1, int j1 );


/**
 * \fn void get_random_cells_grid( Grid *g, int *i0, int *j0, int *i1, int *j1 )
 * \brief Samples uniformly at random a pair of distinct cells (i0,j0) and (i1,j1).
 *
 * \param[in,out] g    Grid structure.
 * \param[in] (i0,j0)  Row and column index of first cell.
 * \param[in] (i1,j1)  Row and column index of second cell.
 * \return void
 */
void get_random_cells_grid( Grid *g, int *i0, int *j0, int *i1, int *j1 );


/**
 * \fn void set_random_letter_grid( Grid *g )
 * \brief Assigns uniformly at random a letter in a cell.
 *
 * \param[in,out] g  Grid structure.
 * \return void
 */
void set_random_letter_grid( Grid *g );


/**
 * \fn Bool are_equal_grid( Grid *g1, Grid *g2 )
 * \brief Returns true if the letters of both grids are the same, false otherwise.
 *
 * \param[in] g1  First Grid structure.
 * \param[in] g2  Second Grid structure.
 * \return Bool
 */
Bool are_equal_grid( Grid *g1, Grid *g2 );


/**
 * \fn void random_shuffle_grid( Grid *g )
 * \brief Shuffles the letters of a grid.
 *
 * \param[in] g  Grid structure.
 * \return void
 */
void random_shuffle_grid( Grid *g );


/**
 * \fn void count_letters_grid( Grid *g )
 * \brief Counts the occurrences of letters in a grid.
 *
 * \param[in] g        Grid structure.
 * \param[out] counts  Histogram.
 * \return void
 */
void count_letters_grid( Grid *g, unsigned int counts[ALPHABET_SIZE] );


/**
 * \fn void delete_grid( Grid **g )
 * \brief Deletes a Grid structure.
 *
 * \param[in,out] g  Grid structure to delete.
 * \return void
 */
void delete_grid( Grid **g );


/**
 * \fn void __compute_neighbors_grid( Grid *g )
 * \brief Computes neighbor coordinate tables of a Grid structure (private helper).
 *
 * \param[in,out] g  Grid structure.
 * \return void
 */
void __compute_neighbors_grid( Grid *g );

#endif //__GRID_H__
