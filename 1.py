# -*- coding: utf-8 -*-
"""
Created on Sun Apr 22 16:41:58 2018

@author: 12588
"""

from tkinter import *
root=Tk()
btnSayHi=Button(root)
btnSayHi["text"]="Hello!"
btnSayHi.pack()
def sayHi(e):
    messagebox.showinfo("Message","Hello world!")
btnSayHi.bind("<Button-1>",sayHi)
root.mainloop()