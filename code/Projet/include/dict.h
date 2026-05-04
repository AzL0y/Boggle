#ifndef __DICT_H__
#define __DICT_H__

#include "node.h"


/**
 * \struct _Dict
 * \brief Definition of Dict structure.
 */
typedef struct _Dict Dict;
struct _Dict
{
	unsigned int nb_words; /**< Number of terminal words stored in the trie. */
	unsigned int nb_nodes; /**< Number of nodes stored in the trie. */
	Node *root;            /**< Root of the trie. */
};


/**
 * \fn Dict* new_dict( char *filename, unsigned int min_word_length, unsigned int max_word_length )
 * \brief Builds a dictionary trie from a text file.
 *
 * Only words whose length belongs to [\p min_word_length, \p max_word_length]
 * are inserted into the trie.
 *
 * \param[in] filename         Text file containing one candidate word per line.
 * \param[in] min_word_length  Minimum accepted word length.
 * \param[in] max_word_length  Maximum accepted word length.
 * \return Dict*
 */
Dict *new_dict( char *filename, unsigned int min_word_length, unsigned int max_word_length );


/**
 * \fn Dict* copy_dict( Dict *d )
 * \brief Returns a deep copy of a Dict structure.
 *
 * The whole trie is duplicated recursively.
 *
 * \param[in] d  Dictionary structure to copy.
 * \return Dict*
 */
Dict *copy_dict( Dict *d );


/**
 * \fn void __insert_word_dict( Dict *d, Node *parent, char *word, unsigned int word_length, unsigned int letter_idx )
 * \brief Inserts a word recursively into a dictionary trie (private helper).
 *
 * \param[in,out] d            Dictionary structure.
 * \param[in,out] parent       Current trie node.
 * \param[in]     word         Word to insert.
 * \param[in]     word_length  Length of \p word.
 * \param[in]     letter_idx   Index of the current letter in \p word.
 * \return void
 */
void __insert_word_dict( Dict *d, Node *parent, char *word, unsigned int word_length, unsigned int letter_idx );


/**
 * \fn void __print_dict( Dict *d, Node *n, char *word )
 * \brief Prints recursively a dictionary trie from a given node (private helper).
 *
 * \param[in] d     Dictionary structure.
 * \param[in] n     Current node.
 * \param[in] word  Current prefix.
 * \return void
 */
void __print_dict( Dict *d, Node *n, char *word );


/**
 * \fn void print_dict( Dict *d )
 * \brief Prints recursively a complete dictionary trie.
 *
 * \param[in] d  Dictionary structure.
 * \return void
 */
void print_dict( Dict *d );


/**
 * \fn void delete_dict( Dict **d )
 * \brief Deletes a Dict structure.
 *
 * \param[in,out] d  Dictionary structure to delete.
 * \return void
 */
void delete_dict( Dict **d );

#endif //__DICT_H__
