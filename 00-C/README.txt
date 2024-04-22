## Configuration

Configure clang++ on mac in vscode: see [here](https://code.visualstudio.com/docs/cpp/config-clang-mac)

> In case you need to change the default compiler, you can run Tasks: Configure default build task.

> You can modify your tasks.json to build multiple C++ files by using an argument like "${workspaceFolder}/*.cpp" instead of ${file}. This will build all .cpp files in your current folder.

Note, need to setup for arm64 target architecture (uses x86_64 by default)
see https://github.com/microsoft/vscode-cpptools/issues/7035

## Usage

`./newProj.sh <ProjectName>` will create a directory called "ProjectName" and copy files from `.template`, including a skeleton `main.c` along with a "Build and Run" VScode task in `.template/.vscode/tasks`, as well as opening the project in VSCode

To build and run in one command use the provided task "Build and Run" with shortcut `CMD+SHIFT+B` or equivalently `Tasks: Run Build Task` from the VSCode command palette 

> Modify paths to `bash` and `gcc` as necessary in `.vscode/tasks.json` (defaults: `/opt/homebrew/bin/bash` and `/usr/bin/gcc`)