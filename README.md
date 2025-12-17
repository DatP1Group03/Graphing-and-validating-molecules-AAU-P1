SMILES Molecular Analyzer

This project is a C-based application for parsing, analyzing, and visualizing organic molecules using SMILES (Simplified Molecular Input Line Entry System) strings.

The molecule is treated as a graph, where atoms are nodes and bonds are edges.

Features

- Parse and validate molecules from SMILES strings

- Represent molecules using:

  - an Adjacency Matrix

  - a Node Feature Matrix (atomic number, valence, aromaticity, etc.)

- Perform a valence (stability) check to verify chemical correctness

- Run DFS and BFS to analyze graph connectivity

- Draw a 2D visualization of the molecule

  - DFS is also used to detect and count ring structures

- Detect substructures within a molecule

  - Referred to as toxicphore in the code, but implemented as a general subgraph-matching algorithm
 

User Interface

The program includes an interactive GUI built with:

raylib – graphics, window handling, rendering

raygui – UI widgets (tabs, buttons, text boxes, scroll panels)

The interface allows step-by-step exploration of validation, algorithms, and molecular structure.


Build & Platform Support

The project currently builds using CMake on Linux

Tested on Arch Linux

Partial Windows support:

- Some Windows systems can build and run the project

- raylib and raygui are included via an external/ directory

- raylib is added as a git submodule, but Windows builds may require additional setup depending on the environment


Purpose

This project is developed for educational purposes and focuses on:

- graph algorithms in C (DFS, BFS)

- molecular graph representations

- algorithm visualization

- combining chemistry concepts with computer science fundamentals
