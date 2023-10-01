import struct

def calculate(f_wid, f_len, c_wid, c_len, c_num, orient):
    RowLen = c_len
    all = []

    if orient == 'Dikey':
        RowWid = (f_len - (c_num * c_wid)) / (c_num - 1)

    elif orient == 'Yatay':
        RowWid = (f_wid - (c_num * c_wid)) / (c_num - 1)

    else:
        print('Geçersiz yön seçildi.')
        return

    needed1 = RowWid + c_wid
    needed2 = c_num
    needed3 = RowLen

    all.append(needed1)
    all.append(needed2)
    all.append(needed3)

    all_int = [struct.pack("I", int(i)) for i in all]

    print('Byte array:')
    print(all_int)

    newFile = open("path_file.txt", "wb")
    for i in all_int:
        newFile.write(i)

if __name__ == '__main__':
    f_wid = 1000
    f_len = 900
    c_wid = 25
    c_len = 500
    c_num = 26
    orient = 'Dikey'

    calculate(f_wid, f_len, c_wid, c_len, c_num, orient)