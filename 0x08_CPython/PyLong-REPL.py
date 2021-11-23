#!/usr/bin/python3

import ctypes

"""
PyLongObject represents one bignum as defined
by CPython and will map to `_longobject` of
its internals.

Here we have preallocated ob_digit array to hold
at max 100 digits/elements.
"""
class PyLongObject(ctypes.Structure):
    _fields_ = [
        ("ob_refcnt", ctypes.c_long),
        ("ob_type",   ctypes.c_void_p),
        ("ob_size",   ctypes.c_long),
        ("ob_digit",  ctypes.c_uint * 100)
    ]

"""
def numberToBase(n, b):
    if n == 0:
        return [0]
    digits = []
    while n:
        digits.append(int(n % b))
        n //= b
    return digits[::-1]
"""

"""
get_digits function returns ob_digits array
representing how a big number is stored
internally as "digits".

The function returns a list of digits starting
from least significant digit to most significant.
The length of the list implies `ob_size`.
"""
def get_digits(bignum):
  obj = PyLongObject.from_address(id(bignum))
  size = abs(obj.ob_size)
  return obj.ob_digit[:size]


if __name__ == '__main__':
    while (1):
        try:
            num = int(input("Enter value:"))
        except ValueError:
            print("Invalid value, try another")
            continue
        except (EOFError, KeyboardInterrupt):
            print()
            exit()
        digits = get_digits(num)
        print(num, " LSD first in base 2**30:", digits)
