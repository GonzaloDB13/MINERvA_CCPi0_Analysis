# Anatuple Production

The MINERvA CCπ⁰ nuclear target analysis operates on specialized ROOT files called **anatuples**, which contain the reconstructed and simulated physics information required by the analysis on an event-by-event basis.

Anatuples are produced by processing thousands of MINERvA **reco files** stored on Fermilab computing storage. This directory contains the scripts used to manage that production workflow, from data preparation and staging to distributed processing, merging, and validation.

## Production environment

- `set_Minerva_CCPi0_submit.sh` – Configures the software environment required for anatuple production. The production workflow uses configuration files based on [Gaudi](https://www.sciencedirect.com/science/article/abs/pii/S0010465501002545), part of the MINERvA legacy software.

## Data preparation

Anatuple production uses **SAM (SAMWeb)**, Fermilab's data handling system for locating and managing files.

Reco files must be available *on disk* before they can be processed for anatuple production. Files in the long-term *tape* archival storage must be *prestaged* to disk before.


- `setup_samweb.sh` – Initializes Samweb.

- `samweb_options.sh` – Checks status of files and decides whether they have to be prestaged.

## Producing anatuples

Once the reco files are available on disk, anatuple production is performed using Fermilab computing infrastructure.

- `submit_jobs.sh` – Submits multiple grid-computing jobs to process data and Monte Carlo reco files into anatuples ready to be analyzable by the CCPi0 analysis suite.

- `submit_test.sh` – Submits a single production job for testing and validation.

## Merging and validation

- `submit_merge.sh` – Merges the individual output anatuples into a consolidated one, which allows much faster subsequent analysis.

- `submit_audit.sh` – Audits the merged output to verify that the expected individual anatuples were successfully incorporated during the merging process.

## Workflow

The complete anatuple production workflow is summarized below:

flowchart LR
    A["Reco files<br/>Tape"] -->|samweb_options.sh| B["Reco files<br/>Disk"]
    B -->|submit_jobs.sh| C["Individual<br/>anatuples"]
    C -->|submit_merge.sh| D["Merged<br/>anatuple"]
    D -->|submit_audit.sh| E["Validated<br/>anatuple"]
    E --> F["CCπ⁰ Analysis"]