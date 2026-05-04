#ifndef __DIRECT_SOLVER_H__
#define __DIRECT_SOLVER_H__

#include "dict.h"
#include "node.h"
#include "grid.h"
#include "words_results.h"


/**
 * \struct _DirectSolver
 * \brief Definition of DirectSolver structure.
 */
typedef struct _DirectSolver DirectSolver;
struct _DirectSolver
{
	Dict *d;                           /**< Dictionary trie. */
	Grid *g;                           /**< Grid of the current solver. */
	char *dict_filename;               /**< Filename of the dictionary. */
	unsigned int min_word_length;      /**< Minimum accepted word length. */
	unsigned int max_word_length;      /**< Maximum accepted word length. */
	unsigned int search_stamp;         /**< Stamp used to deduplicate terminal nodes during a search. */
	Node **touched_terminals;          /**< Terminal nodes found during the current search. */
	unsigned int nb_touched_terminals; /**< Number of touched terminal nodes. */
};


/**
 * \fn DirectSolver* new_direct_solver( Grid *g, unsigned int min_word_length, unsigned int max_word_length, char *dict_filename )
 * \brief Returns a newly allocated DirectSolver structure.
 *
 * The input grid is deep-copied.
 *
 * \param[in] g                Grid structure.
 * \param[in] min_word_length  Minimum accepted word length.
 * \param[in] max_word_length  Maximum accepted word length.
 * \param[in] dict_filename    Filename of the dictionary.
 * \return DirectSolver*
 */
DirectSolver *new_direct_solver( Grid *g, unsigned int min_word_length, unsigned int max_word_length, char *dict_filename );


/**
 * \fn DirectSolver* copy_direct_solver( DirectSolver *ds )
 * \brief Returns a deep copy of a DirectSolver structure.
 *
 * The grid, dictionary trie and filename strings are duplicated. The temporary
 * list of touched terminal nodes is not copied because it is tied to one search
 * execution.
 *
 * \param[in] ds  DirectSolver structure to copy.
 * \return DirectSolver*
 */
DirectSolver *copy_direct_solver( DirectSolver *ds );


/**
 * \fn void __find_words_direct_solver( DirectSolver *ds, unsigned int i, unsigned int j, Node *parent, char *word, unsigned int depth, int previous_i, int previous_j, int previous_previous_i, int previous_previous_j, float curvature_sum )
 * \brief Recursively explores a grid from a given cell and updates terminal-node statistics.
 *
 * This private helper updates the current word buffer and the per-word
 * statistics stored in terminal trie nodes. Distinct words and
 * their statistics are exported later by find_all_words_direct_solver through
 * the touched terminal nodes.
 *
 * \param[in,out] ds                   DirectSolver structure.
 * \param[in]     i                    Current row index.
 * \param[in]     j                    Current column index.
 * \param[in]     parent               Current trie node.
 * \param[in,out] word                 Mutable working buffer storing the current word prefix.
 * \param[in]     depth                Current recursion depth.
 * \param[in]     previous_i           Previous row index, or -1 when unavailable.
 * \param[in]     previous_j           Previous column index, or -1 when unavailable.
 * \param[in]     previous_previous_i  Row index before the previous one, or -1 when unavailable.
 * \param[in]     previous_previous_j  Column index before the previous one, or -1 when unavailable.
 * \param[in]     curvature_sum        Accumulated angular curvature sum along the current path.
 * \return void
 */
void __find_words_direct_solver( DirectSolver *ds, unsigned int i, unsigned int j, Node *parent, char *word, unsigned int depth, int previous_i, int previous_j, int previous_previous_i, int previous_previous_j, float curvature_sum );


/**
 * \fn WordsResults* find_all_words_direct_solver( DirectSolver *ds )
 * \brief Returns all valid words currently present in the DirectSolver grid with their statistics.
 *
 * The returned WordsResults structure contains one entry per distinct word found
 * in the grid. For each word, it stores the number of occurrences together with
 * the minimum, maximum and average occurrence-wise mean angular curvatures. The
 * function uses the touched terminal nodes to avoid traversing the whole trie at the end.
 *
 * \param[in,out] ds  DirectSolver structure.
 * \return WordsResults*
 */
WordsResults *find_all_words_direct_solver( DirectSolver *ds );


/**
 * \fn void set_grid_direct_solver( DirectSolver *ds, Grid *g )
 * \brief Replaces the current grid of the direct solver by a deep copy of g.
 *
 * The input grid remains owned by the caller and can safely be deleted after this function returns.
 *
 * \param[in,out] ds  DirectSolver structure.
 * \param[in]     g   Grid structure.
 * \return void
 */
void set_grid_direct_solver( DirectSolver *ds, Grid *g );


/**
 * \fn void delete_direct_solver( DirectSolver **ds )
 * \brief Deletes a DirectSolver structure.
 *
 * \param[in,out] ds  DirectSolver structure to delete.
 * \return void
 */
void delete_direct_solver( DirectSolver **ds );


/**
 * \fn float __get_angular_curvature_direct_solver( int i0, int j0, int i1, int j1, int i2, int j2 )
 * \brief Returns the normalized angular turn created by three successive cells (curvature).
 *
 * The returned value belongs to [0,1], where 0 means no turn and 1 means a 180-degree turn.
 *
 * \param[in] i0  First cell row index.
 * \param[in] j0  First cell column index.
 * \param[in] i1  Second cell row index.
 * \param[in] j1  Second cell column index.
 * \param[in] i2  Third cell row index.
 * \param[in] j2  Third cell column index.
 * \return float
 */
float __get_angular_curvature_direct_solver( int i0, int j0, int i1, int j1, int i2, int j2 );


/**
 * \fn Bool __add_touched_terminal_direct_solver( DirectSolver *ds, Node *n )
 * \brief Registers a terminal node found for the first time during the current search.
 *
 * \param[in,out] ds  DirectSolver structure.
 * \param[in,out] n   Terminal node to register.
 * \return Bool
 */
Bool __add_touched_terminal_direct_solver( DirectSolver *ds, Node *n );


/**
 * \fn void __update_terminal_stats_direct_solver( DirectSolver *ds, Node *n, unsigned int depth, float curvature_sum )
 * \brief Updates per-search occurrence and curvature statistics stored in a terminal trie node.
 *
 * \param[in,out] ds             DirectSolver structure.
 * \param[in,out] n              Terminal node to update.
 * \param[in]     depth          Current recursion depth.
 * \param[in]     curvature_sum  Accumulated normalized curvature along the current path.
 * \return void
 */
void __update_terminal_stats_direct_solver( DirectSolver *ds, Node *n, unsigned int depth, float curvature_sum );

#endif //__DIRECT_SOLVER_H__
