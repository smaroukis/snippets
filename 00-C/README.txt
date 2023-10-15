Configure clang++ on mac in vscode: see [here](https://code.visualstudio.com/docs/cpp/config-clang-mac)

> In case you need to change the default compiler, you can run Tasks: Configure default build task.

> You can modify your tasks.json to build multiple C++ files by using an argument like "${workspaceFolder}/*.cpp" instead of ${file}. This will build all .cpp files in your current folder.

Note, need to setup for arm64 target architecture (uses x86_64 by default)
see https://github.com/microsoft/vscode-cpptools/issues/7035