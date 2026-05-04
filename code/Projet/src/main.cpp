#include <stdlib.h>
#include <stdio.h>
#include "direct_solver.h"
#include "inverse_solver.h"
#include "words_results.h"
#include "instance.h"


/**
 * \fn int main( int argc, char **argv )
 * \brief Main entry point of the R-Boogle program.
 *
 * The program loads an optimization instance, creates the associated direct
 * and inverse solvers, runs the selected inverse heuristic, and releases all
 * allocated resources before exiting.
 *
 * Expected command line. Example:
 *   ./main data/public_20_hard_16_301_100.txt
 * 
 * \param[in] argc  Number of command-line arguments
 * \param[in] argv  Command-line arguments
 * \return int
 */
int main( int argc, char **argv )
{
	char *heuristic_name=(char*)"my_heuristic";          /* Name of the input heuristic */
	char *instance_filename;                         /* Filename of the text file instance */
	char *dict_filename=(char*)"data/dict.txt";      /* Filename of the dictionary */
	unsigned min_word_length=3, max_word_length=100; /* Minimum and maximum length of found words in a grid */
	InverseSolver *is = NULL;                        /* Inverse solver */
	DirectSolver *ds = NULL;                         /* Direct solver */
	Instance *instance = NULL;                       /* Instance (seed,grid_size,max_nb_iterations) */
	Grid *best_grid = NULL;                          /* Best reached grid */
	WordsResults *best_wr = NULL;                    /* Best reached words results */
	float best_score = 0.0f;                         /* Best reached score */
	float best_curvature = 0.0f;                     /* Best reached curvature */


	/* We parse input arguments */
	if( argc!=2 )
	{
		fprintf(stderr, "usage: %s instance_filename\n", argv[0]);
		fprintf(stderr, "example: ./main data/public_20_hard_16_301_100.txt\n");
		return EXIT_FAILURE;
	}

	if( argc == 2 )
		instance_filename = argv[1];

	/* We load the instance and print it */
	if( !(instance = load_instance(instance_filename)) )
	{
		fprintf(stderr, "error: unable to open the instance file `%s'\n", instance_filename);
		return EXIT_FAILURE;
	}

	/* We set the seed of the PRNG */
	set_seed(instance->seed);

	/* Stdout message */
	printf("----------[ instance `%s' ]----------\n", instance_filename);
	printf("* seed              -> %d\n", instance->seed);
	printf("* max_nb_iterations -> %u\n", instance->max_nb_iterations);
	printf("* grid_size         -> %u\n", instance->g->size);
	print_grid(instance->g);

	/* We instantiate the direct solver using the dictionary and the initial grid */
	if( !(ds = new_direct_solver(instance->g, min_word_length, max_word_length, dict_filename)) )
	{
		fprintf(stderr, "error: unable to load the direct solver\n");
		delete_instance(&instance);
		return EXIT_FAILURE;
	}

	/* We call the inverse solve */
	if( !(is = new_inverse_solver(ds, instance->max_nb_iterations, heuristic_name)) )
	{
		fprintf(stderr, "error: unable to create the inverse solver\n");
		delete_direct_solver(&ds);
		delete_instance(&instance);
		return EXIT_FAILURE;
	}

	run_inverse_solver(is, &best_grid, &best_wr, &best_score);

	if( !best_grid || !best_wr )
	{
		fprintf(stderr, "error: unable to run the inverse solver\n");
		delete_inverse_solver(&is);
		delete_direct_solver(&ds);
		delete_instance(&instance);
		return EXIT_FAILURE;
	}

	/* We print final results */
	printf("\n[ results ]\n");
	print_grid(best_grid);
	stats_float_array(best_wr->mean_curvatures, best_wr->nb_words, NULL, &best_curvature, NULL, NULL);
	printf("final score          -> %f\n", best_score);
	printf("final nb words       -> %d\n", best_wr->nb_words);
	printf("final mean curvature -> %f\n", best_curvature);

	/* We clean memory */
	delete_grid(&best_grid);
	delete_words_results(&best_wr);
	delete_inverse_solver(&is);
	delete_direct_solver(&ds);
	delete_instance(&instance);

	return EXIT_SUCCESS;
}
