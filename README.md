# iir-project

Group project for Introduction to Information Retrieval class

## Brief introduction

The text of *Sherlock Holmes* has 76764 lines and 3868122 characters.

Using the parser in ```SpaCy``` lib, we get 1298 names in total.
There are some strange items like "n\u00e9e Adler". These are letters that are not in English, like special letters in German. Since these cases are very rare, we may ignore them for now.

> You can add here any information and ideas that you want to let others know

## How to develop

To install required packages, run
```shell
$ pip install -r requirements.txt
```
And run the following commands to update the reqirements after  modifying:
```shell
$ pip install pipreqs
$ pipreqs ./
```

### pre-processings

Some pre-processing results are already in the directory ```data```. If you want, you could run the following command to generate a new list of person's name:

```shell
$ cd code
$ python get_person_name.py
```

And you could run the following command to split the raw data into chapters:

```shell
$ cd code
$ python split_chapter.py
```

These two scripts are a little time-consuming. But after run them once, their results will be saved and can be re-used.

The person-name list and chapters are all stored as ```json``` file:

- The person-name list are dumped as a python ```list```
- The chapters are dumped seperately, each as a ```dict```. There are two keys in each dict: ```title``` and ```text```. The title are **capitalized**, and every paragraph in raw data is merged as one string, in convinience of semantic analysis.

You can use the following code to fetch these lists and dicts:
```python
with open(filename, 'r', encoding='utf-8') as inputfd:
    obj = json.load(inputfd)
```

### build the retrieval system

All building functions should defined in directory ```code```, and referenced in [build_main.py](build_main.py).

#### occurrence of a person's name 

The occurrence of names in each chapter is saved as a 0-1 numpy matrix (```dtype=int32```). If the element in position (i, j) is 1, it means the ith chapter has the jth name.

The matrix is saved as ```data/occur_matrix.npy```.

#### data compression

The scale of this matrix is (62, 1298). However, only 2805 of its elements is 1. So it's a sparse matrix, we could only store those indices that is 1. 

Moreover, the row and col indices could be saved as no more than 2 bytes. So we could compress these data using *variable-bytes* algorithm.

The compressed data is saved as ```data/occur_matrix_compressed.bin```. After compression, the size of matrix shrinks from 315kb to 8kb.

### interface

Here we implement several C++ APIs for users to call the retrieval system:

- ```void init_system();```
  - Initialization the system, including read all names and event names into memory, and set bit-strings for every event and name according to occurrence matrix.
  - If the ith bit in the bit-string of the jth name is 1, that means the jth name occurs in the ith event. Other situations are similar.
  - Users could call this function explicitly, or let the query function call it automatically.
- ```char** union_search_for_events(int num, char** names, int &ans_size);```
  - Given a list of name, return events that have at least one name appearred in it.
  - Users should provide the number of names and the list of names. The number of returned events will be stored in ```ans_size```.
  - This function will enumerate queried names, get the bit-strings for each name, and do bitwise or operation among all bit-strings.
- ```char** union_search_for_names(int num, char** events, int &ans_size);```
  - Given a list of events, return names that appear in at least one of the events.
  - The parameter is similar to the above function.
- ```char** intersection_search_for_events(int num, char** names, int &ans_size);```
  - Given a list of names, return events that have all names appearred in it.
  - The parameter is similar to the above function.
  - Obviously, if you set too much different names, the ```ans_size``` will tend to be 0......
- ```char** intersection_search_for_names(int num, char** events, int &ans_size);```
  - Given a list of events, return names that appears in all of the events.
  - The parameter is similar to the above function.

## How to test

The [test.cpp](test.cpp) provides a sample of how to call the retrieval system. You could modify the code to try different queries.

To run the test, you should first compile the code. Make sure you have GNU Make and G++ compiler on your machine.

To compile the testing code, run the following commands:
```shell
$ make clean
$ make
```

It will generate a ```build``` directory in your working path. An executable file ```system_main``` will be generated in this directory.

To run the testing program, run the following command:
```shell
$ ./build/system_main
```

The sample [test.cpp](test.cpp) requires for the events that both "Holmes" and "Henry Baskerville" appear. It should output:
```
The answer is:
THE HOUND OF THE BASKERVILLES
```