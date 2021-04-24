#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAXB 100    // defines max for buffer
#define PPMMAX 100  // defines max for encoding and decoding

typedef struct comment
{
  char *valueOfNode;
  struct comment * nextNode;
}comment;

/*
  Stores the RGB values of a pixel
*/
typedef struct Pixel
{
  int red;
  int green;
  int blue;
} Pixel;


/*
  Stores the contents of a PPM file
*/
typedef struct PPM
{
  /* TODO: Question 1 */
  char *type;   
  comment * comment;
  int width;
  int height;
  int max;
  int n;
  Pixel ** rgb;
} PPM;

/* readLine used from Lab 3 */
char * readLine(FILE * fd)
{

	char buffer[MAXB];
	int j = 0;
	char c;
	char * line;

	c = getc(fd);
	if (c == EOF)
		return NULL;
	while (c!='\n' && j < (MAXB -1)){
		buffer[j] = c;
		j++;
		c = getc(fd);
	}

	if (c != '\n')
		while (c != '\n')
			c = getc(fd);
	buffer[j] = '\0';
	line = malloc(sizeof(char) * (j+1));

	for (int i = 0; i <= j; i++)
		line[j] = buffer[j];
		line[j] = '\0';

	return line;
	free(buffer);
}

// Uses readLine to find the comments
// Referenced from https://www.dreamincode.net/forums/topic/38678-image-steganography/
void readCom(FILE *fd, PPM *i)
{
  char c = getc(fd);    // Get char from the file

  while (c == '#')      // Check if line begins with '#'
  {
    ungetc(c,fd);       // unget c while in the loop

    char *line = malloc(100);    
    fgets(line,100,fd);

    if(i->n == 0)                 // Add comment to LL
    {
      // No previous comments are there
      i->comment = (comment *)malloc(sizeof(comment));  // Allocate memory
      i->comment->valueOfNode = line;   // The node's value is set as the line being read
      i->n = 1;   
    }

    else
    {
      // Previous comments are there
      comment *curr = i->comment;   

      for(int k = 1;k < i->n;i++)
      {
        curr = curr->nextNode;    // Curr points to the next node
      }
      curr->nextNode = (comment *)malloc(sizeof(comment));
      
      curr->nextNode->valueOfNode = line;   // The next node's value is assigned as the line beig read
      i->n = i->n + 1;
    }
    c = getc(fd);   // Move on to next line
  }
  ungetc(c,fd);   // Restore to start position
}

// Function to read the pixels from the PPM
// Referenced from https://www.dreamincode.net/forums/topic/38678-image-steganography/
void readPixels(FILE *fd, PPM *p)
{
  p->rgb = (PPM **)malloc(sizeof(PPM *) * p->height);    // Row allocation

  for (int i = 0; i < p->height; i++)
  {
    // Column allocation
    p->rgb[i] = (PPM *)malloc(sizeof(PPM) * p->width);
    for (int j = 0; j < p->width; j++)
    {
      // Get pixels for each color
      fscanf(fd,"%d",&(p->rgb[i][j].red));    // Getting the memory address
      fscanf(fd,"%d",&(p->rgb[i][j].blue));
      fscanf(fd,"%d",&(p->rgb[i][j].green));
    } 
  }
}

/*
  Returns the PPM struct from the file fd
*/
PPM* getPPM(FILE* fd)
{	
  PPM* ppm = (PPM*) malloc(sizeof(PPM));	

  /* TODO: Question 2a */
  ppm->type = (char*)malloc(2 * sizeof(char));
  ppm->type[0] = getc(fd);
  ppm->type[1] = getc(fd);
  ppm->n = 0;
  
  // Checking if the file is appropriate for encoding
  if(strcmp(ppm->type,"P3") == 0)
  {
    fprintf(stderr,"File can be encoded\n");
  } 
  else
  {
    fprintf(stderr,"File inappropriate for encoding\n");
    exit(0);
  }

  readCom(fd,ppm);    // Reads the comments

  // Scans the details of the ppm file
  fscanf(fd,"%d",&ppm->width);  
  fscanf(fd,"%d",&ppm->height);
  fscanf(fd,"%d",&ppm->max);

  readPixels(fd,ppm);   // Reads the pixels

  return ppm;
}

/*
  Prints a PPM struct in the format of a PPM file
*/
void showPPM(PPM* ppm)
{ /* TODO: Question 2b */

 int pixelSize = ppm->height * ppm->width; 
 printf("%s",ppm->type);    // Printing the type of the ppm

 comment * text = ppm->comment;
 //printf("%s",ppm->comment->valueOfNode);
 for (int i = 0; i < ppm->n; i++)
 {
   text = text->nextNode;
   printf("%s",ppm->comment);
 }

 printf("%d\n",ppm->type);      // Print the type of the PPM
 printf("%d\n",ppm->width);     // Print the width of the ppm 
 printf("%d\n",ppm->height);    // Print the height of the ppm
 printf("%d\n",ppm->max);       // Print the max of the ppm

 // Print the pixels
 for(int i = 0;i < ppm->height;i++)
 { 
   for (int j = 0; j < ppm->width; j++)
   { 
   Pixel p = ppm->rgb[i][j];       
   printf("%i\n",p.red);
   printf("%i\n",p.green);
   printf("%i\n",p.blue);
   }
 } 
}


/*
  Loads a file and returns it
  Prints error message and returns NULL if file can't be found or isn't a valid PPM
*/
PPM* loadPPMFromFile(char* filename)
{
  FILE* file;
  
  /* TODO: step 1, open the file */
  file = fopen(filename,"r");
  /* step 2: Check that file has been loaded correctly; show error otherwise*/
  if(file != NULL)
  {
    PPM* ppm;
    /* TODO: step 3, get the PPM data from the file with the getPPM function */
    ppm = getPPM(file);

    if(ppm == NULL)
    {
      /* TODO: step 4, display error if file cannot been parsed into a PPM struct*/
      printf("File cant be found or PPM isnt valid \n");
      exit(0);
    }

    /* TODO: step 5, close the file */
    fclose(file);

    /* step 6: return the PPM */
    return ppm;
  } 
  else
  {
    fclose(file);
    return NULL;
  }
}

/*
  Write a PPM to a file
*/
/*
 Not used
 */
void writePPMtoFile(PPM* ppm) 
{
  /* write the contents of the ppm to standard out (stdout) */

  /* TODO: write the headers first */

  /* TODO: then write the R G B pixel values */
}


/*
  Encodes text into red field of PPM
  Returns the encoded PPM
*/
PPM* encode(char* text, PPM* i)
{	
  /* TODO: Question 2c */
  srand(time(0));    // Randomize seed. Sets the seed to current time.

  int wid = 0;
  int len ,sum ,random;
  
  wid = i->width;     // Variable to store the width of the ppm image
  sum = 0;
  len = strlen(text);   // Varibale to store the lentgh of the text to be encoded

  

  for(int j = 0;j < len;)
  {
    random = rand() % 100;    // Generates a random number from 0 to 99
    sum += random;

    // Calculating the pixel in terms of row and column
    int row, column;
    row = sum / (wid);
    column = sum - (row * wid);

    Pixel *p = &(i->rgb[row][column]);    // Using memory address

    if(text[j] != p->red)   // Checks if the ascii value of the letter to be encoded is not equal to the red pixel.
    {
      p->red = text[j];     // The letter to be encoded is assigned to the red pixel
      j++;
    }
    else // Error message if the ascii value of the letter is equal to the red pixel
    {
      fprintf(stderr,"char [%j] - [%c] are the same. Retry.\n",j,text[j]);  
    }
  }
  return i;   // Return the encoded PPM
}


/*
  Compares 2 PPM structs and decodes the message hidden within
  Returns the decoded message if the images have the same dimensions
  Returns NULL otherwise
*/
char* decode(PPM* i1, PPM* i2)
{
  char* buffer = malloc(sizeof(char) * PPMMAX);               // Creating a buffer

  /* TODO: Question 2d */
  if(!(i1->height == i2->height && i1->width == i2->width))   // Checking if both files are the same
  {
    // If the dimensions of the file are same, then the files are the same
    printf("Files dont match. Try again\n");
    exit(0);
  }
  
  int a = 0;    // Counter

  for(int i = 0; i < i2->height;i++)    // For each row
  {
    for(int j = 0; j< i2->width;j++)    // For each column
    {
      if(i2->rgb[i][j].red != i1->rgb[i][j].red)  // Check if both file's red pixels are not equal
      {
        // Get the encoded characters
        buffer[a] = i2->rgb[i][j].red;           // Storing the encoded characters in a buffer array
        printf("%c",buffer[a]);
        a++;
      }
    }
  }

  //Return the decoded text

  int b = strlen(buffer) - 1;
  if(buffer[b] == '\n')
  {
    buffer[b] = '\0';
  }

  char* decodedMsg = malloc(sizeof(char) * (b+1));         // Variable to store the decoded message

  for(int i = 0;i < b+1;i++)
  {
    decodedMsg[i] = buffer[i];
  }
  
  free(buffer);     // Freeing the buffer
  return decodedMsg;
}


/* TODO: Question 3 */
int main(int argc, char* argv[])
{
  /* check arguments */
  switch(argc) 
  {
  case 2:
    /* TODO: not enough arguments, print an error message */
    if(argc < 3)
    {
      printf("Not enough arguements!\n");
      exit(0);
    }
    break;
  case 3:
    if(strcmp(argv[1], "e") == 0)   //Argument "e" - encode PPM
    { 
      PPM* ppm = loadPPMFromFile(argv[2]);
				
      /*Check that PPM is valid; continue if it is, exit otherwise */
      if(ppm != NULL)
      {
        PPM* encodedPPM;
        /* TODO: encode the ppm with the text and assign to encodedPPM */
        char encodedText[PPMMAX];
        fprintf(stderr, "Message to Encode-> ");
	      fgets(encodedText, PPMMAX, stdin);

	      int i = strlen(encodedText) - 1;
	      if(encodedText[i] == '\n')
        {
          encodedText[i] = '\0';
        }		      

        encodedPPM = encode(encodedText,ppm);
        fprintf(stderr, "encoding in progress...\n");

        /*Check that no error occured*/
        if(encodedPPM == NULL)
        {
          return EXIT_FAILURE;
        }
        else 
        {
          /* TODO: print a confirmation that encoding was successful */
          fprintf(stderr,"The message was encoded successfully!\n");

          /* TODO: print the PPM to standard output with showPPM */
          showPPM(encodedPPM);		

          return EXIT_SUCCESS;				
        }
      } 
      else 
      {					
        return EXIT_FAILURE;
      }
    } 
    else 
    {
      fprintf(stderr, "Unrecognised or incomplete command line.\n");
      return EXIT_FAILURE;			
    }
    break;
  case 4:
    if(strcmp(argv[1], "d") == 0)   //Argument "d" - decode PPM
    { 
      PPM* comparisonPPM;
      PPM* encodedPPM;

      /* TODO: get comparison file filename from argv, load it with
         loadPPMFromFile then assign to comparisonPPM */
         comparisonPPM = loadPPMFromFile(argv[2]);

      /* TODO: get encoded file filename from argv, load it with
         loadPPMFromFile then assign to encodedPPM */
					encodedPPM = loadPPMFromFile(argv[3]);
          
      /*Check both PPMs are valid; continue if so, exit otherwise */
      if(comparisonPPM != NULL && encodedPPM != NULL)
      {
        char* decodedMsg;
        
        /* TODO: decode the encodedPPM with the comparisonPPM and assign to decodedMsg */ 
        decodedMsg = decode(encodedPPM,comparisonPPM);
        				
        /*Check that the message has been decoded*/
        if(decodedMsg != NULL)
        {
          /* TODO: print a confirmation message that the message was decoded */ 
          printf("The message was decoded successfully!\n");
          fprintf(stderr, "Decoded message:\n");

          /* TODO: print the decoded message */
          printf(decodedMsg,"\n");

          return EXIT_SUCCESS;
        } 
        else 
        {
          return EXIT_FAILURE;
        }				
      } 
      else 
      {
        return EXIT_FAILURE;
      }			
    } 
    else 
    {
      fprintf(stderr, "Unrecognised or incomplete command line.\n\n");
      return EXIT_FAILURE;			
    }
    break;
  default:
    fprintf(stderr, "Unrecognised or incomplete command line.\n\n");
    return EXIT_FAILURE;
  }
}
