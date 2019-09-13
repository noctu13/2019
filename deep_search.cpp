#include<iostream>
#include<vector>
using namespace std;
#define int_p pair<int,int>
struct status {
    const int size;
    int sum = 0;
    int_p point = make_pair(0, 0);
    vector<int> falue_counter;
    vector<int_p> chain = vector<int_p>();
    status(int sz) :size(sz) {
        falue_counter.resize(sz);
    }
};
int f(status *s) {
    if ((*s).point.first == (*s).size && (*s).point.second == 0)
        return 0;
    if(!(*s).chain.empty())
        if ((*s).point.first == (*s).size) {//конец столбца
            (*s).point = (*s).chain.back();
            ++(*s).point.first;
            ++(*s).falue_counter[(*s).point.second];
            for (int i = (*s).point.second + 1; i < (*s).falue_counter.size(); ++i)
                (*s).falue_counter[i] = 0;
            (*s).chain.pop_back();
            return f(s);
        }
    for (int i = 0; i < (*s).chain.size(); ++i) {//пересечение
        int df = abs((*s).chain[i].first - (*s).point.first),
            ds = abs((*s).chain[i].second - (*s).point.second);
        if (df == 0 || ds == 0 || df == ds) {
            ++(*s).falue_counter[(*s).point.second];
            ++(*s).point.first;
            return f(s);
        }
    }
    if ((*s).falue_counter[(*s).point.second] == (*s).size){//конец строки
            (*s).point = (*s).chain.back();
            ++(*s).point.first;
            (*s).chain.pop_back();
        }
    (*s).chain.push_back((*s).point);//добавить в цепь
    if ((*s).chain.size() == (*s).size) {//полная цепь
        (*s).point = (*s).chain.back();
        ++(*s).point.first;
        ++(*s).sum;
        for (int i = 0; i < (*s).size; ++i)
            (*s).falue_counter[i] = 0;
        (*s).chain.pop_back();
        return f(s);
    }
    ++(*s).point.second;//смещение справо
    (*s).point.first = 0;
    return f(s);
}
int main() {
    int n;
    cin >> n;
    status a(n);
    f(&a);
    cout << a.sum;
    return 0;
}