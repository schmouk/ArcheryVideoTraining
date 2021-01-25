# ArcheryVideoTraining
Multi camera video training for Archery, with real-time as well as delayed and slow-motion captures.

## Goal
This project aims at providing a simple and intuitive application to capture frames with small cameras (e.g. webcams) and under different angles, to display them at capture time, with a delay or in slow-motion (simulated or not).  
The archer should then see herself shooting arrows as long as the display is put in its sight of view, ot she should be able to see herself shooting after a small delay of her choice, looking at a display that is available near from her.

## Development
We've chosen to implement this application in Python 3.8 using external libraries (`OpenCV` for instance or `numpy` also) and Anaconda 3 environment for ease and speed. The application should then run under Windows 32- or 64-bits, Linux 32- or 64- bits and macOS X as long as Anaconda 3 and OpenCV library will have been installed on your machine.  
To avoid such installs and for the simplest use of AVT, we aim at providing also a fully executable program which will first be provided as an independent release for Windows PCs.

## Work is in progress
By 2021-01-25, this repository has been created with the sole license and readme files.  
A development branch `dev` has been created. All devs will be done under this branch and its maybe sub-branches. Merges will only be done on the `dev` branch. Final merges on branch `main` will be successive releases. 
If you want to keep an eye on the implementation progress, plesae refer to branch `dev`.
