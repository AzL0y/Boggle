#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "inverse_solver.h"


/* Allocates an inverse solver around an existing direct solver. */
InverseSolver *new_inverse_solver( DirectSolver *ds, unsigned int max_nb_iterations, char *heuristic_name )
{
	InverseSolver *is = NULL;


	if( !ds || !heuristic_name )
		return NULL;

	if( !(is = (InverseSolver*)malloc(sizeof(*is))) )
		return NULL;

	is->ds                = NULL;
	is->heuristic_name    = NULL;
	is->max_nb_iterations = max_nb_iterations;

	/* The inverse solver owns its private copy of the direct solver. */
	if( !(is->ds = copy_direct_solver(ds)) )
	{
		free(is);
		return NULL;
	}

	is->heuristic_name    = duplicate_string(heuristic_name);
	is->max_nb_iterations = max_nb_iterations;

	if( !is->heuristic_name )
	{
		delete_direct_solver(&(is->ds));
		free(is);
		return NULL;
	}

	return is;
}

/* Returns a deep copy of an inverse solver. */
InverseSolver *copy_inverse_solver( InverseSolver *is )
{
	InverseSolver *copy = NULL;


	if( !is )
		return NULL;

	if( !(copy = (InverseSolver*)malloc(sizeof(*copy))) )
		return NULL;

	/* Copy the owned direct solver and the selected heuristic name. */
	copy->ds                = NULL;
	copy->heuristic_name    = NULL;
	copy->max_nb_iterations = is->max_nb_iterations;

	if( !(copy->ds = copy_direct_solver(is->ds)) )
	{
		delete_inverse_solver(&copy);
		return NULL;
	}

	if( !(copy->heuristic_name = duplicate_string(is->heuristic_name)) )
	{
		delete_inverse_solver(&copy);
		return NULL;
	}

	return copy;
}

/* Evaluates the score of the current grid. */
float evaluate_inverse_solver( InverseSolver *is, WordsResults *wr )
{
	float score = 0.0f;


	if( is != NULL && wr != NULL )
	{
		for( unsigned int i=0; i<wr->nb_words; i++ )
		{
			int length = (int)strlen(wr->words[i]);
			score     += (float)(length*length);
		}
	}

	return score;
}

/* Runs the `baseline' optimizer. */
void run_baseline_inverse_solver( InverseSolver *is, Grid **g, WordsResults **wr, float *score )
{
	float best_score = 0.0f, candidate_score = 0.0f, best_curvature = 0.0f;
	WordsResults *best_wr = NULL, *candidate_wr = NULL;
	Grid *best_grid = NULL, *candidate_grid = NULL;


	/* Stdout message */
	printf("\n[ %s optimization ]\n", is->heuristic_name);

	/* We evaluate the score of the current grid */
	best_grid = copy_grid(is->ds->g);
	set_grid_direct_solver(is->ds, best_grid);
	best_wr    = find_all_words_direct_solver(is->ds);
	best_score = evaluate_inverse_solver(is, best_wr);

	/* Stdout message */
	printf("Initialization | score=%f, nb words found=%d\n", best_score, best_wr->nb_words);

	/* We loop for a number of iterations */
	for( unsigned int it=1; it<=is->max_nb_iterations; it++ )
	{
		/* We create a candidate grid and assign it with a random letter at a random grid cell */
		candidate_grid = copy_grid(best_grid);
		set_random_letter_grid(candidate_grid);

		/* We evaluate the score of the candidate grid */
		set_grid_direct_solver(is->ds, candidate_grid);
		candidate_wr    = find_all_words_direct_solver(is->ds);
		candidate_score = evaluate_inverse_solver(is, candidate_wr);

		/* If the score is improved, we keep track of the candidate grid and its score */
		if( candidate_score>best_score )
		{
			delete_grid(&best_grid);
			delete_words_results(&best_wr);
			best_grid      = candidate_grid;
			best_score     = candidate_score;
			best_wr        = candidate_wr;
			candidate_grid = NULL;
			candidate_wr   = NULL;
		}

		/* Stdout message */
		stats_float_array(best_wr->mean_curvatures, best_wr->nb_words, NULL, &best_curvature, NULL, NULL);
		printf("iteration %d | score=%f, nb words=%d, mean curvatures=%f\n", it, best_score, best_wr->nb_words, best_curvature);

		/* We clean memory */
		delete_words_results(&candidate_wr);
		delete_grid(&candidate_grid);
	}

	/* We assign output variables */
	*g     = best_grid;
	*wr    = best_wr;
	*score = best_score;
}

/* Runs your own optimizer. */
void run_my_heuristic_inverse_solver( InverseSolver *is, Grid **g, WordsResults **wr, float *score )
{
	float best_score = 0.0f, best_curvature = 0.0f;
	WordsResults *best_wr = NULL;
	Grid *best_grid = NULL;


	/* Stdout message */
	printf("\n[ %s optimization ]\n", is->heuristic_name);

	/* We evaluate the score of the current grid */
	best_grid = copy_grid(is->ds->g);
	set_grid_direct_solver(is->ds, best_grid);
	best_wr    = find_all_words_direct_solver(is->ds);
	best_score = evaluate_inverse_solver(is, best_wr);

	/* Stdout message */
	stats_float_array(best_wr->mean_curvatures, best_wr->nb_words, NULL, &best_curvature, NULL, NULL);
	printf("score          -> %f\n", best_score);
	printf("nb words       -> %d\n", best_wr->nb_words);
	printf("mean curvature -> %f\n", best_curvature);

	/* TODO : @Rayan */

	/**
 * \brief Phase 1: Frequency-based grid initialization.
 *
 * Instead of starting from the initial random grid, we reinitialize all
 * cells by sampling letters proportionally to their frequency in the
 * language (read from the frequency file). This ensures that common
 * letters such as E, S, A, R, T are well represented from iteration 0,
 * giving the hill-climber a much stronger starting point than a uniform
 * random grid would provide.
 */
{
    char *freq_file = (char*)"data/letters_freq.txt";
    if( non_uniform_init_grid(best_grid, freq_file) )
    {
        set_grid_direct_solver(is->ds, best_grid);
        delete_words_results(&best_wr);
        best_wr    = find_all_words_direct_solver(is->ds);
        best_score = evaluate_inverse_solver(is, best_wr);
        stats_float_array(best_wr->mean_curvatures, best_wr->nb_words, NULL, &best_curvature, NULL, NULL);
        printf("After freq init   -> score=%f, nb words=%d, mean curvature=%f\n",
               best_score, best_wr->nb_words, best_curvature);
    }
    else
    {
        fprintf(stderr, "warning: could not load %s, keeping original grid\n", freq_file);
    }
}

/**
 * \brief Phase 2: Frequency-weighted hill-climbing loop.
 *
 * We run a greedy hill-climber for \p max_nb_iterations steps. The key
 * difference from the baseline is in how the replacement letter is chosen
 * at each mutation step: instead of sampling uniformly over the 26-letter
 * alphabet, we sample proportionally to the letter frequencies loaded from
 * the frequency file. This means rare letters (Q, X, Z, ...) are proposed
 * far less often than common ones (E, S, A, ...), which matches the actual
 * distribution of letters appearing in dictionary words and increases the
 * probability that a mutation improves the score.
 *
 * The CDF is built once before the loop using inverse-transform sampling,
 * and reused at every iteration. If the frequency file cannot be loaded,
 * the loop falls back silently to uniform letter sampling so the program
 * always produces a valid result.
 */
{
    /* Build the cumulative distribution function (CDF) from the frequency file.
     * freqs[i]  holds the raw percentage for the i-th letter (A=0 ... Z=25).
     * cumul[i]  holds the cumulative probability up to and including letter i,
     * normalized to [0,1]. Used for inverse-transform sampling inside the loop. */
    float freqs[ALPHABET_SIZE] = {0}, cumul[ALPHABET_SIZE] = {0};
    char  line[100] = {0};
    char  letter_c;
    float freq_val;
    int   idx = 0;
    Bool  freq_loaded = B_FALSE;
    FILE *fp = fopen("data/letters_freq.txt", "r");

    if( fp )
    {
        while( idx < ALPHABET_SIZE && fgets(line, sizeof(line), fp) != NULL )
        {
            if( sscanf(line, " %c %f", &letter_c, &freq_val) == 2 )
                freqs[idx++] = freq_val; // recup les freq de chaque lettre depuis le fichier
        }
        fclose(fp);

        if( idx == ALPHABET_SIZE )
        {
            /* Normalize frequencies and accumulate into a CDF */
            cumul[0] = freqs[0] / 100.0f;
            for( int k = 1; k < ALPHABET_SIZE; k++ )
                cumul[k] = cumul[k-1] + freqs[k] / 100.0f;	// cumule les freq dans un CDF
            freq_loaded = B_TRUE;
        }
    }

    if( !freq_loaded )
        fprintf(stderr, "warning: frequency-weighted mutation disabled\n");

    /* Greedy hill-climbing: at each iteration we mutate one cell of the best
     * grid found so far and keep the candidate only if it strictly improves
     * the score (sum of squared word lengths). */
    for( unsigned int it = 1; it <= is->max_nb_iterations; it++ )
    {
        Grid         *candidate_grid  = copy_grid(best_grid);
        WordsResults *candidate_wr    = NULL;
        float         candidate_score = 0.0f;

        /* Select a random cell to mutate */
        int ci = random_int(0, (int)candidate_grid->size - 1);
        int cj = random_int(0, (int)candidate_grid->size - 1);

        /* Sample the replacement letter.
         * If the CDF was loaded successfully we use inverse-transform sampling:
         * draw a uniform value r in [0,1] and walk the CDF until we exceed r.
         * The resulting index maps directly to a letter in A..Z.
         * Otherwise we fall back to a uniform draw over the full alphabet. */
        char new_letter;
        if( freq_loaded )
        {
            float r  = random_float(0.0f, 1.0f);
            int   ki = 0;
            while( ki < ALPHABET_SIZE - 1 && r > cumul[ki] )
                ki++;
            new_letter = (char)(FIRST_UPPERCASE_LETTER + ki);
        }
        else
        {
            new_letter = (char)(FIRST_UPPERCASE_LETTER + random_int(0, ALPHABET_SIZE - 1));
        }

        set_letter_grid(candidate_grid, ci, cj, new_letter);

        /* Evaluate the candidate grid and accept it if the score improves */
        set_grid_direct_solver(is->ds, candidate_grid);
        candidate_wr    = find_all_words_direct_solver(is->ds);
        candidate_score = evaluate_inverse_solver(is, candidate_wr);

        if( candidate_score > best_score )
        {
            delete_grid(&best_grid);
            delete_words_results(&best_wr);
            best_grid      = candidate_grid;
            best_score     = candidate_score;
            best_wr        = candidate_wr;
            candidate_grid = NULL;
            candidate_wr   = NULL;
        }

        stats_float_array(best_wr->mean_curvatures, best_wr->nb_words, NULL, &best_curvature, NULL, NULL);
        printf("iteration %d | score=%f, nb words=%d, mean curvature=%f\n",
               it, best_score, best_wr->nb_words, best_curvature);

        /* Release candidate resources (no-ops if ownership was transferred above) */
        delete_words_results(&candidate_wr);
        delete_grid(&candidate_grid);
    }
}

	/* ... */

	/* We assign output variables */
	*g     = best_grid;
	*wr    = best_wr;
	*score = best_score;
}

/* Runs the selected inverse heuristic. */
void run_inverse_solver( InverseSolver *is, Grid **g, WordsResults **wr, float *score )
{
	if( !is || !is->heuristic_name )
		return;

	/* We run the desired optimization algorithm */
	if( strcmp(is->heuristic_name, "baseline")==0 )
		run_baseline_inverse_solver(is, g, wr, score);
	else if( strcmp(is->heuristic_name, "my_heuristic")==0 )
		run_my_heuristic_inverse_solver(is, g, wr, score);
	else
		fprintf(stderr, "error: unknown optimization algorithm\n");
}

/* Releases an inverse solver structure. */
void delete_inverse_solver( InverseSolver **is )
{
	if( !is || !(*is) )
		return;

	delete_direct_solver(&(*is)->ds);
	free((*is)->heuristic_name), (*is)->heuristic_name=NULL;
	free(*is), *is=NULL;
}
