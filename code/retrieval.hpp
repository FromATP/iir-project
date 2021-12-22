void init_system();
char** union_search_place_for_events(int num, char** names, int &ans_size);
char** union_search_event_for_person_names(int num, char** events, int &ans_size);
char** intersection_search_person_for_events(int num, char** names, int &ans_size);
char** intersection_search_place_for_events(int num, char** names, int &ans_size);
char** intersection_search_person_and_place_for_events(int person_num, char** person_queries, int place_num, char** place_queries, int &ans_size);
char** intersection_search_event_for_person_names(int num, char** events, int &ans_size);