#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

// Pecah "a|b|c" menjadi {"a","b","c"}
inline vector<string> Pecah(const string& s, char pemisah = '|') {
    vector<string> hasil;
    string bagian;
    istringstream in(s);
    while (getline(in, bagian, pemisah)) hasil.push_back(bagian);
    return hasil;
}

// Angka bulat tanpa huruf/simbol/desimal; -1 jika tidak valid
inline int ParseInt(const string& s) {
    if (s.empty() || s.size() > 9) return -1;
    for (char c : s) if (c < '0' || c > '9') return -1;
    return stoi(s);
}

// "HH:MM" (00-23, 00-59) -> menit sejak 00:00; -1 jika tidak valid
inline int ParseJam(const string& s) {
    if (s.size() != 5 || s[2] != ':') return -1;
    int jam = ParseInt(s.substr(0, 2)), mnt = ParseInt(s.substr(3, 2));
    if (jam < 0 || jam > 23 || mnt < 0 || mnt > 59) return -1;
    return jam * 60 + mnt;
}

inline string FormatJam(int menit) {
    char buf[16];
    snprintf(buf, sizeof buf, "%02d:%02d", menit / 60, menit % 60);
    return buf;
}

inline string NamaHari(int hari) {
    static const char* nama[] = {"Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};
    return (hari >= 1 && hari <= 6) ? nama[hari - 1] : "?";
}

// do-while: ulangi sampai tidak kosong dan panjang dalam rentang [minPjg, maxPjg]
inline string MintaTeks(const string& label, size_t minPjg = 1, size_t maxPjg = 100) {
    string s;
    do {
        cout << label << ": ";
        getline(cin, s);
        if (!cin) exit(0);  // input habis (Ctrl+D): keluar, hindari loop tak berujung
        if (s.empty() || s.size() < minPjg || s.size() > maxPjg || s.find('|') != string::npos) {
            cout << "  ! " << label << " tidak boleh kosong, panjang " << minPjg << "-" << maxPjg
                 << " karakter, dan tanpa tanda '|'.\n";
            s.clear();
        }
    } while (s.empty());
    return s;
}

// do-while: ulangi sampai angka bulat dalam rentang [lo, hi]
inline int MintaAngka(const string& label, int lo, int hi) {
    int n;
    do {
        cout << label << ": ";
        string s;
        getline(cin, s);
        if (!cin) exit(0);
        n = ParseInt(s);
        if (n < lo || n > hi) cout << "  ! " << label << " harus angka bulat " << lo << "-" << hi << ".\n";
    } while (n < lo || n > hi);
    return n;
}

// do-while: ulangi sampai format HH:MM valid; mengembalikan menit
inline int MintaJam(const string& label) {
    int m;
    do {
        cout << label << " (HH:MM): ";
        string s;
        getline(cin, s);
        if (!cin) exit(0);
        m = ParseJam(s);
        if (m < 0) cout << "  ! " << label << " harus berformat HH:MM (00:00-23:59).\n";
    } while (m < 0);
    return m;
}
