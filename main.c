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
    unsigned short int pixel[512][512][3];
    unsigned int width;
    unsigned int height;
} Image;

void gray_scale(Image* image) {
    for (unsigned int i = 0; i < image->height; ++i) {
        for (unsigned int j = 0; j < image->width; ++j) {
            int media = image->pixel[i][j][0] +
                        image->pixel[i][j][1] +
                        image->pixel[i][j][2];
            media /= 3;
            image->pixel[i][j][0] = media;
            image->pixel[i][j][1] = media;
            image->pixel[i][j][2] = media;
        }
    }
}

void blur(Image* image) {
    int tamanho = 0;
    scanf("%d", &tamanho);
    for (unsigned int i = 0; i < image->height; ++i) {
        for (unsigned int j = 0; j < image->width; ++j) {
            Pixel media = {0, 0, 0};

            int menor_h = (image->height - 1 > i + tamanho/2) ? i + tamanho/2 : image->height - 1;
            int min_w = (image->width - 1 > j + tamanho/2) ? j + tamanho/2 : image->width - 1;
            for(int x = (0 > i - tamanho/2 ? 0 : i - tamanho/2); x <= menor_h; ++x) {
                for(int y = (0 > j - tamanho/2 ? 0 : j - tamanho/2); y <= min_w; ++y) {
                    media.red += image->pixel[x][y][0];
                    media.green += image->pixel[x][y][1];
                    media.blue += image->pixel[x][y][2];
                }
            }

            // printf("%u", media.r)
            media.red /= tamanho * tamanho;
            media.green /= tamanho * tamanho;
            media.blue /= tamanho * tamanho;

            image->pixel[i][j][0] = media.red;
            image->pixel[i][j][1] = media.green;
            image->pixel[i][j][2] = media.blue;
        }
    }
}

void rotate_90_right(Image* image) {
    Image rotated_image;

    rotated_image.width = image->height;
    rotated_image.height = image->width;

    for (unsigned int i = 0, y = 0; i < rotated_image.height; ++i, ++y) {
        for (int j = rotated_image.width - 1, x = 0; j >= 0; --j, ++x) {
            rotated_image.pixel[i][j][0] = image->pixel[x][y][0];
            rotated_image.pixel[i][j][1] = image->pixel[x][y][1];
            rotated_image.pixel[i][j][2] = image->pixel[x][y][2];
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
            image->pixel[i][j][0] = 255 - image->pixel[i][j][0];
            image->pixel[i][j][1] = 255 - image->pixel[i][j][1];
            image->pixel[i][j][2] = 255 - image->pixel[i][j][2];
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
            cortada.pixel[i][j][0] = image->pixel[i + y][j + x][0];
            cortada.pixel[i][j][1] = image->pixel[i + y][j + x][1];
            cortada.pixel[i][j][2] = image->pixel[i + y][j + x][2];
        }
    }
    *image = cortada;
}

void sepia(Image* image) {
    for (unsigned int x = 0; x < image->height; ++x) {
        for (unsigned int j = 0; j < image->width; ++j) {
            unsigned short int pixel[3];
            pixel[0] = image->pixel[x][j][0];
            pixel[1] = image->pixel[x][j][1];
            pixel[2] = image->pixel[x][j][2];

            int p =  pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;
            int menor_r = (255 >  p) ? p : 255;
            image->pixel[x][j][0] = menor_r;

            p =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
            menor_r = (255 >  p) ? p : 255;
            image->pixel[x][j][1] = menor_r;

            p =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
            menor_r = (255 >  p) ? p : 255;
            image->pixel[x][j][2] = menor_r;
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
            aux1.red = image->pixel[i][j][0];
            aux1.green = image->pixel[i][j][1];
            aux1.blue = image->pixel[i][j][2];

            image->pixel[i][j][0] = image->pixel[x][y][0];
            image->pixel[i][j][1] = image->pixel[x][y][1];
            image->pixel[i][j][2] = image->pixel[x][y][2];

            image->pixel[x][y][0] = aux1.red;
            image->pixel[x][y][1] = aux1.green;
            image->pixel[x][y][2] = aux1.blue;
        }
    }
}


void print_pixels(Image image) {
    for (unsigned int i = 0; i < image.height; ++i) {
        for (unsigned int j = 0; j < image.width; ++j) {
            printf("%hu %hu %hu ", image.pixel[i][j][0],
                                   image.pixel[i][j][1],
                                   image.pixel[i][j][2]);

        }
        printf("\n");
    }
}

void print_image(Image image) {
    // print ppm image header
    printf("P3\n");
    printf("%u %u\n255\n", image.width, image.height);
    // print pixels of image
    print_pixels(image);
}

void read_pixels(Image* image) {
    for (unsigned int i = 0; i < image->height; ++i) {
        for (unsigned int j = 0; j < image->width; ++j) {
            scanf("%hu %hu %hu", &image->pixel[i][j][0],
                                 &image->pixel[i][j][1],
                                 &image->pixel[i][j][2]);

        }
    }
}

void read_image(Image* image) {
    // read image header
    char p3[4];
    scanf("%s", p3);
    int max_color;
    scanf("%u %u %d", &image->width, &image->height, &max_color);
    // read all pixels of image
    read_pixels(image);
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
