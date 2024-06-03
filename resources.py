import os
import sys
from statistics import mean


def resource_path(relative_path):
    if getattr(sys, 'frozen', False):  # 是否Bundle Resource
        base_path = sys._MEIPASS
    else:
        base_path = os.path.abspath('.')
    return os.path.join(base_path, relative_path)


def read_lyric_txt():
    lyric_path = resource_path('lyric.txt')
    with open(lyric_path, 'rb') as f:
        data = f.read()
        data_avg = int(mean(data))
        return data_avg
