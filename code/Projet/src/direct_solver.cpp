#include <stdlib.h>
#include <math.h>
#include "utils.h"
#include "direct_solver.h"


/* Allocates a direct solver and copies the input grid. */
DirectSolver *new_direct_solver( Grid *g, unsigned int min_word_length, unsigned int max_word_length, char *dict_filename )
{
	DirectSolver *ds = NULL;


	if( !g || !dict_filename )
		return NULL;

	/* Allocate the solver before initializing all owned fields defensively. */
	if( !(ds = (DirectSolver*)malloc(sizeof(*ds))) )
		return NULL;

	ds->g                    = NULL;
	ds->d                    = NULL;
	ds->dict_filename        = NULL;
	ds->search_stamp         = 0;
	ds->touched_terminals    = NULL;
	ds->nb_touched_terminals = 0;

	if( !(ds->g = copy_grid(g)) )
	{
		delete_direct_solver(&ds);
		return NULL;
	}

	/* The dictionary is loaded once and then used as a trie oracle. */
	if( !(ds->d = new_dict(dict_filename, min_word_length, max_word_length)) )
	{
		delete_direct_solver(&ds);
		return NULL;
	}

	ds->dict_filename   = duplicate_string(dict_filename);
	ds->min_word_length = min_word_length;
	ds->max_word_length = max_word_length;

	if( !ds->dict_filename )
	{
		delete_direct_solver(&ds);
		return NULL;
	}

	return ds;
}

/* Duplicates a direct solver and its owned grid and dictionary. */
DirectSolver *copy_direct_solver( DirectSolver *ds )
{
	DirectSolver *copy = NULL;


	if( !ds )
		return NULL;

	/* Temporary search data are not copied because they belong to one run. */
	if( !(copy = (DirectSolver*)malloc(sizeof(*copy))) )
		return NULL;

	copy->d                    = NULL;
	copy->g                    = NULL;
	copy->dict_filename        = NULL;
	copy->min_word_length      = ds->min_word_length;
	copy->max_word_length      = ds->max_word_length;
	copy->search_stamp         = ds->search_stamp;
	copy->touched_terminals    = NULL;
	copy->nb_touched_terminals = 0;

	if( !(copy->g = copy_grid(ds->g)) )
	{
		delete_direct_solver(&copy);
		return NULL;
	}

	if( !(copy->d = copy_dict(ds->d)) )
	{
		delete_direct_solver(&copy);
		return NULL;
	}

	if( ds->dict_filename )
	{
		if( !(copy->dict_filename = duplicate_string(ds->dict_filename)) )
		{
			delete_direct_solver(&copy);
			return NULL;
		}
	}

	return copy;
}

/* Recursively explores all Boggle paths starting from one cell. */
void __find_words_direct_solver( DirectSolver *ds, unsigned int i, unsigned int j, Node *parent, char *word, unsigned int depth, 
                                 int previous_i, int previous_j, int previous_previous_i, int previous_previous_j, float curvature_sum )
{
	Node *child = NULL;
	char letter;
	int next_i, next_j;
	float local_curvature_sum;


	if( !ds || !parent || !word )
		return;

	/* Follow the trie; this prunes the DFS as soon as no word has this prefix. */
	letter = get_letter_grid(ds->g, (int)i, (int)j);
	if( !(child = get_child_node(parent, letter)) )
		return;

	word[depth]   = letter;
	word[depth+1] = '\0';

	/* The curvature is accumulated incrementally along the current path. */
	local_curvature_sum = curvature_sum;

	if( previous_previous_i>=0 && previous_previous_j>=0 && previous_i>=0 && previous_j>=0 )
		local_curvature_sum += __get_angular_curvature_direct_solver(previous_previous_i, previous_previous_j, previous_i, previous_j, (int)i, (int)j);

	set_visited_grid(ds->g, (int)i, (int)j, B_TRUE);

	if( child->is_terminal )
		__update_terminal_stats_direct_solver(ds, child, depth, local_curvature_sum);

	/* Continue with all unvisited neighbors of the current cell. */
	for( unsigned int k=0; k<get_nb_neighbors_grid(ds->g,i,j); k++ )
	{
		get_neighbor_grid(ds->g, (int)i, (int)j, k, &next_i, &next_j);

		if( get_visited_grid(ds->g, (int)next_i, (int)next_j) )
			continue;

		__find_words_direct_solver(ds, next_i, next_j, child, word, depth+1, (int)i, (int)j, previous_i, previous_j, local_curvature_sum);
	}

	set_visited_grid(ds->g, (int)i, (int)j, B_FALSE);
	word[depth] = '\0';
}

/* Finds all distinct words in the current grid and returns their statistics. */
WordsResults* find_all_words_direct_solver( DirectSolver *ds )
{
	WordsResults *wr = NULL;
	char *word = NULL;
	char *word_copy = NULL;
	Node *terminal = NULL;


	if( !ds )
		return NULL;

	if( !(wr = new_words_results()) )
		return NULL;

	/* The working word buffer is reused by all recursive calls. */
	if( !(word = (char*)calloc(ds->max_word_length+2, sizeof(*word))) )
	{
		delete_words_results(&wr);
		return NULL;
	}

	/* A new stamp invalidates old terminal statistics without traversing the trie. */
	ds->search_stamp++;
	if( ds->search_stamp == 0 )
		ds->search_stamp++;

	ds->nb_touched_terminals = 0;

	for( unsigned int i=0; i<ds->g->size; i++ )
		for( unsigned int j=0; j<ds->g->size; j++ )
			__find_words_direct_solver(ds, i, j, ds->d->root, word, 0, -1, -1, -1, -1, 0.0f);

	/* Export only terminal nodes touched by this search. */
	for( unsigned int i=0; i<ds->nb_touched_terminals; i++ )
	{
		terminal = ds->touched_terminals[i];

		if( !terminal || !terminal->word )
			continue;

		if( !(word_copy = duplicate_string(terminal->word)) )
		{
			free(word), word=NULL;
			delete_words_results(&wr);
			return NULL;
		}

		if( !add_words_results(wr, word_copy, terminal->nb_occurrences, terminal->min_mean_curvature, terminal->max_mean_curvature, terminal->sum_mean_curvatures/(float)terminal->nb_occurrences) )
		{
			free(word_copy), word_copy=NULL;
			free(word), word=NULL;
			delete_words_results(&wr);
			return NULL;
		}
	}

	free(word), word=NULL;

	return wr;
}

/* Replaces the solver grid by a deep copy of another grid of same size. */
void set_grid_direct_solver( DirectSolver *ds, Grid *g )
{
	Grid *copy = NULL;


	if( !ds || !g )
		return;

	if( ds->g->size != g->size )
		return;

	/* Copy first, then replace, so the old grid is preserved on failure. */
	if( !(copy = copy_grid(g)) )
		return;

	delete_grid(&ds->g);
	ds->g = copy;
}

/* Releases a direct solver and all resources it owns. */
void delete_direct_solver( DirectSolver **ds )
{
	if( !ds || !(*ds) )
		return;

	/* The solver owns its grid, dictionary, filename and touched-node array. */
	delete_grid(&(*ds)->g);
	delete_dict(&(*ds)->d);
	free((*ds)->dict_filename), (*ds)->dict_filename=NULL;
	free((*ds)->touched_terminals), (*ds)->touched_terminals=NULL;
	free(*ds), *ds=NULL;
}

/* Computes the normalized angle between two consecutive grid moves. */
float __get_angular_curvature_direct_solver( int i0, int j0, int i1, int j1, int i2, int j2 )
{
	int di1, dj1, di2, dj2;
	float n1, n2, cos_angle;


	/* Build the two consecutive displacement vectors. */
	di1 = i1-i0, dj1 = j1-j0;
	di2 = i2-i1, dj2 = j2-j1;

	n1 = sqrt((float)(di1*di1 + dj1*dj1));
	n2 = sqrt((float)(di2*di2 + dj2*dj2));

	if( n1<EPSILON || n2<EPSILON )
		return 0.0;

	/* Clamp the cosine to avoid numerical issues before acos. */
	cos_angle = ((float)(di1*di2 + dj1*dj2))/(n1*n2);
	cos_angle = MAX(-1.0f, MIN(1.0f, cos_angle));

	return acos(cos_angle)/(float)M_PI;
}

/* Adds a terminal node to the compact list of results touched by this search. */
Bool __add_touched_terminal_direct_solver( DirectSolver *ds, Node *n )
{
	Node **tmp = NULL;


	if( !ds || !n )
		return B_FALSE;

	/* Grow the list only when a new distinct word is first found. */
	if( !(tmp = (Node**)realloc(ds->touched_terminals, (ds->nb_touched_terminals+1)*sizeof(*(ds->touched_terminals)))) )
		return B_FALSE;

	ds->touched_terminals                             = tmp;
	ds->touched_terminals[ds->nb_touched_terminals++] = n;

	return B_TRUE;
}

/* Updates occurrences and normalized curvature statistics for a terminal node. */
void __update_terminal_stats_direct_solver( DirectSolver *ds, Node *n, unsigned int depth, float curvature_sum )
{
	float mean_curvature = 0.0f;
	unsigned int path_length;


	if( !ds || !n )
		return;

	/* A word of length L has L-2 internal turns. */
	path_length = depth+1;

	if( path_length >= 3 )
		mean_curvature = curvature_sum / (float)(path_length-2);
	else
		mean_curvature = 0.0f;

	/* First occurrence of this word during the current grid search. */
	if( n->found_stamp != ds->search_stamp )
	{
		/* First occurrence in this search: register the terminal before marking it. */
		if( !__add_touched_terminal_direct_solver(ds, n) )
			return;

		n->found_stamp         = ds->search_stamp;
		n->nb_occurrences      = 1;
		n->min_mean_curvature  = mean_curvature;
		n->max_mean_curvature  = mean_curvature;
		n->sum_mean_curvatures = mean_curvature;

		return;
	}

	/* Later occurrence: update the counter and all curvature statistics. */
	n->nb_occurrences++;
	n->sum_mean_curvatures += mean_curvature;

	if( mean_curvature < n->min_mean_curvature )
		n->min_mean_curvature = mean_curvature;

	if( mean_curvature > n->max_mean_curvature )
		n->max_mean_curvature = mean_curvature;
}
