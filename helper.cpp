#include <stdlib.h>
#include <vector>
#include <iostream>
#include <Kernel/math.h>

using namespace std;

float rand_normal(float mean, float stddev){
    static int use_last = 0;
    static float y2;
    float x1, x2, w, y1;

    if (use_last) {
        y1 = y2;
        use_last = 0;
    } else {
        do {
            x1 = 2.0 * ((float) rand() / RAND_MAX) - 1.0;
            x2 = 2.0 * ((float) rand() / RAND_MAX) - 1.0;
            w = x1 * x1 + x2 * x2;
        } while (w >= 1.0 || w == 0.0);

        w = sqrt((-2.0 * log(w)) / w);
        y1 = x1 * w;
        y2 = x2 * w;
        use_last = 1;
    }

    return (mean + y1 * stddev);
}

bool check_dims(vector<int> a, vector<int> b){
    for(int i = 0; i < min(a.size(), b.size()); i++){
        if(a[a.size() - 1 - i] != b[b.size() - 1 - i] && a[a.size() - 1 - i] != 1 && b[b.size() - 1 - i] != 1){
            return false;
        }
    }
    return true;
}

vector<int> broadcast_dims(vector<int> a, vector<int> b){
    vector<int> result;
    int a_size = a.size();
    int b_size = b.size();
    int max_size = max(a_size, b_size);
    for(int i = 0; i < max_size; i++){
        int a_index = a_size - 1 - i;
        int b_index = b_size - 1 - i;
        if(a_index < 0){
            result.push_back(b[b_index]);
        } else if(b_index < 0){
            result.push_back(a[a_index]);
        } else {
            result.push_back(max(a[a_index], b[b_index]));
        }
    }
    reverse(result.begin(), result.end());
    return result;
}
