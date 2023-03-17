/*
Puja Chaudhury
helper_csv.cpp

"These C++ functions are designed to read CSV files with a particular structure,
where the first column contains a filename or path as a string,
and all subsequent columns contain numerical data. 
The functions return a vector of char* containing the filenames, 
and a 2D vector of floats containing the data."
*/

#include <cstdio>
#include <cstring>
#include <vector>
#include "opencv2/opencv.hpp"

/*
This function reads a null-terminated string from a CSV file and stores it in a char array os. 
If the string is successfully read, the function returns false. 
If the end of the line or file is reached before the string is fully read, the function returns true.
 */
int getstring( FILE *fp, char os[] ) {
  int p = 0;
  int eol = 0;
  
  for(;;) {
    char ch = fgetc( fp );
    if( ch == ',' ) {
      break;
    }
    else if( ch == '\n' || ch == EOF ) {
      eol = 1;
      break;
    }
    os[p] = ch;
    p++;
  }
  os[p] = '\0';

  return(eol); 
}

int getint(FILE *fp, int *v) {
  char s[256];
  int p = 0;
  int eol = 0;

  for(;;) {
    char ch = fgetc( fp );
    if( ch == ',') {
      break;
    }
    else if(ch == '\n' || ch == EOF) {
      eol = 1;
      break;
    }
      
    s[p] = ch;
    p++;
  }
  s[p] = '\0'; 
  *v = atoi(s);

  return(eol); 
}

/*
This is a utility function that reads a single floating-point value from a CSV file and stores it in the v parameter. 
The function returns true when it reaches the end of a line or the end of the file.
 */
int getfloat(FILE *fp, float *v) {
  char s[256];
  int p = 0;
  int eol = 0;

  for(;;) {
    char ch = fgetc( fp );
    if( ch == ',') {
      break;
    }
    else if(ch == '\n' || ch == EOF) {
      eol = 1;
      break;
    }
      
    s[p] = ch;
    p++;
  }
  s[p] = '\0'; 
  *v = atof(s);

  return(eol); 
}

/*
This function takes a filename, an image filename, and image features as input.
By default, the function appends a new line of data in CSV format to the file. 
If reset_file is set to true, the function will open the file in 'write' mode and clear its existing contents.
The image filename is written to the first position in the row of data, followed by the values in image_data as floats. 
If an error occurs, the function returns a non-zero value.
 */
int append_image_data_csv( char *filename, char *image_filename, std::vector<float> &image_data, int reset_file ) {
  char buffer[256];
  char mode[8];
  FILE *fp;

  strcpy(mode, "a");

  if( reset_file ) {
    strcpy( mode, "w" );
  }
  
  fp = fopen( filename, mode );
  if(!fp) {
    printf("Unable to open output file %s\n", filename );
    exit(-1);
  }

  strcpy(buffer, image_filename);
  std::fwrite(buffer, sizeof(char), strlen(buffer), fp );
  for(int i=0;i<image_data.size();i++) {
    char tmp[256];
    sprintf(tmp, ",%.4f", image_data[i] );
    std::fwrite(tmp, sizeof(char), strlen(tmp), fp );
  }
      
  std::fwrite("\n", sizeof(char), 1, fp); // EOL

  fclose(fp);
  
  return(0);
}

/*
This function is designed to parse a file in CSV format, 
where the first column contains strings and the remaining columns contain floating point numbers. 
The function returns a std::vector of character arrays containing the filenames, 
and a 2D std::vector of floats containing the features calculated from each image.

If echo_file is set to true, 
the function will print the file contents as they are read into memory. 
The function will return a non-zero value if an error occurs during processing.
 */
int read_image_data_csv( char *filename, std::vector<char *> &filenames, std::vector<std::vector<float>> &data, int echo_file ) {
  FILE *fp;
  float fval;
  char img_file[256];

  fp = fopen(filename, "r");
  if( !fp ) {
    printf("Unable to open feature file\n");
    return(-1);
  }

  printf("Reading %s...\n", filename);
  for(;;) {
    std::vector<float> dvec;
    
    

    if( getstring( fp, img_file ) ) {
      break;
    }

    for(;;) {
  
      float eol = getfloat( fp, &fval );
      dvec.push_back( fval );
      if( eol ) break;
    }

    data.push_back(dvec);

    char *fname = new char[strlen(img_file)+1];
    strcpy(fname, img_file);
    filenames.push_back( fname );
  }
  fclose(fp);
  printf("...finished reading CSV file\n");

  if(echo_file) {
    for(int i=0;i<data.size();i++) {
      for(int j=0;j<data[i].size();j++) {
	printf("%.4f  ", data[i][j] );
      }
      printf("\n");
    }
    printf("\n");
  }

  return(0);
}
