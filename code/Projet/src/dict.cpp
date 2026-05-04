#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"
#include "dict.h"

/* Builds a trie from a text dictionary file. */
Dict *new_dict( char *filename, unsigned int min_word_length, unsigned int max_word_length )
{
	char line[100]={0}, word[100]={0};
	size_t word_length;
	FILE *fp = NULL;
	Dict *d = NULL;


	if( !filename )
		return NULL;

	if( !(d = (Dict*)malloc(sizeof(*d))) )
		return NULL;

	/* The root stores no letter and is never terminal. */
	d->root     = NULL;
	d->nb_nodes = 0;
	d->nb_words = 0;

	if( !(d->root = new_node('\0', B_FALSE)) )
	{
		free(d);
		return NULL;
	}

	d->nb_nodes = 1;

	if( !(fp = fopen(filename, "r")) )
	{
		delete_dict(&d);
		return NULL;
	}

	/* Insert only words whose length is useful for the current grid search. */
	while( fgets(line, sizeof(line), fp) != NULL )
	{
		if( sscanf(line, "%99s", word) != 1 )
			continue;

		word_length = strlen(word);

		if( word_length >= min_word_length && word_length <= max_word_length )
			__insert_word_dict(d, d->root, word, (unsigned int)word_length, 0);
	}

	fclose(fp);

	return d;
}

/* Returns a deep copy of a dictionary trie. */
Dict *copy_dict( Dict *d )
{
	Dict *copy = NULL;


	if( !d )
		return NULL;

	if( !(copy = (Dict*)malloc(sizeof(*copy))) )
		return NULL;

	/* The recursive work is delegated to copy_node. */
	copy->nb_words = d->nb_words;
	copy->nb_nodes = d->nb_nodes;
	copy->root     = NULL;

	if( d->root )
	{
		if( !(copy->root = copy_node(d->root)) )
		{
			free(copy);
			return NULL;
		}
	}

	return copy;
}

/* Inserts one word recursively into the trie. */
void __insert_word_dict( Dict *d, Node *parent, char *word, unsigned int word_length, unsigned int letter_idx )
{
	char letter;
	Node *child = NULL;


	if( !d || !parent || !word || letter_idx>=word_length )
		return;

	/* Dictionary words are normalized to uppercase while being inserted. */
	to_uppercase(word);
	letter = word[letter_idx];

	if( !(child = get_child_node(parent, letter)) )
	{
		if( !(child = new_node(letter, B_FALSE)) )
			return;

		if( !add_child_node(parent, child) )
		{
			delete_node(&child);
			return;
		}

		d->nb_nodes++;
	}

	if( letter_idx==(word_length-1) )
	{
		/* Terminal nodes store a copy of their complete word. */
		if( !child->is_terminal )
		{
			if( !(child->word = duplicate_string(word)) )
				return;

			child->is_terminal = B_TRUE;
			d->nb_words++;
		}
	}
	else
		__insert_word_dict(d, child, word, word_length, letter_idx+1);
}

/* Prints recursively all words stored below a trie node. */
void __print_dict( Dict *d, Node *n, char *word )
{
	char *word2 = NULL;


	if( !d || !n || !word )
		return;

	/* The current prefix is a valid dictionary word when the node is terminal. */
	if( n->is_terminal )
		printf("%s\n", word);

	for( unsigned int i=0; i<ALPHABET_SIZE; i++ )
	{
		if( !n->childs[i] )
			continue;

		word2 = append_character(word, n->childs[i]->letter);

		if( word2 )
			__print_dict(d, n->childs[i], word2);

		free(word2), word2=NULL;
	}
}

/* Prints all dictionary words. */
void print_dict( Dict *d )
{
	if( !d )
		return;

	/* Printing starts from the root with an empty prefix. */
	__print_dict(d, d->root, (char*)"");
}

/* Releases a dictionary and its trie. */
void delete_dict( Dict **d )
{
	if( !d || !(*d) )
		return;

	/* The root recursively owns all dictionary nodes. */
	delete_node(&(*d)->root);
	free(*d), *d=NULL;
}
