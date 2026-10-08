# Setup

This directory contains the Bash and ROOT configuration scripts used to initialize, maintain, and build the software environment for the **MINERvA CCπ⁰ analysis on lead and iron** on the Fermilab computing systems.

The analysis depended on several components of the MINERvA software stack, including [MAT](https://github.com/MinervaExpt/MAT), [MAT-MINERvA](https://github.com/MinervaExpt/MAT-MINERvA), [UnfoldUtils](https://github.com/MinervaExpt/UnfoldUtils), [ROOT](https://root.cern/), and specific software distributed through Fermilab infrastructure.

These scripts provided a consistent environment for compiling and running the analysis software.


## Environment Configuration

- **`.profile`** – Configures the analysis environment when logging into the Fermilab computing system.

- **`set_MAT_CCPi0.sh`** – Initializes the analysis by defining environment variables and loading the required MINERvA software stack, ROOT, and other dependencies.

- **`.rootlogon_MAT.C`** – Configures ROOT for the analysis by setting paths, loading the required shared libraries, and applying analysis-specific configuration.


## Dependency Management and Build

- **`update_MAT_CCPi0.sh`** – Updates the source repositories used by the analysis environment.

- **`compile_MAT_CCPi0.sh`** – Builds and installs the C++ software dependencies required by the analysis after initialization or source updates.


## Technical Highlights

This directory demonstrates experience with:

- Scientific software.
- Bash scripting and Linux/UNIX environments.
- C++ build and runtime configuration.
- ROOT framework configuration.
- Environment variables and dependency management.
- Dynamic library loading.


## Historical Disclaimer

This software was developed within the MINERvA experiment software ecosystem and depends on the historical Fermilab computing environment in which the analysis was performed.

The repository preserves the original research software and therefore contains experiment-specific dependencies, filesystem locations, storage systems, dataset definitions, grid services, and paths from that environment. It is intended as a record of the analysis implementation and scientific-computing methodology rather than as a standalone modern software package.