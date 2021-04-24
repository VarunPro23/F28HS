# Who I am

- Name: Varun Senthil Kumar
- Student ID: H00332328 
- Student username: vs55

# Program design
In my program, I have implemented three helper functions, namely readLine, readCom and readPixels. readLine was taken from Lab 3 code while I referenced the internet to implement readCom and readPixels. I have also implemented the four functions asked for in the coursework specifications.
I have used three structures to implement my code.

The program takes in a message from the user which is to be encoded. Then the user prompts the program to decode the ppm file which then returns the encoded message.

Encode:

./steg e img1.ppm > img1-out.ppm

Decode:                               

./steg d img1.ppm img1-out.ppm                


# Choice of data structures and algorithms
**struct comment**

Two structure variables are created. Both are pointers. valueOfNode is of type char while nextNode is of type _comment_. It is used as a Link List, to hold comments.

**struct Pixel**

Three structure variables are created. All are of type int. It is used to store the rgb values of the ppm file.

**struct PPM**

Seven structure variables are created. Three pointers are created, one of char type, one of type comment and one of type Pixel. Teh other four are of type int, used to store the measurements of the ppm file.

**char * readLine**

Reads the line containing text. Takes a file as argument. Its a helper function.

**void readCom**

Reads the comments using readLine. Takes in two arguments, a file and a PPM pointer.Its a helper function.

**void readPixels**

Reads the pixels in a PPM file. Takes in two arguments, a file and a PPM pointer.Its a helper function.

***PPM * getPPM(file *fd)****

Returns the ppm from the file.

The function returns the ppm image's details and takes in a file as an argument. It uses readCom and readPixels to get the details. Deatils include height,width,maxcolor and comments. Before getting these details, the function checks if the type of the image is P3, otherwise it will exit the program. The comments are stored in the struct comment, in a linked list. The function returns the image as struct PPM.

_**void showPPM(PPM * ppm)**_

The function takes in a PPM pointer. It prints out the comments from the struct comment,type, height, width, maxcolor and the rgb values of the PPM image.

`PPM* encode(char* text,PPM* i)`

This function takes in the text to be encoded and PPM pointer as arguments. It encodes the text into the red field of the PPM and returns the encoded ppm. srand() is used to set the seed to current time.

The function checks if the file can be encoded and only then proceeds further. Then, it calculates the pixel address in terms of row and column. After that, the function checks if the ascii value of the letter encoded is not equal to the red pixel's value.Then,the text is encoded into successive red fields of the ppm image. The function returns the PPM with the encoded text.

`char* decode(PPM* i1, PPM* i2)`

This fucntion takes in two PPM images as arguments. It compares the two PPM structs and then decodes the message if both the PPM structs are the same. It checks if both the PPM struct have similar dimensions,otherwise returns NULL.

The function checks if the red pixels in both the PPM structs are not equal, which then means that there is some text encoded. First the text is decoded into a buffer array. Then it is assigned to another variable to store the decoded message. The function then returns the decoded message.

****PPM* loadPPMFromFile(char* filename)**

This function load a file and then returns it. It checks if the file has been loaded correctly. If the file cannot be parsed into a PPM struct, then it return NULL.

`int main(int argc, char *argv[])`

This is the main function. It uses switch case algorithm. In the first case, if there aren't enough arguements, then it exits the program. If "e" is given as an argument, then it does the process for encoding. If "d" is given as an argument, then it does the process for encoding.



