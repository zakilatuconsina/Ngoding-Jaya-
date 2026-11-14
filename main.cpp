#include "jadwal.hpp"

void TampilkanMenu() {
    cout << "\n1. Tambah Ruang\n2. Tambah Jadwal\n3. Tampilkan Ruang dan Jadwal\n4. Keluar\nPilihan: ";
}

void MenuUtama() {
    int pilihan;
    do {
        TampilkanMenu();
        string s;
        getline(cin, s);
        if (!cin) break;
        pilihan = ParseInt(s);
        if (pilihan < 1 || pilihan > 4) {  // tidak kosong, angka bulat, rentang 1-4
            cout << "Pilihan harus angka 1-4.\n";
            pilihan = 0;
            continue;
        }
        switch (pilihan) {
            case 1: TambahRuang(); break;
            case 2: TambahJadwal(); break;
            case 3: TampilkanRuangJadwal(); break;
            case 4: break;
        }
    } while (pilihan != 4);
}

int main() {
    MuatRuang();
    MuatJadwal();
    cout << "Data dimuat: " << daftarRuang.size() << " ruang, " << daftarJadwal.size() << " jadwal.\n";
    MenuUtama();
    cout << "Data tersimpan. Terima kasih!\n";
}
