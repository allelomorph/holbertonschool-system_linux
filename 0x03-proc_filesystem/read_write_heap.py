#!/usr/bin/python3
"""
0x03. Python - /proc filesystem - task 0. Hack the VM

    Searches the heap of a process for a given string and replaces it with
another.
"""
from sys import argv, exit


def read_write_heap(pid, read_str, write_str):
    """
    Search the heap of process correponding to `pid` for `read_str` and replace
    it with `write_str`.

    Args:
        pid (int): process id of currently ongoing process' heap to search
        read_str (str): string to search for in heap
        write_str (str): string to replace `read_str` with
    """

    bad_pid = False
    try:
        pid_n = int(pid)
    except (TypeError, ValueError):
        bad_pid = True
    if pid_n < 0:
        bad_pid = True

    if bad_pid:
        print("Missing or invalid pid!")
        print("Usage: read_write_heap(pid, read_str, write_str)")
        exit(1)

    if type(read_str) != str or read_str == "":
        print("Missing or invalid read string!")
        print("Usage: read_write_heap(pid, read_str, write_str)")
        exit(1)

    if type(write_str) != str:
        print("Invalid write string!")
        print("Usage: read_write_heap(pid, read_str, write_str)")
        exit(1)

    try:
        maps = open("/proc/{}/maps".format(pid), 'r')
    except OSError as error:
        print("Can't open file /proc/{}/maps: OSError: {}".format(pid, error))
        exit(1)
    print("[*] maps: /proc/{}/maps".format(pid))
    print("[*] mem: /proc/{}/mem".format(pid))

    heap_data = None
    for line in maps:
        if "heap" in line:
            heap_data = line.split()
    maps.close()

    if heap_data is None:
        print("No heap found!")
        exit(1)
    else:
        print('\n'.join(("[*] Found: {}:".format(heap_data[-1]),
                         "\tpathname = {}".format(heap_data[-1]),
                         "\taddress range = {}".format(heap_data[0]),
                         "\tpermissions = {}".format(heap_data[1]),
                         "\toffset (in bytes) = {}".format(heap_data[2]),
                         "\tinode = {}".format(heap_data[4]))))

    addr = heap_data[0].split('-')
    print("[*] Addresses start [{}] | end [{}]".format(addr[0].lstrip('0'),
                                                       addr[1].lstrip('0')))

    perms = heap_data[1]
    if 'r' not in perms:
        print("Heap does not have read permission")
        exit(0)
    if 'w' not in perms:
        print("Heap does not have write permission")
        exit(0)

    try:
        mem = open("/proc/{}/mem".format(pid), 'rb+')
    except OSError as error:
        print("Can't open file /proc/{}/maps: OSError: {}".format(pid, error))
        exit(1)

    heap_start = int(addr[0], 16)
    heap_end = int(addr[1], 16)
    mem.seek(heap_start)
    heap = mem.read(heap_end - heap_start)
    str_offset = heap.find(bytes(read_str, "ASCII"))
    if str_offset == -1:
        print("Can't find {} in /proc/{}/mem".format(read_str, pid))
        exit(1)
    else:
        print("[*] Found '{}' at {}".format(read_str, hex(str_offset)[2:]))

    mem.seek(heap_start + str_offset)
    mem.write(bytes(write_str + '\0', "ASCII"))
    print("[*] Writing '{}' at {}".format(write_str,
                                          hex(heap_start + str_offset)[2:]))


if __name__ == "__main__":

    if (len(argv) != 4):
        print("Usage: {} <pid> <string to read> "
              "<string to write>".format(argv[0]))
        exit(1)
    else:
        pid = argv[1]
        target_str = argv[2]
        new_str = argv[3]
        read_write_heap(pid, target_str, new_str)
