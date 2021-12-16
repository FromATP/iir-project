import json
from pathlib import Path

def merge_by_paragraph(text_list):
    res = []
    paragraph = []
    for line in text_list:
        if len(line.strip()) == 0:
            if len(paragraph) != 0:
                res.append(' '.join(paragraph))
                paragraph = []
        else:
            paragraph.append(line.strip())
    return res

def split_chapter(text_path, output_path):
    assert isinstance(output_path, Path), 'output_path must be a POSIX Path object.'
    output_path.mkdir(parents=True, exist_ok=True)
    with open(text_path, 'r', encoding='utf-8') as inputfd:
        text = inputfd.readlines()
        text_iter = iter(text)
        line = next(text_iter, None)

        # get the table of contents
        while line is not None:
            if 'Table of contents' in line:
                line = next(text_iter, None)
                break
            line = next(text_iter, None)
        table_of_contents = []
        while line is not None:
            if 'A STUDY IN SCARLET' in line:
                break
            title = line.strip()
            if len(title) > 0:
                table_of_contents.append(title.upper())
            line = next(text_iter, None)
        print(table_of_contents)
        # there is an epilogue at the end of the text, beginning with '----------'
        table_of_contents.append('----------')

        # split chapters
        title_nums = len(table_of_contents)
        chapter_cnt = 0
        for i in range(0, title_nums - 1):
            cur_title = table_of_contents[i]
            next_title = table_of_contents[i + 1]
            assert cur_title in line
            passage = []
            non_blank_lines = 0
            line = next(text_iter, None)
            while line is not None and next_title not in line:
                if len(line.strip()) > 0:
                    non_blank_lines += 1
                passage.append(line)
                line = next(text_iter, None)
            if non_blank_lines > 0:
                if cur_title == 'PREFACE':
                    cur_title = table_of_contents[i - 1] + ":" + cur_title
                passage = merge_by_paragraph(passage)
                cur_chapter = {
                    'title': cur_title,
                    'text': passage
                }
                file_name = f'chapter_{chapter_cnt}.txt'
                file_path = output_path / file_name
                with open(file_path, 'w', encoding='utf-8') as outputfd:
                    json.dump(cur_chapter, outputfd)
                chapter_cnt += 1
        print(f'Totally {chapter_cnt} chapters.')

if __name__ == '__main__':
    text_path = Path('../data/S-H.txt')
    output_path = Path('../data/chapters')
    split_chapter(text_path, output_path)