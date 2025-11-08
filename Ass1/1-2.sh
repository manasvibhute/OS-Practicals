#!/bin/bash

phonebook="phonebook.txt"

# Create the phonebook if it doesn't exist
create_phonebook() {
    if [ ! -f "$phonebook" ]; then
        touch "$phonebook"
        echo "✅ Phonebook created successfully."
    fi
}

# Add a new entry
add_entry() {
    read -p "Enter First Name: " fname
    read -p "Enter Last Name: " lname
    while true; do
        read -p "Enter Phone Number (10 digits): " phone
        if [[ $phone =~ ^[0-9]{10}$ ]]; then
            break
        else
            echo "❌ Invalid phone number! Must be 10 digits."
        fi
    done

    # Tab-separated: FirstName LastName Phone
    echo -e "$fname\t$lname\t$phone" >> "$phonebook"
    echo "✅ Entry added successfully."
    echo
}

# Search by first name, last name, or phone
search_entry() {
    if [ ! -s "$phonebook" ]; then
        echo "❌ Phonebook is empty."
        echo
        return
    fi

    read -p "Enter name or phone number to search: " query
    matches=$(grep -i "$query" "$phonebook")

    if [ -z "$matches" ]; then
        echo "❌ No matching record found."
    else
        echo "✅ Matches found:"
        echo -e "$matches" | column -t
    fi
    echo
}

# Sort by Last Name (column 2)
sort_by_lastname() {
    if [ ! -s "$phonebook" ]; then
        echo "❌ Phonebook is empty."
        echo
        return
    fi

    echo "----- Phonebook Sorted by Last Name -----"
    sort -k2,2 "$phonebook" | column -t
    echo "-----------------------------------------"
    echo
}

# Delete by phone number
delete_entry() {
    if [ ! -s "$phonebook" ]; then
        echo "❌ Phonebook is empty."
        echo
        return
    fi

    read -p "Enter phone number of entry to delete: " phone
    if ! grep -q "$phone" "$phonebook"; then
        echo "❌ No record found with that phone number."
        echo
        return
    fi

    grep -v "$phone" "$phonebook" > temp && mv temp "$phonebook"
    echo "✅ Record deleted successfully."
    echo
}

# ------------------------------
# Main Menu Loop
# ------------------------------
create_phonebook

while true
do
    echo "====== PHONEBOOK MENU ======"
    echo "1) Add new entry"
    echo "2) Search entry"
    echo "3) Sort by last name"
    echo "4) Delete an entry"
    echo "5) Exit"
    echo "============================"
    
    read -p "Enter your choice: " choice

    case $choice in
        1) add_entry ;;
        2) search_entry ;;
        3) sort_by_lastname ;;
        4) delete_entry ;;
        5) echo "Exiting Phonebook..."; break ;;
        *) echo "❌ Invalid choice! Try again." ; echo ;;
    esac
done
