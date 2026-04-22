#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>

using namespace std;

// Struktur data user (Tanpa saldo tetap)
struct User {
    string username;
    string idGame;
};

int generateKodeUnik() {
    return rand() % 900 + 100; // 100 - 999
}

void catatTransaksi(string nama, string id, string item, long long bayar, int kode) {
    ofstream fileLog("history_topup.txt", ios::app);
    fileLog 
    << "User        : " << nama << endl 
    << "ID pelanggan:" << " (" << id << ")" << endl 
    << "Item        : " << item << endl 
    << "Total       : " << bayar << endl 
    << "Kode        : " << kode << endl 
    << "STATUS      : SUKSES" << endl
    << "-------------------------------------";
    fileLog.close();
}

int main() {
    srand(time(0));
    User pelanggan;
    
    cout << "=== SELAMAT DATANG DI MIDZY-TOPUP ===" << endl;
    cout << "Masukkan Username: ";
    getline(cin, pelanggan.username);
    cout << "Masukkan ID Game : ";
    getline(cin, pelanggan.idGame);

    int pilihan;
    do {
        cout << "\nHalo, " << pelanggan.username << "!" << endl;
        cout << "1. Beli Paket Diamond/VP" << endl;
        cout << "2. Lihat History (File)" << endl;
        cout << "3. Keluar" << endl;
        cout << "Pilih menu: ";
        cin >> pilihan;

        if (pilihan == 1) {
            cout << "\n--- DAFTAR PAKET ---" << endl;
            cout << "1. 50 Diamonds  - Rp10.000" << endl;
            cout << "2. 250 Diamonds - Rp50.000" << endl;
            cout << "3. 1000 Diamonds- Rp150.000" << endl;
            cout << "Pilih paket (1-3): ";
            int pkt; cin >> pkt;

            string namaPaket;
            long long hargaBase;

            if (pkt == 1) { namaPaket = "50 Diamonds"; hargaBase = 10000; }
            else if (pkt == 2) { namaPaket = "250 Diamonds"; hargaBase = 50000; }
            else if (pkt == 3) { namaPaket = "1000 Diamonds"; hargaBase = 150000; }
            else { cout << "Paket tidak valid!" << endl; continue; }

            int kodeUnik = generateKodeUnik();
            long long totalTagihan = hargaBase;

            cout << "\n--- RINCIAN TAGIHAN ---" << endl;
            cout << "Item      : " << namaPaket << endl;
            cout << "Harga     : Rp" << hargaBase << endl;
            cout << "Kode Unik : " << kodeUnik << endl;
            cout << "-----------------------" << endl;
            cout << "TOTAL     : Rp" << totalTagihan << endl;

            // INPUT PEMBAYARAN MANUAL
            long long uangDibayar;
            cout << "\nMasukkan nominal uang yang Anda bayar: Rp";
            cin >> uangDibayar;

            if (uangDibayar < totalTagihan) {
                cout << "\n[!] TRANSAKSI GAGAL [!]" << endl;
                cout << "Uang Anda kurang Rp" << totalTagihan - uangDibayar << endl;
                cout << "Silakan ulangi transaksi dengan uang pas/lebih." << endl;
            } else {
                // Konfirmasi Kode Unik
                int inputKode;
                cout << "Masukkan kembali Kode Unik (" << kodeUnik << ") untuk validasi: ";
                cin >> inputKode;

                if (inputKode == kodeUnik) {
                    cout << "\n[✔] TRANSAKSI BERHASIL!" << endl;
                    if (uangDibayar > totalTagihan) {
                        cout << "Kembalian Anda: Rp" << uangDibayar - totalTagihan << endl;
                    }
                    catatTransaksi(pelanggan.username, pelanggan.idGame, namaPaket, totalTagihan, kodeUnik);
                } else {
                    cout << "\n[!] GAGAL: Kode unik yang dimasukkan salah!" << endl;
                }
            }

        } else if (pilihan == 2) {
            cout << "\n--- ISI FILE HISTORY ---" << endl;
            ifstream fileBaca("history_topup.txt");
            string teks;
            if (fileBaca.is_open()) {
                while (getline(fileBaca, teks)) cout << teks << endl;
                fileBaca.close();
            } else {
                cout << "Belum ada riwayat transaksi." << endl;
            }
        }

    } while (pilihan != 3);

    system("cls");
    cout << "Program selesai. Sampai jumpa!" << endl;
    return 0;
}