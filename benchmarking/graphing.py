import pandas as pd
import matplotlib.pyplot as plt

# Load Google Benchmark CSV output
# skiprows=4 removes the benchmark metadata header
df = pd.read_csv("bench.csv", skiprows=4)

# Parse the benchmarking name and access the group, library and N
df["group"] = df["name"].apply(lambda x: x.split("_")[0])
df["library"] = df["name"].apply(lambda x: x.split("_")[1].split("/")[0])
df["N"] = df["name"].apply(lambda x: int(x.split("/")[-1]))

# List of benchmarked operations to visualize
groups_to_plot = ["Addition", "Accessor", "Multiplication", "Transpose", "InitializationRandom", "InitializationZeros", "EigSym"]

for g in groups_to_plot:
  sub_group = df[df["group"] == g]

  mc = sub_group[sub_group["library"] == "MatrixClass"]
  arma = sub_group[sub_group["library"] == "Armadillo"]

  plt.figure(figsize=(7,5))

  # Plot runtime vs matrix size
  plt.plot(mc["N"], mc["real_time"], marker="o", label="MatrixClass", color = 'red')
  plt.plot(arma["N"], arma["real_time"], marker="o", label="Armadillo", color = 'blue')

  # Plotting with a log-log scale
  plt.xscale("log")
  plt.yscale("log")

  # label 
  plt.xlabel("log Matrix size N")
  plt.ylabel("log Runtime (ns)")
  plt.title(f"{g} Benchmark")
  plt.legend()

  # Save figure for inclusion in report
  plt.savefig(f"{g}_benchmark.png")