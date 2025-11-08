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

insert_record(){
    if [ ! -f "$data" ]; then
        echo "File not created"
        return
    fi
    
    read -p "Enter ID: " id
    read -p "Enter the name: " name
    read -p "Enter the phone number: " phno
    sort -n "$data" -o "$data"
    echo -e "$id\t$name\t$phno" >> "$data"
    echo "Record inserted successfully"
}

search_record(){
    read -p "Enter the id to search: " id
    result=$(grep -i "$id[[:space:]]" "$data")
    if [ -z "$result" ]; then
        echo "No records found"
        return
    else
        echo "$result"
        echo "Record found"
    fi
}

modify_record(){
    read -p "Enter the id to search: " mod_id
    result=$(grep -i "^$mod_id[[:space:]]" "$data")
    if [ -z "$result" ]; then
        echo "No matching id found"
        return
    fi
    
    grep "mod_id" "$data"
    read -p "Enter the name: " name
    read -p "Enter the phone number: " phno
    
    grep -iv "^$mod_id" "$data" > temp
    echo -e "$mod_id\t$name\t$phno" >> temp
    mv temp "$data"
    sort -n "$data" -o "$data"
    echo "Record modified successfully"
}

delete_record() {
    read -p "Enter ID to delete: " del_id
    # Match tab literally using $'\t'
    result=$(grep -i "^${del_id}$'\t'" "$data")
    if [ -z "$result" ]; then
        echo "No matching ID found"
        return
    fi

    echo "Record found:"
    echo "$result"
    read -p "Are you sure you want to delete this record? (y/n): " confirm
    if [[ "$confirm" =~ ^[Yy]$ ]]; then
        grep -iv "^${del_id}$'\t'" "$data" > temp && mv temp "$data"
        echo "Record deleted successfully"
    else
        echo "Deletion cancelled"
    fi
}


view_record(){
    if [ ! -f "$data" ]; then
        echo "File doesn't exist"
        return
    fi
    
    if [ ! -s "$data" ]; then
        echo 
        echo "-----File is empty-----"
        return
    fi
    
    echo "File contents"
    column -t "$data"
}

while true; do
    echo "----MENU----"
    echo "1.Create file"
    echo "2.Insert Record"
    echo "3.Search Record"
    echo "4.View Record"
    echo "5.Modify Record"
    echo "6.Delete Record"
    echo "7.Exit"
    read -p "Enter your choice: " choice
    
    case "$choice" in
        1) create_file ;;
        2) insert_record ;;
        3) search_record ;;
        4) view_record ;;
        5) modify_record ;;
        6) delete_record ;;
        7) echo "Exiting..."; break ;;
    esac
done