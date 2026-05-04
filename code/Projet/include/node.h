#ifndef __NODE_H__
#define __NODE_H__

#include "utils.h"


/**
 * \struct _Node
 * \brief Definition of Node structure.
 */
typedef struct _Node Node;
struct _Node
{
	Node *childs[ALPHABET_SIZE]; /**< Direct-access table of child nodes indexed by letter-FIRST_UPPERCASE_LETTER. */
	unsigned int nb_childs;      /**< Number of non-NULL child nodes. */
	Bool is_terminal;            /**< Ending node flag. */
	char letter;                 /**< Node letter. */
	char *word;                  /**< Stored word when the node is terminal, NULL otherwise. */
	unsigned int found_stamp;    /**< Search stamp of the last grid resolution that found this terminal node. */
	unsigned int nb_occurrences; /**< Number of occurrences found during the current stamped search. */
	float min_mean_curvature;    /**< Minimum mean curvature found during the current stamped search. */
	float max_mean_curvature;    /**< Maximum mean curvature found during the current stamped search. */
	float sum_mean_curvatures;   /**< Sum of mean curvatures over all occurrences in the current stamped search. */
};


/**
 * \fn Node* new_node( char letter, Bool is_terminal )
 * \brief Returns a newly allocated Node structure.
 *
 * \param[in] letter       Node letter.
 * \param[in] is_terminal  Ending node flag.
 * \return Node*
 */
Node *new_node( char letter, Bool is_terminal );


/**
 * \fn Node* copy_node( Node *n )
 * \brief Returns a deep copy of a Node structure.
 *
 * The whole subtree rooted at \p n is duplicated recursively.
 *
 * \param[in] n  Node structure to copy.
 * \return Node*
 */
Node *copy_node( Node *n );


/**
 * \fn Node* get_child_node( Node *parent, char letter )
 * \brief Returns the child node of \p parent associated with \p letter.
 *
 * \param[in] parent  Parent node.
 * \param[in] letter  Child node letter.
 * \return Node*
 */
Node *get_child_node( Node *parent, char letter );


/**
 * \fn Bool add_child_node( Node *parent, Node *child )
 * \brief Adds a child node to a parent node.
 *
 * \param[in,out] parent  Parent node.
 * \param[in]     child   Child node.
 * \return Bool
 */
Bool add_child_node( Node *parent, Node *child );


/**
 * \fn void delete_node( Node **n )
 * \brief Deletes a Node structure recursively.
 *
 * \param[in,out] n  Node structure to delete.
 * \return void
 */
void delete_node( Node **n );

#endif //__NODE_H__
