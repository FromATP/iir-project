import numpy as np
from pathlib import Path
from code.match_name_chapter import match_name_chapter
from code.compress_sparse_matrix import compress_sparse_matrix

if __name__ == '__main__':
    chapter_path = Path('data/chapters')
    name_list_path = Path('data/person_name.json')
    output_path = Path('data/occur_matrix.npy')
    compressed_path = Path('data/occur_matrix_compressed.bin')

    occur_matrix = match_name_chapter(name_list_path, chapter_path, output_path)
    compress_sparse_matrix(occur_matrix, compressed_path)