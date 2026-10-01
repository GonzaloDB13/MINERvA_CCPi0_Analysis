# Setup

This directory contains the environment setup and maintenance scripts used to configure on the Fermilab computing system the MINERvA CCπ⁰ analysis in lead and iron.

The scripts initialize the required MINERvA software suite and configure the environment for building and running the analysis.

- `.profile` – Configures the analysis environment automatically when logging into a Fermilab virtual machine.

- `set_MAT_CCPi0.sh` – Initializes the software environment required by the analysis, including [MAT](https://github.com/MinervaExpt/MAT), [MAT-MINERvA](https://github.com/MinervaExpt/MAT-MINERvA), and [UnfoldUtils](https://github.com/MinervaExpt/UnfoldUtils).

- `update_MAT_CCPi0.sh` – Updates the MINERvA software dependencies used by the analysis.

- `compile_MAT_CCPi0.sh` – Builds the required MINERvA software packages after initialization or in case of dependency updates.

- `.rootlogon_MAT.C` – Configures the ROOT session used by the analysis, including specific libraries and settings for visualization.