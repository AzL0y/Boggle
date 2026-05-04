#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include "grid.h"


/* Allocates a square grid and initializes all cells. */
Grid *new_grid( unsigned int size, char letter )
{
	Grid *g = NULL;
	unsigned int nb_cells;


	if( size<=1 )
		return NULL;

	if( !(g = (Grid*)malloc(sizeof(*g))) )
		return NULL;

	/* Initialize pointers before any allocation so delete_grid is always safe. */
	g->size         = size;
	g->letters      = NULL;
	g->visited      = NULL;
	g->neighbors_i  = NULL;
	g->neighbors_j  = NULL;
	g->nb_neighbors = NULL;

	nb_cells = g->size*g->size;

	if( !(g->letters = (char*)malloc(nb_cells*sizeof(*(g->letters)))) )
	{
		delete_grid(&g);
		return NULL;
	}

	if( !(g->visited = (Bool*)malloc(nb_cells*sizeof(*(g->visited)))) )
	{
		delete_grid(&g);
		return NULL;
	}

	if( !(g->neighbors_i = (unsigned int*)malloc(MAX_GRID_NEIGHBORS*nb_cells*sizeof(*(g->neighbors_i)))) )
	{
		delete_grid(&g);
		return NULL;
	}

	if( !(g->neighbors_j = (unsigned int*)malloc(MAX_GRID_NEIGHBORS*nb_cells*sizeof(*(g->neighbors_j)))) )
	{
		delete_grid(&g);
		return NULL;
	}

	if( !(g->nb_neighbors = (unsigned int*)malloc(nb_cells*sizeof(*(g->nb_neighbors)))) )
	{
		delete_grid(&g);
		return NULL;
	}

	/* Fill letters and reset visited flags. */
	for( int i=0; i<(int)g->size; i++ )
	{
		for( int j=0; j<(int)g->size; j++ )
		{
			set_letter_grid(g, i, j, letter);
			set_visited_grid(g, i, j, B_FALSE);
		}
	}

	__compute_neighbors_grid(g);

	return g;
}

/* Returns a deep copy of a grid. */
Grid *copy_grid( Grid *g )
{
	Grid *copy = NULL;
	size_t nb_cells;


	if( !g )
		return NULL;

	/* new_grid allocates all arrays; we then overwrite them with exact data. */
	if( !(copy = new_grid(g->size, FIRST_UPPERCASE_LETTER)) )
		return NULL;

	nb_cells = (size_t)g->size*g->size;

	memcpy(copy->letters, g->letters, nb_cells*sizeof(*(g->letters)));
	memcpy(copy->visited, g->visited, nb_cells*sizeof(*(g->visited)));
	memcpy(copy->neighbors_i, g->neighbors_i, MAX_GRID_NEIGHBORS*nb_cells*sizeof(*(g->neighbors_i)));
	memcpy(copy->neighbors_j, g->neighbors_j, MAX_GRID_NEIGHBORS*nb_cells*sizeof(*(g->neighbors_j)));
	memcpy(copy->nb_neighbors, g->nb_neighbors, nb_cells*sizeof(*(g->nb_neighbors)));

	return copy;
}

/* Initializes a grid with uniformly sampled uppercase letters. */
void uniform_init_grid( Grid *g )
{
	char letter;


	if( !g )
		return;

	/* Each cell receives one independent letter between A and Z. */
	for( int i=0; i<(int)g->size; i++ )
	{
		for( int j=0; j<(int)g->size; j++ )
		{
			letter = (char)random_int((int)FIRST_UPPERCASE_LETTER, (int)LAST_UPPERCASE_LETTER);
			set_letter_grid(g, i, j, letter);
			set_visited_grid(g, i, j, B_FALSE);
		}
	}
}

/* Initializes a grid using letter frequencies read from a file. */
Bool non_uniform_init_grid( Grid *g, char *filename )
{
	float freq, freqs[ALPHABET_SIZE], cumul[ALPHABET_SIZE], r;
	char line[100]={0}, letter;
	FILE *fp = NULL;
	int idx = 0;


	if( !g || !filename )
		return B_FALSE;

	if( !(fp = fopen(filename, "r")) )
		return B_FALSE;

	/* The expected file contains exactly one frequency per alphabet letter. */
	while( idx<ALPHABET_SIZE && fgets(line, sizeof(line), fp) != NULL )
	{
		if( sscanf(line, " %c %f", &letter, &freq) != 2 || (freq<0.0 || freq>100.0) )
		{
			fclose(fp);
			return B_FALSE;
		}

		freqs[idx++] = freq;
	}

	if( idx != ALPHABET_SIZE )
	{
		fclose(fp);
		return B_FALSE;
	}

	memset(cumul, 0, sizeof(cumul));
	cumul[0] = freqs[0]/100.0f;

	for( int i=1; i<ALPHABET_SIZE; i++ )
		cumul[i] = cumul[i-1]+(freqs[i]/100.0f);

	if( fabsf(cumul[ALPHABET_SIZE-1]-1.0f)>EPSILON )
	{
		fclose(fp);
		return B_FALSE;
	}

	/* Inverse transform sampling chooses each grid letter. */
	for( int i=0; i<(int)g->size; i++ )
	{
		for( int j=0; j<(int)g->size; j++ )
		{
			r   = random_float(0.0f, 1.0f);
			idx = 0;

			while( idx < ALPHABET_SIZE-1 && r>cumul[idx] )
				idx++;

			set_letter_grid(g, i, j, (char)(FIRST_UPPERCASE_LETTER+idx));
			set_visited_grid(g, i, j, B_FALSE);
		}
	}

	fclose(fp);

	return B_TRUE;
}

/* Returns the letter stored at one grid cell. */
char get_letter_grid( Grid *g, int i, int j )
{
	assert(g && i>=0 && j>=0 && (unsigned int)i<g->size && (unsigned int)j<g->size);
	return g->letters[i*g->size+j];
}

/* Assigns a letter to one grid cell. */
void set_letter_grid( Grid *g, int i, int j, char letter )
{
	assert(g && i>=0 && j>=0 && (unsigned int)i<g->size && (unsigned int)j<g->size);
	/* Letters are stored in row-major order. */
	g->letters[i*g->size+j] = (char)toupper((unsigned char)letter);
}

/* Returns the visited flag stored at one grid cell. */
Bool get_visited_grid( Grid *g, int i, int j )
{
	assert(g && i>=0 && j>=0 && (unsigned int)i<g->size && (unsigned int)j<g->size);
	return g->visited[i*g->size+j];
}

/* Assigns a visited flag to one grid cell. */
void set_visited_grid( Grid *g, int i, int j, Bool flag )
{
	assert(g && i>=0 && j>=0 && (unsigned int)i<g->size && (unsigned int)j<g->size);
	/* The visited array is used only during recursive grid exploration. */
	g->visited[i*g->size+j] = flag;
}

/* Prints the grid letters on the standard output. */
void print_grid( Grid *g )
{
	if( !g )
		return;

	/* The printed format is compatible with the ASCII grid file format. */
	for( int i=0; i<(int)g->size; i++ )
	{
		for( int j=0; j<(int)g->size; j++ )
			printf("%c ", get_letter_grid(g, i, j));
		printf("\n");
	}
}

/* Returns the k-th neighbor of cell (i,j). */
void get_neighbor_grid( Grid *g, int i, int j, unsigned int k, int *ni, int *nj )
{
	assert(g && ni && nj && i>=0 && j>=0 && (unsigned int)i<g->size && (unsigned int)j<g->size && k<g->nb_neighbors[i*g->size+j]);
	*ni = g->neighbors_i[(i*g->size+j)*MAX_GRID_NEIGHBORS+k];
	*nj = g->neighbors_j[(i*g->size+j)*MAX_GRID_NEIGHBORS+k];
}

/* Assigns the k-th neighbor of cell (i,j). */
void set_neighbor_grid( Grid *g, int i, int j, unsigned int k, int ni, int nj )
{
	assert(g && i>=0 && j>=0 && (unsigned int)i<g->size && (unsigned int)j<g->size && k<MAX_GRID_NEIGHBORS && ni>=0 && nj>=0 && (unsigned int)ni<g->size && (unsigned int)nj<g->size);
	g->neighbors_i[(i*g->size+j)*MAX_GRID_NEIGHBORS+k] = ni;
	g->neighbors_j[(i*g->size+j)*MAX_GRID_NEIGHBORS+k] = nj;
}

/* Returns the number of neighbors of cell (i,j). */
unsigned int get_nb_neighbors_grid( Grid *g, int i, int j )
{
	assert(g && i>=0 && j>=0 && (unsigned int)i<g->size && (unsigned int)j<g->size);
	return g->nb_neighbors[(unsigned int)i*g->size+(unsigned int)j];
}

/* Assigns the number of neighbors of cell (i,j). */
void set_nb_neighbors_grid( Grid *g, int i, int j, unsigned int nb_neighbors )
{
	assert(g && i>=0 && j>=0 && (unsigned int)i<g->size && (unsigned int)j<g->size && nb_neighbors<=MAX_GRID_NEIGHBORS);
	g->nb_neighbors[(unsigned int)i*g->size+(unsigned int)j] = nb_neighbors;
}

/* Swap the letter of two cells (i0,j0) and (i1,j1). */
void swap_letters_grid( Grid *g, int i0, int j0, int i1, int j1 )
{
	char letter0, letter1;


	if( !g )
		return;

	letter0 = get_letter_grid(g, i0, j0);
	letter1 = get_letter_grid(g, i1, j1);
	set_letter_grid(g, i0, j0, letter1);
	set_letter_grid(g, i1, j1, letter0);
}

/* Samples uniformly at random a pair distinct cells (i0,j0) and (i1,j1) */
void get_random_cells_grid( Grid *g, int *i0, int *j0, int *i1, int *j1 )
{
	int p, q;


	if( !g || !i0 || !j0 || !i1 || !j1 )
		return;

	p = random_int(0, g->size*g->size-1);
	q = random_int(0, g->size*g->size-2);

	if( q>=p )
		q++;

	*i0 = p/g->size;
	*j0 = p%g->size;
	*i1 = q/g->size;
	*j1 = q%g->size;
}

/* Assigns uniformly at random a letter in a cell. */
void set_random_letter_grid( Grid *g )
{
	char letter;
	int i, j;


	if( !g )
		return;

	i      = random_int(0, g->size-1);
	j      = random_int(0, g->size-1);
	letter = (char)random_int((int)FIRST_UPPERCASE_LETTER, (int)LAST_UPPERCASE_LETTER);
	set_letter_grid(g, i, j, letter);
}


/* Returns true if the letters of both grids are the same, false otherwise. */
Bool are_equal_grid( Grid *g1, Grid *g2 )
{
	char letter1, letter2;


	if( !g1 || !g2 || g1->size!=g2->size )
		return B_FALSE;

	for( int i=0; i<(int)g1->size; i++ )
	{
		for( int j=0; j<(int)g1->size; j++ )
		{
			letter1 = get_letter_grid(g1, i, j);
			letter2 = get_letter_grid(g2, i, j);

			if( letter1!=letter2 )
				return B_FALSE;
		}
	}

	return B_TRUE;
}

/* Shuffles the letters of a grid. */
void random_shuffle_grid( Grid *g )
{
	int r, i0, j0, i1, j1;


	if( !g )
		return;

	if( !g || !(g->letters) )
		return;

	for( int k=(g->size*g->size-1); k>0; k-- )
	{
		r  = random_int(0, k);
		i0 = k / (int)g->size;
		j0 = k % (int)g->size;
		i1 = r / (int)g->size;
		j1 = r % (int)g->size;
		swap_letters_grid(g, i0, j0, i1, j1);
	}
}

/* Counts the occurrences of letters in a grid. */
void count_letters_grid( Grid *g, unsigned int counts[ALPHABET_SIZE] )
{
	char letter;


	if( !g || !counts )
		return;

	/* We initialize counters */
	memset(counts, 0, ALPHABET_SIZE*sizeof(*counts));

	/* We count how many times each letter appear in the grid */
	for( int i=0; i<(int)g->size; i++ )
	{
		for( int j=0; j<(int)g->size; j++ )
		{
			letter = get_letter_grid(g, i, j);

			if( letter>=FIRST_UPPERCASE_LETTER && letter<=LAST_UPPERCASE_LETTER )
				counts[letter-FIRST_UPPERCASE_LETTER]++;
		}
	}
}

/* Releases a grid and all its arrays. */
void delete_grid( Grid **g )
{
	if( !g || !(*g) )
		return;

	/* All arrays are owned by Grid and were allocated in new_grid. */
	free((*g)->letters), (*g)->letters=NULL;
	free((*g)->visited), (*g)->visited=NULL;
	free((*g)->neighbors_i), (*g)->neighbors_i=NULL;
	free((*g)->neighbors_j), (*g)->neighbors_j=NULL;
	free((*g)->nb_neighbors), (*g)->nb_neighbors=NULL;
	free(*g), *g=NULL;
}

/* Precomputes the Boggle neighbors of every grid cell. */
void __compute_neighbors_grid( Grid *g )
{
	unsigned int pos, k, ii, jj;


	if( !g )
		return;

	/* Each cell has at most MAX_GRID_NEIGHBORS neighbors in the Boggle adjacency model. */
	for( unsigned int i=0; i<g->size; i++ )
	{
		for( unsigned int j=0; j<g->size; j++ )
		{
			pos = i*g->size + j;
			k   = 0;

			for( ii=MAX((int)i-1,0); ii<=MIN(i+1,g->size-1); ii++ )
			{
				for( jj=MAX((int)j-1,0); jj<=MIN(j+1,g->size-1); jj++ )
				{
					if( ii==i && jj==j )
						continue;

					set_neighbor_grid(g, (int)i, (int)j, k, ii, jj);

					k++;
				}
			}

			g->nb_neighbors[pos] = (unsigned char)k;
		}
	}
}
