import tkinter as tk
from tkinter.ttk import Combobox
from path_finder_controller import calculate

form = tk.Tk()
form.title('Field Inputs')
form.geometry('700x400+300+50')

f_wid = tk.IntVar()
f_len = tk.IntVar()
c_wid = tk.IntVar()
c_len = tk.IntVar()
c_num = tk.IntVar()
orient = tk.StringVar()
data_ps = tk.IntVar()

Direction = ['Horizontal', 'Vertical']


def button_handler():
    calculate(f_wid.get(), f_len.get(), c_wid.get(), c_len.get(), c_num.get(), orient.get(), data_ps.get())


label0 = tk.Label(form, text="Please rotate your field sketch until enter point of the robot is near to the left bottom",
                  font='Times 14 bold')
label0.place(relx=0.01, rely=0.05)
label1 = tk.Label(form, text='Please enter width(y-axis) of the field:', font='Times 12 bold')
label1.place(relx=0.01, rely=0.2)
label2 = tk.Label(form, text='Please enter length(x-axis) of the field:', font='Times 12 bold')
label2.place(relx=0.01, rely=0.3)
label3 = tk.Label(form, text='Please enter width(short edge) of the crop lines:', font='Times 12 bold')
label3.place(relx=0.01, rely=0.4)
label4 = tk.Label(form, text='Please enter length(long edge) of the crop lines:', font='Times 12 bold')
label4.place(relx=0.01, rely=0.5)
label5 = tk.Label(form, text='Please enter number of the crop lines:', font='Times 12 bold')
label5.place(relx=0.01, rely=0.6)
label6 = tk.Label(form, text='Please choose the orientation of the crop lines:', font='Times 12 bold')
label6.place(relx=0.01, rely=0.7)
label7 = tk.Label(form, text='Please enter the number of data points per row:', font='Times 12 bold')
label7.place(relx=0.01, rely=0.8)

input1 = tk.Entry(form, textvariable=f_wid, fg='black', bg='white')
input1.place(relx=0.6, rely=0.2)
input2 = tk.Entry(form, textvariable=f_len, fg='black', bg='white')
input2.place(relx=0.6, rely=0.3)
input3 = tk.Entry(form, textvariable=c_wid, fg='black', bg='white')
input3.place(relx=0.6, rely=0.4)
input4 = tk.Entry(form, textvariable=c_len, fg='black', bg='white')
input4.place(relx=0.6, rely=0.5)
input5 = tk.Entry(form, textvariable=c_num, fg='black', bg='white')
input5.place(relx=0.6, rely=0.6)
input6 = Combobox(form, values=Direction, height=3, textvariable=orient)
input6.place(relx=0.6, rely=0.7)
input7 = tk.Entry(form, textvariable=data_ps, fg='black', bg='white')
input7.place(relx=0.6, rely=0.8)

button1 = tk.Button(form, text='Calculate', activebackground='green', command=button_handler)
button1.place(relx=0.42, rely=0.9)

form.mainloop()
