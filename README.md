# Asymmetric Least Squares C++ Implementation

[![License](https://img.shields.io/badge/license-BSD-blue.svg)](LICENSE)

This is a stripped down implementation of the asymmetric least squares (ALS) algorithm for baseline subtraction, based off of the paper by Paul H. C. Eilers Hans F.M. Buelens, Baseline Correction with Asymmetric Least Squares Smoothing. This C++ implementation uses the Eigen library for its sparse representations and solvers.

I have included a simple chunk of data, saved in a binary, that can be read and processed. The result is then written to another binary file, output.

## Dependencies
Eigen: http://eigen.tuxfamily.org

## Status

This is simple and done.

## Build Instructions

Pretty standard stuff.

`make all`
