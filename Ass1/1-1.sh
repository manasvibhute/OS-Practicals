#!/bin/bash

data="test.txt"

create_file(){
    if [ -f "$data" ]; then
        echo "File exists"
    else
        touch "$data"
        echo "File created successfully"
    fi
}

validate_num(){
    [[ "$1" =~ ^[0-9]{10}$ ]]
}

insert_record(){
    if [ ! -f "$data" ]; then
        echo "Create the file first"
        return
    fi
    
    read -p "Enter ID number: " id
    read -p "Enter Name: " name
    read -p "Enter Phone Number: " phno
    while ! validate_num "$phno"; do
        read -p "Enter valid Phone number: " phno
    done
    
    echo "$id;$name;$phno" >> "$data"
    echo "Record added successfully"
}

search_name(){
    read -p "Enter name to search: " name
    match=$(grep -i "$name" "$data")
    if [ -z "$match" ]; then
        echo "No match record found";
    else
        echo "Match found"
        echo "$match"
    fi
}

view_record() {
    if [ ! -f "$data" ]; then
        echo "File not found. Please create it first."
        return
    fi

    if [ ! -s "$data" ]; then
        echo "No records found."
        return
    fi

    echo "------------- Records -------------"
    column -t "$data"
}

delete_record(){
    read -p "Enter id to delete: " del_id
    if ! grep -q -i "^$del_id" "$data" ; then
        echo "No record found"
        return
    fi
    
    grep -iv "^$del_id" "$data" > temp && mv temp "$data"
    echo "Record deleted successfully"
}

while true
do
    echo "MENU"
    echo "1) Create file"
    echo "2) Insert Record"
    echo "3) Search name"
    echo "4) Delete Record"
    echo "5) View Record"
    echo "6) Exit"
    read -p "Enter the choice: " choice
    
    case $choice in
        1) create_file ;;
        2) insert_record ;;
        3) search_name ;;
        4) delete_record ;;
        5) view_record ;;
        6) echo "Exiting.."; break ;;
        *) echo "Invalid Choice" ;;
    esac
done