#include <cstdio>
#include <string>
#include <cstring>
#include <algorithm>
#include "retrieval.hpp"
using namespace std;
int main()
{
    init_system();

    char** q = new char*[2];
    char* q1 = "Holmes";
    char* q2 = "Henry Baskerville";
    q[0] = q1; q[1] = q2;

    char** ans;
    int ans_size;
    ans = intersection_search_for_events(2, q, ans_size);
    printf("The answer is:\n");
    for (int i = 0; i < ans_size; i ++)
        printf("%s\n", ans[i]);
    return 0;
}