import os
import threading as th
from random import randint




def enteghal_action(from_, to_, amount):
    return f"e,{from_},{to_},{amount}"

def variz_action(to_, amount):
    return f"v,-,{to_},{amount}"

def bardasht_action(from_, amount):
    return f"b,{from_},-,{amount}"

def reset_action():
    return "r"

def read_from_file(fd:int):
    print("READ DATA FROM FILE : \n")
    init_data = os.read(fd, 1024)
    print(init_data.decode())



def write_to_file(fd:int, cmd:str):
    os.write(fd, cmd.encode())



def gateway_1(fd:int):

    for i in range(100):
        dis_acnt = randint(1,99)
        cmd = enteghal_action(str(dis_acnt), '0', 1)
        write_to_file(fd, cmd)


def gateway_2(fd:int):

    for i in range(100):
        dis_acnt = randint(1,99)
        cmd = enteghal_action(str(dis_acnt), '0', 1)
        write_to_file(fd, cmd)





if __name__ == "__main__":
    print(">>>>>>>>>>>>start<<<<<<<<<<<\n")
    path = "/dev/arnanode"
    print("OPEN FILE\n")

    #open file
    fd = os.open(path, os.O_RDWR)

    #read file
    read_from_file(fd)

    
    #write_to_file(fd, bardasht_action(0, 10000))

    # read_from_file(fd)

    # write_to_file(fd, variz_action(0, 20000))

    # read_from_file(fd)

    # write_to_file(fd, enteghal_action(1, 2, 3000))

    # read_from_file(fd)

    # write_to_file(fd, reset_action())

    # read_from_file(fd)  



    print("log1")
    t1 = th.Thread(target= gateway_1, args=(fd,))
    t2 = th.Thread(target= gateway_2, args=(fd,))
    print("log2")
    #start two threades
    t1.start()
    t2.start()
    t1.join()
    t2.join()
    #end two threades

    #write_to_file(fd, bardasht_action(0, 100000000))


    read_from_file(fd)
    

    os.close(fd)
    print("\n\n\t\t\t NAVID & ARSALAN")
    print("\t\t<<<<<<<<<<<<<THE END>>>>>>>>>>>\n\n\n")








    












