#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "gol.h"
#include <errno.h>
#include <string.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
  struct universe v;
  // set defaults
  v.inputName = NULL;
  v.outputName = NULL;
  v.numberOfNewGensToDo = 5;
  v.doPrint = 0;
  v.doTorus = 0;

  v.numberOfNewGensToDoCalled = 0;

  for (int i = 1; i < argc; i++)
  {
    if (argv[i][0] == '-')
    {
      // if there is smth after the only letter
      if (argv[i][2])
      {
        fprintf(stderr, "Error in command line: invalid switch used\n");
        exit(EXIT_FAILURE);
      }
      switch (argv[i][1])
      {
      // -i name, -o name, -g number, -s *, -t *
      case 'i':
        // checks smth after -i
        if (!argv[i + 1])
        {
          fprintf(stderr, "Error in command line: something must follow '-i'\n");
          exit(EXIT_FAILURE);
        }
        else if (v.inputName == NULL)
        {
          i++;
          v.inputName = argv[i];
          break;
        }
        else
        {
          i++;
          if (strcmp(v.inputName, argv[i]) == 0)
          {
            break;
          }
          else
          {
            fprintf(stderr, "Error in command line: '-i' switch already used\n");
            exit(EXIT_FAILURE);
          }
        }

      case 'o':
        // checks smth after -o
        if (!argv[i + 1])
        {
          fprintf(stderr, "Error in command line: something must follow '-o'\n");
          exit(EXIT_FAILURE);
        }
        else if (v.outputName == NULL)
        {
          i++;
          v.outputName = argv[i];
          break;
        }
        else
        {
          i++;
          if (strcmp(v.outputName, argv[i]) == 0)
          {
            break;
          }
          else
          {
            fprintf(stderr, "Error in command line: '-o' switch already used\n");
            exit(EXIT_FAILURE);
          }
        }

      case 'g':
        // checks smth after -g
        if (!argv[i + 1])
        {
          fprintf(stderr, "Error in command line: something must follow '-g'\n");
          exit(EXIT_FAILURE);
        }

        // check what is after g is an int
        if (argv[i + 1][0] == '-')
        {
          fprintf(stderr, "Error in command line: 0 or a positive number must follow '-g'\n");
          exit(EXIT_FAILURE);
        }
        int len = strlen(argv[i + 1]);
        char num;
        for (int j = 0; j <= len - 1; j++)
        {
          num = argv[i + 1][j];
          if (isdigit(num) == 0)
          {
            fprintf(stderr, "Error in command line: non number following '-g'\n");
            exit(EXIT_FAILURE);
          }
        }

        if (v.numberOfNewGensToDoCalled == 0)
        {
          v.numberOfNewGensToDoCalled = 1;
          i++;
          v.numberOfNewGensToDo = atoi(argv[i]);
          break;
        }
        else
        {
          i++;
          if (atoi(argv[i]) == v.numberOfNewGensToDo)
          {
            break;
          }
          else
          {
            fprintf(stderr, "Error in command line: '-g' switch already used\n");
            exit(EXIT_FAILURE);
          }
        }

      case 's':
        v.doPrint = 1;
        break;
      case 't':
        v.doTorus = 1;
        break;
      default:
        fprintf(stderr, "Error in command line: invalid switch used\n");
        exit(EXIT_FAILURE);
      }
    }
  }

  // read in file
  if (v.inputName == NULL)
  {
    read_in_file(stdin, &v);
  }
  else
  {
    FILE *ifp;
    ifp = fopen(v.inputName, "r");
    if (ifp == NULL)
    {
      fprintf(stderr, "Error: couldn't read in data from input file, potential invalid filename\n");
      exit(EXIT_FAILURE);
    };
    read_in_file(ifp, &v);
    fclose(ifp);
  }

  // decide to do evolve with or without torus
  if (v.doTorus == 1)
  {
    // do the number of times specified or 5 times
    for (int i = 0; i < v.numberOfNewGensToDo; i++)
    {
      evolve(&v, will_be_alive_torus);
    }
  }
  else
  {
    // do the number of times specified or 5 times
    for (int i = 0; i < v.numberOfNewGensToDo; i++)
    {
      evolve(&v, will_be_alive);
    }
  }

  // write out the output
  if (v.outputName == NULL)
  {
    write_out_file(stdout, &v);
  }
  else
  {
    FILE *ofp;
    ofp = fopen(v.outputName, "w");
    if (ofp == NULL)
    {
      fprintf(stderr, "Error: couldn't write to output file, potential invalid filename\n");
      exit(EXIT_FAILURE);
    };
    write_out_file(ofp, &v);
    fclose(ofp);
  }

  // print out the statistics
  if (v.doPrint == 1)
  {
    print_statistics(&v);
  }

  return 0;
}