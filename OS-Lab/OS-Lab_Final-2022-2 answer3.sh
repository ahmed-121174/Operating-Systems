#!/bin/bash

# Check if the user has provided the directory path
if [ $# -eq 0 ]; then
    echo "Usage: $0 <directory_path>"
    exit 1
fi

# Get the directory path from the argument
original_dir=$1

# Check if the provided path is a valid directory
if [ ! -d "$original_dir" ]; then
    echo "Error: $original_dir is not a valid directory."
    exit 1
fi

# Extract the directory name and path
dir_name=$(basename "$original_dir")
parent_path=$(dirname "$original_dir")

# Create the backup directory name
backup_dir="${parent_path}/${dir_name}_bak"

# Create the backup directory
if [ -d "$backup_dir" ]; then
    echo "Backup directory $backup_dir already exists. Removing old backup."
    rm -rf "$backup_dir"
fi
mkdir "$backup_dir"

# Copy all files and directories to the backup directory
cp -r "$original_dir/"* "$backup_dir/"

echo "Backup completed. All contents of $original_dir are backed up in $backup_dir."

