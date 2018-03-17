#include <stdio.h>

typedef struct _pixel {
    unsigned short int r;
    unsigned short int g;
    unsigned short int b;
} Pixel;

typedef struct _image {
    // [width][height][rgb]
    // 0 -> r
    // 1 -> g
    // 2 -> b
    unsigned short int pixel[512][512][3];
    unsigned int w;
    unsigned int h;
} Image;

void escala_de_cinza(Image* image) {
    for (unsigned int i = 0; i < image->h; ++i) {
        for (unsigned int j = 0; j < image->w; ++j) {
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
    for (unsigned int i = 0; i < image->h; ++i) {
        for (unsigned int j = 0; j < image->w; ++j) {
            Pixel media = {0, 0, 0};

            int menor_h = (image->h - 1 > i + tamanho/2) ? i + tamanho/2 : image->h - 1;
            int min_w = (image->w - 1 > j + tamanho/2) ? j + tamanho/2 : image->w - 1;
            for(int x = (0 > i - tamanho/2 ? 0 : i - tamanho/2); x <= menor_h; ++x) {
                for(int y = (0 > j - tamanho/2 ? 0 : j - tamanho/2); y <= min_w; ++y) {
                    media.r += image->pixel[x][y][0];
                    media.g += image->pixel[x][y][1];
                    media.b += image->pixel[x][y][2];
                }
            }

            // printf("%u", media.r)
            media.r /= tamanho * tamanho;
            media.g /= tamanho * tamanho;
            media.b /= tamanho * tamanho;

            image->pixel[i][j][0] = media.r;
            image->pixel[i][j][1] = media.g;
            image->pixel[i][j][2] = media.b;
        }
    }
}

Image rotacionar90direita(Image image) {
    Image rotacionada;

    rotacionada.w = image.h;
    rotacionada.h = image.w;

    for (unsigned int i = 0, y = 0; i < rotacionada.h; ++i, ++y) {
        for (int j = rotacionada.w - 1, x = 0; j >= 0; --j, ++x) {
            rotacionada.pixel[i][j][0] = image.pixel[x][y][0];
            rotacionada.pixel[i][j][1] = image.pixel[x][y][1];
            rotacionada.pixel[i][j][2] = image.pixel[x][y][2];
        }
    }

    return rotacionada;
}

void inverter_cores(Image* image) {
    for (unsigned int i = 0; i < image->h; ++i) {
        for (unsigned int j = 0; j < image->w; ++j) {
            image->pixel[i][j][0] = 255 - image->pixel[i][j][0];
            image->pixel[i][j][1] = 255 - image->pixel[i][j][1];
            image->pixel[i][j][2] = 255 - image->pixel[i][j][2];
        }
    }
}

void cortar_imagem(Image* image) {
    int x, y;
    scanf("%d %d", &x, &y);
    int w, h;
    scanf("%d %d", &w, &h);
    Image cortada;
    cortada.w = w;
    cortada.h = h;

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
    for (unsigned int x = 0; x < image->h; ++x) {
        for (unsigned int j = 0; j < image->w; ++j) {
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

void espelhamento_vertical(Image* image) {
    int horizontal = 0;
    scanf("%d", &horizontal);

    int w = image->w, h = image->h;

    if (horizontal == 1) w /= 2;
    else h /= 2;

    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            int x = i, y = j;

            if (horizontal == 1) y = image->w - 1 - j;
            else x = image->h - 1 - i;

            Pixel aux1;
            aux1.r = image->pixel[i][j][0];
            aux1.g = image->pixel[i][j][1];
            aux1.b = image->pixel[i][j][2];

            image->pixel[i][j][0] = image->pixel[x][y][0];
            image->pixel[i][j][1] = image->pixel[x][y][1];
            image->pixel[i][j][2] = image->pixel[x][y][2];

            image->pixel[x][y][0] = aux1.r;
            image->pixel[x][y][1] = aux1.g;
            image->pixel[x][y][2] = aux1.b;
        }
    }
}


void imprimir_pixel(Image image) {
    for (unsigned int i = 0; i < image.h; ++i) {
        for (unsigned int j = 0; j < image.w; ++j) {
            printf("%hu %hu %hu ", image.pixel[i][j][0],
                                   image.pixel[i][j][1],
                                   image.pixel[i][j][2]);

        }
        printf("\n");
    }
}

void imprimir_imagem(Image image) {
    // print type of image
    printf("P3\n");
    // print width height and color of image
    printf("%u %u\n255\n", image.w, image.h);
    // print pixels of image
    imprimir_pixel(image);
}

void ler_pixel(Image* image) {
    for (unsigned int i = 0; i < image->h; ++i) {
        for (unsigned int j = 0; j < image->w; ++j) {
            scanf("%hu %hu %hu", &image->pixel[i][j][0],
                                 &image->pixel[i][j][1],
                                 &image->pixel[i][j][2]);

        }
    }
}


int main() {
    Image image;

    // read type of image
    char p3[4];
    scanf("%s", p3);

    // read width height and color of image
    int max_color;
    scanf("%u %u %d", &image.w, &image.h, &max_color);

    // read all pixels of image
    ler_pixel(&image);

    int n_opcoes;
    scanf("%d", &n_opcoes);

    for(int i = 0; i < n_opcoes; ++i) {
        int opcao;
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: { // Escala de Cinza
                escala_de_cinza(&image);
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
                int quantas_vezes = 0;
                scanf("%d", &quantas_vezes);
                quantas_vezes %= 4;
                for (int j = 0; j < quantas_vezes; ++j) {
                    image = rotacionar90direita(image);
                }
                break;
            }
            case 5: { // Espelhamento
                espelhamento_vertical(&image);
                break;
            }
            case 6: { // Inversao de Cores
                inverter_cores(&image);
                break;
            }
            case 7: { // Cortar Imagem
                cortar_imagem(&image);
                break;
            }
        }
    }
    imprimir_imagem(image);
    return 0;
}
