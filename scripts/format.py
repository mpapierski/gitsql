import os
import subprocess
from fnmatch import fnmatch
from itertools import chain


cpp_extensions = ('.cpp', '.cxx', '.c++', '.cc', '.cp', '.c', '.i', '.ii',
                  '.h', '.h++', '.hpp', '.hxx', '.hh', '.inl', '.inc', '.ipp',
                  '.ixx', '.txx', '.tpp', '.tcc', '.tpl')

ignored_masks = (
)

directories = (
    'src',
    'tests'
)


def main():
    dirs_iterable = (os.walk(directory) for directory in directories)
    all_dirs = chain.from_iterable(dirs_iterable)

    for root, dirs, files in all_dirs:
        for file in files:
            filename = os.path.join(root, file)

            ignore = False
            for ignored_mask in ignored_masks:
                if fnmatch(filename, ignored_mask):
                    ignore = True
                    break

            if ignore:
                continue

            if file.endswith(cpp_extensions):

                subprocess.call([
                    'clang-format',
                    '-i',
                    '-style=file',
                    filename])

if __name__ == '__main__':
    main()
