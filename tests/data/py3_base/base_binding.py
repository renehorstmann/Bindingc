import ctypes as _ct
from typing import Tuple as _Tuple

_lib = _ct.cdll.LoadLibrary('build/libpy3_base.so')


def foo():
    _lib.foo()


_lib.bar.argtypes = [_ct.c_int]
_lib.bar.restype = _ct.c_int


def bar(b: int):
    """
    Bar function

    :return: int
    """
    _return = _lib.bar(b)
    return _return


_lib.sum.argtypes = [_ct.c_float, _ct.c_float, _ct.c_bool]
_lib.sum.restype = _ct.c_float


def sum(a: float, b: float, absolute: bool):
    _return = _lib.sum(a, b, absolute)
    return _return
