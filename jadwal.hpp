#pragma once
#include <algorithm>
#include "ruang.hpp"

struct Jadwal { string kodeMK, namaMK, kodeRuang; int hari, mulai, selesai, jumlahMhs; };

inline vector<Jadwal> daftarJadwal;

// Hari sama dan jam beririsan; jadwal yang hanya bersambung (10:00 | 10:00) aman
inline bool Beririsan(const Jadwal& a, const Jadwal& b) {
    return a.hari == b.hari && a.mulai < b.selesai && b.mulai < a.selesai;
}

// Satu kali telusur: bentrok (ruang sama + beririsan) dan kapasitas kurang
inline void AmanDitambahkan(const Jadwal& j, bool& bentrok, bool& kapasitasKurang) {
    bentrok = false;
    kapasitasKurang = j.jumlahMhs > CariRuang(j.kodeRuang)->kapasitas;
    for (size_t i = 0; i < daftarJadwal.size(); i++)
        if (daftarJadwal[i].kodeRuang == j.kodeRuang && Beririsan(daftarJadwal[i], j)) bentrok = true;
}

// Ruang lain yang cukup dan kosong di waktu itu, kapasitas terkecil dulu
inline vector<Ruang> RuangPengganti(const Jadwal& j) {
    vector<Ruang> hasil;
    for (size_t i = 0; i < daftarRuang.size(); i++) {
        Jadwal coba = j;
        coba.kodeRuang = daftarRuang[i].kode;
        bool b, k;
        AmanDitambahkan(coba, b, k);
        if (!b && !k) hasil.push_back(daftarRuang[i]);
    }
    sort(hasil.begin(), hasil.end(), [](const Ruang& a, const Ruang& b) { return a.kapasitas < b.kapasitas; });
    return hasil;
}

inline void SimpanJadwal(const Jadwal& j) {
    ofstream("jadwal.txt", ios::app) << j.kodeMK << '|' << j.namaMK << '|' << j.kodeRuang << '|' << j.hari << '|'
                                     << FormatJam(j.mulai) << '|' << FormatJam(j.selesai) << '|' << j.jumlahMhs << '\n';
}

inline void MuatJadwal() {
    ifstream f("jadwal.txt");
    string baris;
    while (getline(f, baris)) {
        auto p = Pecah(baris);
        if (p.size() != 7) continue;
        int hari = ParseInt(p[3]), mulai = ParseJam(p[4]), selesai = ParseJam(p[5]), jml = ParseInt(p[6]);
        if (hari < 1 || mulai < 0 || selesai < 0 || jml < 0) continue;
        daftarJadwal.push_back({p[0], p[1], p[2], hari, mulai, selesai, jml});
    }
}

inline void TambahJadwal() {
    if (daftarRuang.empty()) {
        cout << "Tambahkan data ruang terlebih dahulu.\n";
        return;
    }
    Jadwal j{};
    cout << "--- Tambah Jadwal ---\n";
    j.kodeMK = MintaTeks("Kode MK", 3, 10);
    j.namaMK = MintaTeks("Nama MK");
    j.jumlahMhs = MintaAngka("Jumlah mahasiswa", 1, 100);

    while (true) {
        // D: input ruang, hari, jam sampai valid
        while (true) {
            cout << "Daftar ruang:\n";
            for (size_t i = 0; i < daftarRuang.size(); i++)
                cout << "  " << daftarRuang[i].kode << " - " << daftarRuang[i].nama << " (kapasitas " << daftarRuang[i].kapasitas << ")\n";
            j.kodeRuang = MintaTeks("Kode ruang");
            if (!CariRuang(j.kodeRuang)) { cout << "  ! Kode ruang tidak terdaftar.\n"; continue; }
            j.hari = MintaAngka("Hari (1=Senin ... 6=Sabtu)", 1, 6);
            j.mulai = MintaJam("Jam mulai");
            j.selesai = MintaJam("Jam selesai");
            if (j.mulai >= j.selesai) { cout << "  ! Jam mulai harus lebih kecil dari jam selesai.\n"; continue; }
            break;
        }
        // P: pengecekan jadwal baru
        bool bentrok, kapasitasKurang;
        AmanDitambahkan(j, bentrok, kapasitasKurang);
        cout << "Status bentrok   : " << (bentrok ? "BENTROK" : "aman") << '\n';
        int kapasitas = CariRuang(j.kodeRuang)->kapasitas;
        cout << "Status kapasitas : " << (kapasitasKurang ? "KURANG " + to_string(j.jumlahMhs - kapasitas) + " kursi" : "cukup") << '\n';
        if (bentrok || kapasitasKurang) {
            auto saran = RuangPengganti(j);
            cout << "Saran ruang pengganti: ";
            if (saran.empty()) cout << "tidak ada";
            for (const Ruang& r : saran) cout << r.kode << " (" << r.kapasitas << ")  ";
            cout << '\n';
            string yn;
            do {
                cout << "Coba ruang/waktu lain? (y/n/yes/no): ";
                getline(cin, yn);
                if (!cin) exit(0);
                
                // Ubah string langsung jadi lowercase inline
                for (char& c : yn) c = tolower(c);
                
            } while (yn != "y" && yn != "yes" && yn != "n" && yn != "no");

            if (yn == "y" || yn == "yes") continue;  // kembali ke D
            cout << "Penambahan jadwal dibatalkan.\n";
            return;
        }
        daftarJadwal.push_back(j);
        SimpanJadwal(j);
        cout << "Jadwal berhasil ditambahkan.\n";
        return;
    }
}

inline void TampilkanRuangJadwal() {
    cout << "--- Daftar Ruang ---\n";
    if (daftarRuang.empty()) cout << "Belum ada data ruang.\n";
    else
        for (size_t i = 0; i < daftarRuang.size(); i++)
            cout << i + 1 << ". " << daftarRuang[i].kode << " | " << daftarRuang[i].nama << " | kapasitas " << daftarRuang[i].kapasitas << '\n';
    cout << "--- Daftar Jadwal ---\n";
    if (daftarJadwal.empty()) cout << "Belum ada data jadwal.\n";
    else
        for (size_t j = 0; j < daftarJadwal.size(); j++) {
            const Jadwal& d = daftarJadwal[j];
            cout << j + 1 << ". " << d.kodeMK << " | " << d.namaMK << " | " << d.kodeRuang << " | " << NamaHari(d.hari) << " | "
                 << FormatJam(d.mulai) << "-" << FormatJam(d.selesai) << " | " << d.jumlahMhs << " mhs\n";
        }
}