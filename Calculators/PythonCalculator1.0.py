''' 
A very simple Python calculator created with the help of Codemy.com!
This calculator only supports addition and subtraction.
'''
from tkinter import *

# Window Parameters
root = Tk()
root.geometry("310x365")
root.title("Python Calculator 1.0")

# Create and place display
display = Entry(root,width="48")
display.grid(row=0, column=0, columnspan=3,padx=10,pady=10)

# Displays the number
def buttonClick(number):
    currVal =  display.get()
    display.delete(0, END)
    display.insert(0, str(currVal) + str(number))

# Clears the display
def clear():
    display.delete(0, END)

# Displays addition
def add():
    num1 = display.get()
    display.delete(0, END)
    display.insert(0, str(num1) + " + ")

# Displays subtraction
def sub():
    num1 = display.get()
    display.delete(0, END)
    display.insert(0, str(num1) + " - ")

# Calculates the result of the equation
def equals():
    equation = display.get()
    equationList = equation.split(" ")
    if (equationList[0] not in "+-"):
        currVal = int(equationList.pop(0)) # Saves the first number
    for symbol in equationList:
        # If symbol is an operator
        if (symbol in "+-"):
            operator = symbol
        else:
            # Else, symbol is a number
            if (operator == "+"):
                currVal += int(symbol)
            elif (operator == "-"):
                currVal -= int(symbol)
    display.delete(0, END)
    display.insert(0, str(currVal))

# Create buttons
button1 = Button(root, text="1", padx=40, pady=20, command=lambda: buttonClick(1))
button2 = Button(root, text="2", padx=40, pady=20, command=lambda: buttonClick(2))
button3 = Button(root, text="3", padx=40, pady=20, command=lambda: buttonClick(3))
button4 = Button(root, text="4", padx=40, pady=20, command=lambda: buttonClick(4))
button5 = Button(root, text="5", padx=40, pady=20, command=lambda: buttonClick(5))
button6 = Button(root, text="6", padx=40, pady=20, command=lambda: buttonClick(6))
button7 = Button(root, text="7", padx=40, pady=20, command=lambda: buttonClick(7))
button8 = Button(root, text="8", padx=40, pady=20, command=lambda: buttonClick(8))
button9 = Button(root, text="9", padx=40, pady=20, command=lambda: buttonClick(9))
button0 = Button(root, text="0", padx=40, pady=20, command=lambda: buttonClick(0))

buttonClr = Button(root, text="C",padx=39, pady=20,command=clear)
buttonAdd = Button(root, text="+",padx=39, pady=20,command=add)
buttonSub = Button(root, text="-",padx=40, pady=20,command=sub)
buttonEql = Button(root, text="=",padx=91, pady=20,command=equals)

# Place buttons on calculator
button7.grid(row=1,column=0)
button8.grid(row=1,column=1)
button9.grid(row=1,column=2)

button4.grid(row=2,column=0)
button5.grid(row=2,column=1)
button6.grid(row=2,column=2)

button1.grid(row=3,column=0)
button2.grid(row=3,column=1)
button3.grid(row=3,column=2)

button0.grid(row=4,column=0)
buttonAdd.grid(row=4,column=1)
buttonSub.grid(row=4,column=2)

buttonClr.grid(row=5, column=0)
buttonEql.grid(row=5, column=1, columnspan=2)

# Opens Calculator
root.mainloop()