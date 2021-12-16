import json
from pathlib import Path
import numpy as np

def read_name_list(file_path):
    with open(file_path, 'r', encoding='utf-8') as inputfd:
        name_list = json.load(inputfd)
    return name_list


def find_name_in_chapter(name_list, chapter):
    print(f'Find names in {chapter}...')
    row = np.zeros(len(name_list))
    with open(chapter, 'r', encoding='utf-8') as inputfd:
        text = json.load(inputfd)['text']
        for i, name in enumerate(name_list):
            if row[i] == 0:
                for paragraph in text:
                    if name in paragraph:
                        row[i] = 1
                        break
    return row            


def match_name_chapter(name_list_path, chapter_path, output_path):
    if not isinstance(chapter_path, Path):
        chapter_path = Path(chapter_path)
    chapter_num = 62
    name_list = read_name_list(name_list_path)
    rows = []
    for i in range(chapter_num):
        chapter_file_name = chapter_path / f'chapter_{i}.txt'
        occur_row = find_name_in_chapter(name_list, chapter_file_name)
        rows.append(occur_row)
    occur_matrix = np.stack(rows)
    occur_matrix = occur_matrix.astype('int32')

    print(f'Build a name-chapter occurrence matrix of shape {occur_matrix.shape}')
    print(f'Save the occurrence matrix in {output_path}')
    np.save(output_path, occur_matrix)
    return occur_matrix