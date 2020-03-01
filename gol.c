#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "gol.h"
#include <errno.h>

void read_in_file(FILE *infile, struct universe *u)
{
    // get first line and read it in
    char *line = calloc(sizeof(char), 513);
    assert(line != NULL);

    fscanf(infile, "%s", line);

    // make the read in board
    char **board;
    board = malloc(sizeof(char *));
    assert(board != NULL);

    // find out how long a row is
    for (int x = 0; x < 513; x++)
    {
        if (line[x] == '.' || line[x] == '*')
        {
            // if x reaches beyond 512 entrties, x here being 512 means there are 513 entries
            if (x == 512)
            {
                fprintf(stderr, "Error in input: Lines of grid too long, exceeds 512 limit\n");
                exit(EXIT_FAILURE);
            }
            continue;
        }
        else if (line[x] == 0)
        {
            if (x == 0)
            {
                fprintf(stderr, "Error in input: No grid\n");
                exit(EXIT_FAILURE);
            }

            u->xLimit = x;

            line = realloc(line, sizeof(char) * x);
            assert(line != NULL);

            board[0] = line;
            break;
        }
        else
        {
            fprintf(stderr, "Error in input: Invalid grid characters used\n");
            exit(EXIT_FAILURE);
        }
    }

    // set y to use to set the value of rows, yLimit
    int y = 1;
    while (1)
    {
        // make the row
        char *row;
        // read in row and check its not the end of the document

        // use checkRow to make sure the grid given is a full grid
        char *checkRow = calloc(sizeof(char), 513);
        assert(checkRow != NULL);

        int check = fscanf(infile, "%s", checkRow);
        // check to see if file is finished
        if (check == EOF)
        {
            // finish if it is
            u->yLimit = y;
            break;
        }
        else
        {
            int x;
            // check if the size of checkRow is the same as the lines before
            for (x = 0; x < 513; x++)
            {
                if (checkRow[x] == '.' || checkRow[x] == '*')
                {
                    // if x reaches beyond 512 entrties, x here being 512 means there are 513 entries
                    if (x == 512)
                    {
                        fprintf(stderr, "Error in input: Lines of grid too long, exceeds 512 limit, and srows aren't same length\n");
                        exit(EXIT_FAILURE);
                    }
                    continue;
                }
                else if (checkRow[x] == 0)
                {
                    break;
                }
                else
                {
                    fprintf(stderr, "Error in input: Invalid grid characters used\n");
                    exit(EXIT_FAILURE);
                }
            }

            // if the same size then just realloc and set the row to checkRow
            if (u->xLimit == x)
            {
                row = realloc(checkRow, sizeof(char) * u->xLimit);
                assert(row != NULL);
            }
            else if (x == 0)
            {
                fprintf(stderr, "Error in input: grid can't have empty rows\n");
                exit(EXIT_FAILURE);
            }
            else
            {
                fprintf(stderr, "Error in input: Invalid grid, rows aren't same length\n");
                exit(EXIT_FAILURE);
            }
        }

        // redefine memory to give another extra space for the line
        board = realloc(board, sizeof(char *) * (y + 1));
        assert(board != NULL);

        // add that line to the board
        board[y] = row;
        y++;
    }

    u->yLimit = y;
    u->board = board;
    

    // get how many pixels there are
    u->area = (u->xLimit * u->yLimit);

    // get the first bit of info on how many cells are alive
    float *temp = malloc(sizeof(float));
    assert(temp != NULL);
    float numberAlive = 0;
    for (int y = 0; y < u->yLimit; y++)
    {
        for (int x = 0; x < u->xLimit; x++)
        {
            if (is_alive(u, x, y) == 1)
            {
                numberAlive++;
            }
        }
    }

    temp[0] = numberAlive;
    u->stats = temp;

    u->numberOfGens = 1;
}

void write_out_file(FILE *outfile, struct universe *u)
{
    // similar top the printing method, we write to the board
    for (int y = 0; y < u->yLimit; y++)
    {
        char *row = u->board[y];
        for (int x = 0; x < u->xLimit; x++)
        {
            fprintf(outfile, "%c", row[x]);
        }
        fprintf(outfile, "\n");
    }
}

int is_alive(struct universe *u, int column, int row)
{
    char symbol = u->board[row][column];
    if (symbol == '*')
    {
        return 1;
    }
    else if (symbol == '.')
    {
        return 0;
    }
    else
    {
        fprintf(stderr, "Error in execution: Invalid grid characters used\n");
        exit(EXIT_FAILURE);
    }
}

int will_be_alive(struct universe *u, int column, int row)
{
    int number_of_alive_neighbours = 0;

    // Get the ammount of alive neighbours

    // only one row
    if (u->yLimit == 1)
    {
        // if there is only one column
        if (u->xLimit == 1)
        {
        }
        // on left edge
        else if (column == 0)
        {
            // above row

            // middle row
            if (u->board[row][column + 1] == '*')
            {
                number_of_alive_neighbours++;
            }

            // row below
        }
        // on the right edge
        else if (column == (u->xLimit - 1))
        {
            // above row

            // middle row
            if (u->board[row][column - 1] == '*')
            {
                number_of_alive_neighbours++;
            }

            // row below
        }
        // inbetween
        else
        {
            // above row

            // middle row
            if (u->board[row][column - 1] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row][column + 1] == '*')
            {
                number_of_alive_neighbours++;
            }

            // row below
        }
    }
    // on top row only
    else if (row == 0)
    {
        // if there is only one column
        if (u->xLimit == 1)
        {
            // above row

            // middle row

            // row below
            if (u->board[row + 1][column] == '*')
            {
                number_of_alive_neighbours++;
            }
        }
        // on left edge
        else if (column == 0)
        {
            // above row

            // middle row
            if (u->board[row][column + 1] == '*')
            {
                number_of_alive_neighbours++;
            }

            // row below
            if (u->board[row + 1][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row + 1][column + 1] == '*')
            {
                number_of_alive_neighbours++;
            }
        }
        // on the right edge
        else if (column == (u->xLimit - 1))
        {
            // above row

            // middle row
            if (u->board[row][column - 1] == '*')
            {
                number_of_alive_neighbours++;
            }

            // row below
            if (u->board[row + 1][column - 1] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row + 1][column] == '*')
            {
                number_of_alive_neighbours++;
            }
        }
        // inbetween
        else
        {
            // above row

            // middle row
            if (u->board[row][column - 1] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row][column + 1] == '*')
            {
                number_of_alive_neighbours++;
            }

            // row below
            if (u->board[row + 1][column - 1] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row + 1][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row + 1][column + 1] == '*')
            {
                number_of_alive_neighbours++;
            }
        }
    }
    // on bottom row only
    else if (row == (u->yLimit - 1))
    {
        // if there is only one column
        if (u->xLimit == 1)
        {
            // above row
            if (u->board[row - 1][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            // middle row

            // row below
        }
        // on left edge
        else if (column == 0)
        {
            // above row
            if (u->board[row - 1][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row - 1][column + 1] == '*')
            {
                number_of_alive_neighbours++;
            }

            // middle row
            if (u->board[row][column + 1] == '*')
            {
                number_of_alive_neighbours++;
            }

            // row below
        }
        // on the right edge
        else if (column == (u->xLimit - 1))
        {
            // above row
            if (u->board[row - 1][column - 1] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row - 1][column] == '*')
            {
                number_of_alive_neighbours++;
            }

            // middle row
            if (u->board[row][column - 1] == '*')
            {
                number_of_alive_neighbours++;
            }

            // row below
        }
        // inbetween
        else
        {
            // above row
            if (u->board[row - 1][column - 1] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row - 1][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row - 1][column + 1] == '*')
            {
                number_of_alive_neighbours++;
            }

            // middle row
            if (u->board[row][column - 1] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row][column + 1] == '*')
            {
                number_of_alive_neighbours++;
            }

            // row below
        }
    }
    // somewhere inbetween top and bottom rows only
    else
    {
        // if there is only one column
        if (u->xLimit == 1)
        {
            // above row
            if (u->board[row - 1][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            // middle row

            // row below
            if (u->board[row + 1][column] == '*')
            {
                number_of_alive_neighbours++;
            }
        }
        // on left edge
        else if (column == 0)
        {
            // above row
            if (u->board[row - 1][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row - 1][column + 1] == '*')
            {
                number_of_alive_neighbours++;
            }

            // middle row
            if (u->board[row][column + 1] == '*')
            {
                number_of_alive_neighbours++;
            }

            // row below
            if (u->board[row + 1][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row + 1][column + 1] == '*')
            {
                number_of_alive_neighbours++;
            }
        }
        // on the right edge
        else if (column == (u->xLimit - 1))
        {
            // above row
            if (u->board[row - 1][column - 1] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row - 1][column] == '*')
            {
                number_of_alive_neighbours++;
            }

            // middle row
            if (u->board[row][column - 1] == '*')
            {
                number_of_alive_neighbours++;
            }

            // row below
            if (u->board[row + 1][column - 1] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row + 1][column] == '*')
            {
                number_of_alive_neighbours++;
            }
        }
        // inbetween
        else
        {
            // above row
            if (u->board[row - 1][column - 1] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row - 1][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row - 1][column + 1] == '*')
            {
                number_of_alive_neighbours++;
            }

            // middle row
            if (u->board[row][column - 1] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row][column + 1] == '*')
            {
                number_of_alive_neighbours++;
            }

            // row below
            if (u->board[row + 1][column - 1] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row + 1][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row + 1][column + 1] == '*')
            {
                number_of_alive_neighbours++;
            }
        }
    }

    // give verdict depending on current situation
    if (is_alive(u, column, row) == 1)
    {
        if (number_of_alive_neighbours == 2 || number_of_alive_neighbours == 3)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        if (number_of_alive_neighbours == 3)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

int will_be_alive_torus(struct universe *u, int column, int row)
{
    int number_of_alive_neighbours = 0;

    // Get the ammount of alive neighbours

    // only one row
    if (u->yLimit == 1)
    {
        // if there is only one column
        if (u->xLimit == 1)
        {
            number_of_alive_neighbours += 8;
        }
        // on left edge
        else if (column == 0)
        {
            // above row
            if (u->board[row][(u->xLimit - 1)] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row][column + 1] == '*')
            {
                number_of_alive_neighbours++;
            }

            // middle row
            if (u->board[row][(u->xLimit - 1)] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row][column + 1] == '*')
            {
                number_of_alive_neighbours++;
            }

            // row below
            if (u->board[row][(u->xLimit - 1)] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row][column + 1] == '*')
            {
                number_of_alive_neighbours++;
            }
        }
        // on the right edge
        else if (column == (u->xLimit - 1))
        {
            // above row
            if (u->board[row][column - 1] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row][0] == '*')
            {
                number_of_alive_neighbours++;
            }

            // middle row
            if (u->board[row][column - 1] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row][0] == '*')
            {
                number_of_alive_neighbours++;
            }

            // row below
            if (u->board[row][column - 1] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row][0] == '*')
            {
                number_of_alive_neighbours++;
            }
        }
        // inbetween
        else
        {
            // above row
            if (u->board[row][column - 1] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row][column + 1] == '*')
            {
                number_of_alive_neighbours++;
            }

            // middle row
            if (u->board[row][column - 1] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row][column + 1] == '*')
            {
                number_of_alive_neighbours++;
            }

            // row below
            if (u->board[row][column - 1] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row][column + 1] == '*')
            {
                number_of_alive_neighbours++;
            }
        }
    }
    // on top row only
    else if (row == 0)
    {
        // if there is only one column
        if (u->xLimit == 1)
        {
            // above row
            if (u->board[(u->yLimit - 1)][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[(u->yLimit - 1)][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[(u->yLimit - 1)][column] == '*')
            {
                number_of_alive_neighbours++;
            }

            // middle row
            if (u->board[row][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row][column] == '*')
            {
                number_of_alive_neighbours++;
            }

            // row below
            if (u->board[row + 1][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row + 1][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row + 1][column] == '*')
            {
                number_of_alive_neighbours++;
            }
        }
        // on left edge
        else if (column == 0)
        {
            // above row
            if (u->board[(u->yLimit - 1)][(u->xLimit - 1)] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[(u->yLimit - 1)][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[(u->yLimit - 1)][column + 1] == '*')
            {
                number_of_alive_neighbours++;
            }

            // middle row
            if (u->board[row][(u->xLimit - 1)] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row][column + 1] == '*')
            {
                number_of_alive_neighbours++;
            }

            // row below
            if (u->board[row + 1][(u->xLimit - 1)] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row + 1][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row + 1][column + 1] == '*')
            {
                number_of_alive_neighbours++;
            }
        }
        // on the right edge
        else if (column == (u->xLimit - 1))
        {
            // row above
            if (u->board[(u->yLimit - 1)][column - 1] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[(u->yLimit - 1)][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[(u->yLimit - 1)][0] == '*')
            {
                number_of_alive_neighbours++;
            }

            // middle row
            if (u->board[row][column - 1] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row][0] == '*')
            {
                number_of_alive_neighbours++;
            }

            // row below
            if (u->board[row + 1][column - 1] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row + 1][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row + 1][0] == '*')
            {
                number_of_alive_neighbours++;
            }
        }
        // inbetween
        else
        {
            // top row
            if (u->board[(u->yLimit - 1)][column - 1] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[(u->yLimit - 1)][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[(u->yLimit - 1)][column + 1] == '*')
            {
                number_of_alive_neighbours++;
            }

            // middle row
            if (u->board[row][column - 1] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row][column + 1] == '*')
            {
                number_of_alive_neighbours++;
            }

            // row below
            if (u->board[row + 1][column - 1] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row + 1][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row + 1][column + 1] == '*')
            {
                number_of_alive_neighbours++;
            }
        }
    }
    // on bottom row
    else if (row == (u->yLimit - 1))
    {
        // if there is only one column
        if (u->xLimit == 1)
        {
            if (u->board[row - 1][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row - 1][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row - 1][column] == '*')
            {
                number_of_alive_neighbours++;
            }

            // middle row
            if (u->board[row][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row][column] == '*')
            {
                number_of_alive_neighbours++;
            }

            // row below
            if (u->board[0][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[0][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[0][column] == '*')
            {
                number_of_alive_neighbours++;
            }
        }
        // on left edge
        else if (column == 0)
        {
            // above row
            if (u->board[row - 1][(u->xLimit - 1)] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row - 1][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row - 1][column + 1] == '*')
            {
                number_of_alive_neighbours++;
            }

            // middle row
            if (u->board[row][(u->xLimit - 1)] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row][column + 1] == '*')
            {
                number_of_alive_neighbours++;
            }

            // row below
            if (u->board[0][(u->xLimit - 1)] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[0][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[0][column + 1] == '*')
            {
                number_of_alive_neighbours++;
            }
        }
        // on the right edge
        else if (column == (u->xLimit - 1))
        {
            if (u->board[row - 1][column - 1] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row - 1][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row - 1][0] == '*')
            {
                number_of_alive_neighbours++;
            }

            // middle row
            if (u->board[row][column - 1] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row][0] == '*')
            {
                number_of_alive_neighbours++;
            }

            // row below
            if (u->board[0][column - 1] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[0][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[0][0] == '*')
            {
                number_of_alive_neighbours++;
            }
        }
        // inbetween
        else
        {
            if (u->board[row - 1][column - 1] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row - 1][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row - 1][column + 1] == '*')
            {
                number_of_alive_neighbours++;
            }

            // middle row
            if (u->board[row][column - 1] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row][column + 1] == '*')
            {
                number_of_alive_neighbours++;
            }

            // row below
            if (u->board[0][column - 1] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[0][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[0][column + 1] == '*')
            {
                number_of_alive_neighbours++;
            }
        }
    }
    // somewhere inbetween top and bottom rows
    else
    {
        // if there is only one column
        if (u->xLimit == 1)
        {
            // above row
            if (u->board[row - 1][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row - 1][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row - 1][column] == '*')
            {
                number_of_alive_neighbours++;
            }

            // middle row
            if (u->board[row][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row][column] == '*')
            {
                number_of_alive_neighbours++;
            }

            // row below
            if (u->board[row + 1][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row + 1][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row + 1][column] == '*')
            {
                number_of_alive_neighbours++;
            }
        }
        // on left edge
        else if (column == 0)
        {
            // above row
            if (u->board[row - 1][(u->xLimit - 1)] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row - 1][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row - 1][column + 1] == '*')
            {
                number_of_alive_neighbours++;
            }

            // middle row
            if (u->board[row][(u->xLimit - 1)] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row][column + 1] == '*')
            {
                number_of_alive_neighbours++;
            }

            // row below
            if (u->board[row + 1][(u->xLimit - 1)] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row + 1][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row + 1][column + 1] == '*')
            {
                number_of_alive_neighbours++;
            }
        }
        // on the right edge
        else if (column == (u->xLimit - 1))
        {
            if (u->board[row - 1][column - 1] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row - 1][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row - 1][0] == '*')
            {
                number_of_alive_neighbours++;
            }

            // middle row
            if (u->board[row][column - 1] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row][0] == '*')
            {
                number_of_alive_neighbours++;
            }

            // row below
            if (u->board[row + 1][column - 1] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row + 1][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row + 1][0] == '*')
            {
                number_of_alive_neighbours++;
            }
        }
        // inbetween
        else
        {
            // above row
            if (u->board[row - 1][column - 1] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row - 1][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row - 1][column + 1] == '*')
            {
                number_of_alive_neighbours++;
            }

            // middle row
            if (u->board[row][column - 1] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row][column + 1] == '*')
            {
                number_of_alive_neighbours++;
            }

            // row below
            if (u->board[row + 1][column - 1] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row + 1][column] == '*')
            {
                number_of_alive_neighbours++;
            }
            if (u->board[row + 1][column + 1] == '*')
            {
                number_of_alive_neighbours++;
            }
        }
    }

    // give verdict depending on current situation
    if (is_alive(u, column, row) == 1)
    {
        if (number_of_alive_neighbours == 2 || number_of_alive_neighbours == 3)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        if (number_of_alive_neighbours == 3)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

void evolve(struct universe *u, int (*rule)(struct universe *u, int column, int row))
{
    char **board;
    board = malloc(sizeof(char *));
    assert(board != NULL);

    // go through u and make the new board
    for (int y = 0; y < u->yLimit; y++)
    {
        char *line = malloc(sizeof(char) * u->xLimit);
        assert(line != NULL);
        for (int x = 0; x < u->xLimit; x++)
        {
            if (rule(u, x, y) == 1)
            {
                line[x] = '*';
            }
            else
            {
                line[x] = '.';
            }
        }

        board = realloc(board, sizeof(char *) * (y + 1));
        assert(board != NULL);
        board[y] = line;
    }
    free(u->board);
    u->board = board;

    // update stats
    int lengthOfStats = u->numberOfGens;
    u->stats = realloc(u->stats, sizeof(float) * (lengthOfStats + 1));
    assert(u->stats != NULL);

    float numberAlive = 0;
    for (int y = 0; y < u->yLimit; y++)
    {
        for (int x = 0; x < u->xLimit; x++)
        {
            if (is_alive(u, x, y) == 1)
            {
                numberAlive++;
            }
        }
    }
    u->stats[lengthOfStats] = numberAlive;

    u->numberOfGens++;
}

void print_statistics(struct universe *u)
{
    float finalGen = u->stats[u->numberOfGens - 1];
    finalGen = (finalGen / u->area) * 100;
    // function imported from math.h, rounds to nearest
    finalGen = roundf(finalGen * 100) / 100;
    printf("%.3f%c of cells currently alive\n", finalGen, '%');

    float total = 0;
    float temp = 0;
    for (int i = 0; i < u->numberOfGens; i++)
    {
        temp = (u->stats[i] / u->area) * 100;
        total = total + temp;
    }

    total = total / u->numberOfGens;

    // function imported from math.h, rounds to nearest
    total = roundf(total * 1000) / 1000;
    printf("%.3f%c of cells alive on average\n", total, '%');
}