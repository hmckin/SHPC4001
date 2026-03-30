

import numpy as np
import matplotlib.pyplot as plt

# load the data with numpy
data = np.loadtxt("bounce4_vpos5.txt")

# split the columns
t = data[:, 0]
x = data[:, 1]

# plot the data with matplotlib
plt.plot(t, x, 'o', label = "data") # use 'label' to add a legend key
plt.xlabel("Time")
plt.ylabel("Height")
plt.title("Height of Bouncing Ball Over Time with v0=5.0")
plt.legend()
#plt.show() # show the plot
plt.savefig("plot_exercise4_vpos5", dpi = 300) # save it to disk
