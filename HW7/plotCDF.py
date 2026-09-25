import pandas as pd
import matplotlib.pyplot as plt

data = pd.read_csv("cdf.csv")

y = data["y"]
empirical_cdf = data["empirical_cdf"]
exact_cdf = data["exact_cdf"]

plt.step(
    y,
    empirical_cdf,
    where="post",
    label="Empirical CDF"
)

plt.plot(
    y,
    exact_cdf,
    label="Exact CDF",
    linewidth=2
)

plt.xlabel("y")
plt.ylabel("F(y)")
plt.title("Empirical vs Exact CDF - Exponential Distribution")

plt.grid(True)
plt.legend()

plt.savefig("cdf.png")