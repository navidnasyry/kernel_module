import os
import threading as th
from random import randint




def enteghal_action(from_:str, to:str, amount:str):
    return f"e,{from_},{to_},{amount}"

def variz_action(to:str, amount:str):
    return f"v,-,{to},{amount}"

def bardasht_action(from_:str, amount:str):
    return f"b,{from_},-,{amount}"

def reset_action():
    return "r"

def read_from_file(fd:int):
    init_data = os.read(fd, 1024)
    print("READ DATA FROM FILE : \n")
    print(init_data)

def write_to_file(fd:int, cmd:str):
    os.write(fd, cmd.encode())



def gateway_1(fd:int):

    for i in range(1000000):
        dis_acnt = randint(1,99)
        cmd = enteghal_action(str(dis_acnt), '0', 1)
        write_to_file(fd, cmd)





if __name__ == "__main__":
    print(">>>>>>>>>>>>start<<<<<<<<<<<\n")
    path = "dev/arnanode"
    print("OPEN FILE\n")

    #open file
    fd = os.open(path, os.O_RDWR|os.O_CREAT)

    #read file
    read_from_file(fd)

    t1 = th.Thread(target= gateway_1, args=(fd,))
    t2 = th.Thread(target= gateway_1, args=(fd,))
  
    #start two threades
    t1.start()
    t2.start()


    t1.join()
    t2.join()
    #end two threades

    read_from_file(fd)
    










    












