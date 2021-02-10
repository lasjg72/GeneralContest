#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>

using namespace std;
using ll = long long;

void ExceptionHandle(ll X, ll Y, int round_num){
    if(X <= 0){
        throw invalid_argument("参加人数を1人以上にしてください");
    }
    if(X > 1e10){ //世界人口を100億人と仮定
        throw invalid_argument("参加人数を100億人以下にしてください");
    }
    if(X <= Y){
        throw invalid_argument("0勝" + to_string(round_num) + "敗0分");
    }
}

//下当たりの組合わせにおいて勝数が多い人が必ず勝つ場合を計算するのがWinNumPairedDownWin
vector<vector<ll>> WinNumPairedDownWin(vector<vector<ll>> &win_num1, ll X, int round_num){

    //各ラウンド終了時における各勝数の人数を格納する配列win_num1とその初期化
    //ラウンド1終了時には勝数0と1の人が存在し、ラウンド2終了時には勝数0と1と2の人が存在する
    win_num1.resize(round_num+1);
    for(int i = 0; i <= round_num; i++){
        for(int j = 0; j < i+1; j++){
            win_num1[i].push_back(0);
        }
    }
    win_num1[0][0] = X;

    //win_num1のみの計算
    for(int i = 1; i <= round_num; i++){
        bool odd_count = false;
        for(int j = 0; j < (i+1)/2; j++){
            ll org = win_num1[i-1][j];
            //あるラウンドnで勝数0の人々はラウンドn+1において勝数0と1に分布される
            //パスカルの三角形のように左右対称な人数分布となる
            if(org % 2 == 0){
                win_num1[i][j] += org/2;
                win_num1[i][j+1] += org/2;
                if((i-1)%2 == 0 && j == i/2) break;
                win_num1[i][i-j] += org/2;
                win_num1[i][i-(j+1)] += org/2;
            }else{
                //下当たり発生時に勝数が多い人が必ず勝つ場合は下当たりが奇数回目か偶数回目かによってラウンド終了時の各勝数の人数が場合分けされる
                //ラウンド1終了時に勝数0と1の人が5人ずつ存在する場合、ラウンド2終了時の勝数0の人数は(5+1)/2=3人、勝数1の人数は(5-1)/2+(5-1)/2=4人、勝数2の人数は(5+1)/2=3人 
                if(org % 2 == 1) odd_count = !odd_count;
                if(odd_count){
                    win_num1[i][j] += (org+1)/2;
                    win_num1[i][j+1] += (org-1)/2;
                    if((i-1)%2 == 0 && j == i/2) break;
                    win_num1[i][i-j] += (org+1)/2;
                    win_num1[i][i-(j+1)] += (org-1)/2;
                }else{
                    win_num1[i][j] += (org-1)/2;
                    win_num1[i][j+1] += (org+1)/2;
                    if((i-1)%2 == 0 && j == i/2) break;
                    win_num1[i][i-j] += (org-1)/2;
                    win_num1[i][i-(j+1)] += (org+1)/2;
                }
            }
        }
    }
    return win_num1;
}

//下当たりの組合わせにおいて勝数が多い人が必ず勝つ場合を計算するのがWinNumPairedDownLose
void WinNumPairedDownLose(vector<vector<ll>> &win_num2,ll X, int round_num){

    win_num2.resize(round_num+1);
    for(int i = 0; i <= round_num; i++){
        for(int j = 0; j < i+1; j++){
            win_num2[i].push_back(0);
        }
    }
    win_num2[0][0] = X;

    //win_num2の計算
    //こちらは下当たり発生時に勝数が少ない人が必ず勝つ場合ということに注意
    for(int i = 1; i <= round_num; i++){
        bool odd_count = false;
        for(int j = 0; j < (i+1)/2; j++){
            ll org = win_num2[i-1][j];
            if(org % 2 == 0){
                win_num2[i][j] += org/2;
                win_num2[i][j+1] += org/2;
                if((i-1)%2 == 0 && j == i/2) break;
                win_num2[i][i-j] += org/2;
                win_num2[i][i-(j+1)] += org/2;
            }else{
                if(org % 2 == 1) odd_count = !odd_count;
                if(odd_count){
                    win_num2[i][j] += (org-1)/2;
                    win_num2[i][j+1] += (org+1)/2;    
                    if((i-1)%2 == 0 && j == i/2) break;
                    win_num2[i][i-j] += (org-1)/2;
                    win_num2[i][i-(j+1)] += (org+1)/2;
                }else{
                    win_num2[i][j] += (org+1)/2;
                    win_num2[i][j+1] += (org-1)/2;
                    if((i-1)%2 == 0 && j == i/2) break;
                    win_num2[i][i-j] += (org+1)/2;
                    win_num2[i][i-(j+1)] += (org-1)/2;
                }
            }
        }
    }
}

//最終ラウンド開始直前のスタンディングから目標順位Yを達成するための勝敗数を求める
string AnsCalc(vector<vector<ll>> &win_num1, vector<vector<ll>> &win_num2, ll Y, int round_num){
    ll tot1 = 0, tot2 = 0;
    for(int i = round_num-1; i >= 0; i--){
        tot1 += win_num1[round_num-1][i];
        tot2 += win_num2[round_num-1][i];
        if(tot1 == Y && tot2 == Y){
            //最終ラウンドにIDを行えるのは1つ前のラウンド時点で0敗～n敗までの総人数が目標順位Yと等しくなる非負整数nが存在するとき
            return to_string(i) + "勝" + to_string(round_num-i-1) + "敗1分";
        }else if(tot1 >= Y || tot2 >= Y){
            int loses = round_num - (i+1);
            ll top1 = 0, top2 = 0;
            for(int j = 0; j < loses+1; j++){
                top1 += win_num1[round_num][round_num-j];
                top2 += win_num2[round_num][round_num-j];
            }
            if(Y < top1 || Y < top2){
                //参加人数1000人、目標順位10位にてスイス10回戦を行った場合、ラウンド9終了時に9勝が2人、8勝が18人でることがありその場合は10勝しなければ確実に目標の10位以内に入ることはできない
                return to_string(i+1+1) + "勝" + to_string(round_num-(i+1+1)) + "敗0分";
            }else{
                return to_string(i+1) + "勝" + to_string(round_num-(i+1)) + "敗0分";
            }
        }
    }
    return 0;
}

int main()
{
    ll X, Y;
    vector<vector<ll>> win_num1, win_num2;
    cout << "参加人数X:";
    cin >> X;
    cout << "目標順位Y:";
    cin >> Y;
    int round_num = ceil(log2(X));

    try{
        ExceptionHandle(X, Y, round_num);
    }catch(invalid_argument& e){
        cout << e.what() << endl;
        return -1;
    }

    WinNumPairedDownWin(win_num1, X, round_num);
    WinNumPairedDownLose(win_num2, X, round_num);
    cout << AnsCalc(win_num1, win_num2, Y, round_num) << endl; 
    
    return 0;
}
