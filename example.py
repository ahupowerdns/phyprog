#!/usr/bin/env python
import sys


f = open(sys.argv[1], "r")

# read dimensions
x_dim, y_dim, z_dim = [int(x) for x in f.readline().split()]

print x_dim, y_dim, z_dim

# read binary data
data = f.read()

# loop over dimensions
for z in range(z_dim):
    print "Z = ", z
    for y in range(y_dim):
        print "Y = ", y, ":",
        for x in range(x_dim):
            print "%3d" % ord(data[z * (x_dim * y_dim) + y * x_dim + x]),
        print
    print