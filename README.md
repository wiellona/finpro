# TROVA: *Online Shop Simulation*
## *About*
**TROVA** adalah program simulasi *online shop* ini dibuat sebagai tugas Proyek Akhir Semester mata kuliah Pemrograman Lanjut. Program simulasi ini dibuat untuk sisi penjual dan pembeli. Program ini dapat digunakan pengguna baik sebagai penjual maupun pembeli.

## Fitur
### Menu awal
Pada menu awal, pengguna dapat memilih untuk *login* jika sudah memiliki akun, melakukan registrasi jika belum memiliki akun, dan dapat keluar program.

![](./img/Menu%20awal.png)

Setelah *login*, program akan menampilkan bagian utama dari program yang menampilkan saldo yang dimiliki pengguna untuk bertransaksi. Terdapat menu lainnya seperti mencari produk, masuk sebagai penjual, dan top up saldo.

![](/img/Home.png)

### Sisi penjual
1. Membuat toko: Pengguna dapat membuat toko dengan mengisi nama dan alamat toko. Pengguna juga dapat menghapus toko yang dimilikinya
2. Menambah dan menghapus produk
3. Mengedit keterangan produk

### Sisi pembeli
1. Mencari produk yang tersedia
2. Melihat riwayat pencarian
3. Membeli barang

## *Functions*
### checkuser
Mengecek input *username* dan *password* yang dimasukkan pengguna dengan yang di *database*

### tambahakun
Membuat akun dengan menyimpan *username* dan *password* yang dimasukkan pengguna ke dalam database

### main_menu
Menampilkan menu awal di mana pengguna dapat menggunakan aplikasi sebagai pembeli maupun pengguna

### tambahtoko
Menyimpan nama dan alamat toko yang dibuat pengguna ke dalam database

### tambahproduk
Menyimpan detail produk toko ke dalam database

### hapustoko
Menghapus node toko

### hapusproduk
Menghapus node produk

### load_database
Mengambil data dari database sesuai struct

### save_database
Menyimpan semua data yang diinput pada saat program berjalan ke database

### manage_toko, manage_produk, edit_produk
Menjalankan fitur-fitur penjual seperti membuat dan menghapus toko, menambah dan mengurangi produk, mengedit detail produk

### historyProduk
Mengambil address node produk yang terakhir dicari oleh pengguna untuk ditampilkan dalam riwayat pencarian

### searchProductBST, searchProduk
Mencari produk yang terdapat pada databases

### beli
Melakukan pencarian produk, memasukkan nama dan alamat pembeli, menghitung total harga, mencetak ringkasan pembelian

### cari_penjual, cari_produk_penjual
Mendapatkan informasi semua produk yang terdapat di database