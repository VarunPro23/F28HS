/*
 * MasterMind: a cut down version with just the master-mind game logic (purely C) and no external devices

Sample run:
Contents of the sequence (of length 3):  2 1 1
Input seq (len 3): 1 2 3
0 2
Input seq (len 3): 3 2 1
1 1
Input seq (len 3): 2 1 1
3 0
SUCCESS after 3 iterations; secret sequence is  2 1 1

 * Compile:    gcc -o cw3  master-mind-terminal.c
 * Run:        ./cw3

 */

/* --------------------------------------------------------------------------- */

/* Library headers from libc functions */
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>

/* Constants */
#define  COL  3
#define  LEN  3
#define  MAX_ATTEMPTS 10        // Constant variable declared to hold the max number of attempts
#define  MAX_BUF_LEN  100       // Constant for max buffer length for reading guess sequence

/* Global variables */

static const int colors = COL;
static const int seqlen = LEN;

static char* color_names[] = { "red", "green", "blue" };

static int* theSeq = NULL;

/* Aux functions */

/* initialise the secret sequence; by default it should be a random sequence, with -s use that sequence */
void initSeq() 
{
  /* COMPLETE the code here */
  theSeq = malloc(LEN*sizeof(int));       // Allocating memory for the sequence array to hold the secret sequence

  srand(time(0));                         // Initialize the rand()
  int i;
  for(i = 0;i < LEN; i++)             // 'for' loop to initialize the sequence
  {
    theSeq[i] = (rand() % COL) + 1;
  }
}

/* display the sequence on the terminal window, using the format from the sample run above */
void showSeq(int *seq) 
{
  /* COMPLETE the code here */
  fprintf(stdout, "Contents of the sequence (of length %i): ", LEN);
  
  // Print digits of the sequence
  int i;
  for (i = 0; i < LEN; i++)                   // 'for' loop to print the secret sequence on the terminal
  {
    fprintf(stdout, "%i ", seq[i]);
  }
  fprintf(stdout, "\n");
}

/* counts how many entries in seq2 match entries in seq1 */
/* returns exact and approximate matches  */
int * countMatches(int *seq1, int *seq2) 
{
  /* COMPLETE the code here */
  
  // Variables to hold the count of exact and approximate matches
  int exact = 0;
  int approx = 0;

  // Arrays used to mark matched digits
  int matchedGuess[LEN];
  int matchedSeq[LEN];

  // Loop variables
  int i, j;

  // Initialize the above arrays with all elements as 0
  for(i = 0;i < LEN;i++)
  {
    matchedSeq[i] = 0;
    matchedGuess[i] = 0;
  }

  int *matches = malloc(2*sizeof(int));   // Allocating memory to the array that holds the count of exact and approx. matches
  
  for(i = 0;i < LEN;i++)              // 'for' loop
  {
    if(seq2[i] == seq1[i])                // Checking if the number at index 'i' is same in both sequences    
    {
      exact++;                            // Increment exact by 1
      matchedSeq[i] = 1;                  // Change the value at index 'i' of matchedSeq to 1
      matchedGuess[i] = 1;                // Change the value at index 'i' of matchedGuess to 1
    }
  }

  for(i = 0;i < LEN;i++)             // 'for' loop
  {
    if (matchedGuess[i] == 0)           // Checking if the value at index 'i' of matchedGuess is 0 
    {    
      for(j = 0;j < LEN;j++)        // nested 'for' loop
      {
        if(seq2[i] == seq1[j] && matchedSeq[j] == 0)    
        {
          approx++;                   // Increment approx by 1      
          matchedGuess[i] = 1;
          matchedSeq[j] = 1;
          break;                      // break the loop
        }
      }
    }
  }

matches[0] = exact;                 // matches[0] is assigned the value of exact
matches[1] = approx;                // matches[1] is assigned the value of approx  

return matches;                     // Return the matches array

}

/* show the results from calling countMatches on seq1 and seq1 */
void showMatches(int code, /* only for debugging */ int * seq1, int *seq2) 
{
  /* COMPLETE the code here */
  /* Not used at all */
}

/* read a guess sequence fron stdin and store the values in arr */
/* returns 1 if guess sequence was read successfully, 0 in case of error */
int readString(int *arr) 
{
  /* COMPLETE the code here */
  char * buffer;    // Buffer to store input read from command line
  char * token;     // Stores a token read from the input
  int flag = 1;     // Flag used for detecting errors (0 - error, 1 - no error)
  int i;            // Loop variable

  buffer = (char *) malloc(MAX_BUF_LEN * sizeof(char));
                         
  fprintf(stdout, "Input seq (len %i): ", LEN);
  fgets(buffer, MAX_BUF_LEN, stdin);
  fflush(stdin);                                   // Discard any input that was not read
  
  token = strtok(buffer, " ");                     // Read the first token from input (up to first space)
  
  // Read LEN number of integers
  for (i = 0; i < LEN; i++) {
    // If the input was too short or the token does not contain a number, set error flag
    if (token == NULL || sscanf(token, "%i", &arr[i]) == 0) {
      flag = 0;
      break;
    }

    // If the digit that was parsed is not in the range 1 to COL, set error flag
    if (arr[i] < 1 || arr[i] > COL) {
      flag = 0;
      break;
    }

    token = strtok(NULL, " ");    // Read next token (up to next space)
  }

  // If there are more digits/characters than required, set error flag
  if (token != NULL) {
    flag = 0;
  }

  free(buffer);
  return flag;    // Returns 0 for failure, 1 for success
}

/* 
  Parses a sequence string containing digits of the sequence with no spaces in 
  between. Returns NULL if the sequence is invalid.

  Example: parseSeqStr("123") returns {1, 2, 3}
  
  Only suitable for games with up to 9 coloured pegs.
*/
int * parseSeqStr(char *seq) 
{
  int * parsedSeq = (int *) malloc(LEN * sizeof(int));    // Parsed sequence

  int i;
  for (i = 0; i < LEN; i++) 
  {
    // Subtract by ASCII code of '0' to convert char to int 
    parsedSeq[i] = seq[i] - '0';
    
    // Check if the char is a number and if it is in the valid range
    if (parsedSeq[i] < 1 || parsedSeq[i] > COL) 
    {
      // If not, return NULL
      return NULL;
    }
  }

  return parsedSeq;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

int main(int argc, char **argv)
{
  /* DEFINE your variables here */
  int found = 0, attempts = 0;
  /* for getopts() command line processing */
  int verbose = 0, help = 0, unittest = 0, debug = 0;
  char *sseq = NULL;
  
  // Guess sequence
  int *guessSeq = (int *) malloc(LEN * sizeof(int));


  // see: man 3 getopt for docu and an example of command line parsing
  // Use this template to process command line options and to store the input
  {
    int opt;
    while ((opt = getopt(argc, argv, "vuds:")) != -1) 
    {
      switch (opt) 
      {
      case 'v':
	      verbose = 1;
	      break;
      case 'u':
	      unittest = 1;
	      break;
      case 'd':
	      debug = 1;
	      break;
      case 's':
	      sseq = (char *)malloc(LEN*sizeof(char));
	      strcpy(sseq,optarg);
	      break;
      default: /* '?' */
	      fprintf(stderr, "Usage: %s [-v] [-d] [-s] <secret sequence> [-u] <secret sequence> <guess sequence> \n", argv[0]);
	      exit(EXIT_FAILURE);
      }
    }
    if (unittest && optind >= argc) 
    {
      fprintf(stderr, "Expected argument after options\n");
      exit(EXIT_FAILURE);
    }

    if (verbose && unittest) 
    {
      printf("1st argument = %s\n", argv[optind]);
      printf("2nd argument = %s\n", argv[optind+1]);
    }
  }

  if (verbose) 
  {
    fprintf(stdout, "Settings for running the program\n");
    fprintf(stdout, "Verbose is %s\n", (verbose ? "ON" : "OFF"));
    fprintf(stdout, "Debug is %s\n", (debug ? "ON" : "OFF"));
    fprintf(stdout, "Unittest is %s\n", (unittest ? "ON" : "OFF"));
    if (sseq)  fprintf(stdout, "Secret sequence set to %s\n", sseq);
  }

  if (sseq) 
  { // explicitly setting secret sequence
    /* SET the secret sequence here */
    theSeq = parseSeqStr(sseq);

    // Exit if sequence is invalid
    if (theSeq == NULL) 
    {
      fprintf(stderr, "Error: Sequence must have %i digits in the range 1 to %i, without spaces\n", LEN, COL);
      return EXIT_FAILURE;
    }
  } 
    
  if (unittest) 
  {
    /* SET secret and guess sequence here */
    theSeq = parseSeqStr(argv[optind]);
    guessSeq = parseSeqStr(argv[optind+1]);

    // Exit if either sequence is invalid
    if (theSeq == NULL || guessSeq == NULL) 
    {
      fprintf(stderr, "Error: Sequence(s) must have %i digits in the range 1 to %i, without spaces\n", LEN, COL);
      return EXIT_FAILURE;
    }

    /* then run the countMatches function and show the result */
    int * matches = countMatches(theSeq, guessSeq);
    fprintf(stdout, "%i exact matches\n", matches[0]);
    fprintf(stdout, "%i approximate matches\n", matches[1]);

    return EXIT_SUCCESS;
  }

  // -----------------------------------------------------------------------------

  if(theSeq == NULL)
  {
    initSeq();
  }

  if (debug) 
  {    // Show sequence if debug mode is on
    showSeq(theSeq);
  }
  
  // +++++ main loop
  while (!found) 
  {
    attempts++;
    /* IMPLEMENT the main game logic here */
    
    // Read the guess sequence till a valid input is entered
    while (1) {
      // If a valid guess sequence was entered, break out of the loop
      if (readString(guessSeq)) {
        break;
      }
      // If an invalid guess sequence was entered, display an error message
      else {
        fprintf(stderr, "Error: Sequence must have %i digits in the range 1 to %i, seperated by a single space each\n\n", LEN, COL);
      }
    }

    int *matches = countMatches(theSeq,guessSeq);
    fprintf(stdout, "%i %i \n", matches[0],matches[1]);
    
    if (matches[0] == LEN)
    {
      found = 1;
    }
    
    // End program after max attempts
    if (attempts >= MAX_ATTEMPTS) 
    {
      break;
    }
  }

  if (found) 
  {
    /* print SUCCESS and the number of iterations */
    fprintf(stdout,"SUCCESS! You have successfully guessed the sequence in %i attempts! Good Job! \n",attempts);
  } 
  else
  {
    /* print something else */
    fprintf(stdout,"Oooh! Bad luck, you were not able to guess the sequence in %i attempts. \n", MAX_ATTEMPTS);
  }
  return EXIT_SUCCESS;
}
  
