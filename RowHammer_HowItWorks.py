from tkinter import *
from tkinter import messagebox
import random
import time

NUM = 10
window = Tk()  # instantiate an instance of a window
window.title("RowHammer")  # change title
window.config(background="black")
clicks = [[0 for j in range(NUM)] for i in range(NUM)]
values = [[0 for j in range(NUM)] for i in range(NUM)]


def flashing(button, duration=100, num_flashes=1):
    def flash1(count):
        if count < num_flashes:
            if button.cget('bg') == 'orange':
                button.config(bg='white')
            else:
                button.config(bg='orange')
            button.after(duration, flash1, count + 1)
        else:
            button.config(bg='white')

    flash1(0)

def flash_button(button, duration=300, num_flashes=10):
    def flash(count):
        if count < num_flashes:
            if button.cget('bg') == 'orange':
                button.config(bg='white')
            else:
                button.config(bg='orange')
            button.after(duration, flash, count+1)
        else:
            button.config(bg='orange')
    flash(0)

def message():
    messagebox.showerror('error', 'Memory corrupted!!')


def reset_values():
    for i in range(NUM):
        for j in range(NUM):
            clicks[i][j] = 0
            values[i][j] = 0
            buttons[i][j].config(text="0", bg="white")

def red_button(random_button):
    random_button.config(bg="red", text="1")


def change_values(buttons, row, col):
    clicks[row][col] += 1
    for j in range(NUM):
        boh = buttons[row][j]
        boh.after(0, flashing, boh)
    if clicks[row][col] == 5:
        for j in range(NUM):
            boh = buttons[row][j]
            #boh.configure(bg="orange")
            boh.after(0, flash_button, boh)
        next_row_buttons = []
        if row < (NUM - 1):
            for next_col in range(NUM):
                next_button = buttons[row + 1][next_col]
                next_row_buttons.append(next_button)
        else:
            for next_col in range(NUM):
                next_button = buttons[NUM-2][next_col]
                next_row_buttons.append(next_button)
        for i in range(2):
            random_button = random.choice(next_row_buttons)
            next_row_buttons.remove(random_button)
            next_row, next_col = random_button.grid_info()["row"], random_button.grid_info()["column"]
            # clicks[next_row][next_col] = 1 - clicks[next_row][next_col]
            # random_button.config(bg="red", text="1")
            random_button.after(3000, red_button, random_button)
        window.after(5000, message)


buttons = []
for i in range(NUM):
    row = []
    for j in range(NUM):
        button = Button(window, text="0", cursor="pirate", width=5, height=2, bg="white",
                        command=lambda i=i, j=j: change_values(buttons, i, j))
        button.grid(row=i, column=j)
        row.append(button)
    buttons.append(row)
reset_button = Button(window, text="Reset", command=reset_values)
reset_button.grid(row=NUM + 1, column=0, columnspan=NUM)
window.mainloop()  # place window on computer screen, listen for events
