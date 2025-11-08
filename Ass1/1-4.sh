#!/bin/bash

# ------------------------------
# File Management Program
# ------------------------------

# Function: Test if file exists
test_file() {
    read -p "Enter filename to check: " filename
    if [ -f "$filename" ]; then
        echo "✅ File '$filename' exists."
    else
        echo "❌ File '$filename' does not exist."
    fi
    echo
}

# Function: Read a file
read_file() {
    read -p "Enter filename to read: " filename
    if [ ! -f "$filename" ]; then
        echo "❌ File '$filename' does not exist."
        echo
        return
    fi
    echo "----- Contents of '$filename' -----"
    cat "$filename"
    echo "----------------------------------"
    echo
}

# Function: Delete a file
delete_file() {
    read -p "Enter filename to delete: " filename
    if [ ! -f "$filename" ]; then
        echo "❌ File '$filename' does not exist."
        echo
        return
    fi
    rm "$filename"
    echo "✅ File '$filename' deleted successfully."
    echo
}

# Function: Display list of files in current directory
list_files() {
    echo "----- Files in current directory -----"
    ls
}

# ------------------------------
# Menu Loop
# ------------------------------
while true
do
    echo "====== FILE MANAGEMENT MENU ======"
    echo "1) Test if file exists"
    echo "2) Read a file"
    echo "3) Delete a file"
    echo "4) Display list of files"
    echo "5) Exit"
    echo "=================================="
    
    read -p "Enter your choice: " choice

    case $choice in
        1) test_file ;;
        2) read_file ;;
        3) delete_file ;;
        4) list_files ;;
        5) echo "Exiting program..."; break ;;
        *) echo "❌ Invalid choice! Please try again." ; echo ;;
    esac
done
