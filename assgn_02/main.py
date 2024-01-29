import json
import re
from datetime import datetime
from tkinter import *
from tkinter.font import Font
from tkinter import messagebox
from hierarchy import User, Teacher, Student, UG_student, PG_student


# Utility Functions
def create_id_entry(x: int, y: int, anch, tag: str, default: str):
    global id_entry
    id_entry = Entry(root)
    id_entry.insert(0, default)
    canv.create_window(x, y, anchor=anch, window=id_entry, tags=tag)


def create_pwd_entry(x: int, y: int, anch, tag: str, show_as: str, win, c: Canvas):
    global pwd_entry
    pwd_entry = Entry(win, show=show_as)
    c.create_window(x, y, anchor=anch, window=pwd_entry, tags=tag)


def create_name_entry(x: int, y: int, anch, tag: str, default: str):
    global name_entry
    name_entry = Entry(root)
    name_entry.insert(0, default)
    canv.create_window(x, y, anchor=anch, window=name_entry, tags=tag)


def create_dept_entry(x: int, y: int, anch, tag: str, default: str):
    global dept_entry
    dept_entry = Entry(root)
    dept_entry.insert(0, default)
    canv.create_window(x, y, anchor=anch, window=dept_entry, tags=tag)


def create_course_entry(x: int, y: int, anch, tag: str, default: str):
    global course_entry
    course_entry = Entry(root)
    course_entry.insert(0, default)
    canv.create_window(x, y, anchor=anch, window=course_entry, tags=tag)


def create_degree_entry(x: int, y: int, anch, tag: str, default: str):
    global degree_entry
    degree_entry = StringVar(master=root)
    degree_entry.set(default)
    for i, deg in enumerate(degree_options):
        radio_option = Radiobutton(root, text=deg, variable=degree_entry, value=deg)
        canv.create_window(x + i * 100, y, anchor=anch, window=radio_option, tags=tag)


def create_grad_menu(x: int, y: int, anch, tag: str, default: int, num: int):
    global grad_year
    grad_year = IntVar(master=root)
    grad_year.set(default)
    grad_menu = OptionMenu(root, grad_year, *[(cur_year + i) for i in range(num)])
    canv.create_window(x, y, anchor=anch, window=grad_menu, tags=tag)


def create_field_entry(x: int, y: int, anch, tag: str, default: str):
    global field_entry
    field_entry = Entry(root)
    field_entry.insert(0, default)
    canv.create_window(x, y, anchor=anch, window=field_entry, tags=tag)


def create_type_menu(x: int, y: int, anch, tag: str, default: str):
    global user_type
    user_type = StringVar(master=root)
    user_type.set(default)
    type_menu = OptionMenu(root, user_type, *user_options, command=utype_select)
    canv.create_window(x, y, anchor=anch, window=type_menu, tags=tag)


def verify_pwd_pattern(pwd):
    reg = r"^(?=.*[a-z])(?=.*[A-Z])(?=.*[!@#$%&*])(?=.*[0-9]).*$"
    if (
        len(pwd) not in range(8, 13)
        or re.fullmatch(reg, pwd) == None
        or re.search(r"[\s]", pwd) != None
    ):
        return False

    return True


def back(back_page):
    canv.delete("all")
    back_page()


# Load User Data
def upload_user_data(filename):
    with open(filename, "w", encoding="utf-8") as f:
        f.write(
            json.dumps(
                [d.__dict__ for d in User.all],
                ensure_ascii=False,
                separators=(",", ": "),
                indent=2,
            )
        )


def download_user_data(filename):
    try:
        User.instantiate_from_json(filename)
    except FileNotFoundError:
        with open(filename, "w", encoding="utf-8"):
            return


# Profile
def profile(idx):
    canv.delete("all")

    edit_button = Button(root, text="Edit Profile", command=lambda: edit_profile(idx))
    canv.create_window(350, 450, window=edit_button, tags="profile_page")

    log_out_button = Button(root, text="Log Out", command=logout)
    canv.create_window(200, 450, window=log_out_button, tags="profile_page")

    dereg_button = Button(
        root, text="Deregister", bg="#ed333b", fg="white", command=lambda: dereg(idx)
    )
    canv.create_window(500, 450, window=dereg_button, tags="profile_page")

    canv.create_text(250, 100, anchor=W, text="User ID:", tags="profile")
    canv.create_text(400, 100, anchor=W, text=f"{User.all[idx].id}", tags="profile")
    canv.create_text(250, 150, anchor=W, text="Name:", tags="profile")
    canv.create_text(
        400, 150, anchor=W, text=f"{User.all[idx].name}", tags="profile_page"
    )
    canv.create_text(250, 200, anchor=W, text="Profile Type:", tags="profile")
    canv.create_text(400, 200, anchor=W, text=f"{User.all[idx].utype}", tags="profile")
    canv.create_text(250, 250, anchor=W, text="Department:", tags="profile")
    canv.create_text(
        400, 250, anchor=W, text=f"{User.all[idx].dept}", tags="profile_page"
    )

    utype = User.all[idx].utype
    if utype == "Teacher":
        canv.create_text(250, 300, anchor=W, text="Course:", tags="profile")
        canv.create_text(
            400, 300, anchor=W, text=f"{User.all[idx].course}", tags="profile_page"
        )
    elif utype == "UG Student":
        canv.create_text(250, 300, anchor=W, text="Graduation Year:", tags="profile")
        canv.create_text(
            400, 300, anchor=W, text=f"{User.all[idx].grad}", tags="profile_page"
        )
        canv.create_text(250, 350, anchor=W, text="Degree:", tags="profile")
        canv.create_text(
            400, 350, anchor=W, text=f"{User.all[idx].degree}", tags="profile_page"
        )
    else:
        canv.create_text(250, 300, anchor=W, text="Graduation Year:", tags="profile")
        canv.create_text(
            400, 300, anchor=W, text=f"{User.all[idx].grad}", tags="profile_page"
        )
        canv.create_text(250, 350, anchor=W, text="Field:", tags="profile")
        canv.create_text(
            400, 350, anchor=W, text=f"{User.all[idx].field}", tags="profile_page"
        )


def edit_profile(idx):
    canv.delete("profile_page")

    back_button = Button(root, text="Back", command=lambda: back(lambda: profile(idx)))
    canv.create_window(200, 450, window=back_button, tags="edit_profile")

    save_button = Button(
        root, text="Save", bg="#26a269", fg="white", command=lambda: save_edits(idx)
    )
    canv.create_window(350, 450, window=save_button, tags="edit_profile")

    change_pwd_button = Button(
        root, text="Change Password", command=lambda: change_pwd(idx)
    )
    canv.create_window(500, 450, window=change_pwd_button, tags="edit_page")

    create_name_entry(400, 150, W, "", User.all[idx].name)
    create_dept_entry(400, 250, W, "", User.all[idx].dept)

    utype = User.all[idx].utype
    if utype == "Teacher":
        create_course_entry(400, 300, W, "", User.all[idx].course)
    elif utype == "UG Student":
        create_grad_menu(400, 300, W, "", User.all[idx].grad, 5)
        create_degree_entry(400, 350, W, "", User.all[idx].degree)
    else:
        create_grad_menu(400, 300, W, "", User.all[idx].grad, 3)
        create_field_entry(400, 350, W, "", User.all[idx].field)


def change_pwd(idx):
    global top
    top = Toplevel()
    top.title("Profile System")
    app_width = 400
    app_height = 400
    x = screen_width / 2 - app_width / 2
    y = screen_height / 2 - app_height / 2
    top.geometry(f"{app_width}x{app_height}+{int(x)}+{int(y)}")
    top.resizable(False, False)
    global top_canv
    top_canv = Canvas(top, width=app_width, height=app_height, bd=0)
    top_canv.place(x=0, y=0, relwidth=1, relheight=1)

    create_pwd_entry(200, 100, CENTER, "", "", top, top_canv)
    top_canv.create_text(200, 120, text="Enter Current Password", tags="curr_pwd")

    proceed_button = Button(top, text="Proceed", command=lambda: get_new_pwd(idx))
    top_canv.create_window(100, 300, window=proceed_button, tags="curr_pwd")

    cancel_button = Button(top, text="Cancel", command=top.destroy)
    top_canv.create_window(300, 300, window=cancel_button)


def get_new_pwd(idx):
    if pwd_entry.get() != User.all[idx].pwd:
        messagebox.showerror(None, "Wrong Password")
        return

    pwd_entry.configure(state="disabled", show="*")
    top_canv.delete("curr_pwd")

    create_pwd_entry(200, 170, CENTER, "", "", top, top_canv)
    top_canv.create_text(200, 190, text="Enter New Password", tags="curr")

    save_button = Button(
        top, text="Save", bg="#26a269", fg="white", command=lambda: save_new_pwd(idx)
    )
    top_canv.create_window(100, 300, window=save_button)


def save_new_pwd(idx):
    new_pwd = pwd_entry.get()
    if not verify_pwd_pattern(new_pwd):
        messagebox.showerror(
            "Password",
            "Password must contain 8-12 characters, consisting atleast one lowercase, one uppercase, one digit, one special character among !@#$%&* and no spaces",
        )
        return

    if not messagebox.askokcancel("Password", "Are you sure?"):
        return

    User.all[idx].pwd = new_pwd
    top.destroy()


def save_edits(idx):
    canv.delete("error")
    ok = True

    name = name_entry.get()
    name = name.strip()
    name_entry.delete(0, END)
    name_entry.insert(0, name)

    if len(name) == 0:
        canv.create_text(400, 170, anchor=W, text="Required", tags="error")
        ok = False

    dept = dept_entry.get()
    dept = dept.strip()
    dept_entry.delete(0, END)
    dept_entry.insert(0, dept)
    if len(dept) == 0:
        canv.create_text(400, 270, anchor=W, text="Required", tags="error")
        ok = False

    if not ok:
        return

    User.all[idx].name = name
    User.all[idx].dept = dept
    utype = User.all[idx].utype

    if utype == "Teacher":
        User.all[idx].course = course_entry.get()
    else:
        User.all[idx].grad = grad_year.get()
        if utype == "UG Student":
            User.all[idx].degree = degree_entry.get()
        else:
            User.all[idx].field = field_entry.get()

    messagebox.showinfo(None, "Changes Saved")
    back(lambda: profile(idx))


def dereg(idx):
    if messagebox.askyesno("Deregister", "Are you Sure?"):
        User.all.pop(idx)
        messagebox.showinfo(None, "User Deregistered")
        back(home)
    else:
        pass


def logout():
    messagebox.showinfo(None, "Logged Out Successfully")
    back(home)


# Log In
def login():
    canv.delete("home_page")

    login_button = Button(root, text="Log In", command=verify_login)
    canv.create_window(500, 450, window=login_button, tags="login_page")

    back_button = Button(root, text="Back", command=lambda: back(home))
    canv.create_window(200, 450, window=back_button, tags="login_page")

    canv.create_text(225, 150, text="User ID: ", tags="login_page")
    create_id_entry(350, 150, CENTER, "login_page", "")

    canv.create_text(225, 250, text="Password: ", tags="login_page")
    create_pwd_entry(350, 250, CENTER, "login_page", "*", root, canv)

    is_shown = IntVar()
    pwd_show_check = Checkbutton(
        root, text="Show", variable=is_shown, command=lambda: show_pwd(is_shown)
    )
    canv.create_window(450, 250, anchor=W, window=pwd_show_check, tags="login_page")


def show_pwd(is_shown):
    if is_shown.get():
        pwd_entry.configure(show="")
    else:
        pwd_entry.configure(show="*")


def verify_login():
    id = id_entry.get()
    id = id.strip()
    id_entry.delete(0, END)
    id_entry.insert(0, id)

    pwd = pwd_entry.get()
    idx = -1
    for i, d in enumerate(User.all):
        if d.__dict__.get("id") == id:
            idx = i
            break

    if idx == -1:
        messagebox.showerror(None, "User Not Found")
        id_entry.delete(0, END)
        pwd_entry.delete(0, END)
        return

    if User.all[idx].pwd != pwd:
        User.all[idx].attempts -= 1

        if User.all[idx].attempts == 0:
            User.all.pop(idx)
            messagebox.showinfo(None, "Too many password attempts.\nUser Deactivated")
            back(home)
            return

        messagebox.showwarning(
            None, f"Incorrect Password\n{User.all[idx].attempts} attempt(s) remaining"
        )
        pwd_entry.delete(0, END)
        return

    User.all[idx].attempts = 3
    messagebox.showinfo(None, "Logged In Successfully")
    profile(idx)


# Registration
def register():
    canv.delete("home_page")

    reg_button = Button(root, text="Register", command=verify_registration)
    canv.create_window(500, 450, window=reg_button, tags="reg_page")

    back_button = Button(root, text="Back", command=lambda: back(home))
    canv.create_window(200, 450, window=back_button, tags="reg_page")

    canv.create_text(225, 100, text="User ID: ", tags="reg_page")
    create_id_entry(350, 100, CENTER, "reg_page", "")

    create_pwd_entry(350, 150, CENTER, "reg_page", "", root, canv)
    canv.create_text(225, 150, text="Password: ", tags="reg_page")

    canv.create_text(225, 200, text="Name: ", tags="reg_page")
    create_name_entry(350, 200, CENTER, "reg_page", "")

    create_type_menu(350, 250, CENTER, "reg_page", "Select")
    canv.create_text(225, 250, text="User Type: ", tags="reg_page")


def utype_select(utype):
    canv.delete("error")

    if utype == "Teacher":
        teacher_register()
    elif utype == "UG Student":
        ug_student_register()
    else:
        pg_student_register()


def teacher_register():
    canv.delete("pg", "ug")

    create_dept_entry(350, 300, CENTER, "teacher", "")
    canv.create_text(225, 300, text="Department: ", tags="teacher")

    create_course_entry(350, 350, CENTER, "teacher", "")
    canv.create_text(225, 350, text="Course: ", tags="teacher")


def ug_student_register():
    canv.delete("pg", "teacher")

    create_dept_entry(350, 300, CENTER, "ug", "")
    canv.create_text(225, 300, text="Department: ", tags="ug")

    create_grad_menu(350, 350, CENTER, "ug", cur_year, 5)
    canv.create_text(225, 350, text="Graduation Year: ", tags="ug")

    create_degree_entry(300, 400, CENTER, "ug", "")
    canv.create_text(225, 400, text="Degree: ", tags="ug")


def pg_student_register():
    canv.delete("ug", "teacher")

    create_dept_entry(350, 300, CENTER, "pg", "")
    canv.create_text(225, 300, text="Department: ", tags="pg")

    create_grad_menu(350, 350, CENTER, "pg", cur_year, 3)
    canv.create_text(225, 350, text="Graduation Year: ", tags="pg")

    create_field_entry(350, 400, CENTER, "pg", "")
    canv.create_text(225, 400, text="Field: ", tags="pg")


def verify_registration():
    canv.delete("error")
    ok = True

    # id
    id = id_entry.get()
    id = id.strip()
    id_entry.delete(0, END)
    id_entry.insert(0, id)

    if len(id) == 0:
        canv.create_text(350, 120, text="Required", tags="error")
        ok = False

    if " " in id:
        canv.create_text(350, 120, text="No spaces allowed", tags="error")
        ok = False

    if any(u.id == id for u in User.all):
        canv.create_text(350, 120, text="User ID in use", tags="error")
        ok = False

    # pwd
    pwd = pwd_entry.get()
    if not verify_pwd_pattern(pwd):
        messagebox.showerror(
            "Password",
            "Password must contain 8-12 characters, consisting atleast one lowercase, one uppercase, one digit, one special character among !@#$%&* and no spaces",
        )
        ok = False

    # name
    name = name_entry.get()
    name = name.strip()
    name_entry.delete(0, END)
    name_entry.insert(0, name)

    if len(name) == 0:
        canv.create_text(350, 220, text="Required", tags="error")
        ok = False

    # utype
    utype = user_type.get()

    if utype not in user_options:
        canv.create_text(350, 275, text="Required", tags="error")
        ok = False
    else:
        dept = dept_entry.get()
        dept = dept.strip()
        dept_entry.delete(0, END)
        dept_entry.insert(0, dept)
        if len(dept) == 0:
            canv.create_text(350, 320, text="Required", tags="error")
            ok = False

        if utype == "UG Student":
            if degree_entry.get() not in degree_options:
                canv.create_text(350, 420, text="Required", tags="error")
                ok = False

    if not ok:
        return

    if not messagebox.askokcancel("Register", "Are you sure?"):
        return

    if utype == "Teacher":
        Teacher(id, pwd, name, dept_entry.get(), course_entry.get())
    elif utype == "UG Student":
        UG_student(id, pwd, name, dept_entry.get(), grad_year.get(), degree_entry.get())
    else:
        PG_student(id, pwd, name, dept_entry.get(), grad_year.get(), field_entry.get())

    messagebox.showinfo(None, "Registered Successfully")
    profile(len(User.all) - 1)


# Home Page
def home():
    login_button = Button(root, text="Log In", font=font_halv15, command=login)
    canv.create_window(350, 200, window=login_button, tags="home_page")

    reg_button = Button(root, text="Register", font=font_halv15, command=register)
    canv.create_window(350, 300, window=reg_button, tags="home_page")


# Main
def main():
    # Global Variables
    global user_options, degree_options, cur_year
    user_options = ["Teacher", "UG Student", "PG Student"]
    degree_options = ["B. Tech.", "Dual"]
    cur_year = datetime.now().year

    # Read and Store Data from JSON file
    filename = "users.json"
    download_user_data(filename)

    # Stage
    global root
    root = Tk()
    root.title("Profile System")
    app_width = 700
    app_height = 500
    global screen_width, screen_height
    screen_width = root.winfo_screenwidth()
    screen_height = root.winfo_screenheight()
    x = screen_width / 2 - app_width / 2
    y = screen_height / 2 - app_height / 2
    root.geometry(f"{app_width}x{app_height}+{int(x)}+{int(y)}")
    root.resizable(False, False)

    # Fonts
    global font_halv15
    font_halv15 = Font(family="Halvetica", size=15)

    # Display
    global canv
    canv = Canvas(root, width=app_width, height=app_height, bd=0, highlightthickness=0)
    canv.place(x=0, y=0, relwidth=1, relheight=1)

    home()
    root.mainloop()

    # Write Data to JSON file
    upload_user_data(filename)


if __name__ == "__main__":
    main()
