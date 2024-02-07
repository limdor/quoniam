# Quoniam installer

In this folder there is everything needed in order to generate an installer for Quoniam for Windows.

## Step by step howto

1. Make sure that we do not have any old generated files
  `bazel clean --expunge`
1. Make sure all tests are passing
  `bazel test //...`
1. Build all targets
  `bazel build //...`
1. Downaload and install [Inno Setup](https://jrsoftware.org/isinfo.php)
1. Open and run the Inno Setup script [Quoniam-x64.iss](./Quoniam-x64.iss)

## Note

This has been tested with Windows 10 and Inno Setup 6.2.2.
