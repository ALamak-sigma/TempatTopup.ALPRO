#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>

using namespace std;

// DATA GAME & HARGA
string NAMA_PAKET[3] = 
    {"50 Diamonds/UC", 
    "250 Diamonds/UC", 
    "1000 Diamonds/UC"};
string NAMA_GAME[3] = 
    {"Mobile Legends", 
    "Free Fire", 
    "PUBG Mobile"};
long long TABEL_HARGA_GAME[3][3] = {
    {10000, 50000, 150000},  // Mobile Legends
    {12000, 55000, 160000},  // Free Fire
    {15000, 60000, 170000}}; // PUBG Mobile

// FUNGSI SORTING & SEARCHING

// Bubble Sort: urutkan angka dari kecil ke besar
void bubbleSortAsc(long long arr[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                // tukar posisi arr[j] dan arr[j+1]
                long long temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Linear Search (iteratif): cek data satu-satu dari awal sampai ketemu
int cariLinearIteratif(long long arr[], int n, long long target)
{
    for (int i = 0; i < n; i++)
    {
        if (arr[i] == target)
            return i;
    }
    return -1; // tidak ketemu
}

// Binary Search (rekursif): cari di array yang SUDAH terurut
int cariBinerRekursif(long long arr[], int kiri, int kanan, long long target)
{
    if (kiri > kanan)
        return -1; // tidak ketemu

    int tengah = (kiri + kanan) / 2;

    if (arr[tengah] == target)
        return tengah;
    else if (arr[tengah] < target)
        return cariBinerRekursif(arr, tengah + 1, kanan, target); // cari di bagian kanan
    else
        return cariBinerRekursif(arr, kiri, tengah - 1, target); // cari di bagian kiri
}

// Jumlahkan semua isi array (iteratif)
long long totalIteratif(long long arr[], int n)
{
    long long total = 0;
    for (int i = 0; i < n; i++)
    {
        total += arr[i];
    }
    return total;
}

// ================== CLASS SISTEM TOP UP ==================

class SistemTopUp
{
private:
    // Data user yang lagi login, disimpan sdi variabel
    string username;
    string idGame;
    string namaGame;
    int gameIndex;
    string namaFile = "history_topup.txt";

    int generateKodeUnik()
    {
        return rand() % 900 + 100; // hasilnya 100-999
    }

    void catatTransaksi(string item, long long bayar, int kode, string cara)
    {
        ofstream fileLog(namaFile, ios::app);

        if (!fileLog.is_open())
        {
            cout << "[!] Gagal membuka file history untuk menulis." << endl;
            return;
        }

        fileLog << "User        : " << username << endl;
        fileLog << "ID pelanggan: (" << idGame << ")" << endl;
        fileLog << "Game        : " << namaGame << endl;
        fileLog << "Item        : " << item << endl;
        fileLog << "Total       : Rp" << bayar << endl;
        fileLog << "Cara Bayar  : " << cara << endl;
        fileLog << "Kode        : " << kode << endl;
        fileLog << "STATUS      : SUKSES" << endl;
        fileLog << "-------------------------------------" << endl;

        fileLog.close();
    }

    // Ambil semua nominal "Total" dari file history, masukkan ke array arr[]
    int bacaNominalKeArray(long long arr[], int maxSize)
    {
        ifstream fileBaca(namaFile);
        int jumlah = 0;

        if (!fileBaca.is_open())
            return 0;

        string teks;
        while (getline(fileBaca, teks) && jumlah < maxSize)
        {
            if (teks.find("Total") != string::npos)
            {
                size_t pos = teks.find("Rp");
                if (pos != string::npos)
                {
                    string angkaStr = teks.substr(pos + 2);
                    arr[jumlah] = stoll(angkaStr);
                    jumlah++;
                }
            }
        }
        fileBaca.close();
        return jumlah;
    }

    void prosesPembayaran(string namaPaket, long long hargaBase, string namaBayar)
    {
        int kodeUnik = generateKodeUnik();
        long long totalTagihan = hargaBase;

        cout << "\n--- RINCIAN TAGIHAN ---" << endl;
        cout << "Game      : " << namaGame << endl;
        cout << "Item      : " << namaPaket << endl;
        cout << "Harga     : Rp" << hargaBase << endl;
        cout << "Kode Unik : " << kodeUnik << endl; 
        cout << "Cara Bayar: " << namaBayar << endl;
        cout << "-----------------------" << endl;
        cout << "TOTAL     : Rp" << totalTagihan << endl;

        long long uangDibayar;
        cout << "\nMasukkan nominal uang yang Anda bayar: Rp";
        cin >> uangDibayar;

        if (uangDibayar < totalTagihan)
        {
            cout << "\n[!] TRANSAKSI GAGAL [!]" << endl;
            cout << "Uang Anda kurang Rp" << totalTagihan - uangDibayar << endl;
            cout << "Silakan ulangi transaksi dengan uang pas/lebih." << endl;
            return;
        }

        int inputKode;
        cout << "Masukkan kembali Kode Unik (" << kodeUnik << ") untuk validasi: ";
        cin >> inputKode;

        if (inputKode != kodeUnik)
        {
            cout << "\n[!] GAGAL: Kode unik yang dimasukkan salah!" << endl;
            return;
        }

        cout << "\n[OK] TRANSAKSI BERHASIL!" << endl;
        if (uangDibayar > totalTagihan)
        {
            cout << "Kembalian Anda: Rp" << uangDibayar - totalTagihan << endl;
        }
        catatTransaksi(namaPaket, totalTagihan, kodeUnik, namaBayar);
    }

    void tampilkanMenuPembelian()
    {
        long long *hargaGameIni = TABEL_HARGA_GAME[gameIndex];

        cout << "\n--- DAFTAR PAKET UNTUK " << namaGame << " ---" << endl;
        for (int i = 0; i < 3; i++)
        {
            cout << (i + 1) << ". " << NAMA_PAKET[i] << " - Rp" << hargaGameIni[i] << endl;
        }
        cout << "Pilih paket (1-3): ";
        int pkt;
        cin >> pkt;

        if (pkt < 1 || pkt > 3)
        {
            cout << "Paket tidak valid!" << endl;
            return;
        }

        cout << "\n--- Pilih Cara Bayar ---" << endl;
        cout << "1. QRIS" << endl;
        cout << "2. Transfer Bank" << endl;
        cout << "Pilih cara bayar (1-2): ";
        int cr;
        cin >> cr;

        string namaBayar;
        if (cr == 1)
            namaBayar = "QRIS";
        else if (cr == 2)
            namaBayar = "Transfer Bank";
        else
        {
            cout << "Cara bayar tidak valid!" << endl;
            return;
        }

        string namaPaket = NAMA_PAKET[pkt - 1];
        long long hargaBase = hargaGameIni[pkt - 1];

        prosesPembayaran(namaPaket, hargaBase, namaBayar);
    }

    void mulai()
    {
        srand(time(0));
        cout << "=== SELAMAT DATANG DI MIDZY-TOPUP ===" << endl;

        cout << "Masukkan Username: ";
        getline(cin, username);

        cout << "Masukkan ID Game : ";
        getline(cin, idGame);

        int pilihGame;
        bool valid;
        do
        {
            cout << "\n--- PILIH GAME ---" << endl;
            for (int i = 0; i < 3; i++)
                cout << (i + 1) << ". " << NAMA_GAME[i] << endl;
            cout << "Pilih game (1-3): ";
            cin >> pilihGame;
            cin.ignore();

            valid = (pilihGame >= 1 && pilihGame <= 3);
            if (!valid)
                cout << "Pilihan game tidak valid, coba lagi." << endl;
        } while (!valid);

        gameIndex = pilihGame - 1;
        namaGame = NAMA_GAME[gameIndex];

        cout << "\nGame terpilih: " << namaGame << endl;
    }

public:
    void bacaHistory()
    {
        ifstream fileBaca(namaFile);

        if (!fileBaca.is_open())
        {
            cout << "Belum ada riwayat transaksi." << endl;
            return;
        }

        cout << "\n--- ISI FILE HISTORY ---" << endl;
        string teks;
        while (getline(fileBaca, teks))
        {
            cout << teks << endl;
        }
        fileBaca.close();
    }

    void urutkanDanCariRiwayat()
    {
        long long nominal[100];
        int jumlah = bacaNominalKeArray(nominal, 100);

        if (jumlah == 0)
        {
            cout << "\nBelum ada riwayat transaksi untuk diurutkan/dicari." << endl;
            return;
        }

        // nyimpen salinan data asli (belum diurutkan) buat linear search
        long long asli[100];
        for (int i = 0; i < jumlah; i++)
            asli[i] = nominal[i];

        cout << "\n--- NOMINAL SEBELUM DIURUTKAN ---" << endl;
        for (int i = 0; i < jumlah; i++)
            cout << "Rp" << nominal[i] << "  ";
        cout << endl;

        bubbleSortAsc(nominal, jumlah);

        cout << "--- NOMINAL SESUDAH DIURUTKAN (ASCENDING) ---" << endl;
        for (int i = 0; i < jumlah; i++)
            cout << "Rp" << nominal[i] << "  ";
        cout << endl;

        if (jumlah >= 2)
        {
            long long *P; // pointer yang akan menunjuk ke elemen array

            cout << "\n>>> DEMO PERPINDAHAN POINTER DATA RIWAYAT <<<" << endl;

            P = &nominal[0]; // pointer menunjuk ke data terkecil
            cout << "Awal pointer menunjuk (Data Terkecil) : Rp" << *P << endl;
            cout << "Alamat memori                         : " << P << endl;

            P = &nominal[jumlah - 1]; // pointer pindah menunjuk ke data terbesar
            cout << "Setelah pindah pointer (Data Terbesar): Rp" << *P << endl;
            cout << "Alamat memori                         : " << P << endl;
            cout << "---------------------------------------------" << endl;
        }

        cout << "Total seluruh nominal transaksi : Rp" << totalIteratif(nominal, jumlah) << endl;

        long long target;
        cout << "\nMasukkan nominal yang ingin dicari: Rp";
        cin >> target;

        int idxLinear = cariLinearIteratif(asli, jumlah, target);
        int idxBiner = cariBinerRekursif(nominal, 0, jumlah - 1, target);

        if (idxLinear != -1)
            cout << "[Linear Search - Iteratif] Ditemukan di data asli, posisi ke-" << (idxLinear + 1) << endl;
        else
            cout << "[Linear Search - Iteratif] Data tidak ditemukan." << endl;

        if (idxBiner != -1)
            cout << "[Binary Search - Rekursif] Ditemukan di data terurut, posisi ke-" << (idxBiner + 1) << endl;
        else
            cout << "[Binary Search - Rekursif] Data tidak ditemukan." << endl;
    }

    void tampilkanTabelHargaSemuaGame()
    {
        cout << "\n=== TABEL HARGA PAKET SEMUA GAME (ARRAY 2 DIMENSI) ===" << endl;
        cout << "Game\t\t";
        for (int j = 0; j < 3; j++)
            cout << NAMA_PAKET[j] << "\t\t";
        cout << endl;

        for (int i = 0; i < 3; i++)
        {
            cout << NAMA_GAME[i] << "\t\t";
            for (int j = 0; j < 3; j++)
            {
                cout << "Rp" << TABEL_HARGA_GAME[i][j] << "\t\t";
            }
            cout << endl;
        }

        cout << "\nPilih nomor game untuk lihat detail sebagai array 1D (1-3): ";
        int pilihGame;
        cin >> pilihGame;

        if (pilihGame < 1 || pilihGame > 3)
        {
            cout << "Pilihan game tidak valid." << endl;
            return;
        }

        long long *baris = TABEL_HARGA_GAME[pilihGame - 1];

        cout << "\nDetail harga " << NAMA_GAME[pilihGame - 1] << " (hasil ekstraksi jadi array 1D):" << endl;
        for (int j = 0; j < 3; j++)
        {
            cout << NAMA_PAKET[j] << "\t: Rp" << baris[j] << endl;
        }
        cout << "Total harga semua paket         : Rp" << totalIteratif(baris, 3) << endl;
    }

    void run()
    {
        mulai();

        int pilihan;
        do
        {
            cout << "\nHalo, " << username << "!" << endl;
            cout << "1. Beli Paket Diamond/VP" << endl;
            cout << "2. Lihat History (File)" << endl;
            cout << "3. Urutkan & Cari Riwayat Transaksi (Sorting & Searching)" << endl;
            cout << "4. Bandingkan Harga Semua Game (Array 2D)" << endl;
            cout << "5. Keluar" << endl;
            cout << "Pilih menu: ";
            cin >> pilihan;

            switch (pilihan)
            {
            case 1:
                tampilkanMenuPembelian();
                break;
            case 2:
                bacaHistory();
                break;
            case 3:
                urutkanDanCariRiwayat();
                break;
            case 4:
                tampilkanTabelHargaSemuaGame();
                break;
            case 5:
                cout << "Program selesai. Sampai jumpa!" << endl;
                break;
            default:
                cout << "Pilihan tidak valid, silakan coba lagi." << endl;
            }
        } while (pilihan != 5);
    }
};

// ================== PROGRAM UTAMA ==================

int main()
{
    SistemTopUp apk;
    apk.run();

    return 0;
}