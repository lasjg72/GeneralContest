#include <iostream>
#include <fstream>
#include <algorithm>
#include <tuple>
#include <vector>
#include <string>
#include <queue>
#include <cmath>
#include <set>
#include <map>
#include <cassert>
#include <array>

#define MAXCNT 10

struct card {
    string name_en;
    char rarity;
    char color;
    int cost;
    
};

using namespace std;
using ll = long long;



int main()
{
    array<card, MAXCNT> stars;  // 構造体配列の宣言

    ifstream stream("starsdata.txt");
    if( !stream.is_open() ) {
        cout << "ファイルがオープンできません\n";
        exit( 1 );
    }

    for ( int i = 0; i < MAXCNT; i++ ) {
        if ( !(stream >> stars[i].name >> stars[i].height >> stars[i].weight) ) break;
    }

    // 本来なら、入力したデータを関数などに渡して使います
    // たとえば一番背の高い人を調べる関数
    // int whoIsTallest( shape_t stars[], int num ) など
    // 関数との受け渡しは次回のTipsで ...

    int mt = 3;  // ここでは3が返されたことにします
    cout << "The tallest is " << stars[mt].name << '(' << stars[mt].height << "cm)\n";
    return 0;
}