#include <argmax.hpp>

template <class T>
int argmax(vector<T> v) {
    return distance(v.begin(), max_element(v.begin(), v.end()));
}

// Tの型が分からない以上、オブジェクトファイルに実体を生成できていないので、undefined　referenceとなってしまう。
// よって、以下にてdouble,int型でargmax関数が呼び出される可能性があることを明示しておき、実体を生成しておく。
template int argmax(vector<double> v);
template int argmax(vector<int> v);