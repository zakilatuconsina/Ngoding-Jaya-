#pragma once
#include <fstream>
#include "util.hpp"

struct Ruang { string kode, nama; int kapasitas; };

inline vector<Ruang> daftarRuang;

// Pola pencarian: telusuri vector berdasarkan kode; nullptr jika tidak ada
inline const Ruang* CariRuang(const string& kode) {
    for (size_t i = 0; i < daftarRuang.size(); i++)
        if (daftarRuang[i].kode == kode) return &daftarRuang[i];
    return nullptr;
}

inline void SimpanRuang(const Ruang& r) {
    ofstream("ruang.txt", ios::app) << r.kode << '|' << r.nama << '|' << r.kapasitas << '\n';
}

inline void MuatRuang() {
    ifstream f("ruang.txt");
    string baris;
    while (getline(f, baris)) {
        auto p = Pecah(baris);
        if (p.size() != 3 || ParseInt(p[2]) < 0) continue;  // lewati baris rusak
        daftarRuang.push_back({p[0], p[1], ParseInt(p[2])});
    }
}

inline void TambahRuang() {
    string kode;
    while (true) {  // kode tidak valid -> kembali ke tampilan awal
        cout << "--- Tambah Ruang ---\n";
        kode = MintaTeks("Kode ruang", 3, 10);
        if (!CariRuang(kode)) break;
        cout << "  ! Kode ruang sudah terdaftar.\n";
    }
    string nama = MintaTeks("Nama ruang");
    int kapasitas = MintaAngka("Kapasitas kursi", 1, 100);
    Ruang r{kode, nama, kapasitas};
    daftarRuang.push_back(r);
    SimpanRuang(r);
    cout << "Ruang berhasil ditambahkan.\n";
}
