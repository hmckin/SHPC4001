import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("exercise3_convergence.txt")

fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(8, 6))

fig.suptitle(r"Taylor Series Convergence of $e^{-5.5}$", fontsize=14)

ax1.plot(df["i"], df["approximation"])
ax1.axhline(y=0.004087, color='r', linestyle='--', label=r'actual value of $e^{-5.5}$')
ax1.set_ylabel("Approximation")
ax1.xaxis.set_major_locator(plt.MaxNLocator(integer=True))
ax1.legend()

ax2.plot(df["i"], df["error_percent"])
ax2.axhline(y=1.0, color='r', linestyle='--', label='1% threshold')
ax2.set_ylabel("Error %")
ax2.set_xlabel("Number of terms")
ax2.xaxis.set_major_locator(plt.MaxNLocator(integer=True))
ax2.set_yscale("log")  # log scale makes convergence clearer
ax2.legend()

plt.tight_layout()
plt.savefig("convergence3.png")
plt.show()