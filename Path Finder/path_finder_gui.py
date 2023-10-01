import tkinter as tk
from tkinter.ttk import Combobox
from path_finder_controller import calculate

form = tk.Tk()
form.title('Tarla Bilgileri')
form.geometry('400x350+450+200')

f_wid = tk.IntVar()
f_len = tk.IntVar()
c_wid = tk.IntVar()
c_len = tk.IntVar()
c_num = tk.IntVar()
orient = tk.StringVar()

Direction = ['Yatay', 'Dikey']

def button_handler():
    calculate(f_wid.get(), f_len.get(), c_wid.get(), c_len.get(), c_num.get(), orient.get())
    

label1 = tk.Label(form, text='Tarlanın enini girin:', font='Times 12 bold')
label1.place(relx=0.01, rely=0.1)
label2 = tk.Label(form, text='Tarlanın boyunu girin:', font='Times 12 bold')
label2.place(relx=0.01, rely=0.2)
label3 = tk.Label(form, text='Ekin sırasının genişliğini girin:', font='Times 12 bold')
label3.place(relx=0.01, rely=0.3)
label4 = tk.Label(form, text='Ekin sırasının uzunluğunu girin:', font='Times 12 bold')
label4.place(relx=0.01, rely=0.4)
label5 = tk.Label(form, text='Ekin sıra sayısını girin:', font='Times 12 bold')
label5.place(relx=0.01, rely=0.5)
label6 = tk.Label(form, text='Ekin sırasının yönünü seçin:', font='Times 12 bold')
label6.place(relx=0.01, rely=0.6)

input1 = tk.Entry(form, textvariable=f_wid, fg='black', bg='white')
input1.place(relx=0.6, rely=0.1)
input2 = tk.Entry(form, textvariable=f_len, fg='black', bg='white')
input2.place(relx=0.6, rely=0.2)
input3 = tk.Entry(form, textvariable=c_wid, fg='black', bg='white')
input3.place(relx=0.6, rely=0.3)
input4 = tk.Entry(form, textvariable=c_len, fg='black', bg='white')
input4.place(relx=0.6, rely=0.4)
input5 = tk.Entry(form, textvariable=c_num, fg='black', bg='white')
input5.place(relx=0.6, rely=0.5)
input6 = Combobox(form, values=Direction, height=3, textvariable=orient)
input6.place(relx=0.6, rely=0.6)

button1 = tk.Button(form, text='Hesapla', activebackground='green', command=button_handler)
button1.place(relx=0.42, rely=0.9)

form.mainloop()
