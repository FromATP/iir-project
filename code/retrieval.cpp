#include <map>
#include <bitset>
#include <cstdio>
#include <string>
#include <cstring>
#include <algorithm>
#include <filesystem>
#include <unordered_map>
#include "retrieval.hpp"
using namespace std;

const int names_num = 1298;
const int events_num = 62;

static bool init = false;
static string working_dir;

static unordered_map<string, int> name_index;
static unordered_map<string, int> event_index;
static vector<string> names;
static vector<string> events;
static bitset<events_num> name2events[names_num];
static bitset<names_num> events2name[events_num];


static void init_names() {
    string name_dir = working_dir + "/data/person_name.json";
    FILE* name_file = fopen(name_dir.c_str(), "r");
    string cur_name;
    int name_cnt = 0;

    char c = fgetc(name_file);
    printf("Init name list from %s\n", name_dir.c_str());
    while (c != EOF && c != '"')
        c = fgetc(name_file);

    while (c != EOF) {
        cur_name.clear();  // c == '"'
        c = fgetc(name_file);  // c = first character
        while (c != '"') {
            if (c == '\\')  // handling escape characters
                c = fgetc(name_file);
            cur_name.push_back(c);
            c = fgetc(name_file);
        }  // c == '"'

        names.push_back(cur_name);
        name_index[cur_name] = name_cnt ++;

        c = fgetc(name_file);  // c == ',' or ']'
        c = fgetc(name_file);  // c == ' ' or EOF
        c = fgetc(name_file);  // c == '"' or EOF
    }
    printf("Read %d names in total.\n", name_cnt);
    fclose(name_file);
}


static void init_events() {
    string cur_title, filename;
    int event_cnt = 0;

    for (int i = 0; i < events_num; i ++) {
        filename = working_dir + "/data/chapters/chapter_" + to_string(i) + ".txt";
        FILE* cur_chapter = fopen(filename.c_str(), "r");
        char c;

        cur_title.clear();
        // skip "{\"title\": \""
        for (int j = 0; j < 12; j ++)
            c = fgetc(cur_chapter);
        while (c != '"') {
            if (c == '\\')  // handling escape characters
                c = fgetc(cur_chapter);
            cur_title.push_back(c);
            c = fgetc(cur_chapter);
        }

        events.push_back(cur_title);
        event_index[cur_title] = event_cnt ++;
        fclose(cur_chapter);
    }
}

#define FIRST_BIT(x) (x >> 7)
static bool parse(unsigned char* buffer, int len, int &ptr, int &cur_num) {
    do {
        cur_num = (cur_num << 7) + (buffer[ptr] & 0b01111111);
        ptr ++;
        if (FIRST_BIT(buffer[ptr - 1]) == 1) break; 
    } while (ptr < len);
    if (FIRST_BIT(buffer[ptr - 1]) == 1)
        return true;
    return false;
}

static void init_occurrence() {
    const int buffer_len = 1024;
    string filename = working_dir + "/data/occur_matrix_compressed.bin";
    FILE* inputfd = fopen(filename.c_str(), "rb");
    unsigned char buffer[buffer_len];
    int read_size, cur_num = 0, ptr = 0, row_idx = -1;

    read_size = fread(buffer, sizeof(unsigned char), buffer_len, inputfd);
    while (read_size != 0) {
        while (ptr < read_size && parse(buffer, read_size, ptr, cur_num)) {
            if (row_idx == -1) row_idx = cur_num;
            else {
                events2name[row_idx].set(cur_num);
                name2events[cur_num].set(row_idx);
                row_idx = -1;
            }
            cur_num = 0;
        }
        read_size = fread(buffer, sizeof(unsigned char), buffer_len, inputfd);
        ptr = 0;
    }
    if (cur_num != 0) {
        fprintf(stderr, "Error: There is still incomplete number. ");
        fprintf(stderr, "Please check the correctness of the binary file.\n");
    }
    fclose(inputfd);
}


void init_system() {
    filesystem::path cwd = std::filesystem::current_path();
    working_dir = cwd.string();
    printf("Working directory: %s\n", working_dir.c_str());

    init_names();
    init_events();
    init_occurrence();
    init = true;
}


char** union_search_for_events(int num, char** names, int &ans_size) {
    if (!init) {
        fprintf(stderr, "You should init the system before using it.\n");
        init_system();
    }
    bitset<events_num> ans;
    ans.reset();
    for (int i = 0; i < num; i ++) {
        string query = names[i];
        if (name_index.find(query) == name_index.end())
            continue;
        ans = ans | name2events[name_index[query]];
    }
    ans_size = ans.count();
    
    int cnt = 0;
    char** ans_list = (char**)malloc(ans_size * sizeof(char*));
    for (int i = 0; i < events_num; i ++) {
        if (ans[i]) {
            int cur_len = events[i].size();
            char* cur = (char*)malloc(cur_len + 1);
            memcpy(cur, events[i].c_str(), cur_len);
            cur[cur_len] = 0;
            ans_list[cnt ++] = cur;
        }
    }
    return ans_list;
}

char** union_search_for_names(int num, char** events, int &ans_size) {
    if (!init) {
        fprintf(stderr, "You should init the system before using it.\n");
        init_system();
    }
    bitset<names_num> ans;
    ans.reset();
    for (int i = 0; i < num; i ++) {
        string query = events[i];
        if (event_index.find(query) == event_index.end())
            continue;
        ans = ans | events2name[event_index[query]];
    }
    ans_size = ans.count();
    
    int cnt = 0;
    char** ans_list = (char**)malloc(ans_size * sizeof(char*));
    for (int i = 0; i < names_num; i ++) {
        if (ans[i]) {
            int cur_len = names[i].size();
            char* cur = (char*)malloc(cur_len + 1);
            memcpy(cur, names[i].c_str(), cur_len);
            cur[cur_len] = 0;
            ans_list[cnt ++] = cur;
        }
    }
    return ans_list;
}


char** intersection_search_for_events(int num, char** names, int &ans_size) {
    if (!init) {
        fprintf(stderr, "You should init the system before using it.\n");
        init_system();
    }
    bitset<events_num> ans;
    ans.set();  // notice: different from "union search"
    for (int i = 0; i < num; i ++) {
        string query = names[i];
        if (name_index.find(query) == name_index.end())
            continue;
        ans = ans & name2events[name_index[query]];
    }
    ans_size = ans.count();
    
    int cnt = 0;
    char** ans_list = (char**)malloc(ans_size * sizeof(char*));
    for (int i = 0; i < events_num; i ++) {
        if (ans[i]) {
            int cur_len = events[i].size();
            char* cur = (char*)malloc(cur_len + 1);
            memcpy(cur, events[i].c_str(), cur_len);
            cur[cur_len] = 0;
            ans_list[cnt ++] = cur;
        }
    }
    return ans_list;
}

char** intersection_search_for_names(int num, char** events, int &ans_size) {
    if (!init) {
        fprintf(stderr, "You should init the system before using it.\n");
        init_system();
    }
    bitset<names_num> ans;
    ans.set();
    for (int i = 0; i < num; i ++) {
        string query = events[i];
        if (event_index.find(query) == event_index.end())
            continue;
        ans = ans & events2name[event_index[query]];
    }
    ans_size = ans.count();
    
    int cnt = 0;
    char** ans_list = (char**)malloc(ans_size * sizeof(char*));
    for (int i = 0; i < names_num; i ++) {
        if (ans[i]) {
            int cur_len = names[i].size();
            char* cur = (char*)malloc(cur_len + 1);
            memcpy(cur, names[i].c_str(), cur_len);
            cur[cur_len] = 0;
            ans_list[cnt ++] = cur;
        }
    }
    return ans_list;
}