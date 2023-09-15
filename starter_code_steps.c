#include <stdio.h>
#include <stdlib.h>

// Structure to hold grayscale pixel data
typedef struct {
    unsigned char value;
} PixelGray;

// Function to read a PGM image into a 2D array
PixelGray** readPGM(const char* filename, int* width, int* height) {
    // Open the file with the provided file name
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    // Read PGM header
    char magic[3];
    fscanf(file, "%2s", magic);
    if (magic[0] != 'P' || magic[1] != '5') {
        fprintf(stderr, "Not a valid PGM file\n");
        exit(1);
    }

    // Skip comments
    char c;
    while ((c = fgetc(file)) == '#') {
        while (fgetc(file) != '\n');
    }
    ungetc(c, file);

    // Read dimensions
    fscanf(file, "%d %d", width, height);

    // Read maximum pixel value
    int max_val;
    fscanf(file, "%d", &max_val);

    // Validate maximum pixel value
    if (max_val != 255) {
        fprintf(stderr, "Unsupported max pixel value. Only 8-bit PGM images are supported.\n");
        exit(1);
    }

    // Allocate memory for image data
    PixelGray** image = (PixelGray**)malloc(sizeof(PixelGray*) * (*height));
    for (int i = 0; i < *height; i++) {
        image[i] = (PixelGray*)malloc(sizeof(PixelGray) * (*width));
    }

    // Read image data
    for (int i = 0; i < *height; i++) {
        for (int j = 0; j < *width; j++) {
            image[i][j].value = fgetc(file);
        }
    }

    // Close the file
    fclose(file);

    return image;
}

// Function to write a 2D matrix as a PGM image
void writePGM(const char* filename, PixelGray** matrix, int* width, int* height) {
    // Create a new file with the file name provided for writing
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    // Write PGM header
    fprintf(file, "P5\n");
    fprintf(file, "%d %d\n", *width, *height);
    fprintf(file, "255\n");

    // Write image data
    for (int i = 0; i < *height; i++) {
        for (int j = 0; j < *width; j++) {
            fputc(matrix[i][j].value, file);
        }
    }

    // Close the file
    fclose(file);
}

// Function to threshold the image matrix
PixelGray** threshold(PixelGray** matrix, int* width, int* height) {
    // Create a new 2D matrix
    PixelGray** newMatrix = (PixelGray**)malloc(sizeof(PixelGray*) * (*height));
    for (int i = 0; i < *height; i++) {
        newMatrix[i] = (PixelGray*)malloc(sizeof(PixelGray) * (*width));
    }

    // Thresholding
    for (int i = 0; i < *height; i++) {
        for (int j = 0; j < *width; j++) {
            if (matrix[i][j].value > 80) {
                newMatrix[i][j].value = 255;
            } else {
                newMatrix[i][j].value = 0;
            }
        }
    }

    return newMatrix;
}

// Function to rotate the image matrix
PixelGray** rotate(PixelGray** matrix, int* width, int* height) {
    // Create a new 2D matrix
    PixelGray** newMatrix = (PixelGray**)malloc(sizeof(PixelGray*) * (*width));
    for (int i = 0; i < *width; i++) {
        newMatrix[i] = (PixelGray*)malloc(sizeof(PixelGray) * (*height));
    }

    // Rotate the original 2D matrix and store the result as a new matrix
    for (int i = 0; i < *height; i++) {
        for (int j = 0; j < *width; j++) {
            newMatrix[j][*height - 1 - i].value = matrix[i][j].value;
        }
    }

    // Update width and height for the new matrix
    int temp = *width;
    *width = *height;
    *height = temp;

    return newMatrix;
}

// Main function - DO NOT MODIFY
int main() {
    int width, height;  // variables to hold width and height. Use reference in other functions

    PixelGray** image_original = readPGM("lenna.pgm", &width, &height);
    // Now you have the grayscale image data in the 'image_original' 2D array

    // Access pixel data using image[row][col].value
    // For example, to access the pixel at row=2, col=3:
    // unsigned char pixel_value = image_original[2][3].value;

    // Create a new 2D array 'image_thresh' to store the threshold image
    PixelGray** image_thresh = threshold(image_original, &width, &height);
    // Write the image data as "threshold.pgm"
    writePGM("threshold.pgm", image_thresh, &width, &height);

    // Create a new 2D array 'image_rotate' to store the rotated image
    PixelGray** image_rotate = rotate(image_original, &width, &height);
    // Write the image data as "rotate.pgm"
    writePGM("rotate.pgm", image_rotate, &width, &height);

    image_rotate = rotate(image_rotate, &width, &height);
    // Write the image data as "rotate_again.pgm"
    writePGM("rotate_again.pgm", image_rotate, &width, &height);

    // Free the allocated memory when you're done
    for (int i = 0; i < height; ++i) {
        free(image_original[i]);
        free(image_thresh[i]);
        free(image_rotate[i]);
    }
    free(image_original);
    free(image_thresh);
    free(image_rotate);
    return 0;
}



