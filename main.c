/*
Proyek Akhir Semester
No. Grup : 23
Nama anggota :
Salim (2306204604)
Wiellona Darlene Oderia Saragih (2306264396)

Penjelasan program :
Pada Proyek Akhir Semester Pemrograman Lanjut 2024, kami membuat simulasi online shop yang bernama Trova. Program ini dibuat untuk sisi penjual dan pembeli. Program ini mengimplementasi materi Pemrograman Lanjut: function, dynamic data structure (dalam hal ini digunakan BST), searching/sorting, dan file handling.

Pada menu awal, pengguna dapat melakukan registrasi jika belum memiliki akun, kemudian dapat login. Terdapat fitur lainnya yaitu top up saldo yang dapat digunakan untuk mensimulasikan transaksi pada program ini. Setelah login, pengguna dapat memilih ingin login sebagai penjual atau pembeli. Pada sisi penjual, pengguna dapat membuat toko, menghapus toko, menambah produk, menghapus produk, dan mengedit variabel-variabel produk. Di sisi lain, pengguna dapat menggunakan program ini sebagai pembeli, yang dapat melakukan pencarian produk-produk yang dijual oleh toko-toko yang terdaftar pada program ini. Program dapat menampilkan riwayat pencarian terakhir yang dilakukan pengguna. Pengguna dapat melakukan pencarian produk dan dapat melakukan pembelian produk.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAMA 30
#define MAX_PASSWORD 50

// STRUCT
typedef struct produk {
  char nama[MAX_NAMA];
  int berat;
  int jumlah;
  int harga;
  char detail[100];
  struct produk *nextproduk;
  struct produk *left;
  struct produk *right;
} Produk;

typedef struct toko {
  char nama[MAX_NAMA];
  char alamat[50];
  struct produk *Produkptr;
  struct toko *nexttoko;
} Toko;

typedef struct history {
  int tgl, bln, thn;
  struct produk *Produkptr;
  struct history *nexthistory;
} History;

typedef struct akun {
  struct akun *leftptr;
  char username[MAX_NAMA];
  char password[MAX_PASSWORD];
  int saldo;
  struct biodata *Bioptr;
  struct toko *Tokoptr;
  struct history *Historyptr;
  struct akun *rightptr;
  struct produk *produkBST; // Untuk BST produk
} Akun;

typedef Produk *Produkptr;
typedef Toko *Tokoptr;
typedef History *Historyptr;
typedef Akun *Akunptr;

// FUNCTION PROTOTYPES
int checkuser(Akunptr start, char username[], char password[], Akunptr *akunterpilih);
int tambahakun(Akunptr *akunptr, char username[MAX_NAMA], char password[MAX_PASSWORD], int saldo);
void tambahtoko(Akunptr *akunptr, Tokoptr *tokoptr, char nama_toko[], char alamat_toko[]);
void tambahproduk(Tokoptr *tokoptr, Produkptr *produkptr, char nama[MAX_NAMA], int berat, char detail[100], int jumlah, int harga);
void hapustoko(Akunptr *akunptr, int nomor);
void hapusproduk(Tokoptr *tokoptr, int nomor);
void registrasi(Akunptr *akunptr);
int masuk(Akunptr *startakun, Akunptr *akunterpilih);
void keluar(Akunptr startakun);
void printakun(Akunptr akunptr);
void load_database(Akunptr *startakun);
void save_database(Akunptr startakun, FILE *database);
void main_menu(Akunptr *startakun, Akunptr *akun);
void manage_toko(Akunptr *akun);
void manage_produk(Tokoptr *toko);
void detail_produk(Produkptr *produk);
void edit_produk(Produkptr *produk);
void free_memory(Akunptr start);
void searchProduk(Akunptr *startakun, Akunptr akun);
void customerMenu(Akunptr *startakun, Akunptr akun);
void historyProduct(Historyptr history, Produkptr last_two[2]);
int topup();
void opsi_pembelian(Akunptr *startakun, Akunptr akun, Produkptr produk);
void beli(Akunptr *startakun, Akunptr akun, Produkptr produk);
void insertProdukBST(Produk **root, Produk *newProduk);
void checkProduk(Produkptr produk, char keyword[], Produkptr *foundproduk);
void searchProdukBST(Akunptr *akun, char keyword[], Produkptr *foundproduk);
void printInOrder(Produk *root);
void cari_penjual(Akunptr startakun, Produkptr produk, int nominal, int jumlah);
int cari_produk_penjual(Produkptr produkptr, Produkptr produk, int jumlah);

//Function untuk mengecek username dan password pengguna saat login
int checkuser(Akunptr start, char username[], char password[], Akunptr *akunterpilih) {
  if (start == NULL)
    return 1;

  if (strcmp(start->username, username) == 0) {
    if (strcmp(start->password, password) == 0) {
      *akunterpilih = start;
      return 0;
    } else
      return 2;
  } else if (strcmp(start->username, username) < 0) {
    return checkuser(start->rightptr, username, password, akunterpilih);
  } else {
    return checkuser(start->leftptr, username, password, akunterpilih);
  }
}

// Function untuk membuat akun baru
int tambahakun(Akunptr *akunptr, char username[MAX_NAMA], char password[MAX_PASSWORD], int saldo) {
  if (*akunptr == NULL) {
    *akunptr = malloc(sizeof(Akun));
    if (*akunptr != NULL) {
      strcpy((*akunptr)->username, username);
      strcpy((*akunptr)->password, password);
      (*akunptr)->saldo = saldo;
      (*akunptr)->Tokoptr = NULL;
      (*akunptr)->Bioptr = NULL;
      (*akunptr)->Historyptr = NULL;
      (*akunptr)->leftptr = NULL;
      (*akunptr)->rightptr = NULL;
      return 0;
    } else {
      printf("Ruang memori habis\n");
      return -1;
    }
  } else {
    if (strcmp((*akunptr)->username, username) < 0)
      return tambahakun(&((*akunptr)->rightptr), username, password, saldo);
    else if (strcmp((*akunptr)->username, username) > 0)
      return tambahakun(&((*akunptr)->leftptr), username, password, saldo);
    else
      return 1;
  }
}

// Function untuk menambah toko
void tambahtoko(Akunptr *akunptr, Tokoptr *tokoptr, char nama_toko[], char alamat_toko[]) {
  Tokoptr tokobaru;
  tokobaru = malloc(sizeof(Toko));
  if (tokobaru != NULL) {
    strcpy(tokobaru->nama, nama_toko);
    strcpy(tokobaru->alamat, alamat_toko);
    tokobaru->Produkptr = NULL;
    tokobaru->nexttoko = NULL;
    if ((*akunptr)->Tokoptr == NULL) {
      (*akunptr)->Tokoptr = tokobaru;
    } else {
      Tokoptr current = (*akunptr)->Tokoptr;
      while (current->nexttoko != NULL) {
        current = current->nexttoko;
      }
      current->nexttoko = tokobaru;
    }
    *tokoptr = tokobaru;
    return;
  } else {
    printf("Ruang memori habis\n");
    return;
  }
  system("cls");
}

//Function untuk menambah produk tokos
void tambahproduk(Tokoptr *tokoptr, Produkptr *produkptr, char nama[MAX_NAMA], int berat, char detail[100], int jumlah, int harga) {
  Produkptr produkbaru;
  produkbaru = malloc(sizeof(Produk));
  if (produkbaru != NULL) {
    strcpy(produkbaru->nama, nama);
    produkbaru->berat = berat;
    strcpy(produkbaru->detail, detail);
    produkbaru->jumlah = jumlah;
    produkbaru->harga = harga;
    produkbaru->nextproduk = NULL;
    if ((*tokoptr)->Produkptr == NULL) {
      (*tokoptr)->Produkptr = produkbaru;
    } else {
      Produkptr current = (*tokoptr)->Produkptr;
      while (current->nextproduk != NULL) {
        current = current->nextproduk;
      }
      current->nextproduk = produkbaru;
    }
    *produkptr = produkbaru;
    return;
  } else {
    printf("Ruang memori habis\n");
    return;
  }
}

//Function untuk menghapus toko 
void hapustoko(Akunptr *akunptr, int nomor) {
  Tokoptr previous = NULL;
  Tokoptr current = (*akunptr)->Tokoptr;
  Tokoptr next = NULL;
  if (nomor == 1) {
    (*akunptr)->Tokoptr = current->nexttoko;
  } else {
    while (nomor > 1) {
      previous = current;
      current = current->nexttoko;
      nomor--;
    }
    next = current->nexttoko;
    previous->nexttoko = next;
  }
  free(current);
}

//Function untuk menghapus produk
void hapusproduk(Tokoptr *tokoptr, int nomor) {
  Produkptr previous = NULL;
  Produkptr current = (*tokoptr)->Produkptr;
  Produkptr next = NULL;
  if (nomor == 1) {
    (*tokoptr)->Produkptr = current->nextproduk;
  } else {
    while (nomor > 1) {
      previous = current;
      current = current->nextproduk;
      nomor--;
    }
    next = current->nextproduk;
    previous->nextproduk = next;
  }
  free(current);
}

//Function registrasi akun baru
void registrasi(Akunptr *akunptr) {
  char username[MAX_NAMA], password[MAX_PASSWORD];

  printf("Masukkan username baru: ");
  scanf("%s", username);
  printf("Masukkan password baru: ");
  scanf("%s", password);

  // Memanggil function tambahakun untuk mendaftarkan akun baru, kemudian nanti disimpan dalam database
  int result = tambahakun(akunptr, username, password, 0);
  if (result == 1) {
    printf("Username sudah dipakai\n");
  } else if (result == 0) {
    printf("Registrasi berhasil\n");
  } else {
    printf("Registrasi gagal\n");
  }
  system("cls");
}

//Function untuk mengecek kebenaran username dan password saat pengguna login
int masuk(Akunptr *startakun, Akunptr *akunterpilih) {
  Akunptr akuncurrent = *startakun;
  char username[MAX_NAMA], password[MAX_PASSWORD];
  int opsi, check;

  printf("Username: ");
  scanf("%s", username);
  printf("Password: ");
  scanf("%s", password);

  // memanggil function checkuser untuk memastikan input username dan password dari pengguna sama dengan di database
  check = checkuser(akuncurrent, username, password, akunterpilih);
  if (check == 0) {
    printf("Login berhasil!\n");
    system("pause");
    system("cls");
    return 1;
  } else if (check == 2) {
    printf("Password salah!\n");
  } else {
    printf("Username tidak ditemukan!\n");
  }
  system("pause");
  system("cls");
  return 0;
}

// Menyimpan data yang diinput selama program berjalan ke database
void keluar(Akunptr startakun) {
  FILE *database = fopen("database.txt", "w");
  if (database == NULL) {
    printf("Tidak bisa membuka file database.txt\n");
    return;
  }
  save_database(startakun, database);
  fclose(database);
  system("cls");
}

void printakun(Akunptr akunptr) {
  if (akunptr != NULL) {
    printakun(akunptr->leftptr);
    printf("Username: %s\nPassword: %s\n", akunptr->username,
           akunptr->password);
    printakun(akunptr->rightptr);
  }
}

// Mengambil data pada database untuk setiap jenis struct: AKUN, TOKO, dan PRODUK
void load_database(Akunptr *startakun) {
  FILE *database = fopen("database.txt", "r");
  if (database == NULL) {
    printf("Tidak bisa membuka file database.txt\n");
    return;
  }

  char line[256];
  Akunptr current_akun = NULL;
  Tokoptr current_toko = NULL;
  Produkptr current_produk = NULL;

  while (fgets(line, sizeof(line), database)) {
    if (strncmp(line, "AKUN", 4) == 0) {
      char username[MAX_NAMA], password[MAX_PASSWORD];
      int saldo;
      sscanf(line, "AKUN %s %s %d", username, password, &saldo);
      tambahakun(startakun, username, password, saldo);
      current_akun = *startakun;
      while (current_akun && strcmp(current_akun->username, username) != 0) {
        if (strcmp(current_akun->username, username) < 0)
          current_akun = current_akun->rightptr;
        else
          current_akun = current_akun->leftptr;
      }
    } else if (strncmp(line, "TOKO", 4) == 0) {
      char nama_toko[MAX_NAMA], alamat_toko[50];
      sscanf(line, "TOKO %s %s", nama_toko, alamat_toko);
      tambahtoko(&current_akun, &current_toko, nama_toko, alamat_toko);
    } else if (strncmp(line, "PRODUK", 6) == 0) {
      char nama_produk[MAX_NAMA], detail_produk[100];
      int jumlah_produk, harga_produk, berat_produk;
      sscanf(line, "PRODUK %s %d %s %d %d", nama_produk, &berat_produk,
             detail_produk, &jumlah_produk, &harga_produk);
      tambahproduk(&current_toko, &current_produk, nama_produk, berat_produk,
                   detail_produk, jumlah_produk, harga_produk);
    }
  }

  fclose(database);
}

//Meyimpan data pada database per masing-masing jenis data: AKUN, TOKO, dan PRODUK
void save_database(Akunptr startakun, FILE *database) {
  if (startakun != NULL) {
    save_database(startakun->leftptr, database);
    fprintf(database, "AKUN %s %s %d\n", startakun->username,
            startakun->password, startakun->saldo);
    Tokoptr tokoptr = startakun->Tokoptr;
    while (tokoptr != NULL) {
      fprintf(database, "TOKO %s %s\n", tokoptr->nama, tokoptr->alamat);
      Produkptr produkptr = tokoptr->Produkptr;
      while (produkptr != NULL) {
        fprintf(database, "PRODUK %s %d %s %d %d\n", produkptr->nama,
                produkptr->berat, produkptr->detail, produkptr->jumlah,
                produkptr->harga);
        produkptr = produkptr->nextproduk;
      }
      tokoptr = tokoptr->nexttoko;
    }
    save_database(startakun->rightptr, database);
  }
}

// Free memory
void free_memory(Akunptr start) {
  if (start == NULL)
    return;

  free_memory(start->leftptr);
  free_memory(start->rightptr);

  Toko *toko = start->Tokoptr;
  while (toko) {
    Toko *nextToko = toko->nexttoko;
    Produk *produk = toko->Produkptr;
    while (produk) {
      Produk *nextProduk = produk->nextproduk;
      free(produk);
      produk = nextProduk;
    }
    free(toko);
    toko = nextToko;
  }

  free(start->Bioptr);
  free(start->Historyptr);
  free(start);
}

// Seller side

// Function fitur untuk pengguna sebagai penjual untuk membuat dan menghapus toko
void manage_toko(Akunptr *akun) {
  Tokoptr tokoptr = NULL;
  char nama_toko[MAX_NAMA], alamat_toko[50];
  int counter = 1, opsi, opsi_lanjutan;
  do {
    counter = 1;
    if ((*akun)->Tokoptr == NULL) {
      printf("Anda tidak memiliki toko.\n");
      char nama_toko[MAX_NAMA], alamat_toko[50];
      printf("\n%d. Buat toko\n%d. Kembali\n> ", counter, counter + 1);
      scanf("%d", &opsi);
      switch (opsi) {
      case 1:
        printf("Nama toko : ");
        scanf("%s", nama_toko);
        printf("Alamat : ");
        scanf("%s", alamat_toko);
        tambahtoko(akun, &tokoptr, nama_toko, alamat_toko);
        system("cls");
        break;
      case 2:
        system("cls");
        return;
      default :
        return;
      }
      counter = -1;
    } else {
      printf("Toko yang Anda miliki:\n");
      Tokoptr tokoptr = (*akun)->Tokoptr;

      while (tokoptr != NULL) {
        printf("%d. %s\n", counter, tokoptr->nama);
        tokoptr = tokoptr->nexttoko;
        counter++;
      }
      printf("\n%d. Buat toko\n%d. Hapus toko\n%d. Kembali\n> ", counter + 1,
             counter + 2, counter + 3);
      scanf("%d", &opsi);
      opsi_lanjutan = opsi / counter;
      if (opsi_lanjutan) {
        switch (opsi - counter) {
        case 1:

          printf("\nNama toko : ");
          scanf(" %s", nama_toko);
          printf("Alamat : ");
          scanf("%s", alamat_toko);
          tambahtoko(akun, &tokoptr, nama_toko, alamat_toko);
          break;
        case 2:
          printf("\nPilih toko yang ingin dihapus : ");
          scanf("%d", &opsi);
          hapustoko(akun, opsi);
          break;
        case 3:
          system("cls");
          return;
        default :
          return;        }
      } else {
        system("cls");
        tokoptr = (*akun)->Tokoptr;
        while (opsi > 1) {
          tokoptr = tokoptr->nexttoko;
          opsi--;
        }
        manage_produk(&tokoptr);
      }
    }
  } while (opsi != counter + 3);
}


// Function fitur untuk pengguna sebagai penjual untuk menambah, menghapus dan mengedit produk yang dijual di toko
void manage_produk(Tokoptr *toko) {
  Produkptr produkptr = NULL;
  char nama[MAX_NAMA], detail[100];
  int jumlah, harga, counter, opsi, opsi_lanjutan, berat;
  do {
    counter = 1;
    printf("%s, %s\n", (*toko)->nama, (*toko)->alamat);
    if ((*toko)->Produkptr == NULL) {
      printf("Anda tidak memiliki produk.\n");
      printf("\n%d. Tambah produk\n%d. Kembali\n> ", counter, counter + 1);
      scanf("%d", &opsi);
      switch (opsi) {
      case 1:
        printf("Nama produk : ");
        scanf("%s", nama);
        printf("Berat produk (dalam gram) : ");
        scanf("%d", &berat);
        printf("Detail produk : ");
        scanf("%s", detail);
        printf("Jumlah produk : ");
        scanf("%d", &jumlah);
        printf("Harga produk : ");
        scanf("%d", &harga);
        tambahproduk(toko, &produkptr, nama, berat, detail, jumlah, harga);
        system("cls");
        break;
      case 2:
        system("cls");
        return;
      default :
        return;
      }
      counter = -1;
    } else {
      printf("Produk yang Anda miliki:\n");
      Produkptr produkptr = (*toko)->Produkptr;
      while (produkptr != NULL) {
        printf("%d. %s\n", counter, produkptr->nama);
        produkptr = produkptr->nextproduk;
        counter++;
      }
      printf("\n%d. Tambah produk\n%d. Hapus produk\n%d. Kembali\n> ",
             counter + 1, counter + 2, counter + 3);
      scanf("%d", &opsi);
      opsi_lanjutan = opsi / (counter);
      if (opsi_lanjutan) {
        switch (opsi - counter) {
        case 1:
          printf("Nama produk : ");
          scanf("%s", nama);
          printf("Berat produk (dalam gram) : ");
          scanf("%d", &berat);
          printf("Detail produk : ");
          scanf("%s", detail);
          printf("Jumlah produk : ");
          scanf("%d", &jumlah);
          printf("Harga produk : ");
          scanf("%d", &harga);
          tambahproduk(toko, &produkptr, nama, berat, detail, jumlah, harga);
          break;
        case 2:
          printf("\nPilih produk yang ingin dihapus : ");
          scanf("%d", &opsi);
          hapusproduk(toko, opsi);
          break;
        case 3:
          system("cls");
          return;
        default :
          return;  
        }
      } else {
        system("cls");
        produkptr = (*toko)->Produkptr;
        while (opsi > 1) {
          produkptr = produkptr->nextproduk;
          opsi--;
        }
        detail_produk(&produkptr);
      }
    }
  } while (opsi != counter + 3);
}

// Menampilkan detail produk yang dijual
void detail_produk(Produkptr *produk) {
  int opsi;
  printf("Nama produk : %s\n", (*produk)->nama);
  printf("Berat produk (dalam gram) : %d\n", (*produk)->berat);
  printf("Detail produk : %s\n", (*produk)->detail);
  printf("Jumlah produk : %d\n", (*produk)->jumlah);
  printf("Harga produk : %d\n", (*produk)->harga);
  printf("1. Edit produk\n2. Kembali\n> ");
  scanf(" %d", &opsi);
  switch (opsi) {
  case 1:
    edit_produk(produk);
    system("cls");
    break;
  case 2:
    system("cls");
    return;
  default :
    return;
  }
}

// Function untuk mengedit detail produk: mengubah nama, menambah/mengurangi stok produk, dan lainnya
void edit_produk(Produkptr *produk) {
  int opsi;
  printf(
      "1. Edit nama produk\n2. Edit berat produk (dalam gram)\n3. Edit detail "
      "produk\n4. Edit jumlah produk\n5. Edit harga produk\n6. Kembali\n> ");
  scanf("%d", &opsi);
  switch (opsi) {
  case 1:
    printf("Masukkan nama baru : ");
    scanf("%s", (*produk)->nama);
    break;
  case 2:
    printf("Masukkan berat baru : ");
    scanf("%d", &(*produk)->berat);
    break;
  case 3:
    printf("Masukkan detail baru : ");
    scanf("%s", (*produk)->detail);
    break;
  case 4:
    printf("Masukkan jumlah baru : ");
    scanf("%d", &(*produk)->jumlah);
    break;
  case 5:
    printf("Masukkan harga baru : ");
    scanf("%d", &(*produk)->harga);
    break;
  case 6:
    system("cls");
    return;
  default :
    return;
  }
}

// Customer side

// Function untuk menampilkan produk terakhir yang dicari oleh pengguna (sebagai pembeli)
void historyProduct(Historyptr history, Produkptr last_two[2]) {
  if (history == NULL) {
    last_two[0] = NULL;
    last_two[1] = NULL;
    return;
  }

  int count = 0;
  Historyptr current = history;
  while (current != NULL) {
    count++;
    current = current->nexthistory;
  }

  // Menampilkan 2 produk terakhir yang dicari
  int i = 0;
  while (history != NULL && i < 2) {
    if (count <= 2 || count - i <= 2) {
      last_two[i] = history->Produkptr;
      i++;
    }
    history = history->nexthistory;
    count--;
  }

  if (i < 2) {
    last_two[1] = NULL;
  }
}

void tampilDetail(Produkptr produk) {
  if (produk != NULL) {
    printf("Nama produk : %s\n", produk->nama);
    printf("Berat produk (dalam gram) : %d\n", produk->berat);
    printf("Detail produk : %s\n", produk->detail);
    printf("Jumlah produk : %d\n", produk->jumlah);
    printf("Harga produk : %d\n", produk->harga);
  } else {
    printf("Produk tidak ditemukan\n");
  }
}

// Function untuk membuat node produk baru
Produk *createProdukNode(char nama[], int berat, char detail[], int jumlah,
                         int harga) {
  Produk *newProduk = (Produk *)malloc(sizeof(Produk));
  if (newProduk != NULL) {
    strcpy(newProduk->nama, nama);
    newProduk->berat = berat;
    strcpy(newProduk->detail, detail);
    newProduk->jumlah = jumlah;
    newProduk->harga = harga;
    newProduk->left = NULL;
    newProduk->right = NULL;
  }
  return newProduk;
}

// Membuat BST untuk produk
void insertProdukBST(Produk **root, Produk *newProduk) {
  if (*root == NULL) {
    *root = newProduk;
    return;
  }
  if (strcmp(newProduk->nama, (*root)->nama) < 0) {
    insertProdukBST(&((*root)->left), newProduk);
  } else {
    insertProdukBST(&((*root)->right), newProduk);
  }
}

void checkProduk(Produkptr produkptr, char keyword[], Produkptr *foundproduk) {
  if (produkptr != NULL) {

    if (strcmp(produkptr->nama, keyword) == 0) {
        *foundproduk = produkptr;
    } else if (strcmp(produkptr->nama, keyword) < 0) {
      checkProduk(produkptr->right, keyword, foundproduk);
    } else {
      checkProduk(produkptr->left, keyword, foundproduk);
    }
  }
}
// Function search produk
void searchProdukBST(Akunptr *akun, char keyword[], Produkptr *foundproduk) {
  if(*akun!=NULL) {
    Tokoptr tokoptr = (*akun)->Tokoptr;
    while(tokoptr!=NULL){
      Produkptr produkptr = tokoptr->Produkptr;
      while(produkptr!=NULL){
        if (strcmp(produkptr->nama, keyword) == 0)
          *foundproduk = produkptr;
        produkptr = produkptr->nextproduk;
      }
      tokoptr = tokoptr->nexttoko;
    }
    searchProdukBST(&(*akun)->leftptr, keyword, foundproduk);
    searchProdukBST(&(*akun)->rightptr, keyword, foundproduk);
  }
  
}

// Updated searchProduk function using BST
void searchProduk(Akunptr *startakun, Akunptr akun) {
  char keyword[MAX_NAMA];
  int found = 0;

  printf("Cari : ");
  scanf("%s",
        keyword); // keyword menyimpan nama produk yang ingin dicari pengguna

  Akun *currentAkun = akun;
  Toko *currentToko;
  Produk *foundProduk = NULL;
  History *newHistory;

  searchProdukBST(startakun, keyword, &foundProduk);
    currentToko = currentAkun->Tokoptr;
      if (foundProduk != NULL) {
        printf("Produk ditemukan!\n");
        printf("Nama : %s\n", foundProduk->nama);
        printf("Toko : %s\n", currentToko->nama);
        printf("Alamat : %s\n", currentToko->alamat);
        printf("Berat : %d\n", foundProduk->berat);
        printf("Detail : %s\n", foundProduk->detail);
        printf("Jumlah : %d\n", foundProduk->jumlah);
        printf("Harga : %d\n", foundProduk->harga);
        found = 1;

        // Tambahkan ke history
        newHistory = (History *)malloc(sizeof(History));
        if (newHistory != NULL) {
          newHistory->Produkptr = foundProduk;
          newHistory->nexthistory = currentAkun->Historyptr;
          currentAkun->Historyptr = newHistory;
      currentToko = currentToko->nexttoko;
    }
    currentAkun = currentAkun->rightptr;
  }

  if (!found) {
    printf("\"%s\" tidak ditemukan.\n", keyword);
  }
}

void customerMenu(Akunptr *startakun, Akunptr akun) {
  int choice;
  Produk *last_two[2] = {NULL, NULL};

  // Mendapatkan data 2 produk terakhir yang dicari pengguna
  historyProduct(akun->Historyptr, last_two);

  while (1) {
    printf("===== Menu Customer =====\n");

    // Menampilkan riwayat pencarian
    if (last_two[0] != NULL) {
      printf("1. %s\n", last_two[0]->nama);
    } else {
      printf("1. Tidak ada riwayat pencarian.\n");
    }
    if (last_two[1] != NULL) {
      printf("2. %s\n", last_two[1]->nama);
    } else {
      printf("2. Tidak ada riwayat pencarian.\n");
    }

    printf("3. Cari produk\n");
    printf("4. Kembali\n");
    printf("Pilihan: ");
    scanf("%d", &choice);

    switch (choice) {
    case 1:
    case 2:
      system("cls");
      if (last_two[choice-1] != NULL) {
        tampilDetail(last_two[choice-1]);
        opsi_pembelian(startakun, akun, last_two[choice-1]);
      } else {
        printf("Tidak ada riwayat pencarian.\n");
        system("pause");
        system("cls");
      }
      system("pause");
      system("cls");
      break;
    case 3:
      system("cls");
      // Function searching produk
      searchProduk(startakun, akun);
      // Update riwayat pencarian
      historyProduct(akun->Historyptr, last_two);
      system("pause");
      system("cls");
      break;
    case 4:
      system("cls");
      return;
    }
  }
}

void main_menu(Akunptr *startakun, Akunptr *akun) {
  int pilihan;
  do {
    printf("Selamat datang, %s\nSaldo anda : %d\n", (*akun)->username,
           (*akun)->saldo);
    printf("1. Cari produk di Trova\n");
    printf("2. Masuk ke Trova Seller\n");
    printf("3. Top up saldo\n");
    printf("4. Logout\n");
    printf("> ");
    scanf("%d", &pilihan);

    switch (pilihan) {
    case 1:
      system("cls");
      customerMenu(startakun, *akun);
      break;
    case 2:
      system("cls");
      manage_toko(akun);
      break;
    case 3:
      (*akun)->saldo += topup();
      break;
    case 4:
      keluar(*startakun);
      *akun = NULL;
      return;
    default:
      printf("Pilihan tidak valid\n");
    }
  } while (pilihan != 4);
}

int topup() {
  int nominal;
  printf("Masukkan nominal top up : ");
  scanf("%d", &nominal);
  system("cls");
  return nominal;
}

void opsi_pembelian(Akunptr *startakun, Akunptr akun, Produkptr produk) {
  while (1) {
    int opsi;
    printf("1. Beli\n2. Kembali\n> ");
    scanf("%d", &opsi);
    switch (opsi) {
    case 1:
      beli(startakun, akun, produk);
      return;
    case 2:
      return;
    }
  }
}

void beli(Akunptr *startakun, Akunptr akun, Produkptr produk) {
  char nama_pembeli[MAX_NAMA], alamat_pembeli[50], nomor_pembeli[20];
  int jumlah, total_berat, total_harga;
  char konfirmasi;
  do {
    printf("Masukkan jumlah produk yang ingin dibeli : ");
    scanf("%d", &jumlah);
    total_berat = produk->berat * jumlah;
    total_harga = produk->harga * jumlah + 9000 * total_berat * 0.001 + 1000;
    if (jumlah <= produk->jumlah) {
      printf("Masukkan nama : ");
      scanf("%s", nama_pembeli);
      printf("Masukkan alamat : ");
      scanf("%s", alamat_pembeli);
      printf("Masukkan nomor telp : ");
      scanf("%s", nomor_pembeli);
      printf("|==============================|\n"
             "|             Nama             |\n"
             "|%30s|\n"
             "|            Alamat            |\n"
             "|%30s|\n"
             "|          Nomor telp          |\n"
             "|%30s|\n"
             "|            Produk            |\n"
             "|%30s|\n"
             "|         Total Berat          |\n"
             "|%30d|\n"
             "|         Total Produk         |\n"
             "|%30d|\n"
             "|         Biaya Admin          |\n"
             "|            1000              |\n"
             "|         Total Harga          |\n"
             "|%30d|\n"
             "|==============================|\n"
             "Konfirmasi pembayaran (y/n)\n",
             nama_pembeli, alamat_pembeli, nomor_pembeli, produk->nama,
             total_berat, jumlah, total_harga);
      scanf(" %c", &konfirmasi);
      switch (konfirmasi) {
      case 'y':
      case 'Y':
        if (akun->saldo >= total_harga) {
          akun->saldo -= total_harga;
          cari_penjual(*startakun, produk, total_harga, jumlah);
        } else {
          printf("Saldo anda kurang!\n");
          system("pause");
          system("cls");
        }
        return;
      case 'n':
      case 'N':
        return;
      }
    } else {
      printf("Stok produk tidak cukup\n");
    }
  } while (jumlah > produk->jumlah);
}

void cari_penjual(Akunptr akun, Produkptr produk, int nominal, int jumlah) {
  if(akun!=NULL) {
    Tokoptr tokoptr = akun->Tokoptr;
    while(tokoptr!=NULL){
      if(tokoptr->Produkptr!=NULL) { 
        if(cari_produk_penjual(tokoptr->Produkptr, produk, jumlah)) {
          akun->saldo += nominal;
        }
      }
      tokoptr = tokoptr->nexttoko;
    }
    cari_penjual(akun->leftptr, produk, nominal, jumlah);
    cari_penjual(akun->rightptr, produk, nominal, jumlah);
  }
}

int cari_produk_penjual(Produkptr produkptr, Produkptr produk, int jumlah) {
  if (produkptr != NULL) {

    if (strcmp(produkptr->nama, produk->nama) == 0) {
        produkptr->jumlah -= jumlah;
        return 1;
    } else if (strcmp(produkptr->nama, produk->nama) < 0) {
      return cari_produk_penjual(produkptr->right, produk, jumlah);
    } else {
      return cari_produk_penjual(produkptr->left, produk, jumlah);
    }
  }
}

int main() {
  Akunptr startakun = NULL, akunterpilih = NULL;
  int opsi;

  load_database(&startakun);
  do {
    printf("Selamat Datang di Trova!\n");
    printf("===== LOGIN =====\n");
    printf("1. Login\n2. Register\n3. Keluar\n> ");
    scanf("%d", &opsi);
    switch (opsi) {
    case 1:
      if (masuk(&startakun, &akunterpilih))
        main_menu(&startakun, &akunterpilih);
      break;
    case 2:
      registrasi(&startakun);
      break;
    case 3:
      keluar(startakun);
      break;
    default:
      printf("Input salah\n");
      system("pause");
      system("cls");
    }
  } while (opsi != 3);

  free_memory(startakun);

  return 0;
}
