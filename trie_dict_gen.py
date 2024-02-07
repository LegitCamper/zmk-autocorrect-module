import sys
import os
import string

sys.setrecursionlimit(100)

N = 26  # 26 lowercase english letters
TrieNodeInit = f"""#define N {N} // 26 lowercase english letters

struct TrieNode {{
    char data;
    struct TrieNode *children[N];
    int is_leaf;
}};    

#define S(NAME, ...) struct TrieNode NAME = __VA_ARGS__

"""

lines_to_write = []


def add_to_lines(new_word, new_name, new_data, new_children, new_leaf):
    for word, name, data, children, leaf in lines_to_write:
        if name == new_name:
            children.update(new_children)
            return
    lines_to_write.append([new_word, new_name, new_data, new_children, new_leaf])


def format_struct(word, name, data, children, leaf):
    children_array = ["0"] * N
    for k, v in children.items():
        children_array[string.ascii_lowercase.index(k)] = "&" + v
    children_array = (
        str(children_array).replace("'", "").replace("[", "{").replace("]", "}")
    )
    line = f"S({name}, {{ .data = '{data}', .children = {children_array}, .is_leaf = {leaf} }}); \n"
    return line


def add_children(loc, cloc, children, char, func_name):
    if cloc > loc:
        children.update({char: func_name})


def add_char_to_trie(fword, word, loc, children):
    leaf = False
    if len(word) > 1:
        char = word[0]
        word = word[1:]
        (uchar, ufunc_name) = add_char_to_trie(fword, word, loc + 1, children)
        add_children(loc, loc + 1, children, uchar, ufunc_name)

    else:
        leaf = True
        char = word[0]
        word = word[1:]

    if leaf:
        add_to_lines(fword, f"trie_node_l{loc + 1}_{fword}_leaf", char, {}, 1)
    func_name = f"trie_node_l{loc}_{fword}"
    add_to_lines(fword, func_name, char, children, 0)

    return (char, func_name)


def main():
    with open("words.txt", "r") as file:
        dict_words = [line.strip() for line in file]

    for word in dict_words:
        add_char_to_trie(word, word, loc=1, children={})

    header_file = "trie_dict.h"
    if os.path.exists(header_file):
        os.remove(header_file)
    with open(header_file, "w") as file:
        file.write(TrieNodeInit)

        root_children = {}
        for word, name, data, children, leaf in lines_to_write:
            if "l0" in name:
                root_children.update({data: name})
            file.write(format_struct(word, name, data, children, leaf))

        # make root
        file.write(format_struct("", "root", "", root_children, 0))


main()
