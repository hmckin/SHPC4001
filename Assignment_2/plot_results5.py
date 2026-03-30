import pandas as pd
import matplotlib.pyplot as plt


df = pd.read_csv("exercise5_first_order.txt")
fig, ax = plt.subplots(figsize=(8, 6))

ax.plot(df["time"], df["approximation"], label="Euler", color="orange")
ax.plot(df["time"], df["exact"], label="Exact", color="blue")
ax.set_xlabel("Time t")
ax.set_ylabel("N(t)")
ax.legend()
ax.set_title("Euler Method Approximation of Atomic Decay", fontsize=14)

plt.tight_layout(rect=[0, 0, 1, 0.95])

plt.savefig("convergence5b.png", dpi=300)
plt.show()