#pragma once

using namespace std;

#include <cstdlib>
#include <iostream>
#include <string>
#include <tuple>

// 駒の種類を識別するクラス
// 駒の種類だけでなく、その向きを指定し、識別する。

class Piece {
   public:
    static const int PIECE_SIZE = 3;
    static const int RC_SIZE = 3;
    static const int PIECE_KIND = 4;

    enum { XC = 0, YC, ZC };

    Piece();
    Piece(int kind, int rdx, int rdy);

    Piece(const Piece &rhs);
    Piece &operator=(const Piece &rhs);

    bool operator==(const Piece &rhs) const;

    int get_kind() const;
    int get_rdx() const;
    int get_rdy() const;
    void get_rcs(tuple<int, int, int> *dst_rcs) const;

    string get_string() const;

   private:
    /*

    *     *
    #  *# ## *##
    ## ##  #  #

    基準点を*として上に示した。

    */

    int kind;  // LOSTを0123として割り振る
    int rdx, rdy;
    // 回転方向を指定。0,1,2,3から指定する。時計回りに(90n)度回転させた時のnを指定する。y軸については上記の駒を左側から見た時に時計回りでの角度で考える。

    tuple<int, int, int> rcs[RC_SIZE];
    // relative_coordinates
    // 基準小体に対して、その他小体の相対的な座標の情報。

    void init_rcs(tuple<int, int, int> *rcs);

    string str_rcs(int z) const;
};