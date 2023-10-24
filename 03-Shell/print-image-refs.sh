#!/bin/zsh

# Set the directory to search for Markdown files
directory="/tmp/test"

# Use find to locate all Markdown files in the specified directory
find "$directory" -type f -name "*.md" | while read -r file; do
  # Use grep to search for references to embedded images
  grep -o -E '!\[.*\]\([^)]+\)' "$file"

  grep -o -E '!\[\[([^|]+(\.(png|PNG|jpg|JPG|jpeg|JPEG|pdf|PDF))(?:\|(.*))?)\]\]' "$file"

done

