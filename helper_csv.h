/*
Puja Chaudhury
*/

#ifndef CVS_UTIL_H
#define CVS_UTIL_H

int append_image_data_csv(char *filename, char *image_filename, std::vector<float> &image_data, int reset_file = 0);

int read_image_data_csv(char *filename, std::vector<char *> &filenames, std::vector<std::vector<float>> &data, int echo_file = 0);

#endif
