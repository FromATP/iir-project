#include <cstdio>
#include <string>
#include <cstring>
#include <algorithm>
#include "retrieval.hpp"
using namespace std;

void test_inter_search_place_for_events() {
    printf("test_inter_search_place_for_events");
    char** q_place = new char*[1];
    char* q_place1 = "Birdy Edwards";
    q_place[0] = q_place1;

    char** ans;
    int ans_size;
    ans = intersection_search_place_for_events(1, q_place, ans_size);
    printf("The answer is:\n");
    for (int i = 0; i < ans_size; i ++)
        printf("%s\n", ans[i]);
}

void test_inter_search_person_for_events() {
    printf("test_inter_search_person_for_events");
    char** q_person = new char*[1];
    char* q_person1 = "Holmes";
    q_person[0] = q_person1;

    char** ans;
    int ans_size;
    ans = intersection_search_person_for_events(1, q_person, ans_size);
    printf("The answer is:\n");
    for (int i = 0; i < ans_size; i ++)
        printf("%s\n", ans[i]);
}

void test_inter_search_person_place_for_events() {
    printf("test_inter_search_person_place_for_events");
    
    char** q_place = new char*[1];
    char* q_place1 = "Birdy Edwards";
    q_place[0] = q_place1;

    char** q_person = new char*[1];
    char* q_person1 = "Holmes";
    q_person[0] = q_person1;

    char** ans;
    int ans_size;
    ans = intersection_search_person_and_place_for_events(1, q_person, 1, q_place, ans_size);
    printf("The answer is:\n");
    for (int i = 0; i < ans_size; i ++)
        printf("%s\n", ans[i]);
}

int main()
{
    init_system();

    test_inter_search_place_for_events();
    test_inter_search_person_for_events();
    test_inter_search_person_place_for_events();

    return 0;
}