#!/usr/bin/env bash

# Check if a project name was given
if [ -z "$1" ]; then
    echo "Usage: $0 <project_name>"
    exit 1
fi

# Assign the project name to a variable
project_name=$1

# Create a directory for the project
mkdir -p "$project_name"

# Copy the template project
cp -R .template/ "$project_name"

if [[ "$OSTYPE" == "darwin"* ]]; then
    sed -i '' "s/funcToTest/$project_name/g" "$project_name/main.c"
else
    sed -i "s/funcToTest/$project_name/g" "$project_name/main.c"
fi

echo "Project '$project_name' created successfully with a skeleton C program."
cd "$project_name" && code .

