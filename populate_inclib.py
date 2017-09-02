import os
from shutil import copyfile

import string
from fnmatch import fnmatch

root = 'PaTFramework\\source\\'
root_1 = 'Debug\\'

header_list = []
lib_list = []

def do_copy(src, dst, r):
    src = src.replace(r,"")
    print("copying file '" + src + "'")

    path = dst + src
    path = path[:path.rfind('\\')]
    if not os.path.exists(path):
        print("making directory" + path)
        os.makedirs(path)
    
    copyfile(r + src, dst + src)
    

for path, subdirs, files in os.walk(root):
    for name in files:
        if(fnmatch(name, "*.h")):
            header_list = header_list + [os.path.join(path, name)]

for i in range(len(header_list)):
    do_copy(header_list[i], "include\\", root)


for path, subdirs, files in os.walk(root_1):
    for name in files:
        if(fnmatch(name, "*.lib")):
            lib_list = lib_list + [os.path.join(path, name)]
            
for i in range(len(lib_list)):
    do_copy(lib_list[i], "lib\\", root_1)


