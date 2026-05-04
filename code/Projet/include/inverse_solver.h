#ifndef __INVERSE_SOLVER_H__
#define __INVERSE_SOLVER_H__

#include "grid.h"
#include "words_results.h"
#include "direct_solver.h"


/**
 * \struct _InverseSolver
 * \brief Definition of InverseSolver structure.
 */
typedef struct _InverseSolver InverseSolver;
struct _InverseSolver
{
	DirectSolver *ds;              /**< Direct solver used as evaluation oracle. */
	unsigned int max_nb_iterations;/**< Maximum number of iterations of the selected heuristic. */
	char *heuristic_name;          /**< Name of the selected heuristic. */
};


/**
 * \fn InverseSolver *new_inverse_solver( DirectSolver *ds, unsigned int max_nb_iterations, char *heuristic_name )
 * \brief Creates a new inverse solver from a direct solver.
 *
 * The input direct solver is deep-copied. The returned inverse solver owns its
 * internal copy, while the caller remains responsible for deleting the original
 * direct solver passed as argument.
 *
 * \param[in] ds                 Direct solver used as a template
 * \param[in] max_nb_iterations  Maximum number of optimization iterations
 * \param[in] heuristic_name     Name of the inverse heuristic
 * \return InverseSolver*
 */
InverseSolver *new_inverse_solver( DirectSolver *ds, unsigned int max_nb_iterations, char *heuristic_name );


/**
 * \fn InverseSolver* copy_inverse_solver( InverseSolver *is )
 * \brief Returns a deep copy of an InverseSolver structure.
 *
 * \param[in] is  InverseSolver structure to copy.
 * \return InverseSolver*
 */
InverseSolver *copy_inverse_solver( InverseSolver *is );


/**
 * \fn float evaluate_inverse_solver( InverseSolver *is, WordsResults *wr )
 * \brief Evaluates the current grid according to the inverse objective.
 *
 * \param[in] is  InverseSolver structure.
 * \param[in] wr  WordsResults structure.
 * \return float
 */
float evaluate_inverse_solver( InverseSolver *is, WordsResults *wr );


/**
 * \fn void run_baseline_inverse_solver( InverseSolver *is, Grid **g, WordsResults **wr, float *score )
 * \brief Runs the `baseline' optimizer.
 *
 * \param[in,out] is  InverseSolver structure.
 * \param[out] g      Resulting grid structure.
 * \param[out] wr     Resulting WordsResults structure.
 * \param[out] score  Resulting score.
 * \return void
 */
void run_baseline_inverse_solver( InverseSolver *is, Grid **g, WordsResults **wr, float *score );


/**
 * \fn void run_my_heuristic_inverse_solver( InverseSolver *is, Grid **g, WordsResults **wr, float *score )
 * \brief Runs your own optimizer.
 *
 * \param[in,out] is  InverseSolver structure.
 * \param[out] g      Resulting grid structure.
 * \param[out] wr     Resulting WordsResults structure.
 * \param[out] score  Resulting score.
 * \return void
 */
void run_my_heuristic_inverse_solver( InverseSolver *is, Grid **g, WordsResults **wr, float *score );


/**
 * \fn void run_inverse_solver( InverseSolver *is, Grid **g, WordsResults **wr, float *score )
 * \brief Runs the InverseSolver instance.
 *
 * \param[in,out] is  InverseSolver structure.
 * \param[out] g      Resulting grid structure.
 * \param[out] wr     Resulting WordsResults structure.
 * \param[out] score  Resulting score.
 * \return void
 */
void run_inverse_solver( InverseSolver *is, Grid **g, WordsResults **wr, float *score );


/**
 * \fn void delete_inverse_solver( InverseSolver **is )
 * \brief Deletes an InverseSolver structure.
 *
 * \param[in,out] is  InverseSolver structure to delete.
 * \return void
 */
void delete_inverse_solver( InverseSolver **is );

#endif //__INVERSE_SOLVER_H__
