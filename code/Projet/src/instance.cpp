#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include "instance.h"


/* Creates a new random instance. */
Instance* new_instance( unsigned int grid_size, int seed, unsigned int max_nb_iterations, char *letters_freq_filename )
{
	Instance *instance = NULL;


	if( grid_size<=1 || !letters_freq_filename )
		return NULL;

	/* Allocate the instance container. */
	if( !(instance = (Instance*)malloc(sizeof(*instance))) )
		return NULL;

	instance->seed              = seed;
	instance->max_nb_iterations = max_nb_iterations;
	instance->g                 = NULL;

	/* Initialize the pseudo-random generator before generating the grid. */
	set_seed(seed);

	/* Generate the initial grid using the letter frequency file. */
	if( !(instance->g = new_grid(grid_size, FIRST_UPPERCASE_LETTER)) )
	{
		delete_instance(&instance);
		return NULL;
	}

	if( !non_uniform_init_grid(instance->g, letters_freq_filename) )
	{
		delete_instance(&instance);
		return NULL;
	}

	return instance;
}

/* Loads an instance from an ASCII text file. */
Instance* load_instance( char *filename )
{
	Instance *instance = NULL;
	FILE *fp = NULL;
	unsigned int size = 0;
	char letter = 0;


	if( !filename )
		return NULL;

	if( !(fp = fopen(filename, "r")) )
		return NULL;

	/* Allocate the instance container. */
	if( !(instance = (Instance*)malloc(sizeof(*instance))) )
	{
		fclose(fp);
		return NULL;
	}

	instance->seed              = 0;
	instance->max_nb_iterations = 0;
	instance->g                 = NULL;

	/* Read the scalar parameters. */
	if( fscanf(fp, "%d", &(instance->seed)) != 1 )
	{
		delete_instance(&instance);
		fclose(fp);
		return NULL;
	}

	if( fscanf(fp, "%u", &(instance->max_nb_iterations)) != 1 )
	{
		delete_instance(&instance);
		fclose(fp);
		return NULL;
	}

	if( fscanf(fp, "%u", &size) != 1 || size<=1 )
	{
		delete_instance(&instance);
		fclose(fp);
		return NULL;
	}

	/* Create the grid before reading its letters. */
	if( !(instance->g = new_grid(size, FIRST_UPPERCASE_LETTER)) )
	{
		delete_instance(&instance);
		fclose(fp);
		return NULL;
	}

	for( unsigned int i=0; i<size; i++ )
	{
		for( unsigned int j=0; j<size; j++ )
		{
			if( fscanf(fp, " %c", &letter) != 1 )
			{
				delete_instance(&instance);
				fclose(fp);
				return NULL;
			}

			/* Reject non alphabetic characters. */
			if( !isalpha(letter) )
			{
				delete_instance(&instance);
				fclose(fp);
				return NULL;
			}

			/* We convert the letter to uppercase and assign it to the grid cell (i,j) */
			letter = (char)toupper((int)letter);
			set_letter_grid(instance->g, (int)i, (int)j, letter);
			set_visited_grid(instance->g, (int)i, (int)j, B_FALSE);
		}
	}

	/* We close the file handler */
	fclose(fp);

	return instance;
}

/* Saves an Instance structure to an ASCII text file. */
void save_instance( Instance *instance, char *filename )
{
	FILE *fp = NULL;


	if( !instance || !filename || !(instance->g) )
		return;

	if( !(fp = fopen(filename, "w")) )
		return;

	/* Write scalar parameters before the grid content. */
	fprintf(fp, "%d\n", instance->seed);
	fprintf(fp, "%u\n", instance->max_nb_iterations);
	fprintf(fp, "%u\n", instance->g->size);

	for( unsigned int i=0; i<instance->g->size; i++ )
	{
		for( unsigned int j=0; j<instance->g->size; j++ )
		{
			fprintf(fp, "%c", get_letter_grid(instance->g, (int)i, (int)j));

			if( j<instance->g->size-1 )
				fprintf(fp, " ");
		}

		fprintf(fp, "\n");
	}

	/* We close the file handler */
	fclose(fp);
}

/* Deletes an Instance structure. */
void delete_instance( Instance **instance )
{
	if( !instance || !(*instance) )
		return;

	delete_grid(&((*instance)->g));
	free(*instance), *instance=NULL;
}

/* Creates and saves a new instance. */
void __create_instance( char *type, char *difficulty, unsigned int grid_size, unsigned int max_nb_iterations, int seed, char *letters_freq_filename )
{
	Instance *instance = NULL;
	char filename[256]={0};
	static int instance_idx = 1;


	instance = new_instance(grid_size, seed, max_nb_iterations, letters_freq_filename);
	sprintf(filename, "data/%s_%d_%s_%d_%d_%d.txt", type, instance_idx++, difficulty, grid_size, seed, max_nb_iterations);
	save_instance(instance, filename);
	delete_instance(&instance);
}
