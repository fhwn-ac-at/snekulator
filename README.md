# Snekulator

Snekulator is a small tool that models the absorbing Markov chain of the game Snakes and Ladders.
It calculates the average number of moves needed to finish the game and prints it to standard output.

It does so by calculating the fundamental matrix of the transition matrix used to model the game.

## Requirements

- C99 compliant compiler
- CMake 3.5 or higher
- GNU Scientific Library (GSL)

## Implementation

The implementation hard-codes the information for the Milton Bradley version of the game.
It is **NOT** a general implementation that can be used for arbitrary board sizes, different snakes and ladders, or different dice sizes.
It **CAN** be modified to support different configurations.

## Usage

Execute the built binary to calculate the average number of moves needed to finish the game.

```bash
$ ./snekulator
Average number of moves to win the MB version of Snakes and Ladders: 39.225122
```