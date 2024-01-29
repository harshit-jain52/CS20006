import json


class User:
    all = []

    def __init__(self, id: str, pwd: str, name: str, utype: str):
        assert utype in ["Teacher", "UG Student", "PG Student"], f"Invalid User Type"
        self.id = id
        self.pwd = pwd
        self.name = name
        self.utype = utype
        self.attempts = 3
        User.all.append(self)

    @classmethod
    def instantiate_from_json(cls, file):
        with open(file, "r") as json_file:
            json_array = json.load(json_file)
            for user in json_array:
                if user["utype"] == "Teacher":
                    Teacher(
                        user["id"],
                        user["pwd"],
                        user["name"],
                        user["dept"],
                        user["course"],
                    )
                elif user["utype"] == "UG Student":
                    UG_student(
                        user["id"],
                        user["pwd"],
                        user["name"],
                        user["dept"],
                        user["grad"],
                        user["degree"],
                    )
                else:
                    PG_student(
                        user["id"],
                        user["pwd"],
                        user["name"],
                        user["dept"],
                        user["grad"],
                        user["field"],
                    )


class Teacher(User):
    all = []

    def __init__(self, id: str, pwd: str, name: str, dept: str, course: str):
        super().__init__(id, pwd, name, "Teacher")
        self.dept = dept
        self.course = course
        Teacher.all.append(self)


class Student(User):
    all = []

    def __init__(
        self, id: str, pwd: str, name: str, utype: str, dept: str, grad_year: int
    ):
        super().__init__(id, pwd, name, utype)
        self.dept = dept
        self.grad = grad_year
        Student.all.append(self)


class UG_student(Student):
    all = []

    def __init__(
        self, id: str, pwd: str, name: str, dept: str, grad_year: int, degree: str
    ):
        super().__init__(id, pwd, name, "UG Student", dept, grad_year)
        assert degree in ["B. Tech.", "Dual"], f"Invalid Degree"
        self.degree = degree
        UG_student.all.append(self)


class PG_student(Student):
    all = []

    def __init__(
        self, id: str, pwd: str, name: str, dept: str, grad_year: int, field: str
    ):
        super().__init__(id, pwd, name, "PG Student", dept, grad_year)
        self.field = field
        PG_student.all.append(self)
