from tkinter import *
from tkinter.font import Font
from tkinter import messagebox
import random
import numpy as np
import cv2 as cv


def back(back_page):
    canv.delete("all")
    back_page()


def create_num_entry(x: int, y: int, anch, tag: str):
    canv.create_text(x - 100, y, anchor=anch, text="Enter no. of values M:", tags=tag)
    global m_entry
    m_entry = Entry(root)
    canv.create_window(x, y, anchor=anch, window=m_entry, tags=tag, width=50)


def create_datatype_entry(x: int, y: int, anch, tag: str, default: str):
    canv.create_text(x - 100, y, anchor=anch, text="Select Data Type:", tags=tag)
    global datatype_entry
    datatype_entry = StringVar(master=root)
    datatype_entry.set(default)
    for i, par in enumerate(parity):
        radio_option = Radiobutton(root, text=par, variable=datatype_entry, value=par)
        canv.create_window(x + i * 100, y, anchor=anch, window=radio_option, tags=tag)


def create_range_box(x: int, y: int, anch, tag: str):
    canv.create_text(x - 100, y, anchor=anch, text="Select Range:", tags=tag)
    global lower_limit, upper_limit
    lower_limit = Entry(root)
    canv.create_window(x, y, anchor=anch, window=lower_limit, tags=tag, width=50)
    upper_limit = Entry(root)
    canv.create_window(x + 100, y, anchor=anch, window=upper_limit, tags=tag, width=50)


def create_major_entry(x: int, y: int, anch, tag: str, default: str):
    canv.create_text(x - 100, y, anchor=anch, text="Select Major:", tags=tag)
    global major_entry
    major_entry = StringVar(master=root)
    major_entry.set(default)
    for i, deg in enumerate(["Row", "Column"]):
        radio_option = Radiobutton(root, text=deg, variable=major_entry, value=deg)
        canv.create_window(x + i * 100, y, anchor=anch, window=radio_option, tags=tag)


def create_display_options(x: int, y: int, anch, tag: str, default: str):
    canv.create_text(x, y - 50, anchor=anch, text="Select Image:", tags=tag)
    global display_entry
    display_entry = StringVar(master=root)
    display_entry.set(default)
    for i, img in enumerate(["Odd", "Even", "Both"]):
        radio_option = Radiobutton(root, text=img, variable=display_entry, value=img)
        canv.create_window(x, y + i * 50, anchor=anch, window=radio_option, tags=tag)


def create_mult_menu(x: int, y: int, anch, tag: str, default: str):
    global mat_A, mat_B

    mat_A = StringVar(master=root)
    mat_A.set(default)
    mat_A_menu = OptionMenu(root, mat_A, *parity)
    canv.create_text(x - 100, y, anchor=anch, text="Matrix A:", tags=tag)
    canv.create_window(x, y, anchor=anch, window=mat_A_menu, tags=tag)

    mat_B = StringVar(master=root)
    mat_B.set(default)
    mat_B_menu = OptionMenu(root, mat_B, *parity)
    canv.create_text(x - 100, y + 50, anchor=anch, text="Matrix B:", tags=tag)
    canv.create_window(x, y + 50, anchor=anch, window=mat_B_menu, tags=tag)


def upload_data():
    m_num = m_entry.get().strip()
    low = lower_limit.get().strip()
    up = upper_limit.get().strip()

    if not m_num.isnumeric() or int(m_num) <= 0:
        messagebox.showerror(None, "M should be a positive integer")
        return

    if not low.isnumeric() or not up.isnumeric():
        messagebox.showerror(None, "Range should be whole numbers")
        return

    low, up, m_num = int(low), int(up), int(m_num)
    if low > up:
        low, up = up, low
        lower_limit.delete(0, END)
        lower_limit.insert(0, str(low))
        upper_limit.delete(0, END)
        upper_limit.insert(0, str(up))

    if up == low:
        messagebox.showerror(None, "Range limits can't be same")
        return

    with open(f"{datatype_entry.get()}.dat", "w") as f:
        for _ in range(m_num):
            val = random.randint(low, up)

            if val % 2 == parity.index(datatype_entry.get()):
                if val < up:
                    val += 1
                else:
                    val -= 1

            f.write(f"{val}\n")

    messagebox.showinfo(None, "Data written")
    back(home)


def create_data_file():
    canv.delete("home_page")

    create_num_entry(200, 100, CENTER, "create_file")
    create_datatype_entry(200, 200, CENTER, "create_file", "Odd")
    create_range_box(200, 300, CENTER, "create_file")
    submit_but = Button(root, text="Create", command=upload_data)
    canv.create_window(350, 400, window=submit_but, tags="create_file")

    back_button = Button(root, text="Back", command=lambda: back(home))
    canv.create_window(350, 450, window=back_button, tags="create_file")


def store_matrix():
    m_num = m_entry.get().strip()
    n_num = n_entry.get().strip()

    if (
        not m_num.isnumeric()
        or int(m_num) <= 0
        or not n_num.isnumeric()
        or int(n_num) <= 0
    ):
        messagebox.showerror(None, "M and N should be positive integers")
        return

    listnums = []

    try:
        with open(f"{datatype_entry.get()}.dat", "r") as f:
            listnums = [int(x) for x in f.readlines()]
    except:
        pass

    m_num, n_num = int(m_num), int(n_num)
    matrix = [[0 for _ in range(n_num)] for __ in range(m_num)]
    idx = 0

    if major_entry.get() == "Row":
        for i in range(m_num):
            for j in range(n_num):
                if idx < len(listnums):
                    matrix[i][j] = listnums[idx]
                    idx += 1
    else:
        for j in range(n_num):
            for i in range(m_num):
                if idx < len(listnums):
                    matrix[i][j] = listnums[idx]
                    idx += 1

    if datatype_entry.get() == "Odd":
        global odd_mat
        odd_mat = np.array(matrix)
    else:
        global even_mat
        even_mat = np.array(matrix)

    messagebox.showinfo(None, "Matrix Created")
    back(home)


def get_matrix():
    canv.delete("home_page")

    global m_entry
    canv.create_text(150, 100, text="m:", tags="get_matrix")
    m_entry = Entry(root)
    canv.create_window(200, 100, window=m_entry, tags="get_matrix", width=50)

    global n_entry
    canv.create_text(250, 100, text="n:", tags="get_matrix")
    n_entry = Entry(root)
    canv.create_window(300, 100, window=n_entry, tags="get_matrix", width=50)

    create_datatype_entry(200, 200, CENTER, "get_matrix", "Odd")
    create_major_entry(200, 300, CENTER, "get_matrix", "Row")

    submit_but = Button(root, text="Create", command=store_matrix)
    canv.create_window(350, 400, window=submit_but, tags="get_matrix")

    back_button = Button(root, text="Back", command=lambda: back(home))
    canv.create_window(350, 450, window=back_button, tags="get_matrix")


def multiply_matrices():
    global a_mat, b_mat
    try:
        if mat_A.get() == "Odd":
            a_mat = odd_mat
        else:
            a_mat = even_mat

        if mat_B.get() == "Odd":
            b_mat = odd_mat
        else:
            b_mat = even_mat
    except NameError:
        messagebox.showerror(None, "Create the matrices first")
        return

    if a_mat.shape[1] != b_mat.shape[0]:
        messagebox.showerror(None, "Invalid Matrix Dimensions")
        return

    global result_mat
    result_mat = np.matmul(a_mat, b_mat)

    messagebox.showinfo(None, "Matrix Created")
    back(home)


def merge_matrices():
    canv.delete("home_page")

    create_mult_menu(350, 200, CENTER, "merge_matrix", "Odd")

    submit_but = Button(root, text="Merge", command=multiply_matrices)
    canv.create_window(350, 400, window=submit_but, tags="merge_matrix")

    back_button = Button(root, text="Back", command=lambda: back(home))
    canv.create_window(350, 450, window=back_button, tags="merge_matrix")


def display_image():

    canv.delete("home_page")
    create_display_options(350, 100, CENTER, "display", "Both")

    submit_but = Button(root, text="Display", command=show_image)
    canv.create_window(350, 400, window=submit_but, tags="display")

    back_button = Button(root, text="Back", command=lambda: back(home))
    canv.create_window(350, 450, window=back_button, tags="display")


def show_image():
    option = display_entry.get()
    try:
        if option == "Odd":
            img = odd_mat
        elif option == "Even":
            img = even_mat
        else:
            img = result_mat
        cv.imshow(f"{option} image", (img % 255).astype(np.uint8))
        cv.waitKey(2000)
    except NameError:
        messagebox.showerror(None, "Image doesn't exist")


def home():
    but1 = Button(root, text="Create Data File", command=create_data_file)
    canv.create_window(350, 100, window=but1, tags="home_page")

    but2 = Button(root, text="Get Matrix", command=get_matrix)
    canv.create_window(250, 250, window=but2, tags="home_page")

    but3 = Button(root, text="Merge Matrices", command=merge_matrices)
    canv.create_window(450, 250, window=but3, tags="home_page")

    but4 = Button(root, text="Display Image", command=display_image)
    canv.create_window(350, 400, window=but4, tags="home_page")


# Main
def main():

    global parity
    parity = ["Odd", "Even"]

    # Stage
    global root
    root = Tk()
    root.title("Python Imaging")
    app_width = 700
    app_height = 500
    screen_width = root.winfo_screenwidth()
    screen_height = root.winfo_screenheight()
    x = screen_width / 2 - app_width / 2
    y = screen_height / 2 - app_height / 2
    root.geometry(f"{app_width}x{app_height}+{int(x)}+{int(y)}")
    root.resizable(False, False)

    # Display
    global canv
    canv = Canvas(root, width=app_width, height=app_height, bd=0, highlightthickness=0)
    canv.place(x=0, y=0, relwidth=1, relheight=1)

    home()
    root.mainloop()


if __name__ == "__main__":
    main()
