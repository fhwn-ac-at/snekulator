/// This tool models the Markov chain of a simple game of snakes and ladders
/// The game has 100 squares, and the player starts at square 0
/// The player rolls a 6-sided die and moves that many squares
/// If the player lands on a snake, they move back to the tail of the snake
/// If the player lands on a ladder, they move to the top of the ladder
/// The player wins if they land on square 100 exactly

#include <gsl/gsl_linalg.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_vector.h>

#include <stdio.h>

gsl_matrix *create_transition_matrix(void) {
    gsl_matrix *const transition_matrix = gsl_matrix_calloc(101, 101);

    // final square is absorbing
    gsl_matrix_set(transition_matrix, 100, 100, 1.0);

    // then we handle the remaining squares with the die roll
    for (int start = 0; start < 100; ++start) {
        // ignore the states we can never reach because that's where the snakes and ladders start
        // ladders
        if (start ==  1 || start ==  4 || start ==  9 ||
            start == 21 || start == 28 || start == 36 ||
            start == 51 || start == 71 || start == 80)
            continue;

        // snakes
        if (start == 16 || start == 47 || start == 49 || start == 56 || start == 62 ||
            start == 64 || start == 87 || start == 93 || start == 95 || start == 98)
            continue;

        // we can move 1-6 squares
        for (int die = 1; die <= 6; ++die) {
            int target = start + die;

            // if we land on a ladder or snake, we move to the target square instead
            switch (target) {
                // ladders
                case 1:
                    target = 38;
                    break;
                case 4:
                    target = 14;
                    break;
                case 9:
                    target = 31;
                    break;
                case 21:
                    target = 42;
                    break;
                case 28:
                    target = 84;
                    break;
                case 36:
                    target = 44;
                    break;
                case 51:
                    target = 67;
                    break;
                case 71:
                    target = 91;
                    break;
                case 80:
                    target = 100;
                    break;

                    // snakes
                case 16:
                    target = 6;
                    break;
                case 47:
                    target = 26;
                    break;
                case 49:
                    target = 11;
                    break;
                case 56:
                    target = 53;
                    break;
                case 62:
                    target = 19;
                    break;
                case 64:
                    target = 60;
                    break;
                case 87:
                    target = 24;
                    break;
                case 93:
                    target = 73;
                    break;
                case 95:
                    target = 75;
                    break;
                case 98:
                    target = 78;
                    break;

                default:
                    break;
            }

            // if the target exceeds 100, we remain in the same square
            target = target > 100 ? start : target;

            // we add 1/6 to the probability of moving to the target square
            double const current_probability = gsl_matrix_get(transition_matrix, start, target);
            gsl_matrix_set(transition_matrix, start, target, current_probability + 1.0 / 6.0);
        }
    }

    return transition_matrix;
}

gsl_matrix *compute_fundamental_matrix(gsl_matrix_const_view const transition_matrix_view) {
    // the fundamental matrix is the inverse of the identity matrix minus the transition matrix
    gsl_matrix *const fundamental_matrix = gsl_matrix_calloc(100, 100);

    // start out with the identity matrix
    gsl_matrix_set_identity(fundamental_matrix);

    // subtract the transition matrix from the identity matrix
    gsl_matrix_sub(fundamental_matrix, &transition_matrix_view.matrix);

    // now invert the result using the LU decomposition
    gsl_permutation *const p = gsl_permutation_alloc(100);
    int signum;
    gsl_linalg_LU_decomp(fundamental_matrix, p, &signum);
    gsl_linalg_LU_invx(fundamental_matrix, p);

    // free the allocated memory
    gsl_permutation_free(p);

    return fundamental_matrix;
}

int main(void) {
    gsl_matrix *const transition_matrix = create_transition_matrix();

    // calculate the fundamental matrix
    // get a matrix view of the transition matrix for computation
    gsl_matrix_const_view const transition_matrix_view = gsl_matrix_const_submatrix(transition_matrix, 0, 0, 100, 100);
    gsl_matrix *const fundamental_matrix = compute_fundamental_matrix(transition_matrix_view);

    // calculate the average number of moves to win the game from the start position (0)
    // it's the sum of the first row of the fundamental matrix
    gsl_vector_const_view const first_row = gsl_matrix_const_row(fundamental_matrix, 0);
    double const expected_moves = gsl_vector_sum(&first_row.vector);
    printf("Average number of moves to win the MB version of Snakes and Ladders: %f\n", expected_moves);

    // free the allocated memory
    gsl_matrix_free(fundamental_matrix);
    gsl_matrix_free(transition_matrix);

    return 0;
}
