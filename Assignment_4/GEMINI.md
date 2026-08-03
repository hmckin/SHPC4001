# LATEX Style Reference

Construct main.tex by following the document structure and LaTeX syntax examples in GEMINI.md. Use the code files in this directory to derive technical descriptions, equations, and figures from the /images folder into their corresponding sections. Your goal is to build a complete, standalone main.tex template including the preamble, sections, and mathematical environments that reflects my code, plots and established style for this project.

Use the follow syntax to assist in building the .tex file by substituting the appropriate data from the current repository. All figures are stored in the /image directory.

## DOCUMENT STRUCTURE:

- Section 1: Quantum Wavepacket Evolution.
  - Scenario: Gaussian wavepacket in free space.
  - Observations: Position space (|psi|^2) spreads over time (t=0, 1, 5). Momentum space (|phi|^2) is invariant (conservation of momentum).
  - Figure 1 (Subfigures): Position/Momentum plots for t=0, 1, 5, saved in images directory.
- Section 2: Monte Carlo Integration.
  - Goal: Evaluate triple integral of exp(-(x+y+z)) over [0,2]x[0,3]x[0,4].
  - Analytical: (1-e^-2)(1-e^-3)(1-e^-4) approx 0.806567.
  - Algorithm: Random sampling in C using rand(), scaled to bounds.
  - Figure 2: Use the Convergence plot saved in images directory.
  - Result: Slope ~ -0.5 (O(1/sqrt(N))), consistent with Central Limit Theorem.


## LaTeX EXAMPLES

### 1. STANDARD FIGURE
```latex
\begin{figure}[H]
    \centering
    \includegraphics[width=0.85\textwidth]{images/NAME.png}
    \caption{DESCRIPTION.}
    \label{fig:LABEL}
\end{figure}
```

### 2. SUBFIGURES (t=0, 1, 5)
```latex
\begin{figure}[H]
    \centering
    \begin{subfigure}[b]{0.85\textwidth}
        \centering
        \includegraphics[width=\textwidth]{images/wp_t0.png}
        \caption{$t=0$}
    \end{subfigure}
    \\[1ex]
    \begin{subfigure}[b]{0.85\textwidth}
        \centering
        \includegraphics[width=\textwidth]{images/wp_t1.png}
        \caption{$t=1$}
    \end{subfigure}
    \caption{COMPOSITE CAPTION.}
\end{figure}
```

### 3. MATH & SECTIONS
```latex
\section*{Part X: Title}
\subsection*{Question X.X}
$E \approx C n^{-p}$
\begin{equation*}
    I = \int_{0}^{2} e^{-x} \, dx
\end{equation*}
Convergence at $O(n^{-1})$ or $O(k^{-2})$.
```
