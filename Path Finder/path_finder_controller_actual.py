import struct

def calculate(f_wid, f_len, c_wid, c_len, c_num, orient):
    RowLen = c_len
    left_bottom = (0, 0)
    all = []

    if orient == 'Dikey':

        RowWid = (f_len - (c_num * c_wid)) / (c_num - 1)

        first_start_y = left_bottom[1] + (f_wid - c_len) / 2
        first_start_x = left_bottom[0] + RowWid + c_wid + (RowWid / 2)
        first_start = (first_start_x, first_start_y)

        first_end_y = first_start_y + RowLen
        first_end_x = first_start[0]
        first_end = (first_end_x, first_end_y)

        start_points = [first_start]
        all += first_start
        end_points = [first_end]
        all += first_end

        for i in range(1, c_num - 1):
            new_x = start_points[i - 1][0] + RowWid + c_wid

            if i % 2 == 0:
                new_start = (new_x, first_start_y)
                new_end = (new_x, first_end_y)
            else:
                new_start = (new_x, first_end_y)
                new_end = (new_x, first_start_y)

            start_points.append(new_start)
            all += new_start
            end_points.append(new_end)
            all += new_end
    elif orient == 'Yatay':
        RowWid = (f_wid - (c_num * c_wid)) / (c_num - 1)

        first_start_y = left_bottom[1] + RowWid + c_wid + (RowWid / 2)
        first_start_x = left_bottom[0] + (f_len - c_len) / 2
        first_start = (first_start_x, first_start_y)

        first_end_y = first_start_y
        first_end_x = left_bottom[0] + c_len
        first_end = (first_end_x, first_end_y)

        start_points = [first_start]
        all += first_start
        end_points = [first_end]
        all += first_end

        new_y = first_start_y

        for i in range(1, c_num - 1):
            new_y = new_y + RowWid + c_wid

            if i % 2 == 0:
                new_start = (first_start_x, new_y)
                new_end = (first_end_x, new_y)
            else:
                new_start = (first_end_x, new_y)
                new_end = (first_start_x, new_y)

            start_points.append(new_start)
            all += new_start
            end_points.append(new_end)
            all += new_end

    for i in range(len(start_points)):
        print('{}) Start:{} - End:{}'.format(i + 1, start_points[i], end_points[i]))

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