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

Some pre-processing results are already in the folder ```data```. If you want, you could run the following command to generate a new list of person's name:

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

All building functions should defined in folder ```code```, and referenced in [build_main.py](build_main.py).

#### occurrence of a person's name 

The occurrence of names in each chapter is saved as a 0-1 numpy matrix (```dtype=int32```). If the element in position $(i, j)$ is 1, it means the $i$th chapter has the $j$th name.

The matrix is saved as ```data/occur_matrix.npy```.

#### data compression

The scale of this matrix is (62, 1298). However, only 2805 of its elements is 1. So it's a sparse matrix, we could only store those indices that is 1. 

Moreover, the row and col indices could be saved as no more than 2 bytes. So we could compress these data using *variable-bytes* algorithm.

The compressed data is saved as ```data/occur_matrix_compressed.bin```. After compression, the size of matrix shrinks from 315kb to 8kb.

### interface