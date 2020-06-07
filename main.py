import numpy as np
import matplotlib.pyplot as plt
import csv
import sys


def draw_hist(x, y, title, x_label, y_label):
    fig, ax = plt.subplots(figsize=(40, 40))
    ax.bar(x, y, color='red')
    ax.set_title(title)
    ax.set_xlabel(x_label)
    ax.set_ylabel(y_label)
    ax.set_xlim(np.min(x)-1, np.max(x)+1)
    ax.set_ylim(0, np.max(y)+10)
    plt.xticks(np.arange(np.min(x), np.max(x), 10))
    plt.yticks(np.arange(0, np.max(y)+10, 10))

    #    fig.tight_layout()
    plt.show()


if __name__ == "__main__":
    filename = sys.argv[1]
    freq = list()
    freq_title = ""
    value = list()
    value_title = ""
    with open(filename) as file:
        reader = csv.reader(file, delimiter=";")
        count = 0
        for row in reader:
            if count == 0:
                freq_title = row[0]
                value_title = row[1]
                count = -1
            else:
                freq.append(row[0])
                value.append(row[1])
    draw_hist(np.array(freq, dtype=int), np.array(value, dtype=int), "frequency histogram", freq_title, value_title)
