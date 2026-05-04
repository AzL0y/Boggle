#ifndef __WORDS_RESULTS_H__
#define __WORDS_RESULTS_H__

#include "utils.h"


/**
 * \struct _WordsResults
 * \brief Definition of WordsResults structure.
 *
 * A WordsResults structure stores the distinct words found during a direct
 * grid resolution together with per-word statistics computed by the solver.
 */
typedef struct _WordsResults WordsResults;
struct _WordsResults
{
	char **words;                 /**< Dynamic array of found words. */
	unsigned int *nb_occurrences; /**< Number of occurrences for each found word. */
	float *min_curvatures;        /**< Minimum mean angular curvature for each found word. */
	float *max_curvatures;        /**< Maximum mean angular curvature for each found word. */
	float *mean_curvatures;       /**< Mean of occurrence-wise mean curvatures for each found word. */
	unsigned int nb_words;        /**< Number of stored word results. */
};


/**
 * \fn WordsResults* new_words_results()
 * \brief Returns a newly allocated WordsResults structure.
 *
 * \return WordsResults*
 */
WordsResults *new_words_results();


/**
 * \fn WordsResults* copy_words_results( WordsResults *wr )
 * \brief Returns a deep copy of a WordsResults structure.
 *
 * All stored words and statistics are duplicated.
 *
 * \param[in] wr  WordsResults structure to copy.
 * \return WordsResults*
 */
WordsResults *copy_words_results( WordsResults *wr );


/**
 * \fn Bool add_words_results( WordsResults *wr, char *word, unsigned int nb_occurrences, float min_curvature, float max_curvature, float mean_curvature )
 * \brief Adds a word and its associated statistics to a WordsResults structure.
 *
 * Ownership of \p word is transferred to \p wr on success.
 *
 * \param[in,out] wr              WordsResults structure.
 * \param[in]     word            Word to add.
 * \param[in]     nb_occurrences  Number of occurrences of the word.
 * \param[in]     min_curvature   Minimum mean angular curvature of the word.
 * \param[in]     max_curvature   Maximum mean angular curvature of the word.
 * \param[in]     mean_curvature  Mean of occurrence-wise mean curvatures of the word.
 * \return Bool
 */
Bool add_words_results( WordsResults *wr, char *word, unsigned int nb_occurrences, float min_curvature, float max_curvature, float mean_curvature );


/**
 * \fn Bool has_words_results( WordsResults *wr, char *word )
 * \brief Returns true if a word already belongs to a WordsResults structure.
 *
 * This function is mainly intended for diagnostics. During direct solving,
 * duplicate detection is performed through terminal trie nodes, not by scanning
 * the WordsResults structure.
 *
 * \param[in] wr    WordsResults structure.
 * \param[in] word  Word to check.
 * \return Bool
 */
Bool has_words_results( WordsResults *wr, char *word );


/**
 * \fn void print_words_results( WordsResults *wr )
 * \brief Prints the words, occurrence counts and curvature statistics contained in a WordsResults structure.
 *
 * \param[in] wr  WordsResults structure.
 * \return void
 */
void print_words_results( WordsResults *wr );


/**
 * \fn void delete_words_results( WordsResults **wr )
 * \brief Deletes a WordsResults structure.
 *
 * \param[in,out] wr  WordsResults structure to delete.
 * \return void
 */
void delete_words_results( WordsResults **wr );

#endif //__WORDS_RESULTS_H__
