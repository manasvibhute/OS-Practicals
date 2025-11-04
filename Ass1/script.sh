#!/bin/bash

# File to store customer data
customer_data="ass1.txt"

# --- Create file if not exists ---
create_file() {
    if [ -f "$customer_data" ]; then
        echo "File exists"
    else
        touch "$customer_data"
        echo "CustID Name Product Quantity" > "$customer_data"
        echo "File created successfully"
    fi
}

# --- Validators ---
validate_string() {
    [[ "$1" =~ ^[A-Za-z0-9\ ]+$ ]]
}

validate_num() {
    [[ "$1" =~ ^[0-9]+$ ]]
}

# --- Generate next auto ID (no reuse) ---
generate_id() {
    if [ ! -s "$customer_data" ] || [ "$(wc -l < "$customer_data")" -le 1 ]; then
        echo 1
    else
        awk 'NR>1 {print $1}' "$customer_data" | sort -n | tail -1 | awk '{print $1+1}'
    fi
}

# --- Insert Record ---
insert_record() {
    if [ ! -f "$customer_data" ]; then
        echo "File not found. Please create it first."
        return
    fi

    cust_id=$(generate_id)
    echo "Generated CustID: $cust_id"

    read -p "Enter Name: " name
    while ! validate_string "$name"; do
        read -p "Enter valid name: " name
    done

    read -p "Enter Product: " prod
    while ! validate_string "$prod"; do
        read -p "Enter valid product: " prod
    done

    read -p "Enter Quantity: " quantity
    while ! validate_num "$quantity"; do
        read -p "Enter valid quantity: " quantity
    done

    echo "$cust_id $name $prod $quantity" >> "$customer_data"
    sort -n "$customer_data" -o "$customer_data"
    echo "Record added successfully"
}

# --- Modify Record ---
modify_record() {
    echo -n "Enter ID to modify: "
    read mod_id
    while ! validate_num "$mod_id"; do
        read -p "Enter valid number: " mod_id
    done

    match=$(grep -i "^$mod_id " "$customer_data")
    if [ -z "$match" ]; then
        echo "No matching record found"
        return
    fi

    grep "$mod_id" "$customer_data"
    read -p "Enter New Name: " name
    while ! validate_string "$name"; do
        read -p "Enter valid name: " name
    done

    read -p "Enter New Product: " prod
    while ! validate_string "$prod"; do
        read -p "Enter valid product: " prod
    done

    read -p "Enter New Quantity: " quantity
    while ! validate_num "$quantity"; do
        read -p "Enter valid quantity: " quantity
    done

    grep -iv "^$mod_id " "$customer_data" > temp
    echo "$mod_id $name $prod $quantity" >> temp
    mv temp "$customer_data"
    sort -n "$customer_data" -o "$customer_data"
    echo "Record modified successfully"
}

# --- Delete Record ---
delete_record() {
    echo "Enter ID to delete: "
    read del_id
    if ! grep -q -i "^$del_id " "$customer_data"; then
        echo "No matching record found"
        return
    fi

    grep -iv "^$del_id " "$customer_data" > temp && mv temp "$customer_data"
    echo "Record deleted successfully"
}

# --- View Records ---
view_record() {
    if [ ! -f "$customer_data" ]; then
        echo "File not found. Please create it first."
        return
    fi

    if [ ! -s "$customer_data" ]; then
        echo "No records found."
        return
    fi

    echo "------------- Records -------------"
    column -t "$customer_data"
}

--- Menu ---
while true; do
    echo ""
    echo "========= MENU ========="
    echo "1. Create File"
    echo "2. Insert Record"
    echo "3. Modify Record"
    echo "4. Delete Record"
    echo "5. View Records"
    echo "6. Exit"
    echo "========================"
    read -p "Enter your choice: " choice

    case $choice in
        1) create_file ;;
        2) insert_record ;;
        3) modify_record ;;
        4) delete_record ;;
        5) view_record ;;
        6) echo "Exiting..."; break ;;
        *) echo "Invalid choice. Try again." ;;
    esac
done

# manasvi@manasvi:/mnt/c/Users/bhute/Desktop/oslab/Ass1$ ./script.sh

# ========= MENU =========
# 1. Create File
# 2. Insert Record
# 3. Modify Record
# 4. Delete Record
# 5. View Records
# 6. Exit
# ========================
# Enter your choice: 1
# File created successfully

# ========= MENU =========
# 1. Create File
# 2. Insert Record
# 3. Modify Record
# 4. Delete Record
# 5. View Records
# 6. Exit
# ========================
# Enter your choice: 2
# Generated CustID: 1
# Enter Name: Manasvi
# Enter Product: Shirts
# Enter Quantity: 3
# Record added successfully

# ========= MENU =========
# 1. Create File
# 2. Insert Record
# 3. Modify Record
# 4. Delete Record
# 5. View Records
# 6. Exit
# ========================
# Enter your choice: 5
# ------------- Records -------------
# CustID  Name     Product  Quantity
# 1       Manasvi  Shirts   3

# ========= MENU =========
# 1. Create File
# 2. Insert Record
# 3. Modify Record
# 4. Delete Record
# 5. View Records
# 6. Exit
# ========================
# Enter your choice: 3
# Enter ID to modify: 1
# 1 Manasvi Shirts 3
# Enter New Name: Siya
# Enter New Product: Shirts
# Enter New Quantity: 3
# Record modified successfully

# ========= MENU =========
# 1. Create File
# 2. Insert Record
# 3. Modify Record
# 4. Delete Record
# 5. View Records
# 6. Exit
# ========================
# Enter your choice: 5
# ------------- Records -------------
# CustID  Name  Product  Quantity
# 1       Siya  Shirts   3

# ========= MENU =========
# 1. Create File
# 2. Insert Record
# 3. Modify Record
# 4. Delete Record
# 5. View Records
# 6. Exit
# ========================
# Enter your choice: 4
# Enter ID to delete:
# 1
# Record deleted successfully

# ========= MENU =========
# 1. Create File
# 2. Insert Record
# 3. Modify Record
# 4. Delete Record
# 5. View Records
# 6. Exit
# ========================
# Enter your choice: 5
# ------------- Records -------------
# CustID  Name  Product  Quantity

# ========= MENU =========
# 1. Create File
# 2. Insert Record
# 3. Modify Record
# 4. Delete Record
# 5. View Records
# 6. Exit
# ========================
# Enter your choice: 6
# Exiting...
