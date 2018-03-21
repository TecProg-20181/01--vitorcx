#include <stdio.h>

typedef struct _pixel {
    unsigned short int red;
    unsigned short int green;
    unsigned short int blue;
} Pixel;

typedef struct _image {
    // [width][height][rgb]
    // 0 -> r
    // 1 -> g
    // 2 -> b
    Pixel pixel[512][512];
    unsigned int width;
    unsigned int height;
} Image;

unsigned int line;
unsigned int column;

int calculate_mean_rgb(Image* image) {
    int mean;
    int sum_rgb = image->pixel[line][column].red +
                  image->pixel[line][column].green +
                  image->pixel[line][column].blue;
    int pixels_amount = 3;
    mean = sum_rgb/pixels_amount;
    return mean;
}

void gray_scale(Image* image) {
    Pixel new_pixel_value;
    int mean;
    for (line = 0; line < image->height; ++line) {
        for (column = 0; column < image->width; ++column) {
            mean = calculate_mean_rgb(image);
            new_pixel_value.red = mean;
            new_pixel_value.green = mean;
            new_pixel_value.blue = mean;
            image->pixel[line][column].red = new_pixel_value.red;
            image->pixel[line][column].green = new_pixel_value.green;
            image->pixel[line][column].blue = new_pixel_value.blue;
        }
    }
}

int minimum_value(int value_a, int value_b) {
  if(value_a < value_b) {
      return value_a;
  } else {
      return value_b;
  }
}

void blur(Image* image) {
    int blur_amount = 0;
    scanf("%d", &blur_amount);

    int minimum_height;
    int minimum_width;
    for (unsigned int i = 0; i < image->height; ++i) {
        for (unsigned int j = 0; j < image->width; ++j) {
            Pixel media = {0, 0, 0};

            minimum_height = minimum_value(image->height - 1, i + blur_amount/2);
            minimum_width = minimum_value(image->width - 1, j + blur_amount/2);
            for(int x = (0 > i - blur_amount/2 ? 0 : i - blur_amount/2); x <= minimum_height; ++x) {
                for(int y = (0 > j - blur_amount/2 ? 0 : j - blur_amount/2); y <= minimum_width; ++y) {
                    media.red += image->pixel[x][y].red;
                    media.green += image->pixel[x][y].green;
                    media.blue += image->pixel[x][y].blue;
                }
            }

            media.red /= blur_amount * blur_amount;
            media.green /= blur_amount * blur_amount;
            media.blue /= blur_amount * blur_amount;

            image->pixel[i][j].red = media.red;
            image->pixel[i][j].green = media.green;
            image->pixel[i][j].blue = media.blue;
        }
    }
}

void rotate_90_right(Image* image) {
    Image rotated_image;

    rotated_image.width = image->height;
    rotated_image.height = image->width;

    for (unsigned int i = 0, y = 0; i < rotated_image.height; ++i, ++y) {
        for (int j = rotated_image.width - 1, x = 0; j >= 0; --j, ++x) {
            rotated_image.pixel[i][j].red = image->pixel[x][y].red;
            rotated_image.pixel[i][j].green = image->pixel[x][y].green;
            rotated_image.pixel[i][j].blue = image->pixel[x][y].blue;
        }
    }
    *image = rotated_image;
}

void rotate_90_right_multiple_times(Image* image) {
    int rotations_number;
    scanf("%d", &rotations_number);
    rotations_number =rotations_number % 4;
    for (int j = 0; j < rotations_number; ++j) {
        rotate_90_right(image);
    }
}

void color_inversion(Image* image) {
    for (unsigned int i = 0; i < image->height; ++i) {
        for (unsigned int j = 0; j < image->width; ++j) {
            image->pixel[i][j].red = 255 - image->pixel[i][j].red;
            image->pixel[i][j].green = 255 - image->pixel[i][j].green;
            image->pixel[i][j].blue = 255 - image->pixel[i][j].blue;
        }
    }
}

void image_cut(Image* image) {
    int x, y;
    scanf("%d %d", &x, &y);
    int w, h;
    scanf("%d %d", &w, &h);
    Image cortada;
    cortada.width = w;
    cortada.height = h;

    for(int i = 0; i < h; ++i) {
        for(int j = 0; j < w; ++j) {
            cortada.pixel[i][j].red = image->pixel[i + y][j + x].red;
            cortada.pixel[i][j].green = image->pixel[i + y][j + x].green;
            cortada.pixel[i][j].blue = image->pixel[i + y][j + x].blue;
        }
    }
    *image = cortada;
}

void sepia(Image* image) {
  int minimum_red;
  int p;
    for (unsigned int x = 0; x < image->height; ++x) {
        for (unsigned int j = 0; j < image->width; ++j) {
            Pixel pixel;
            pixel.red = image->pixel[x][j].red;
            pixel.green = image->pixel[x][j].green;
            pixel.blue = image->pixel[x][j].blue;

            p =  pixel.red * .393 + pixel.green * .769 + pixel.blue * .189;
            minimum_red = minimum_value(255, p);
            image->pixel[x][j].red = minimum_red;

            p =  pixel.red * .349 + pixel.green * .686 + pixel.blue * .168;
            minimum_red = minimum_value(255, p);
            image->pixel[x][j].green = minimum_red;

            p =  pixel.red * .272 + pixel.green * .534 + pixel.blue * .131;
            minimum_red = minimum_value(255, p);
            image->pixel[x][j].blue = minimum_red;
        }
    }
}

void swap_pixels(Pixel* pixels_a, Pixel* pixels_b) {

}

void vertical_mirroring(Image* image) {
    int horizontal = 0;
    scanf("%d", &horizontal);

    int w = image->width, h = image->height;

    if (horizontal == 1) w /= 2;
    else h /= 2;

    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            int x = i, y = j;

            if (horizontal == 1) y = image->width - 1 - j;
            else x = image->height - 1 - i;

            Pixel aux1;
            aux1.red = image->pixel[i][j].red;
            aux1.green = image->pixel[i][j].green;
            aux1.blue = image->pixel[i][j].blue;

            image->pixel[i][j].red = image->pixel[x][y].red;
            image->pixel[i][j].green = image->pixel[x][y].green;
            image->pixel[i][j].blue = image->pixel[x][y].blue;

            image->pixel[x][y].red = aux1.red;
            image->pixel[x][y].green = aux1.green;
            image->pixel[x][y].blue = aux1.blue;
        }
    }
}


void print_pixels(Image image) {
    for (unsigned int i = 0; i < image.height; ++i) {
        for (unsigned int j = 0; j < image.width; ++j) {
            printf("%hu %hu %hu ", image.pixel[i][j].red,
                                   image.pixel[i][j].green,
                                   image.pixel[i][j].blue);

        }
        printf("\n");
    }
}

void read_pixels(Image* image) {
    for (unsigned int i = 0; i < image->height; ++i) {
        for (unsigned int j = 0; j < image->width; ++j) {
            scanf("%hu %hu %hu", &image->pixel[i][j].red,
                                 &image->pixel[i][j].green,
                                 &image->pixel[i][j].blue);

        }
    }
}

void read_image_header(Image* image) {
    char p3[4];
    scanf("%s", p3);
    int max_color;
    scanf("%u %u %d", &image->width, &image->height, &max_color);
}

void read_image(Image* image) {
    read_image_header(image);
    read_pixels(image);
}


void print_image_header(Image image) {
    printf("P3\n");
    printf("%u %u\n255\n", image.width, image.height);
}

void print_image(Image image) {
    print_image_header(image);
    print_pixels(image);
}

int main() {
    Image image;

    read_image(&image);

    int n_opcoes;
    scanf("%d", &n_opcoes);

    for(int i = 0; i < n_opcoes; ++i) {
        int opcao;
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: { // Escala de Cinza
                gray_scale(&image);
                break;
            }
            case 2: { // Filtro Sepia
                sepia(&image);
                break;
            }
            case 3: { // Blur
                blur(&image);
                break;
            }
            case 4: { // Rotacao
                rotate_90_right_multiple_times(&image);
                break;
            }
            case 5: { // Espelhamento
                vertical_mirroring(&image);
                break;
            }
            case 6: { // Inversao de Cores
                color_inversion(&image);
                break;
            }
            case 7: { // Cortar Imagem
                image_cut(&image);
                break;
            }
        }
    }
    print_image(image);
    return 0;
}
