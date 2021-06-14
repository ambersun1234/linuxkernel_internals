#include <iostream>
#include <string>

#include "xs.h"

#define MAX_LEN 300

using namespace std;

string tmp[MAX_LEN];
xs tmp2[MAX_LEN];

string gen_random(const int len) {
    string tmp_s;
    static const char alphanum[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    
    srand((unsigned)time(NULL) * getpid());
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) 
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    return tmp_s;
}

void put_xs(string tmp3, int i) {
    tmp2[i] = *xs_new(&xs_literal_empty(), tmp3.c_str());
}

void put_stdstring(string tmp3, int i) {
    tmp[i] = tmp3;
}

int main(int argc, const char *argv[]) {
    for (int i = 1; i < MAX_LEN; i++) {
        string tmp3 = gen_random(i);

        put_xs(tmp3, i);
        put_stdstring(tmp3, i);

        cout << "xs " << i << ": " << xs_data(&tmp2[i]) << endl;
        cout << "std " << i << ": " << tmp[i] << endl;
        cout << endl;
    }

    return 0;
}