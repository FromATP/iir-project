import struct
import numpy as np
from numpy.core.fromnumeric import nonzero

def get_variable_bytes(num):
    byte_list = []
    flag = 0b10000000
    while True:
        cur = num & 0b01111111
        cur += flag
        byte_list.append(cur)
        flag = 0
        num = num >> 7
        if num == 0:
            break
    byte_list.reverse()
    return byte_list

def compress_sparse_matrix(matrix, output_path):
    print('Compress occurrenct matrix into variable-bytes')
    with open(output_path, 'wb') as outputfd:
        nonzero_index = np.transpose(np.nonzero(matrix))
        nonzero_index = nonzero_index.tolist()
        for index in nonzero_index:
            for num in index:
                byte_list = get_variable_bytes(num)
                for byte in byte_list:
                    byte = struct.pack('B', byte)
                    outputfd.write(byte)