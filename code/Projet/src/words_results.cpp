#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "words_results.h"


/* Allocates an empty word-result container. */
WordsResults *new_words_results()
{
	WordsResults *wr = NULL;


	if( !(wr = (WordsResults*)malloc(sizeof(*wr))) )
		return NULL;

	/* Arrays grow together and share the same logical size nb_words. */
	wr->words           = NULL;
	wr->nb_occurrences  = NULL;
	wr->min_curvatures  = NULL;
	wr->max_curvatures  = NULL;
	wr->mean_curvatures = NULL;
	wr->nb_words        = 0;

	return wr;
}

/* Returns a deep copy of a word-result container. */
WordsResults *copy_words_results( WordsResults *wr )
{
	WordsResults *copy = NULL;
	char *word_copy = NULL;


	if( !wr )
		return NULL;

	if( !(copy = new_words_results()) )
		return NULL;

	/* Copy each word string and its associated statistics. */
	for( unsigned int i=0; i<wr->nb_words; i++ )
	{
		if( !(word_copy = duplicate_string(wr->words[i])) )
		{
			delete_words_results(&copy);
			return NULL;
		}

		if( !add_words_results(copy, word_copy, wr->nb_occurrences[i], wr->min_curvatures[i], wr->max_curvatures[i], wr->mean_curvatures[i]) )
		{
			free(word_copy), word_copy=NULL;
			delete_words_results(&copy);
			return NULL;
		}
	}

	return copy;
}

/* Appends one found word and its statistics. */
Bool add_words_results( WordsResults *wr, char *word, unsigned int nb_occurrences, float min_curvature, float max_curvature, float mean_curvature )
{
	char **tmp_words = NULL;
	unsigned int *tmp_nb_occurrences = NULL;
	float *tmp_min_curvatures = NULL;
	float *tmp_max_curvatures = NULL;
	float *tmp_mean_curvatures = NULL;


	if( !wr || !word )
		return B_FALSE;

	/* Resize each parallel array before writing the new result. */
	if( !(tmp_words = (char**)realloc(wr->words, (wr->nb_words+1)*sizeof(*(wr->words)))) )
		return B_FALSE;

	wr->words = tmp_words;

	if( !(tmp_nb_occurrences = (unsigned int*)realloc(wr->nb_occurrences, (wr->nb_words+1)*sizeof(*(wr->nb_occurrences)))) )
		return B_FALSE;

	wr->nb_occurrences  = tmp_nb_occurrences;

	if( !(tmp_min_curvatures = (float*)realloc(wr->min_curvatures, (wr->nb_words+1)*sizeof(*(wr->min_curvatures)))) )
		return B_FALSE;

	wr->min_curvatures = tmp_min_curvatures;

	if( !(tmp_max_curvatures = (float*)realloc(wr->max_curvatures, (wr->nb_words+1)*sizeof(*(wr->max_curvatures)))) )
		return B_FALSE;

	wr->max_curvatures = tmp_max_curvatures;

	if( !(tmp_mean_curvatures = (float*)realloc(wr->mean_curvatures, (wr->nb_words+1)*sizeof(*(wr->mean_curvatures)))) )
		return B_FALSE;

	wr->mean_curvatures = tmp_mean_curvatures;

	wr->words[wr->nb_words]           = word;
	wr->nb_occurrences[wr->nb_words]  = nb_occurrences;
	wr->min_curvatures[wr->nb_words]  = min_curvature;
	wr->max_curvatures[wr->nb_words]  = max_curvature;
	wr->mean_curvatures[wr->nb_words] = mean_curvature;
	wr->nb_words++;

	return B_TRUE;
}

/* Returns true when a word is already stored. */
Bool has_words_results( WordsResults *wr, char *word )
{
	if( !wr || !word )
		return B_FALSE;

	/* This linear scan is intended for diagnostics, not for the hot DFS path. */
	for( unsigned int i=0; i<wr->nb_words; i++ )
		if( wr->words[i] && !strcmp(wr->words[i], word) )
			return B_TRUE;

	return B_FALSE;
}

/* Prints all stored words with their statistics. */
void print_words_results( WordsResults *wr )
{
	if( !wr )
		return;

	/* Curvatures are normalized in [0,1]. */
	for( unsigned int i=0; i<wr->nb_words; i++ )
	{
		if( wr->words[i] )
			printf("%u | %s | %u | %f | %f | %f\n", i, wr->words[i], wr->nb_occurrences[i], wr->min_curvatures[i], wr->max_curvatures[i], wr->mean_curvatures[i]);
		else
			printf("%u | NULL | %u | %f | %f | %f\n", i, wr->nb_occurrences[i], wr->min_curvatures[i], wr->max_curvatures[i], wr->mean_curvatures[i]);
	}
}

/* Releases a word-result container and all owned words. */
void delete_words_results( WordsResults **wr )
{
	if( !wr || !(*wr) )
		return;

	/* WordsResults owns every word pointer inserted with add_words_results. */
	for( unsigned int i=0; i<(*wr)->nb_words; i++ )
		free((*wr)->words[i]), (*wr)->words[i]=NULL;

	free((*wr)->words), (*wr)->words=NULL;
	free((*wr)->nb_occurrences), (*wr)->nb_occurrences=NULL;
	free((*wr)->min_curvatures), (*wr)->min_curvatures=NULL;
	free((*wr)->max_curvatures), (*wr)->max_curvatures=NULL;
	free((*wr)->mean_curvatures), (*wr)->mean_curvatures=NULL;

	free(*wr), *wr=NULL;
}
