import struct


def calculate(f_wid, f_len, c_wid, c_len, c_num, orient, data_ps):
    row_len = c_len
    step_size = row_len / data_ps
    all_needed = []

    if orient == 'Vertical':
        RowWid = (f_len - (c_num * c_wid)) / (c_num - 1)
        needed5 = 1

    elif orient == 'Horizontal':
        RowWid = (f_wid - (c_num * c_wid)) / (c_num - 1)
        needed5 = 0

    else:
        print('Geçersiz yön seçildi.')
        return

    needed1 = row_len
    needed2 = step_size
    needed3 = RowWid
    needed4 = c_num - 1

    all_needed.append(needed1)
    all_needed.append(needed2)
    all_needed.append(needed3)
    all_needed.append(needed4)
    all_needed.append(needed5)

    all_int = [struct.pack("I", int(i)) for i in all_needed]

    print('Byte array:')
    print(all_int)

    newFile = open("path_file.txt", "wb")
    for i in all_int:
        newFile.write(i)
