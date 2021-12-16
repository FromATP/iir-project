import spacy
import json

def get_person_name(file_path):
    parser = spacy.load("en_core_web_sm")
    name_list = []
    with open(file_path, 'r', encoding='utf-8') as inputfd:
        text = inputfd.readlines()
        text_iter = iter(text)
        total_lines = len(text)
        i = 0
        line = next(text_iter, None)
        while line is not None:
            if (i % 100 == 0):
                print(f'parsing {i} / {total_lines} lines...')

            # divide paragraphs by >1 continuous '\n'
            while line == '\n':
                line = next(text_iter, None)
                i = i + 1
            
            paragraph = []
            while line is not None and line != '\n':
                paragraph.append(line.strip())
                line = next(text_iter, None)
                i = i + 1
            paragraph = ' '.join(paragraph)
            
            # using SpaCy to get entity tokens
            token_list = parser(paragraph)
            name = []
            for token in token_list:
                if token.ent_type_ != 'PERSON':
                    if len(name) != 0:
                        # delete redudant '\n', ' ' and '\'s'
                        cur_name = ' '.join(name)
                        cur_name = cur_name.split()
                        if cur_name[-1] == '\'s':
                            cur_name = ' '.join(cur_name[:-1])
                        else:
                            cur_name = ' '.join(cur_name)
                        name_list.append(cur_name)
                        name = []
                else:
                    name.append(token.text)

    # unique
    name_list = list(set(name_list))
    name_num = len(name_list)
    print(f'Totally {name_num} names.')
    return name_list


if __name__ == '__main__':
    text_path = '../data/S-H.txt'
    output_path = '../data/person_name.json'
    name_list = get_person_name(text_path)
    with open(output_path, 'w', encoding='utf-8') as outputfd:
        json.dump(name_list, outputfd)