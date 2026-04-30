#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>

using namespace std;

class User {
public:
    string username;
    string idGame;
};
class TransaksiManager{
    public:
        int generateKodeUnik() {
        return rand() % 900 + 100; // 100 - 999
        }
        bool getDetailPaket(int pilihan, string &namaPaket, long long &hargaBase) {
        if (pilihan == 1) { namaPaket = "50 Diamonds"; hargaBase = 10000; return true;}
        else if (pilihan == 2) { namaPaket = "250 Diamonds"; hargaBase = 50000; return true;}
        else if (pilihan == 3) { namaPaket = "1000 Diamonds"; hargaBase = 150000; return true;}
        return false; 
        }

        bool getBayar(int pilihan, string &pay){
            if (pilihan == 1) {pay = "QRIS"; return true;}
            else if (pilihan == 2 ) {pay = "Transfer Bank"; return true;}
            return false;
        }
};

class FileLogger {
private:
    string namaFile = "history_topup.txt";

public:
    void catatTransaksi(string nama, string id, string item, long long bayar, int kode, string cara) {
        ofstream fileLog(namaFile, ios::app);
        if (fileLog.is_open()) {
            fileLog 
            << "User        : " << nama << endl 
            << "ID pelanggan:" << " (" << id << ")" << endl 
            << "Item        : " << item << endl 
            << "Total       : Rp" << bayar << endl 
            << "Cara Bayar  : " << cara << endl
            << "Kode        : " << kode << endl 
            << "STATUS      : SUKSES" << endl
            << "-------------------------------------" << endl;
            fileLog.close();
        } else {
            cout << "[!] Gagal membuka file history untuk menulis." << endl;
        }
    }

    void bacaHistory() {
        ifstream fileBaca(namaFile);
        string teks;
        if (fileBaca.is_open()) {
            cout << "\n--- ISI FILE HISTORY ---" << endl;
            while (getline(fileBaca, teks)) {
                cout << teks << endl;
            }
            fileBaca.close();
        } else {
            cout << "Belum ada riwayat transaksi." << endl;
        }
    }
};

class SistemTopUp {
private:
    User pelanggan;
    TransaksiManager managerTransaksi;
    FileLogger logger;

    void tampilkanMenuPembelian() {
        cout << "\n--- DAFTAR PAKET ---" << endl;
        cout << "1. 50 Diamonds  - Rp10.000" << endl;
        cout << "2. 250 Diamonds - Rp50.000" << endl;
        cout << "3. 1000 Diamonds- Rp150.000" << endl;
        cout << "Pilih paket (1-3): ";
        
        int pkt;
        cin >> pkt;

        cout << "\n--- Pilih Cara Bayar ---" << endl;
        cout << "1. QRIS" << endl;
        cout << "2. Transfer Bank" << endl;
        cout << "Pilih paket (1-2): ";
        
        int cr;
        cin >> cr;

        string namaPaket;
        long long hargaBase;

        // Validasi paket menggunakan TransaksiManager
        if (!managerTransaksi.getDetailPaket(pkt, namaPaket, hargaBase)) {
            cout << "Paket tidak valid!" << endl;
            return; // Kembali ke menu utama
        }
        
        string namaBayar;
        if (!managerTransaksi.getBayar(cr, namaBayar)) {
            cout << "Paket tidak valid!" << endl;
            return; // Kembali ke menu utama
        }

        prosesPembayaran(namaPaket, hargaBase, namaBayar);
    }

    void prosesPembayaran(string namaPaket, long long hargaBase, string namaBayar) {
        int kodeUnik = managerTransaksi.generateKodeUnik();
        long long totalTagihan = hargaBase; 

        cout << "\n--- RINCIAN TAGIHAN ---" << endl;
        cout << "Item      : " << namaPaket << endl;
        cout << "Harga     : Rp" << hargaBase << endl;
        cout << "Kode Unik : " << kodeUnik << endl;
        cout << "Cara Bayar: " << namaBayar << endl;
        cout << "-----------------------" << endl;
        cout << "TOTAL     : Rp" << totalTagihan << endl;

        long long uangDibayar;
        cout << "\nMasukkan nominal uang yang Anda bayar: Rp";
        cin >> uangDibayar;

        if (uangDibayar < totalTagihan) {
            cout << "\n[!] TRANSAKSI GAGAL [!]" << endl;
            cout << "Uang Anda kurang Rp" << totalTagihan - uangDibayar << endl;
            cout << "Silakan ulangi transaksi dengan uang pas/lebih." << endl;
            return;
        }

        int inputKode;
        cout << "Masukkan kembali Kode Unik (" << kodeUnik << ") untuk validasi: ";
        cin >> inputKode;

        if (inputKode == kodeUnik) {
            cout << "\n[✔] TRANSAKSI BERHASIL!" << endl;
            if (uangDibayar > totalTagihan) {
                cout << "Kembalian Anda: Rp" << uangDibayar - totalTagihan << endl;
            }
            
            logger.catatTransaksi(pelanggan.username, pelanggan.idGame, namaPaket, totalTagihan, kodeUnik, namaBayar);
        } else {
            cout << "\n[!] GAGAL: Kode unik yang dimasukkan salah!" << endl;
        }
    }

public:
    void inisialisasi() {
        srand(time(0));
        cout << "=== SELAMAT DATANG DI MIDZY-TOPUP ===" << endl;
        cout << "Masukkan Username: ";
        getline(cin, pelanggan.username);
        cout << "Masukkan ID Game : ";
        getline(cin, pelanggan.idGame);
    }

    void jalankan() {
        inisialisasi();
        int pilihan;

        do {
            cout << "\nHalo, " << pelanggan.username << "!" << endl;
            cout << "1. Beli Paket Diamond/VP" << endl;
            cout << "2. Lihat History (File)" << endl;
            cout << "3. Keluar" << endl;
            cout << "Pilih menu: ";
            cin >> pilihan;

            switch (pilihan) {
                case 1:
                    tampilkanMenuPembelian();
                    break;
                case 2:
                    logger.bacaHistory();
                    break;
                case 3:
                    system("cls || clear");
                    cout << "Program selesai. Sampai jumpa!" << endl;
                    break;
                default:
                    cout << "Pilihan tidak valid, silakan coba lagi." << endl;
            }
        } while (pilihan != 3);
    }
};

int main() {
    SistemTopUp aplikasi;
    aplikasi.jalankan();
    
    return 0;
}
