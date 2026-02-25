import numpy as np
import matplotlib.pyplot as plt

# load the data with numpy
data = np.loadtxt("bounce.txt") # whitespace delimited data
#data = np.loadtxt("data_comma.txt", delimiter = ",") # comma delimited data
#data = np.loadtxt("data_comma_header.txt", delimiter = ",", skiprows = 1) # comma delimited data with headings

# split the columns
t = data[:, 0]
x = data[:, 1]

# plot the data with matplotlib
plt.plot(t, x, 'o', label = "data") # use 'label' to add a legend key
plt.xlabel("Time")
plt.ylabel("Height")
plt.title("Height of Bouncing Ball Over Time")
plt.legend()
#plt.show() # show the plot
plt.savefig("plot_filename", dpi = 300) # save it to disk
