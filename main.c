#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct yzr{
    char ad[20], soyad[20];
    int yazarID;
    struct yzr *next;
}YAZAR;

typedef struct ogr{
    char ad[20], soyad[20];
    int studentID, puan, kitapSayi;
    struct ogr *next;
    struct ogr *prev;
}OGR;

typedef struct kitapOrnek{
    char etiketNo[20]; // ISBN_1 -> ISBN_2
    char durum[10]; // rafta ya da ogrenciNo
    int flag;
    struct kitapOrnek *next;
}KitapOrnek;

typedef struct kitap{
    char ad[20], isbnNo[14];
    int adet;
    KitapOrnek *ornek;
    struct kitap *next;
}KITAP;

typedef struct{
    char isbn[14];
    int yazarID;
}KitapYazar;

typedef struct{
    int gun, ay, yil; //işlem tarihi
    int islem; //ödünç al ya da teslim et (0/1) 
    KITAP* kitap;
    OGR* ogrenci;
}ODUNC;

//Fonksiyon prototipleri
void anaMenu(OGR**, KITAP**, YAZAR**, int*, KitapYazar**, int*); //Bitti
void ogrenciMenu(OGR**, KITAP*); //Bitti
void kitapMenu(KITAP**, YAZAR*, KitapYazar**, int*); //Bitti
void yazarMenu(YAZAR**, KITAP*, int*); //Bitti
void dosyadanYukle(OGR**, KITAP**, YAZAR**, KitapYazar**, int*, int*);

YAZAR* yazarOlustur(int*); //Bitti
YAZAR* yazarBul(YAZAR*, int); //Bitti
int yazarSil(YAZAR**, int); //Bitti
void yazarGuncelle(YAZAR*, int); //Bitti
void yazarGoruntule(YAZAR*, KITAP*, int); //Bitti
void yazarListe(YAZAR**, YAZAR*); //Bitti

OGR* ogrenciOlustur(); //Bitti
OGR* ogrenciBul(OGR*, int); //Bitti
int ogrenciSil(OGR**, int, KITAP*); //Bitti
void ogrenciGuncelle(OGR*, int); //Bitti
void ogrenciGoruntule(OGR*, int); //Bitti
void teslimEtmeyenOgrenciler(OGR*);
void cezaliOgrenciler(OGR*); //Bitti
void tumOgrenciler(OGR*); //Bitti
void ogrenciListe(OGR**, OGR*); //Bitti

KITAP* kitapOlustur(); //Bitti
KITAP* kitapBul(KITAP*, char*); //Bitti
int kitapSil(KITAP**, char*); //Bitti
void kitapGuncelle(KITAP*, char*); //Bitti
void kitapGoruntule(KITAP*, char*); //Bitti
void raftaKitaplar(KITAP*); //Bitti
void zamanindaTeslimOlmayan(KITAP*); //Bitti
void kitapListe(KITAP**, KITAP*); //Bitti

void kitapYazarEsle(KitapYazar**, YAZAR*, KITAP*, int*); //Kitap ve yazar eşlemesi yapmayı sağlar, dizide saklar ve dosyaya bilgileri yazar.
void kitapYazarGuncelle(KitapYazar**, YAZAR*, KITAP*, int*);
void kitapOdunc(KITAP*, OGR*);
int oduncCtrl(KITAP*, OGR*);
int alimTarihi(KITAP*, OGR*);
void kitapOrnekDurumGuncelle(char*, char*);
void freeBellek(OGR*, KITAP*, YAZAR*);

int main(){
    OGR* ogrHead = NULL;
    KITAP* kitapHead = NULL;
    YAZAR* yazarHead = NULL;
    KitapYazar* esleme = NULL;
    int yazarNum = 1;
    int satir = 1;
    dosyadanYukle(&ogrHead, &kitapHead, &yazarHead, &esleme, &yazarNum, &satir);
    anaMenu(&ogrHead, &kitapHead, &yazarHead, &yazarNum, &esleme, &satir);

    free(esleme);
    freeBellek(ogrHead, kitapHead, yazarHead);
    return 0;
}

void anaMenu(OGR** ogrHead, KITAP** kitapHead, YAZAR** yazarHead, int* yazarNum, KitapYazar** esleme, int* satir){
    int choice;
    printf("***Hosgeldiniz***\n");
    do{
        printf("1. Ogrenci islemleri\n2. Kitap islemleri\n3. Yazar islemleri\n0. Cikis\nHangi islemi yapmak istiyorsunuz: ");
        scanf("%d", &choice);
        switch(choice){
        case 1:
            ogrenciMenu(ogrHead, *kitapHead);
            break;
        case 2:
            kitapMenu(kitapHead, (*yazarHead), esleme, satir);
            break;
        case 3:
            yazarMenu(yazarHead, (*kitapHead), yazarNum);
            break;
        default:
            break;
        }
    }while(choice != 0);
    printf("Program kapatiliyor...");
}
void ogrenciMenu(OGR** ogrHead, KITAP* kitapHead){
    int choice, studentID;
    char isbn[14];
    OGR* ogrenci = NULL;
    KITAP* kitap = NULL;
    printf("Ogrenci Islemleri:\n");
    do{
        printf("1. Ogrenci ekle\n2. Ogrenci sil\n3. Ogrenci guncelle\n4. Ogrenci bilgisi goruntule\n5. Kitap teslim etmeyen ogrencileri listele\n");
        printf("6. Cezali ogrencileri listele\n7. Tum ogrencileri listele\n8. Kitap odunc al/teslim et\n0. Ana menu\nHangi islemi yapmak istiyorsunuz: ");
        scanf("%d", &choice);
        switch(choice){
        case 1:
            ogrenci = ogrenciOlustur();
            ogrenciListe(ogrHead, ogrenci);
            break;
        case 2:
            printf("Silmek istediginiz ogrencinin numarasini giriniz: ");
            scanf("%d", &studentID);
            if(ogrenciSil(ogrHead, studentID, kitapHead) == 1){
                printf("Ogrenci silindi.\n");
            }
            else{
                printf("Ogrenci bulunamadi.\n");
            }
            break;
        case 3:
            printf("Guncellemek istediginiz ogrencinin numarasini giriniz: ");
            scanf("%d", &studentID);
            ogrenciGuncelle(*ogrHead, studentID);
            break;
        case 4:
            printf("Bilgilerini gormek istediginiz ogrencinin numarasini giriniz: ");
            scanf("%d", &studentID);
            ogrenciGoruntule(*ogrHead, studentID);
            break;
        case 5:
            teslimEtmeyenOgrenciler(*ogrHead);
            break;
        case 6:
            cezaliOgrenciler(*ogrHead);
            break;
        case 7:
            tumOgrenciler(*ogrHead);
            break;
        case 8:
            printf("Islem yapmak istediginiz ogrencinin numarasini giriniz: ");
            scanf("%d", &studentID);
            printf("Islem yapmak istediginiz kitabin ISBN numarasini giriniz: ");
            scanf("%s", isbn);
            ogrenci = ogrenciBul(*ogrHead, studentID);
            kitap = kitapBul(kitapHead, isbn);
            if(ogrenci == NULL || kitap == NULL){
                printf("Ogrenci veya kitap bulunamadi!\n");
            }
            else{
                kitapOdunc(kitap, ogrenci);
            }
            break;
        case 0:
            printf("Ana menuye donuluyor.\n");
            break;
        default:
            break;
        }
    }while(choice != 0);
}
void kitapMenu(KITAP** kitapHead, YAZAR* yazarHead, KitapYazar** esleme, int* satir){
    int choice;
    char isbn[14];
    printf("Kitap Islemleri:\n");
    do{
        printf("1. Kitap ekle\n2. Kitap sil\n3. Kitap guncelle\n4. Kitap bilgisi goruntule\n5. Raftaki kitaplari listele\n");
        printf("6. Zamaninda teslim edilmeyen kitaplari listele\n7. Kitap yazar eslestir\n8. Kitabin yazarini guncelle\n0. Ana menu\nHangi islemi yapmak istiyorsunuz: ");
        scanf("%d", &choice);
        switch(choice){
        case 1:
            KITAP* kitap = kitapOlustur();
            kitapListe(kitapHead, kitap);
            break;
        case 2:
            printf("Silmek istediginiz kitabin ISBN numarasini giriniz: ");
            scanf("%s", &isbn);
            if(kitapSil(kitapHead, isbn) == 1){
                printf("Kitap silindi.\n");
            }
            else{
                printf("Kitap bulunamadi.\n");
            }
            break;
        case 3:
            printf("Guncellemek istediginiz kitabin ISBN numarasini giriniz: ");
            scanf("%s", isbn);
            kitapGuncelle(*kitapHead, isbn);
            break;
        case 4:
            printf("Bilgilerini gormek istediginiz kitabin ISBN numarasini giriniz: ");
            scanf("%s", isbn);
            kitapGoruntule(*kitapHead, isbn);
            break;
        case 5:
            raftaKitaplar(*kitapHead);
            break;
        case 6:
            zamanindaTeslimOlmayan(*kitapHead);
            break;
        case 7:
            kitapYazarEsle(esleme, yazarHead, *kitapHead, satir);
            break;
        case 8:
            kitapYazarGuncelle(esleme, yazarHead, *kitapHead, satir);
            break;
        case 0:
            printf("Ana menuye donuluyor.\n");
        default:
            break;
        }
    }while(choice != 0);
}
void yazarMenu(YAZAR** yazarHead, KITAP* kitapHead, int* yazarNum){
    int choice, yazarID;
    printf("Yazar Islemleri:\n");
    do{
        printf("1. Yazar ekle\n2. Yazar sil\n3. Yazar guncelle\n4. Yazar bilgisi goruntule\n0. Ana menu\nHangi islemi yapmak istiyorsunuz: ");
        scanf("%d", &choice);
        switch(choice){
        case 1:
            YAZAR* yazar = yazarOlustur(yazarNum);
            yazarListe(yazarHead, yazar);
            break;
        case 2:
            printf("Silmek istediginiz yazarin ID numarasini giriniz: ");
            scanf("%d", &yazarID);
            if(yazarSil(yazarHead, yazarID) == 1){
                printf("Yazar silindi.\n");
            }
            else{
                printf("Yazar bulunamadi.\n");
            }
            break;
        case 3:
            printf("Guncellemek istediginiz yazarin ID numarasini giriniz: ");
            scanf("%d", &yazarID);
            yazarGuncelle(*yazarHead, yazarID);
            break;
        case 4:
            printf("Bilgilerini gormek istediginiz yazarin ID numarasini giriniz: ");
            scanf("%d", &yazarID);
            yazarGoruntule(*yazarHead, kitapHead, yazarID);
            break;
        case 0:
            printf("Ana menuye donuluyor.\n");
            break;
        default:
            break;
        }
    }while(choice != 0);
}
void dosyadanYukle(OGR** ogrenci, KITAP** kitap, YAZAR** yazar, KitapYazar** esleme, int* yazarNum, int* satir){
    FILE* fp1;
    char line[150];
    char etiketNo[16];
    char durum[10];
    char isbn[14];
    char tarih[15];
    int i, index, ogrID, bulundu;
    int ogrIDcsv, islem, gun, ay, yil;
    char etiketNocsv[20];
    char line1[200], line2[200];
    int ogrID1, ogrID2, islem1, islem2;
    int gun1, ay1, yil1, gun2, ay2, yil2;
    char etiketNo1[20], etiketNo2[20];
    // Yazarları oku
    fp1 = fopen("data/yazarlar.csv", "r");
    if (fp1 != NULL) {
        while (fgets(line, sizeof(line), fp1)) {
            YAZAR* yeni = (YAZAR*)malloc(sizeof(YAZAR));
            if(yeni != NULL){
                sscanf(line, "%[^,],%[^,],%d", yeni->ad, yeni->soyad, &yeni->yazarID);
                yeni->next = NULL;
                
                if(*yazar == NULL){
                    *yazar = yeni;
                }
                else{
                    YAZAR* curr = *yazar;
                    while(curr->next != NULL){
                        curr = curr->next;
                    }
                    curr->next = yeni;
                }
                
                if(yeni->yazarID >= *yazarNum){
                    *yazarNum = yeni->yazarID + 1;
                }
            }
            else{
                printf("Bellek ayirma hatasi!\n");
            }
        }
        fclose(fp1);
    }
    // Öğrencileri oku
    fp1 = fopen("data/ogrenciler.csv", "r");
    if(fp1 != NULL){
        while(fgets(line, sizeof(line), fp1)){
            OGR* yeni = (OGR*)malloc(sizeof(OGR));
            OGR* curr = *ogrenci;
            if(yeni != NULL){
                sscanf(line, "%[^,],%[^,],%d,%d", yeni->ad, yeni->soyad, &yeni->studentID, &yeni->puan);
                yeni->kitapSayi = 0; // Ödünç dosyasından hesaplanacak
                yeni->next = NULL;
                
                if(*ogrenci == NULL){
                    *ogrenci = yeni;
                    yeni->prev = NULL;
                    yeni->next = NULL;
                }
                else{
                    while(curr->next != NULL){
                        curr = curr->next;
                    }
                    curr->next = yeni;
                    yeni->prev = curr;
                    yeni->next = NULL;
                }
            }
            else{
                printf("Bellek ayirma hatasi!\n");
            }
        }
        fclose(fp1);
    }
    // Kitapları oku
    fp1 = fopen("data/kitaplar.csv", "r");
    if(fp1 != NULL){
        while(fgets(line, sizeof(line), fp1)){
            KITAP* yeni = (KITAP*)malloc(sizeof(KITAP));
            if(yeni != NULL){
                yeni->ornek = NULL;
                yeni->next = NULL;
                
                sscanf(line, "%[^,],%[^,],%d", yeni->ad, yeni->isbnNo, &yeni->adet);
                
                if(*kitap == NULL){
                    *kitap = yeni;
                }
                else{
                    KITAP* curr = *kitap;
                    while (curr->next != NULL) {
                        curr = curr->next;
                    }
                    curr->next = yeni;
                }
            }
            else{
                printf("Bellek ayirma hatasi!\n");
            }
        }
        fclose(fp1);
    }
    // Kitap örneklerini oku
    fp1 = fopen("data/kitapOrnekleri.csv", "r");
    if (fp1 != NULL) {
        while (fgets(line, sizeof(line), fp1)) {
            sscanf(line, "%[^,],%s", etiketNo, durum);
            sscanf(etiketNo, "%[^_]_%d", isbn, &index);
            KITAP* kitap1 = kitapBul(*kitap, isbn);
            if (kitap1 != NULL) {
                KitapOrnek* yeni = (KitapOrnek*)malloc(sizeof(KitapOrnek));
                if (yeni != NULL) {
                    strcpy(yeni->etiketNo, etiketNo);
                    strcpy(yeni->durum, durum);
                    yeni->flag = 1; // Başlangıçta zamanında teslim edilmiş kabul et
                    yeni->next = NULL;
                    if (kitap1->ornek == NULL) {
                        kitap1->ornek = yeni;
                    } else {
                        KitapOrnek* curr = kitap1->ornek;
                        while (curr->next != NULL) curr = curr->next;
                        curr->next = yeni;
                    }
                }
            }
        }
        fclose(fp1);
    }
    fp1 = fopen("data/kitapYazar.csv", "r");
    if(fp1 != NULL){
        // Satır sayısını bul
        *satir = 0;
        while(fgets(line, sizeof(line), fp1)){
            (*satir)++;
        }
        rewind(fp1);
        
        // Bellek ayır
        *esleme = (KitapYazar*)malloc(*satir * sizeof(KitapYazar));
        if(*esleme != NULL){
            // Verileri oku
            i = 0;
            while(fgets(line, sizeof(line), fp1) && i < *satir){
                sscanf(line, "%[^,],%d", (*esleme)[i].isbn, &(*esleme)[i].yazarID);
                i++;
            }
        } else {
            printf("Bellek ayirma hatasi!\n");
        }
        fclose(fp1);
    }
    
    // Kitap Ödünç bilgilerini oku ve kitap sayılarını hesapla
    fp1 = fopen("data/kitapOdunc.csv", "r");
    if (fp1 != NULL) {
        // Her öğrenci için şu anda elindeki kitap sayısını hesapla
        
        while(fgets(line, sizeof(line), fp1)){
            sscanf(line, "%[^,],%d,%d,%d.%d.%d", etiketNocsv, &ogrIDcsv, &islem, &gun, &ay, &yil);
            
            OGR* ogrenci1 = ogrenciBul(*ogrenci, ogrIDcsv);
            if (ogrenci1 != NULL) {
                if(islem == 0) { // Kitap ödünç alma
                    ogrenci1->kitapSayi++;
                } else if(islem == 1) { // Kitap teslim etme
                    ogrenci1->kitapSayi--;
                    if(ogrenci1->kitapSayi < 0) ogrenci1->kitapSayi = 0; // Negatif olmasını engelle
                }
            }
        }
        fclose(fp1);
    }
    
    // Geç teslim edilen kitapları kontrol et ve flag'leri güncelle
    fp1 = fopen("data/kitapOdunc.csv", "r");
    if (fp1 != NULL) {
        
        rewind(fp1);
        while(fgets(line1, sizeof(line1), fp1)) {
            sscanf(line1, "%[^,],%d,%d,%d.%d.%d", etiketNo1, &ogrID1, &islem1, &gun1, &ay1, &yil1);
            
            if(islem1 == 0) { // Ödünç alma kaydı
                FILE* fp2 = fopen("data/kitapOdunc.csv", "r");
                int bulundu = 0;
                
                while(fgets(line2, sizeof(line2), fp2) && !bulundu) {
                    sscanf(line2, "%[^,],%d,%d,%d.%d.%d", etiketNo2, &ogrID2, &islem2, &gun2, &ay2, &yil2);
                    
                    // Aynı etiket ve öğrenci, ama teslim kaydı
                    if(strcmp(etiketNo1, etiketNo2) == 0 && ogrID1 == ogrID2 && islem2 == 1) {
                        int alimTarih = 365 * yil1 + 30 * (ay1-1) + gun1;
                        int teslimTarih = 365 * yil2 + 30 * (ay2-1) + gun2;
                        
                        if(teslimTarih - alimTarih > 15) {
                            // Geç teslim edilmiş, flag'i güncelle
                            sscanf(etiketNo1, "%[^_]_%d", isbn, &index);
                            KITAP* kitap1 = kitapBul(*kitap, isbn);
                            if(kitap1 != NULL) {
                                KitapOrnek* ornek = kitap1->ornek;
                                while(ornek != NULL) {
                                    if(strcmp(ornek->etiketNo, etiketNo1) == 0) {
                                        ornek->flag = 0;
                                        break;
                                    }
                                    ornek = ornek->next;
                                }
                            }
                        }
                        bulundu = 1;
                    }
                }
                fclose(fp2);
            }
        }
        fclose(fp1);
    }
}

YAZAR* yazarOlustur(int* yazarID){
    YAZAR* yazar = (YAZAR*)malloc(sizeof(YAZAR));
    printf("Yazarin adini giriniz: ");
    getchar();
    fgets(yazar->ad, sizeof(yazar->ad), stdin); //Boşluklu string almak için
    yazar->ad[strcspn(yazar->ad, "\n")] = '\0';
    printf("Yazarin soyadini giriniz: ");
    scanf("%s", yazar->soyad);
    yazar->yazarID = *yazarID;
    *yazarID += 1;
    yazar->next = NULL;
    return yazar;
}
YAZAR* yazarBul(YAZAR* head, int yazarID){
    YAZAR* curr = head;

    while(curr != NULL){
        if(curr->yazarID == yazarID){
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}
int yazarSil(YAZAR** head, int yazarID){
    YAZAR* curr = *head;
    YAZAR* prev = NULL;
    FILE* fp1, *fp2;
    int idFile;
    char satir[150], etiketNo[20];;
    while(curr != NULL && curr->yazarID != yazarID){
        prev = curr;
        curr = curr->next;
    }
    if(curr == NULL){
        printf("Yazar bulunamadi.\n");
        return 0;
    }
    //Linked list'ten silme
    if(prev == NULL){
        *head = curr->next;
    }
    else{
        prev->next = curr->next;
    }
    free(curr);
    //yazarlar.csv dosyasından silme
    fp1 = fopen("data/yazarlar.csv", "r");
    fp2 = fopen("data/yazarlar_temp.csv", "w");
    while(fgets(satir, sizeof(satir), fp1)){
        sscanf(satir, "%*[^,],%*[^,],%d", &idFile);
        if(idFile != yazarID){    
            fputs(satir, fp2);
        }
    }
    fclose(fp1), fclose(fp2);
    remove("data/yazarlar.csv");
    rename("data/yazarlar_temp.csv", "data/yazarlar.csv");
    fp1 = fopen("data/kitapYazar.csv", "r");
    fp2 = fopen("data/kitapYazar_temp.csv", "w");

    while(fgets(satir, sizeof(satir), fp1)){
        sscanf(satir, "%[^,],%d", etiketNo, &idFile);
        if(idFile == yazarID){
            fprintf(fp2, "%s,-1\n", etiketNo); // yazar ID -1 yapılıyor
        }
        else{
            fputs(satir, fp2);
        }
    }
    fclose(fp1), fclose(fp2);
    remove("data/kitapYazar.csv");
    rename("data/kitapYazar_temp.csv", "data/kitapYazar.csv");
    return 1;
}
void yazarGuncelle(YAZAR* head, int yazarID){
    YAZAR* curr = yazarBul(head, yazarID);
    FILE* fp1;
    if(curr == NULL){
        printf("Yazar bulunamadi.\n");
        return;
    }
    printf("Yazarin guncellenen adini giriniz: ");
    getchar();
    fgets(curr->ad, sizeof(curr->ad), stdin); //Boşluklu string almak için
    curr->ad[strcspn(curr->ad, "\n")] = '\0';
    printf("Yazarin guncellenen soyadini giriniz: ");
    scanf("%s", curr->soyad);
    fp1 = fopen("data/yazarlar.csv", "w");
    curr = head;
    while(curr != NULL){
        fprintf(fp1, "%s,%s,%d\n", curr->ad, curr->soyad, curr->yazarID);
        curr = curr->next;
    }
    fclose(fp1);
    printf("Yazar guncellendi.\n");
}
void yazarGoruntule(YAZAR* head, KITAP* kitapHead, int yazarID){
    YAZAR* curr = yazarBul(head, yazarID);
    FILE* fp1 = fopen("data/kitapYazar.csv", "r");
    char satirStr[150], isbn[14];
    int yazarIDcsv, ctrl = 0;
    if(curr == NULL){
        printf("Yazar bulunamadi.\n");
        return;
    }
    printf("Ad: %s\nSoyad: %s\nYazarID: %d\n", curr->ad, curr->soyad, curr->yazarID);
    printf("Yazdigi kitaplar:\n");
    while(fgets(satirStr, sizeof(satirStr), fp1)){
        sscanf(satirStr, "%[^,],%d", isbn, &yazarIDcsv);
        if(yazarID == yazarIDcsv){
            KITAP* kitap = kitapBul(kitapHead, isbn);
            if(kitap != NULL){
                printf("Kitap: %s | ISBN: %s\n", kitap->ad, kitap->isbnNo);
                ctrl = 1;
            }
        }
    }
    if(ctrl == 0){
        printf("Bu yazara ait kitap bilgisi bulunamadi.\n");
    }
    fclose(fp1);
}
void yazarListe(YAZAR** head, YAZAR* yazar){
    YAZAR* curr = *head;
    FILE *fp1 = fopen("data/yazarlar.csv", "a");
    if(*head == NULL){
        *head = yazar;
    }
    else{
        while(curr->next != NULL){
            curr = curr->next;
        }
        curr->next = yazar;
    }
    fprintf(fp1, "%s,%s,%d\n", yazar->ad, yazar->soyad, yazar->yazarID);
    fclose(fp1);
}

OGR* ogrenciOlustur(){
    OGR* ogrenci = (OGR*)malloc(sizeof(OGR));
    printf("Ogrencinin adini giriniz: ");
    getchar();
    fgets(ogrenci->ad, sizeof(ogrenci->ad), stdin); //Boşluklu string almak için
    ogrenci->ad[strcspn(ogrenci->ad, "\n")] = '\0';
    printf("Ogrencinin soyadini giriniz: ");
    scanf("%s", ogrenci->soyad);
    printf("Ogrencinin 8 haneli numarasini giriniz: ");
    scanf("%d", &ogrenci->studentID);
    ogrenci->puan = 100;
    ogrenci->kitapSayi = 0;
    ogrenci->prev = NULL;
    ogrenci->next = NULL;
    printf("Ogrenci olusturuldu.\n");
    return ogrenci;
}
OGR* ogrenciBul(OGR* head, int studentID) {
    OGR* curr = head;

    while(curr != NULL){
        if(curr->studentID == studentID){
            return curr;  // Aranan öğrenci bulundu
        }
        curr = curr->next;
    }
    return NULL;  // Öğrenci bulunamadı
}
int ogrenciSil(OGR** head, int studentID, KITAP* headKitap){
    OGR* curr = ogrenciBul(*head, studentID);
    FILE* fp1, *fp2;
    int id;
    int kitapSayi = curr->kitapSayi;
    char satir[150];
    char etiket[17], durum[10];
    if(curr == NULL){
        printf("Ogrenci bulunamadi.\n");
        return 0;
    }
    if(curr->prev != NULL){
        curr->prev->next = curr->next;
    }
    else{
        *head = curr->next;
    }
    if(curr->next != NULL){
        curr->next->prev = curr->prev;
    }
    free(curr);
    //ogrenciler.csv dosyasından silme
    fp1 = fopen("data/ogrenciler.csv", "r");
    fp2 = fopen("data/ogrenciler_temp.csv", "w");
    while(fgets(satir, sizeof(satir), fp1)){
        sscanf(satir, "%*[^,],%*[^,],%d", &id); // Ad, Soyad, StudentID
        if(id != studentID){
            fputs(satir, fp2);
        }
    }
    fclose(fp1), fclose(fp2);
    remove("data/ogrenciler.csv");
    rename("data/ogrenciler_temp.csv", "data/ogrenciler.csv");
    if(kitapSayi > 0){ //Teslim etmediği kitap varsa kitap örnekleri dosyasından o kitabı rafta olarak değiştir
        fp1 = fopen("data/kitapOrnekleri.csv", "r");
        fp2 = fopen("data/kitapOrnekleri_temp.csv", "w");

        if(fp1 != NULL && fp2 != NULL){

            while(fgets(satir, sizeof(satir), fp1)){
                sscanf(satir, "%[^,],%s", etiket, durum);

                // durum öğrenci numarasıysa ve silinen öğrenciye aitse
                if(strcmp(durum, "") != 0 && atoi(durum) == studentID){
                    fprintf(fp2, "%s,rafta\n", etiket);
                }
                else{
                    fputs(satir, fp2);
                }
            }
            KITAP* kitapCurr = headKitap;
            while(kitapCurr != NULL){
                KitapOrnek* ornekCurr = kitapCurr->ornek;
                while(ornekCurr != NULL){
                    if(strcmp(ornekCurr->durum, "") != 0 && atoi(ornekCurr->durum) == studentID){
                        strcpy(ornekCurr->durum, "rafta");
                    }
                    ornekCurr = ornekCurr->next;
                }
                kitapCurr = kitapCurr->next;
            }

            fclose(fp1);
            fclose(fp2);
            remove("data/kitapOrnekleri.csv");
            rename("data/kitapOrnekleri_temp.csv", "data/kitapOrnekleri.csv");
        }
    }
    return 1;
}
void ogrenciGuncelle(OGR* head, int studentID){
    OGR* curr = ogrenciBul(head, studentID);
    FILE* fp1;
    if(curr == NULL){
        printf("Ogrenci bulunamadi.\n");
        return;
    }
    printf("Ogrencinin guncellenen adini giriniz: ");
    getchar();
    fgets(curr->ad, sizeof(curr->ad), stdin); //Boşluklu string almak için
    curr->ad[strcspn(curr->ad, "\n")] = '\0';
    printf("Ogrencinin guncellenen soyadini giriniz: ");
    scanf("%s", curr->soyad);
    printf("Ogrencinin puanini giriniz: ");
    scanf("%d", &curr->puan);
    fp1 = fopen("data/ogrenciler.csv", "w");
    curr = head;
    while(curr != NULL){
        fprintf(fp1, "%s,%s,%d,%d\n", curr->ad, curr->soyad, curr->studentID, curr->puan);
        curr = curr->next;
    }
    fclose(fp1);
    printf("Ogrenci bilgileri guncellendi.\n");
}
void ogrenciGoruntule(OGR* head, int studentID){
    OGR* curr = ogrenciBul(head, studentID);
    FILE* fp1 = fopen("data/kitapOdunc.csv", "r");
    char satir[150], etiketNo[16];
    int ogrIDcsv, islem, gun, ay, yil, ctrl = 0;
    if(curr == NULL){
        printf("Ogrenci bulunamadi.\n");
        return;
    }
    printf("Ad: %s\nSoyad: %s\nStudentID: %d\nPuan: %d\n", curr->ad, curr->soyad, curr->studentID, curr->puan);
    printf("\nKitap Hareketleri:\n");

    while(fgets(satir, sizeof(satir), fp1)){
        sscanf(satir, "%[^,],%d,%d,%d.%d.%d", etiketNo, &ogrIDcsv, &islem, &gun, &ay, &yil);
        if(ogrIDcsv == studentID){
            printf("Etiket No: %s | Islem: %d | Tarih: %d.%d.%d\n", etiketNo, islem, gun, ay, yil);
            ctrl = 1;
        }
    }

    if(ctrl == 0){
        printf("Bu ogrenciye ait kitap hareketi bulunamadi.\n");
    }

    fclose(fp1);
}
void teslimEtmeyenOgrenciler(OGR* head){
    OGR* curr = head;
    if(curr == NULL){
        printf("Sistemde kayitli kitap teslim etmeyen ogrenci yok.\n");
        return;
    }
    printf("Kitap teslim etmeyen ogrenciler:\n");
    while(curr != NULL){
        if(curr->kitapSayi > 0)
            printf("Ad: %s\nSoyad: %s\nStudentID: %d\nPuan: %d\n", curr->ad, curr->soyad, curr->studentID, curr->puan);
        curr = curr->next;
    }
}
void cezaliOgrenciler(OGR* head){
    OGR* curr = head;
    if(curr == NULL){
        printf("Sistemde kayitli cezali ogrenci yok.\n");
        return;
    }
    printf("Cezali ogrenciler:\n");
    while(curr != NULL){
        if(curr->puan < 100){
            printf("Ad: %s\nSoyad: %s\nStudentID: %d\nPuan: %d\n", curr->ad, curr->soyad, curr->studentID, curr->puan);
        }
        curr = curr->next;
    }
}
void tumOgrenciler(OGR* head){
    OGR* curr = head;
    if(curr == NULL){
        printf("Sistemde kayitli ogrenci yok.\n");
        return;
    }
    printf("Tum ogrenciler:\n");
    while(curr != NULL){
        printf("Ad: %s\nSoyad: %s\nStudentID: %d\nPuan: %d\n", curr->ad, curr->soyad, curr->studentID, curr->puan);
        curr = curr->next;
    }
}
void ogrenciListe(OGR** head, OGR* ogrenci){
    OGR* curr = *head, *temp;
    FILE *fp1 = fopen("data/ogrenciler.csv", "a");
    if(*head == NULL){
        *head = ogrenci;
        ogrenci->prev = NULL;
        ogrenci->next = NULL;
    }
    else{
        while(curr->next != NULL){
            curr = curr->next;
        }
        curr->next = ogrenci;
        ogrenci->prev = curr;
        ogrenci->next = NULL;
    }
    fprintf(fp1, "%s,%s,%d,%d\n", ogrenci->ad, ogrenci->soyad, ogrenci->studentID, ogrenci->puan);
    fclose(fp1);
}

KITAP* kitapOlustur(){
    int i;
    KITAP* kitap = (KITAP*)malloc(sizeof(KITAP));
    KITAP* adet = NULL, *head;
    printf("Kitabin adini giriniz: ");
    getchar();
    fgets(kitap->ad, sizeof(kitap->ad), stdin); //Boşluklu string almak için
    kitap->ad[strcspn(kitap->ad, "\n")] = '\0';
    printf("Kitabin ISBN numarasini giriniz: ");
    scanf("%s", kitap->isbnNo);
    printf("Kitabin adet sayisini giriniz: ");
    scanf("%d", &kitap->adet);
    kitap->ornek = NULL;
    kitap->next = NULL;
    KitapOrnek* tail = NULL;
    for(i = 0; i < kitap->adet; i++){
        KitapOrnek* ornek = (KitapOrnek*)malloc(sizeof(KitapOrnek));
        sprintf(ornek->etiketNo, "%s_%d", kitap->isbnNo, i+1);
        strcpy(ornek->durum, "rafta");
        ornek->flag = 1;
        ornek->next = NULL;

        // 2. Listeye ekleme
        if(kitap->ornek == NULL){
            // Bu ilk örnekse, head burası olur
            kitap->ornek = ornek;
            tail = ornek;
        }
        else{
            // Önceki örnek (tail) sonuna bağlanır
            tail->next = ornek;
            tail = ornek;
        }
    }
    return kitap;
}
KITAP* kitapBul(KITAP* head, char* isbn){
    KITAP* curr = head;

    while(curr != NULL){
        if(strcmp(curr->isbnNo, isbn) == 0){
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}
int kitapSil(KITAP** head, char* isbn){
    KITAP* curr = *head;
    KITAP* prev = NULL;
    KitapOrnek* ornek, *temp;
    FILE* fp1, *fp2;
    char satir[150], isbnCsv[20];

    while(curr != NULL && strcmp(curr->isbnNo, isbn) != 0){
        prev = curr;
        curr = curr->next;
    }
    if(curr == NULL){
        printf("Kitap bulunamadi.\n");
        return 0;
    }
    //Linked listten silme
    if(prev == NULL){
        *head = curr->next;
    }
    else{
        prev->next = curr->next;
    }
    ornek = curr->ornek;
    while(ornek != NULL){
        temp = ornek;
        ornek = ornek->next;
        free(temp);
    }
    free(curr);
    //kitaplar.csv dosyasından silme
    fp1 = fopen("data/kitaplar.csv", "r");
    fp2 = fopen("data/kitaplar_temp.csv", "w");
    while(fgets(satir, sizeof(satir), fp1)){
        if(strstr(satir, isbn) == NULL){
            fputs(satir, fp2);
        }
    }
    fclose(fp1), fclose(fp2);

    remove("data/kitaplar.csv");
    rename("data/kitaplar_temp.csv", "data/kitaplar.csv");
    //kitapOrnekleri.csv dosyasından silme
    fp1 = fopen("data/kitapOrnekleri.csv", "r");
    fp2 = fopen("data/kitapOrnekleri_temp.csv", "w");
    while(fgets(satir, sizeof(satir), fp1)){
        if(strstr(satir, isbn) == NULL){
            fputs(satir, fp2);
        }
    }
    fclose(fp1), fclose(fp2);

    remove("data/kitapOrnekleri.csv");
    rename("data/kitapOrnekleri_temp.csv", "data/kitapOrnekleri.csv");

    fp1 = fopen("data/kitapYazar.csv", "r");
    fp2 = fopen("data/kitapYazar_temp.csv", "w");
    while(fgets(satir, sizeof(satir), fp1)){
        char isbnCsv[20];
        int yazarID;
        sscanf(satir, "%[^,],%d", isbnCsv, &yazarID);

        if(strcmp(isbnCsv, isbn) != 0){
            fputs(satir, fp2);
        }
    }

    fclose(fp1), fclose(fp2);
    remove("data/kitapYazar.csv");
    rename("data/kitapYazar_temp.csv", "data/kitapYazar.csv");
    return 1;
}
void kitapGuncelle(KITAP* head, char* isbn){
    KITAP* curr = kitapBul(head, isbn);
    KITAP* temp = head; // Dosya yazma için ayrı pointer
    KitapOrnek* ornek, *prev;
    int i, ctrl = 0, eskiAdet, fark;
    FILE* fp1, *fp2;
    if(curr == NULL){
        printf("Kitap bulunamadi.\n");
        return;
    }
    eskiAdet = curr->adet;
    printf("Kitabin guncellenen adini giriniz: ");
    getchar();
    fgets(curr->ad, sizeof(curr->ad), stdin); //Boşluklu string almak için
    curr->ad[strcspn(curr->ad, "\n")] = '\0';
    printf("Kitabin guncellenen adet sayisini giriniz: ");
    scanf("%d", &curr->adet);
    ctrl = 1;
    
    if(curr->adet > eskiAdet){
        fark = curr->adet - eskiAdet;
        for(i = 0; i < fark; i++){
            KitapOrnek* yeni = (KitapOrnek*)malloc(sizeof(KitapOrnek));
            sprintf(yeni->etiketNo, "%s_%d", curr->isbnNo, eskiAdet + i + 1);
            strcpy(yeni->durum, "rafta");
            yeni->next = NULL;

            // Liste sonuna ekle
            if(curr->ornek == NULL){
                curr->ornek = yeni;
            } else {
                ornek = curr->ornek;
                while(ornek->next != NULL) ornek = ornek->next;
                ornek->next = yeni;
            }
        }
    }
    else if(curr->adet < eskiAdet){
        fark = eskiAdet - curr->adet;
        for(i = 0; i < fark; ){
            ornek = curr->ornek;
            prev = NULL;

            // Listenin sonuna git
            while(ornek && ornek->next != NULL){
                prev = ornek;
                ornek = ornek->next;
            }

            if(ornek && strcmp(ornek->durum, "rafta") == 0){
                if(prev == NULL){
                    // sadece 1 eleman varsa
                    curr->ornek = NULL;
                } else {
                    prev->next = NULL;
                }
                free(ornek);
                i++;
            }
            else{
                // silinecek rafta örnek kalmadı
                printf("Yeterli sayida rafta kitap yok. Azaltma yapilamadi!\n");
                // eski sayıya geri döndür
                curr->adet = eskiAdet;
                return;
            }
        }
    }
    
    // Dosya güncellemesi - sadece kitaplar.csv'yi güncelle
    fp1 = fopen("data/kitaplar.csv", "w");
    temp = head; // head'den başla
    while(temp != NULL){
        fprintf(fp1, "%s,%s,%d\n", temp->ad, temp->isbnNo, temp->adet);
        temp = temp->next;
    }
    fclose(fp1);
    
    // kitapOrnekleri.csv'yi sadece değişen kitap için güncelle
    fp1 = fopen("data/kitapOrnekleri.csv", "r");
    fp2 = fopen("data/kitapOrnekleri_temp.csv", "w");
    
    char satir[150], etiketNo[20], durum[20], isbnTemp[14];
    int index;
    
    // Mevcut dosyayı oku ve sadece bu kitabın örnekleri dışındakileri kopyala
    while(fgets(satir, sizeof(satir), fp1)){
        sscanf(satir, "%[^,],%s", etiketNo, durum);
        sscanf(etiketNo, "%[^_]_%d", isbnTemp, &index);
        
        // Eğer bu kitabın örneği değilse, olduğu gibi kopyala
        if(strcmp(isbnTemp, isbn) != 0){
            fputs(satir, fp2);
        }
    }
    
    // Şimdi güncellenmiş kitabın örneklerini ekle
    ornek = curr->ornek;
    while(ornek != NULL){
        fprintf(fp2, "%s,%s\n", ornek->etiketNo, ornek->durum);
        ornek = ornek->next;
    }
    
    fclose(fp1);
    fclose(fp2);
    remove("data/kitapOrnekleri.csv");
    rename("data/kitapOrnekleri_temp.csv", "data/kitapOrnekleri.csv");
    
    printf("Kitap ve ornekleri guncellendi.\n");
}
void kitapGoruntule(KITAP* head, char* isbn){
    KITAP* curr = kitapBul(head, isbn);
    KitapOrnek* ornek;
    if(curr == NULL){
        printf("Kitap bulunamadi.\n");
        return;
    }
    ornek = curr->ornek;
    printf("Ad: %s\nISBN: %s\nAdet: %d\n", curr->ad, curr->isbnNo, curr->adet);
    while(ornek != NULL){
        printf("Etiket: %s -> Durum: %s\n", ornek->etiketNo, ornek->durum);
        ornek = ornek->next;
    }
}
void raftaKitaplar(KITAP* head){
    KITAP* curr = head;
    KitapOrnek* ornek;
    int ctrl = 0;
    if(curr == NULL){
        printf("Sistemde kayitli kitap yok.\n");
        return;
    }
    printf("Raftaki kitaplar:\n");
    while(curr != NULL){
        ornek = curr->ornek;
        while(ornek != NULL){
            if(strcmp(ornek->durum, "rafta") == 0){
                printf("Ad: %s\nEtiket: %s\nDurum: %s\n", curr->ad, ornek->etiketNo, ornek->durum);
                ctrl = 1;
            }
            ornek = ornek->next;
        }
        curr = curr->next;
    }
    if(ctrl == 0){
        printf("Rafta kitap yok.\n");
    }
}
void zamanindaTeslimOlmayan(KITAP* head){
    KITAP* curr = head;
    KitapOrnek* ornek;
    if(curr == NULL){
        printf("Zamaninda teslim edilmeyen kitap yok!\n");
        return;
    }
    while(curr != NULL){
        ornek = curr->ornek;
        while(ornek != NULL){
            if(ornek->flag == 0){
                printf("Etiket: %s Durum: %s\n", ornek->etiketNo, ornek->durum);
            }
            ornek = ornek->next;
        }
        curr = curr->next;
    }
}
void kitapListe(KITAP** head, KITAP* kitap){
    KITAP* curr = *head;
    KitapOrnek* ornek = kitap->ornek;
    FILE *fp1 = fopen("data/kitaplar.csv", "a");
    FILE *fp2 = fopen("data/kitapOrnekleri.csv", "a");
    if(*head == NULL){
        *head = kitap;
    }
    else{
        while(curr->next != NULL){
            curr = curr->next;
        }
        curr->next = kitap;
    }
    fprintf(fp1, "%s,%s,%d\n", kitap->ad, kitap->isbnNo, kitap->adet);
    ornek = kitap->ornek;
    while(ornek != NULL){
        fprintf(fp2, "%s,%s\n", ornek->etiketNo, ornek->durum);
        ornek = ornek->next;
    }
    fclose(fp1), fclose(fp2);
}

void kitapYazarEsle(KitapYazar** eslesmeler, YAZAR* headYazar, KITAP* headKitap, int* satir){
    int yazarID;
    char isbn[14];
    FILE* fp1 = fopen("data/kitapYazar.csv", "a");
    KITAP* currKit = headKitap;
    YAZAR* currYaz = headYazar;
    *eslesmeler = realloc(*eslesmeler, (*satir + 1) * sizeof(KitapYazar));

    do{
        printf("Kitabin ISBN numarasini giriniz: ");
        scanf("%s", isbn);
        currKit = kitapBul(headKitap, isbn);
        if(currKit == NULL){
            printf("Kitap bulunamadi, gecerli bir ISBN numarasi giriniz!\n ");
        }
    }while(currKit == NULL);

    do{
        printf("Yazarin ID numarasini giriniz: ");
        scanf("%d", &yazarID);
        currYaz = yazarBul(headYazar, yazarID);
        if(currYaz == NULL){
            printf("Yazar bulunamadi, gecerli bir ID numarasi giriniz!\n");
        }
    }while(currYaz == NULL);
    
    (*eslesmeler)[*satir].yazarID = yazarID;
    strcpy((*eslesmeler)[*satir].isbn, isbn);
    fprintf(fp1, "%s,%d\n", (*eslesmeler)[*satir].isbn, (*eslesmeler)[*satir].yazarID);
    (*satir)++;
    fclose(fp1);
}
void kitapYazarGuncelle(KitapYazar** eslesmeler, YAZAR* headYazar, KITAP* headKitap, int* satir){
    int yazarID, eskiID, yeniID, yazarIDcsv, i = 0;
    char isbn[14], satirStr[150], isbnInfile[17];
    FILE* fp1 = fopen("data/kitapYazar.csv", "r");
    KITAP* currKit = headKitap;
    YAZAR* currYaz = headYazar;
    *eslesmeler = realloc(*eslesmeler, (*satir + 1) * sizeof(KitapYazar));

    do{
        printf("Kitabin ISBN numarasini giriniz: ");
        scanf("%s", isbn);
        currKit = kitapBul(headKitap, isbn);
        if(currKit == NULL){
            printf("Kitap bulunamadi, gecerli bir ISBN numarasi giriniz!\n ");
        }
    }while(currKit == NULL);

    while(fgets(satirStr, sizeof(satirStr), fp1)){
        sscanf(satirStr, "%[^,],%d", isbnInfile, &yazarIDcsv);
        if(strcmp(isbnInfile, isbn) == 0){
            YAZAR* yazar = yazarBul(headYazar, yazarIDcsv);
            if(yazar != NULL){
                printf("%d. %s %s (ID: %d)\n", ++i, yazar->ad, yazar->soyad, yazar->yazarID);
            }
        }
    }
    fclose(fp1);

    if(i == 0){
        printf("Bu kitaba ait yazar kaydi bulunamadi.\n");
        return;
    }
    printf("\nDegistirilecek yazar ID'yi secin: ");
    scanf("%d", &eskiID);

    printf("Yeni yazar ID'yi girin: ");
    scanf("%d", &yeniID);

    // Dosyada güncelleme işlemi
    fp1 = fopen("data/kitapYazar.csv", "r");
    FILE* fp2 = fopen("data/kitapYazar_temp.csv", "w");

    while(fgets(satirStr, sizeof(satirStr), fp1)) {
        sscanf(satirStr, "%[^,],%d", isbnInfile, &yazarID);
        if(strcmp(isbnInfile, isbn) == 0 && yazarID == eskiID){
            fprintf(fp2, "%s,%d\n", isbn, yeniID);
        } else {
            fputs(satirStr, fp2);
        }
    }

    fclose(fp1);
    fclose(fp2);
    remove("data/kitapYazar.csv");
    rename("data/kitapYazar_temp.csv", "data/kitapYazar.csv");

    printf("Guncelleme tamamlandi.\n");
}
void kitapOdunc(KITAP* kitap, OGR* ogrenci){
    ODUNC* odunc = (ODUNC*)malloc(sizeof(ODUNC));
    KitapOrnek* ornek = kitap->ornek;
    KitapOrnek* temp = NULL;
    char durum[10], satir[150], ad[20], soyad[20];
    int ctrl = 0, teslimTarih = 0, alimTarih = 0, studentID, puan;
    FILE* fp1 = fopen("data/kitapOdunc.csv", "a");
    FILE* fp2, *fp3;
    odunc->kitap = kitap;
    odunc->ogrenci = ogrenci;
    while(ornek != NULL && ctrl == 0){
        if(strcmp(ornek->durum, "rafta") == 0){
            ctrl = 1;
            temp = ornek;
        }
        ornek = ornek->next;
    }
    if(ctrl == 0){
        printf("Kitap stokta yok!\n");
        free(odunc);
        fclose(fp1);
        return;
    }

    printf("Tarihi giriniz(gg.aa.yyyy): ");
    scanf("%d.%d.%d", &odunc->gun, &odunc->ay, &odunc->yil);
    printf("0. Kitap odunc al\n1. Kitap teslim et\nHangi islemi yapmak istiyorsunuz: ");
    scanf("%d", &odunc->islem);
    if(odunc->islem == 1 && oduncCtrl(kitap, ogrenci) == 0){
        printf("Kitap odunc alinmamis, dolayisiyla teslim edilemez.\n");
        free(odunc);
        fclose(fp1);
        return;
    }

    if(odunc->islem == 0){ //Ödünç alma
        sprintf(durum, "%d", ogrenci->studentID);
        strcpy(temp->durum, durum);
        kitapOrnekDurumGuncelle(temp->etiketNo, durum);
        ogrenci->kitapSayi++;
    }
    else{ //Teslim etme
        sprintf(durum, "%d", ogrenci->studentID);
        ornek = kitap->ornek;
        while(ornek != NULL){
            if(strcmp(ornek->durum, durum) == 0){
                temp = ornek;
            }
            ornek = ornek->next;
        }
        if(temp == NULL){
            printf("Bu ogrenciye ait kitap bu kitap listesinde bulunamadi.\n");
            fclose(fp1);
            free(odunc);
            return;
        }
        alimTarih = alimTarihi(kitap, ogrenci);
        teslimTarih = 365 * odunc->yil + 30 * (odunc->ay-1) + odunc->gun;
        if(teslimTarih - alimTarih > 15){
            ogrenci->puan -= 10;
            fp2 = fopen("data/ogrenciler.csv", "r");
            fp3 = fopen("data/ogrenciler_temp.csv", "w");
            while(fgets(satir, sizeof(satir), fp2)){
                sscanf(satir, "%[^,],%[^,],%d,%d", ad, soyad, &studentID, &puan);
                if (studentID == ogrenci->studentID){
                    fprintf(fp3, "%s,%s,%d,%d\n", ad, soyad, studentID, ogrenci->puan);
                } else {
                    fputs(satir, fp3);
                }
            }
            fclose(fp2), fclose(fp3);
            remove("data/ogrenciler.csv");
            rename("data/ogrenciler_temp.csv", "ogrenciler.csv");
            temp->flag = 0;
        }
        strcpy(temp->durum, "rafta");
        kitapOrnekDurumGuncelle(temp->etiketNo, "rafta");
        ogrenci->kitapSayi--;
    }
    fprintf(fp1, "%s,%d,%d,%d.%d.%d\n", temp->etiketNo, ogrenci->studentID, odunc->islem, odunc->gun, odunc->ay, odunc->yil);
    fclose(fp1);
    free(odunc);
}
int oduncCtrl(KITAP* kitap, OGR* ogrenci){
    char satir[200];
    KitapOrnek* ornek = kitap->ornek;
    char studentID[10], etiketNo[15], islemStr[2], ogrID[10];
    int islem;
    FILE* fp1 = fopen("data/kitapOdunc.csv", "r");

    sprintf(studentID, "%d", ogrenci->studentID);
    while(ornek != NULL){
        rewind(fp1); // her örnek için başa sar
        while(fgets(satir, sizeof(satir), fp1)){
            sscanf(satir, "%[^,],%[^,],%[^,]", etiketNo, ogrID, islemStr);
            islem = atoi(islemStr);
            if(strcmp(etiketNo, ornek->etiketNo) == 0 && atoi(ogrID) == ogrenci->studentID && islem == 0){
                fclose(fp1);
                return 1; // gerçekten ödünç alınmış
            }
        }
        ornek = ornek->next;
    }
    fclose(fp1);
    return 0;
}
int alimTarihi(KITAP* kitap, OGR* ogrenci){
    char satir[200];
    KitapOrnek* ornek = kitap->ornek;
    char studentID[10], etiketNo[15], islemStr[2], ogrID[10];
    int islem, gun, ay, yil, tarih = 0;
    FILE* fp1 = fopen("data/kitapOdunc.csv", "r");

    sprintf(studentID, "%d", ogrenci->studentID);
    while(ornek != NULL){
        rewind(fp1); // her örnek için başa sar
        while(fgets(satir, sizeof(satir), fp1)){
            sscanf(satir, "%[^,],%[^,],%[^,],%d.%d.%d", etiketNo, ogrID, islemStr, &gun, &ay, &yil);
            islem = atoi(islemStr);
            if(strcmp(etiketNo, ornek->etiketNo) == 0 && atoi(ogrID) == ogrenci->studentID && islem == 0){
                tarih = 365 * yil + 30 * (ay-1) + gun;
                fclose(fp1);
                return tarih;
            }
        }
        ornek = ornek->next;
    }
    fclose(fp1);
}
void kitapOrnekDurumGuncelle(char* etiketNo, char* yeniDurum){
    FILE* fp1 = fopen("data/kitapOrnekleri.csv", "r");
    FILE* fp2 = fopen("data/kitapOrnekleri_temp.csv", "w");

    char satir[256];
    char etiket[20], durum[20];
    int i = 0;

    while(fgets(satir, sizeof(satir), fp1)){
        // EtiketNo ve Durum'u ayrıştır (örnek: "ETK1234,rafta\n")
        sscanf(satir, "%[^,],%s", etiket, durum);

        if(strcmp(etiket, etiketNo) == 0){
            // Aynı etiketNo ise yeni durumla yaz
            fprintf(fp2, "%s,%s\n", etiket, yeniDurum);
        }
        else{
            // Değilse olduğu gibi yaz
            fputs(satir, fp2);
        }
    }

    fclose(fp1);
    fclose(fp2);
    remove("data/kitapOrnekleri.csv");
    rename("data/kitapOrnekleri_temp.csv", "data/kitapOrnekleri.csv");
}
void freeBellek(OGR* ogrHead, KITAP* kitapHead, YAZAR* yazarHead){
    // Öğrenciler
    OGR* ogrTemp;
    while(ogrHead != NULL){
        ogrTemp = ogrHead;
        ogrHead = ogrHead->next;
        free(ogrTemp);
    }

    // Kitaplar ve örnekleri
    KITAP* kitapTemp;
    while(kitapHead != NULL){
        kitapTemp = kitapHead;
        kitapHead = kitapHead->next;

        KitapOrnek* ornek = kitapTemp->ornek;
        KitapOrnek* ornekTemp;
        while(ornek != NULL){
            ornekTemp = ornek;
            ornek = ornek->next;
            free(ornekTemp);
        }

        free(kitapTemp);
    }

    // Yazarlar
    YAZAR* yazarTemp;
    while(yazarHead != NULL){
        yazarTemp = yazarHead;
        yazarHead = yazarHead->next;
        free(yazarTemp);
    }
}