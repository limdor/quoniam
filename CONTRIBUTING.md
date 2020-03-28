Contributing to Quoniam
=======
**Quoniam** is a windows application developed using Qt framework and Bazel as a build system.
In order to contribute you will need to fork the repository and create a pull request.
The pull request will be accepted only after passing all CI tests and a code review.

## Building an testing it locally
Currently we are in the middle of the migration from Qt pro files to Bazel. Even though the bazel migration is not complete,
the recomended options is to prepare everything for a bazel compilation.

You will need:
* A windows machine
* Bazel installed and working [(official instructions)](https://docs.bazel.build/versions/master/getting-started.html)
* Qt [installed in your system](https://www.qt.io/download-qt-installer) (check version [here](README.md))
* Change the Qt path in the WORKSPACE file to match your installation

Once everything is set up you should be able to compile the application doing:

`bazel build //...`

And test doing:

`bazel test //...`
