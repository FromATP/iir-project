# iir-project

Group project for Introduction to Information Retrieval class

## Brief introduction

The text of *Sherlock Holmes* has 76764 lines and 3868122 characters.

Using the parser in ```SpaCy``` lib, we get 1240 names in total.
Unfortunately, there are several dirty items, like "n\u00e9e Adler". This may due to the raw text itself or the incapability of ```SpaCy``` lib. However, since these cases are very rare, we may ignore them for now.

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
- The chapters are dumped seperately, each as a ```dict```. There are two keys in each dict: ```title``` and ```text```. The title are **capitalized**, and the text preserves the **original spaces and blank lines** in raw data.

You can use the following code to fetch these lists and dicts:
```python
with open(filename, 'r', encoding='utf-8') as inputfd:
    obj = json.load(inputfd)
```