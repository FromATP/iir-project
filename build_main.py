import numpy as np
from pathlib import Path
from code.match_name_chapter import match_name_chapter

if __name__ == '__main__':
    chapter_path = Path('data/chapters')
    name_list_path = Path('data/person_name.json')
    output_path = Path('data/occur_matrix.npy')
    occur_matrix = match_name_chapter(name_list_path, chapter_path, output_path)