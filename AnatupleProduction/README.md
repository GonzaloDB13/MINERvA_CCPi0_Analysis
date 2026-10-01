# Anatuple Production

The MINERvA CCπ⁰ analysis in lead and iron operates on specialized ROOT files called **anatuples**, which contain the reconstructed and simulated physics information required by the analysis on an event-by-event basis.

Anatuples are produced by processing thousands of MINERvA **reco files** stored on Fermilab computing infrastructure. This directory contains the scripts used to manage that production workflow, from data preparation and staging to distributed processing, merging, and validation.


## Production environment

- `set_Minerva_CCPi0_submit.sh` – Configures the software environment required for anatuple production. The production workflow uses configuration files based on [Gaudi](https://www.sciencedirect.com/science/article/abs/pii/S0010465501002545), part of the MINERvA legacy software.


## Data preparation

Anatuple production uses **SAMWeb**, Fermilab's data handling system for locating and managing experiment files.

Reco files must be available *on disk* before they can be processed for anatuple production. Files living only in long-term *tape* archival storage must first be *prestaged* to disk.

- `setup_samweb.sh` – Initializes the SAMWeb environment.

- `samweb_options.sh` – Checks the availability of the required reco files and decides whether they need to be prestaged from tape to disk.


## Producing anatuples

Once the reco files are available on disk, anatuple production is performed using Fermilab's distributed computing infrastructure.

- `submit_jobs.sh` – Submits multiple grid-computing jobs to process data and Monte Carlo reco files into anatuples ready to be analyzable by the CCπ⁰ analysis.

- `submit_test.sh` – Submits a single production job for testing and validation before larger production runs.


## Merging and validation

- `submit_merge.sh` – Merges the individual output anatuples into a consolidated one, which allows much faster subsequent analysis.

- `submit_audit.sh` – Audits the merged output to verify that the expected individual anatuples were successfully incorporated during the merging process.