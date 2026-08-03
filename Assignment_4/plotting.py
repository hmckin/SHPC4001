import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import os

# Ensure output directories exist
os.makedirs("csv", exist_ok=True)
os.makedirs("images", exist_ok=True)

def plot_fourier_sums():
    df = pd.read_csv("csv/fourier_sums.csv")
    
    # Combined partial sums plots
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(15, 6))
    
    # Triangle wave subplot
    ax1.plot(df['x'], df['f_tri'], 'k--', label='Exact')
    ax1.plot(df['x'], df['tri5'], label='n=5')
    ax1.plot(df['x'], df['tri10'], label='n=10')
    ax1.plot(df['x'], df['tri20'], label='n=20')
    ax1.set_title("Triangle Wave Fourier Series Partial Sums")
    ax1.set_xlabel("x")
    ax1.set_ylabel("f(x)")
    ax1.legend()
    
    # Sawtooth wave subplot
    ax2.plot(df['x'], df['f_saw'], 'k--', label='Exact')
    ax2.plot(df['x'], df['saw5'], label='n=5')
    ax2.plot(df['x'], df['saw10'], label='n=10')
    ax2.plot(df['x'], df['saw20'], label='n=20')
    ax2.set_title("Sawtooth Wave Fourier Series Partial Sums")
    ax2.set_xlabel("x")
    ax2.set_ylabel("f(x)")
    ax2.legend()
    
    plt.tight_layout()
    plt.savefig("images/1.1.1_fourier_sums.png")

    # Combined error plots
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(15, 6))

    # Triangle wave error subplot
    ax1.plot(df['x'], df['f_tri'] - df['tri5'], label='n=5')
    ax1.plot(df['x'], df['f_tri'] - df['tri10'], label='n=10')
    ax1.plot(df['x'], df['f_tri'] - df['tri20'], label='n=20')
    ax1.set_title("Triangle Wave Error (f(x) - partial sum)")
    ax1.set_xlabel("x")
    ax1.set_ylabel("Error")
    ax1.legend()

    # Sawtooth wave error subplot
    ax2.plot(df['x'], df['f_saw'] - df['saw5'], label='n=5')
    ax2.plot(df['x'], df['f_saw'] - df['saw10'], label='n=10')
    ax2.plot(df['x'], df['f_saw'] - df['saw20'], label='n=20')
    ax2.set_title("Sawtooth Wave Error (f(x) - partial sum)")
    ax2.set_xlabel("x")
    ax2.set_ylabel("Error")
    ax2.legend()

    plt.tight_layout()
    plt.savefig("images/1.1.2_fourier_errors.png")


def plot_fourier_convergence():
    df = pd.read_csv("csv/fourier_convergence.csv")
    subset = df[df['n'] > 50]
    
    # Estimate slopes first to include in legend
    def get_slope(x, y):
        valid = (y > 1e-15)
        if not np.any(valid): return 0.0
        logx = np.log10(x[valid])
        logy = np.log10(y[valid])
        if len(logx) < 2: return 0.0
        coeffs = np.polyfit(logx, logy, 1)
        return -coeffs[0] # Return p as in 1/n^p
    
    p_tri0 = get_slope(subset['n'], subset['err_tri_0'])
    p_tri125 = get_slope(subset['n'], subset['err_tri_0125'])
    p_saw0 = get_slope(subset['n'], subset['err_saw_0'])
    p_saw125 = get_slope(subset['n'], subset['err_saw_0125'])

    eps = 1e-16
    plt.figure(figsize=(10, 6))
    plt.loglog(df['n'], df['err_tri_0'] + eps, 'o-', markersize=3, label=f'Tri @ x=0.0 (p={p_tri0:.2f})')
    plt.loglog(df['n'], df['err_tri_0125'] + eps, 's-', markersize=3, label=f'Tri @ x=0.125 (p={p_tri125:.2f})')
    plt.loglog(df['n'], df['err_saw_0'] + eps, 'v-', markersize=3, label=f'Saw @ x=0.0 (p={p_saw0:.2f})')
    plt.loglog(df['n'], df['err_saw_0125'] + eps, 'd-', markersize=3, label=f'Saw @ x=0.125 (p={p_saw125:.2f})')
    
    plt.title("Fourier Series Pointwise Convergence ($1/n^p$)")
    plt.xlabel("n (number of terms)")
    plt.ylabel("Absolute Error (+1e-16)")
    plt.grid(True, which="both", ls="-", alpha=0.2)
    plt.legend()
    plt.savefig("images/1.2_fourier_convergence.png")

def plot_lanczos():
    df = pd.read_csv("csv/fourier_lanczos.csv")

    fig, axes = plt.subplots(1, 3, figsize=(18, 5))
    for i, n in enumerate([5, 10, 20]):
        ax = axes[i]
        ax.plot(df['x'], df['f_saw'], 'k--', label='Exact')
        ax.plot(df['x'], df[f'saw{n}'], label=f'Standard n={n}')
        ax.plot(df['x'], df[f'lan{n}'], label=f'Lanczos n={n}')
        ax.set_title(f"Gibbs Phenomenon vs Lanczos (n={n})")
        ax.set_xlabel("x")
        ax.set_ylabel("f(x)")
        ax.legend(fontsize='small')
    plt.tight_layout()
    plt.savefig("images/1.3_lanczos_comparison.png")
def plot_dft():
    Ns = [32, 128]

    # Figure 1: Magnitudes comparison
    fig, axes = plt.subplots(1, 2, figsize=(15, 6))
    for i, N in enumerate(Ns):
        df = pd.read_csv(f"csv/dft_results_N{N}.csv")
        axes[i].stem(df['i'], df['mag_dft'])
        axes[i].set_title(fr"DFT Magnitude $|F_m|$ ($N={N}$)")
        axes[i].set_xlabel(r"$m$")
        axes[i].set_ylabel(r"$|F_m|$")
    plt.tight_layout()
    plt.savefig("images/2.1_dft_magnitudes.png")

    # Figure 2: Reconstruction comparison
    fig, axes = plt.subplots(1, 2, figsize=(15, 6))
    for i, N in enumerate(Ns):
        df = pd.read_csv(f"csv/dft_results_N{N}.csv")
        axes[i].plot(df['x'], df['real_in'], 'k-', alpha=0.6, label='Original')
        axes[i].plot(df['x'], df['real_rec'], 'r--', label='IDFT Reconstruction')
        axes[i].set_title(fr"Reconstruction for $N={N}$")
        axes[i].set_xlabel(r"$x$")
        axes[i].set_ylabel(r"$f(x)$")
        axes[i].legend(fontsize='small')
    plt.tight_layout()
    plt.savefig("images/2.2_dft_reconstruction.png")


def plot_wavepacket():
    for t in [0.0, 1.0, 5.0]:
        try:
            df = pd.read_csv(f"csv/wavepacket_t{t:.0f}.csv")
            
            fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(15, 6))
            
            # Position space
            ax1.plot(df['x'], df['psi_sq'])
            ax1.set_title(fr"$|\psi(x,t)|^2$ at $t={t}$")
            ax1.set_xlabel(r"$x$")
            ax1.set_ylabel(r"$|\psi|^2$")
            
            # Momentum space (Centered)
            # We need to map j to k.
            N = len(df)
            dx = 0.1
            dk = 2.0 * np.pi / (N * dx)
            k = (np.arange(N) - N/2) * dk
            
            ax2.plot(k, df['fft_sq'])
            ax2.set_title(fr"Centered $|\Phi(k)|^2$ at $t={t}$")
            ax2.set_xlabel(r"$k$")
            ax2.set_ylabel(r"$|\Phi(k)|^2$")
            
            plt.tight_layout()
            plt.savefig(f"images/wavepacket_plots_t{t:.0f}.png")
        except:
            print(f"No data for t={t}")

def plot_monte_carlo():
    df = pd.read_csv("csv/monte_carlo_results.csv")
    
    plt.figure(figsize=(10, 6))
    plt.loglog(df['N'], df['error'], 'o-', label='MC Error')
    
    # Slope of convergence
    subset = df[df['N'] > 1000]
    logN = np.log10(subset['N'])
    logErr = np.log10(subset['error'])
    coeffs = np.polyfit(logN, logErr, 1)
    print(f"Monte Carlo Convergence Slope: {coeffs[0]:.4f} (Expected -0.5)")
    
    # Reference lines
    N_ref = np.array([1e2, 1e6])
    plt.loglog(N_ref, 1.0/np.sqrt(N_ref), 'k--', label='O(1/sqrt(N))')
    
    plt.title("Monte Carlo Integration Convergence")
    plt.xlabel("N (samples)")
    plt.ylabel("Relative Error")
    plt.legend()
    plt.savefig("images/monte_carlo_convergence.png")

if __name__ == "__main__":
    # plot_fourier_sums()
    # plot_fourier_convergence()
    # plot_lanczos()
    # plot_dft()
    #plot_wavepacket()
    plot_monte_carlo()
