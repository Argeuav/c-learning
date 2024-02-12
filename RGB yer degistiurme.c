#include <stdio.h>
#include <stdlib.h>

typedef struct {// image yapisi ekliyoruz
    int width;
    int height;
    unsigned char *data;
} Image;

Image* readPPM(const char *filename) {// PPM dosyasini okuyor 
    FILE *file = fopen(filename, "rb");// pointer kullanarak file dosyasini tanimladik
    if (!file) {//eger degilse 
        printf("Dosya acilamadi.\n");
        exit(1);
    }
    char format[3];//dizi tanimliyoruyz 
    fscanf(file, "%s", format);
    if (format[0] != 'P' || format[1] != '6') {//PPM genellikle P6 seklinde olur ve bu bicimi saglayamazsa gecersiz olarak sayiyoruz
        printf("Gecersiz PPM \n");
        exit(1);// program sonlandir 
    }
    Image *image = (Image*)malloc(sizeof(Image));//dinamik bellek te yer ayirdik 
    if (!image) {
        printf("Bellek yeri acilamadi\n");
        exit(1);
    }
    fscanf(file, "%d %d", &image->width, &image->height);//max renk degerini belirtik ve maxColor a attik
    int maxColor;
    fscanf(file, "%d", &maxColor);
    fgetc(file);// bi karakter okur ve sonunu gosterir 
    image->data = (unsigned char*)malloc(3 * image->width * image->height * sizeof(unsigned char));// her pixelde 3 renk kanali vardir RGB red, green, blue bu yuzden 3 * image kadar yer ayirdik
    if (!image->data) {
        printf("Bellek yeri acilamadi\n");
        exit(1);
    }
    fread(image->data, sizeof(unsigned char), 3 * image->width * image->height, file);//bu kismi tam olarak anlayamadim 
    fclose(file);
    return image;
}
void writePPM(const char *filename, Image *image) {// PPM dosyasina yazan fonksiyon
    FILE *file = fopen(filename, "wb");//yazma modu wb
    if (!file) {//acilmazsa null ve !file bir olur
        printf("Dosya acilamadi.\n");
        exit(1);
    }
    fprintf(file, "P6\n");
    fprintf(file, "%d %d\n", image->width, image->height);//goruntu yuksekligi ve genisligi dosyaya yazar
    fprintf(file, "255\n");//PPM dosyalarinda renkler 0 ile 255 arasinda deger alir
    fwrite(image->data, sizeof(unsigned char), 3 * image->width * image->height, file);//goruntu verilerini dosyaya yazar
    fclose(file);
}
void swapRGB(Image *image) {// Goruntunun renk kanallarini yer degistiren fonksiyon
    for (int i = 0; i < image->width * image->height * 3; i += 3) {//Her pikselde üç renk kanali var bu yuzden i uc artar
        unsigned char r = image->data[i];
        unsigned char g = image->data[i + 1];
        unsigned char b = image->data[i + 2];
        // R, G ve B kanallarini yer degistir
        image->data[i] = g;
        image->data[i + 1] = b;
        image->data[i + 2] = r;
    }
}
int main() {
    const char *inputFilename = "input.ppm"; // giris dosya adi
    const char *outputFilename = "output.ppm"; // cikis dosya adi
    Image *image = readPPM(inputFilename); // giris goruntusunu oku
    swapRGB(image); // renk kanallarini yer degistir
    writePPM(outputFilename, image);// yer degistirilmis goruntuyu çikis dosyasina yaz
    free(image->data);// Bellekten sil
    free(image);
    printf("Renk kanallari yer degistrimis '%s' dosyasina aktrarildi.\n", outputFilename);
    return 0;
}

