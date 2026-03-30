import matplotlib.pyplot as plt
import csv

iterations = []
errors = []

try:
    with open('convergence.csv', 'r') as f:
        reader = csv.DictReader(f)
        for row in reader:
            iterations.append(int(row['iteration']))
            errors.append(float(row['error']))

    plt.figure(figsize=(10, 6))
    plt.semilogy(iterations, errors, 'b-o', markersize=4)
    
    # Using LaTeX for proper subscripts and symbols
    plt.xlabel('Iteration')
    plt.ylabel(r'$Error = |\lambda_{intermediate} - \lambda_{exact}|$')
    plt.title('Convergence of the Inverse Power Iteration Method')
    
    plt.grid(True, which="both", ls="-", alpha=0.5)
    plt.tight_layout()
    plt.savefig('convergence_plot.png')
    print("Plot saved to convergence_plot.png")
except FileNotFoundError:
    print("Error: convergence.csv not found. Run the C program first.")
except Exception as e:
    print(f"An error occurred: {e}")
