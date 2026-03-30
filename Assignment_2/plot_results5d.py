import pandas as pd
import matplotlib.pyplot as plt


df = pd.read_csv("exercise5d_errors.txt")
fig, ax = plt.subplots(figsize=(8, 6))

ax.plot(df["dt"], df["error_euler"], label="Euler", color="orange")
ax.plot(df["dt"], df["error_leapfrog"], label="Leap Frog", color="blue")
ax.set_xlabel("Time step dt")
ax.set_ylabel("Error")
ax.legend()
ax.set_title("Fractional Error in Euler Method and Leap Frog Method", fontsize=14)

plt.xscale('log')
plt.yscale('log') 

plt.tight_layout(rect=[0, 0, 1, 0.95])

plt.savefig("convergence5d.png", dpi=300)
plt.show()