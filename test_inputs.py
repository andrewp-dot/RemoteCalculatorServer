import subprocess as sp
import sys
import re

prog_name="./ipkcpd"
localhost="127.0.0.1"
port="2023"

#stasts variables
num_of_tests=0
passed_tests=0
failed_tests=0

#funckcie pre farebne zobrazenie v terminali
def print_red(msg): print("\033[91m {}\033[00m" .format(msg),flush=True)
def print_green(msg): print("\033[92m {}\033[00m" .format(msg),flush=True)
def print_purple(msg): print("\033[95m {}\033[00m" .format(msg),flush=True)


#get usage
USAGE=sp.run([prog_name,"-h"],text=True,capture_output=True)

def run_prog(args=[]):
    return sp.run(args,text=True,capture_output=True)

def print_result(test_condition,test_name):
    global passed_tests
    global failed_tests

    if(test_condition):
        print_green(test_name + ": Passed")
        passed_tests += 1
        return True
    else:
        print_red(test_name + ": Failed")
        failed_tests += 1
        return False

# input format control tests
def test_ip_format():
    
    global num_of_tests
    global passed_tests
    global failed_tests

    test_name="ip format"
    print_purple
    wrong_ip_adressses = [
        ".127.0.0.1",
        "127.0.0.1.",
        "127.0.0.1.0",
        "-123.0.0.1",
        "256.0.0.1",
        "....",
        "ip_address",
        "1279992"
    ]

    good_ip_adresses = [
        "0.0.0.0",
        "255.255.255.255",
        localhost
    ]

    #wrong formats
    print_purple("\tWrong formats of ip adresses")
    for ip in wrong_ip_adressses:
        num_of_tests += 1
        out = run_prog([prog_name,"-h",ip, "-p",port,"-m","tcp"])
        if(not print_result(out.returncode == 10,"ip_wrong")):
            print(ip)
    
    #good formats
    print_purple("\tGood formats of ip adresses")
    for ip in good_ip_adresses:
        num_of_tests += 1
        out = run_prog([prog_name,"-h",ip, "-p",port,"-m","tcp"])
        if(not print_result((out.returncode != 10),"ip_good")):
            print(ip)

    wrong_ports = [
        "65536",
        "-1",
        "port"
    ]

    good_ports = [
        "65535",
        "0",
        "2023"
    ]

    #port testing
    print_purple("\tWrong port formats")
    for prt in wrong_ports:
        num_of_tests += 1
        out = run_prog([prog_name,"-h",localhost, "-p",prt,"-m","tcp"])
        if(not print_result(out.returncode == 10,"port_wrong")):
            print(prt)

    print_purple("\tGood port formats")
    for prt in good_ports:
        num_of_tests += 1
        out = run_prog([prog_name,"-h",localhost, "-p",prt,"-m","tcp"])
        if(not print_result(out.returncode != 10, test_name="port_good")):
            print(prt)



test_ip_format()

if(passed_tests == num_of_tests): print_green("All test passed.")
else: print_red("Failed " + str(failed_tests) + " of " + str(num_of_tests))
