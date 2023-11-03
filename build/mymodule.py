print("MyPython: hello")

a = 0
count = 0

conf = {
    "width":1920,
    "height":1080,
    "title":"C++ call Python"
}

def Main():
    print("MyPython: Main")
    
def RList(lis):
    print("MyPython: RList", lis)
    return [1,2,3,4,5,6]

class MyPython:
    id = 2023
    def __init__(self):
        print("MyPython: init")
        
    def nRnArg(self):
        print("MyPython: nRnArg")
        
    def nRyArg(self, a, b):
        print("MyPython: nRyArg", a, b)
        
    def yRyArg(self, a, b):
        print("MyPython: yRyArg", a, b)
        return a+b
    def yRnArg(self):
        print("MyPython: yRnArg")
        return "yRnArg"
        
print(a)
print(count)