''' 
A slightly more advanced Python calculator than the one created with
Codemy.com! This calculator supports addition, subtraction, multiplication,
and division! A few more features have also been added to the calculator
like decimals, negation, and a backspace button!

Code Written and Edited by Daniel Sacro
'''
from tkinter import *

errMsg = "Error: division by zero, press \"C\" to clear"

# Window Parameters
root = Tk()
root.geometry("323x355")
root.title("Python Calculator 2.0")

# Create and place display
display = Entry(root,width="51",borderwidth="3")
display.grid(row=0, column=0, columnspan=4, padx=5, pady=5)
display.insert(0, "0")

def errCheck(currDisp):
    if (currDisp == errMsg):
        display.insert(0, errMsg)
        return True
    else:
        return False
    
# Displays the number
def placeNum(num):
    currDisp = display.get()
    display.delete(0, END)
    dispList = str(currDisp).split(" ")

    # Don't allow any functionality upon error
    if (errCheck(currDisp)):
        return

    # Check for unnecessary zeroes
    if (dispList[-1] == "0"):
        display.insert(0, str(currDisp)[:-1] + str(num))
    else:
        display.insert(0, str(currDisp) + str(num))

# Displays the operator
def placeOp(op):
    currDisp = display.get()
    display.delete(0, END)
    
    # Don't allow any functionality upon error
    if (errCheck(currDisp)):
        return   

    # Operators should only be placed after numbers
    if (currDisp == ""):
        display.insert(0, str(currDisp))
    elif (currDisp[-1] == " "):
        # Operators can be replaced by other operators
        display.insert(0, str(currDisp)[:-3] + " " + op + " ")
    elif(currDisp[-1] == "."):
        # Decimals after operators should be deleted
        display.insert(0, str(currDisp)[:-1] + " " + op + " ")
    else:
        # Check for unnecessary zeroes after the decimal and remove them
        dispList = str(currDisp).split(" ")
        lastNum = str(float(dispList[-1]))
        if (lastNum[-2] == "." and lastNum[-1] == "0"):
            dispList[-1] = str(int(lastNum[:-2]))
        else:
            dispList[-1] = lastNum
        
        newDisp = ""
        for i in range(0, len(dispList) - 1):
            newDisp += str(dispList[i])
            newDisp += " "
        newDisp += str(dispList[-1])
        display.insert(0, str(newDisp) + " " + op + " ")

# Apply Negation
def neg():
    currDisp = display.get()
    display.delete(0, END)

    # Don't allow any functionality upon error
    if (errCheck(currDisp)):
        return

    # Only apply negation to numbers
    if (currDisp == "" or currDisp[-1] == " "):
        display.insert(0, str(currDisp))
    else:
        dispList = str(currDisp).split(" ")
        newNum = str(float(dispList[-1]) * -1)

        # Numbers like 10.0 should show up as 10
        if (newNum[-2] == "." and newNum[-1] == "0"):
            dispList[-1] = int(newNum[:-2])
        else:
            dispList[-1] = newNum
        
        # Build the new display
        newDisp = ""
        for element in dispList:
            newDisp += str(element)
            newDisp += " "
        display.insert(0, str(newDisp)[:-1])

# Apply a decimal to the number
def dec():                                                      
    currDisp = display.get()
    display.delete(0, END)
    dispList = str(currDisp).split(" ")
    lastNum = dispList[-1]

    # Don't allow any functionality upon error
    if (errCheck(currDisp)):
        return

    # If no number before decimal, assume one's place value is zero
    if (currDisp == ""):
        display.insert(0, "0.")
    elif (currDisp[-1] == " "):
        display.insert(0, str(currDisp) + "0.")
    elif (currDisp[-1] == "." or lastNum.find(".") != -1):
        # Don't place another decimal if there's already a decimal in the number
        display.insert(0, str(currDisp))
    else:
        display.insert(0, str(currDisp) + ".")
    return

# Clears the display
def clear():
    display.delete(0, END)
    display.insert(0, "0")

# Removes the last operator or number
def bkspc():
    currDisp = display.get()
    display.delete(0, END)

    # Don't allow any functionality upon error
    if (errCheck(currDisp)):
        return

    if (currDisp == "0" or currDisp == ""):
        # Cannot backspace on the default display
        display.insert(0, str(currDisp))
    elif (currDisp[-1] == " "):
        # Removes operator
        display.insert(0, str(currDisp)[:-3])
    elif (len(currDisp) == 1):
        # Returns to default display
        display.insert(0, "0")
    else:
        # Removes numbers individually
        display.insert(0, str(currDisp)[:-1])

# Display the result of the equation
def equals():
    currDisp = display.get()
    display.delete(0, END)
    dispList = str(currDisp).split(" ")

    # Don't allow any functionality upon error
    if (errCheck(currDisp)):
        return

    # If the last element is an operator, delete it
    if (dispList[-1] in ["*", "/", "+", "-", ""]):
        dispList.pop()
        dispList.pop()

    # No need to solve if there's a single number
    if (len(dispList) == 1):
        display.insert(0, dispList[0])
        return

    # Solve multiplication/division first
    nextDispList = []
    for i in range(0, len(dispList)):
        if (i == 0):
            # Set initial number
            currNum = dispList[i]
        elif(i == len(dispList) - 1):
            # Save the last number in the equation after performing last operation
            currNum = dispList[i]
            if (operation == "*"):
                currNum = str(float(lastNum) * float(currNum))
            elif (operation == "/"):
                try:
                    currNum = str(float(lastNum) / float(currNum))
                except ZeroDivisionError:
                    display.insert(0, errMsg)
                    return
            else:
                nextDispList.append(str(lastNum))
                nextDispList.append(str(operation))
            nextDispList.append(str(currNum))
        elif (i % 2 == 0):
            # If i is even, element in list is a number
            currNum = dispList[i]
            if (operation == "*"):
                currNum = str(float(lastNum) * float(currNum))
            elif (operation == "/"):
                try:
                    currNum = str(float(lastNum) / float(currNum))
                except ZeroDivisionError:
                    display.insert(0, errMsg)
                    return
            else:
                nextDispList.append(str(lastNum))
                nextDispList.append(str(operation))
        elif (i % 2 == 1):
            # If i is odd, element in list is an operator
            lastNum = currNum
            operation = dispList[i]

    # Solve addition/subtraction second
    operation = ""
    for i in range(0, len(nextDispList)):
        if (i == 0):
            soln = nextDispList[i]
        elif (i % 2 == 0):
            currNum = nextDispList[i]
            if (operation == "+"):
                soln = str(float(soln) + float(nextDispList[i]))
            elif (operation == "-"):
                soln = str(float(soln) - float(nextDispList[i]))
        elif (i % 2 == 1):
            operation = nextDispList[i]

    # Check for unnecessary decimals
    if (soln[-2] == "." and soln[-1] == "0"):
        display.insert(0, soln[:-2])
    else:
        display.insert(0, soln)

# Create buttons
button1 = Button(root, text="1", padx=30, pady=20, command=lambda: placeNum(1))
button2 = Button(root, text="2", padx=30, pady=20, command=lambda: placeNum(2))
button3 = Button(root, text="3", padx=30, pady=20, command=lambda: placeNum(3))
button4 = Button(root, text="4", padx=30, pady=20, command=lambda: placeNum(4))
button5 = Button(root, text="5", padx=30, pady=20, command=lambda: placeNum(5))
button6 = Button(root, text="6", padx=30, pady=20, command=lambda: placeNum(6))
button7 = Button(root, text="7", padx=30, pady=20, command=lambda: placeNum(7))
button8 = Button(root, text="8", padx=30, pady=20, command=lambda: placeNum(8))
button9 = Button(root, text="9", padx=30, pady=20, command=lambda: placeNum(9))
button0 = Button(root, text="0", padx=30, pady=20, command=lambda: placeNum(0))

buttonClr = Button(root, text="C",padx=29, pady=20,command=clear)
buttonBkspc = Button(root, text="<-",padx=26.4, pady=20,command=bkspc)

buttonAdd = Button(root, text="+",padx=28, pady=20,command=lambda: placeOp("+"))
buttonSub = Button(root, text="-",padx=30, pady=20,command=lambda: placeOp("-"))
buttonMul = Button(root, text="*",padx=30, pady=20,command=lambda: placeOp("*"))
buttonDiv = Button(root, text="/",padx=30, pady=20,command=lambda: placeOp("/"))
buttonDec = Button(root, text=".",padx=31, pady=20,command=dec)
buttonNeg = Button(root, text="+/-",padx=24, pady=20,command=neg)
buttonEql = Button(root, text="=",padx=69, pady=20,command=equals)

# Place buttons on calculator
button7.grid(row=1,column=0)
button8.grid(row=1,column=1)
button9.grid(row=1,column=2)
buttonAdd.grid(row=1,column=3)

button4.grid(row=2,column=0)
button5.grid(row=2,column=1)
button6.grid(row=2,column=2)
buttonSub.grid(row=2,column=3)

button1.grid(row=3,column=0)
button2.grid(row=3,column=1)
button3.grid(row=3,column=2)
buttonMul.grid(row=3,column=3)

buttonNeg.grid(row=4,column=0)
button0.grid(row=4,column=1)
buttonDec.grid(row=4,column=2)
buttonDiv.grid(row=4,column=3)

buttonClr.grid(row=5, column=0)
buttonBkspc.grid(row=5,column=1)
buttonEql.grid(row=5, column=2, columnspan=2)

# Open Calculator
root.mainloop()