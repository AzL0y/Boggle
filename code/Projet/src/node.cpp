#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "node.h"


/* Allocates and initializes one trie node. */
Node *new_node( char letter, Bool is_terminal )
{
	Node *n = NULL;


	if( !(n = (Node*)malloc(sizeof(*n))) )
		return NULL;

	/* Children are stored in a direct-access table indexed by letter-FIRST_UPPERCASE_LETTER. */
	memset(n->childs, 0, sizeof(n->childs));

	n->nb_childs           = 0;
	n->letter              = letter;
	n->is_terminal         = is_terminal;
	n->word                = NULL;
	n->found_stamp         = 0;
	n->nb_occurrences      = 0;
	n->min_mean_curvature  = 0.0f;
	n->max_mean_curvature  = 0.0f;
	n->sum_mean_curvatures = 0.0f;

	return n;
}

/* Recursively duplicates a trie node and all its descendants. */
Node *copy_node( Node *n )
{
	Node *copy = NULL;


	if( !n )
		return NULL;

	/* The recursive copy preserves terminal words and per-node statistics. */
	if( !(copy = new_node(n->letter, n->is_terminal)) )
		return NULL;

	copy->nb_childs           = n->nb_childs;
	copy->found_stamp         = n->found_stamp;
	copy->nb_occurrences      = n->nb_occurrences;
	copy->min_mean_curvature  = n->min_mean_curvature;
	copy->max_mean_curvature  = n->max_mean_curvature;
	copy->sum_mean_curvatures = n->sum_mean_curvatures;

	if( n->word )
	{
		if( !(copy->word = duplicate_string(n->word)) )
		{
			delete_node(&copy);
			return NULL;
		}
	}

	for( unsigned int i=0; i<ALPHABET_SIZE; i++ )
	{
		if( !n->childs[i] )
			continue;

		if( !(copy->childs[i] = copy_node(n->childs[i])) )
		{
			delete_node(&copy);
			return NULL;
		}
	}

	return copy;
}

/* Returns the child of a node associated with one uppercase letter. */
Node* get_child_node( Node *parent, char letter )
{
	unsigned int idx;


	if( !parent || !isalpha(letter) )
		return NULL;

	/* Direct indexing avoids a linear scan over the children. */
	idx = (unsigned int)(letter-FIRST_UPPERCASE_LETTER);

	return parent->childs[idx];
}

/* Adds a child node to a parent node. */
Bool add_child_node( Node *parent, Node *child )
{
	unsigned int idx;


	if( !parent || !child || !isalpha(child->letter) )
		return B_FALSE;

	/* A parent can have at most one child per uppercase letter. */
	idx = (unsigned int)(child->letter-FIRST_UPPERCASE_LETTER);

	if( parent->childs[idx] )
		return B_FALSE;

	parent->childs[idx] = child;
	parent->nb_childs++;

	return B_TRUE;
}

/* Recursively releases a trie node and all its descendants. */
void delete_node( Node **n )
{
	if( !n || !(*n) )
		return;

	/* Delete children first, then free the terminal word and the node itself. */
	for( unsigned int i=0; i<ALPHABET_SIZE; i++ )
		delete_node(&(*n)->childs[i]);

	free((*n)->word), (*n)->word=NULL;
	free(*n), *n=NULL;
}
