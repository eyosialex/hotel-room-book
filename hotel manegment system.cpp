#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <climits>
using namespace std;
const int MAX_ROOMS = 100;
class Customer {
public:
    int id;
    string name;
    string phone_number;
    string from_date;
    string to_date;
    string payment_method;
    string payment_reference;
    bool payment_completed;
};

class Room {
public:
    int room_id;
    bool is_ac;
    bool is_large;
    int floor_number;
    int daily_rent;
    bool is_occupied;
    Customer customer;
};

class HotelManager {
private:
    Room rooms[MAX_ROOMS];
    int totalRooms;
    string current_date;

   bool  parseDate(const string& dateStr, tm& date) {
        istringstream iss(dateStr);
       char slash;
        return (iss >> date.tm_mday >> slash >> date.tm_mon >> slash >> date.tm_year) &&
            (slash == '/');
    }

    bool isDateBeforeOrEqual(const string& date1, const string& date2) {
        tm tm1 = { 0 }, tm2 = { 0 };

        if (!parseDate(date1, tm1)) return false;
        if (!parseDate(date2, tm2)) return false;

        tm1.tm_mon -= 1;
        tm1.tm_year -= 1900;
        tm2.tm_mon -= 1;
        tm2.tm_year -= 1900;

        time_t time1 = mktime(&tm1);
        time_t time2 = mktime(&tm2);

        if (time1 == -1 || time2 == -1) return false;

        return difftime(time2, time1) >= 0;
    }

    int calculateDaysBetween(const string& fromDate, const string& toDate) {
        tm tm1 = { 0 };
        tm tm2 = { 0 };

        if (!parseDate(fromDate, tm1)) return 0;
        if (!parseDate(toDate, tm2)) return 0;

        tm1.tm_mon -= 1;
        tm1.tm_year -= 1900;
        tm2.tm_mon -= 1;
        tm2.tm_year -= 1900;

        time_t time1 = mktime(&tm1);
        time_t time2 = mktime(&tm2);

        if (time1 == -1 || time2 == -1) return 0;

        double seconds = difftime(time2, time1);
        return static_cast<int>(seconds / (60 * 60 * 24));
    }

    int calculateStayCost(const Room& room, const string& fromDate, const string& toDate) {
        int days = calculateDaysBetween(fromDate, toDate);
        if (days <= 0) days = 1;
        return room.daily_rent * days;
    }

    void processPayment(int amount, Customer& customer) {
        cout << "\n=== Payment Options ===\n";
        cout << "1. Tele Birr (Mobile Money)\n";
        cout << "2. M-Pesa (Mobile Money)\n";
        cout << "3. CBE (Commercial Bank of Ethiopia)\n";
        cout << "4. BOA (Bank of Abyssinia)\n";
        cout << "Select payment method (1-4): ";

        int paymentChoice;
        while (!(cin >> paymentChoice) || paymentChoice < 1 || paymentChoice > 4) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid choice. Please select 1-4: ";
        }
        cin.ignore();

        switch (paymentChoice) {
        case 1: customer.payment_method = "Tele Birr"; break;
        case 2: customer.payment_method = "M-Pesa"; break;
        case 3: customer.payment_method = "CBE"; break;
        case 4: customer.payment_method = "BOA"; break;
        }

        cout << "Enter your payment reference (phone/account number): ";
        getline(cin, customer.payment_reference);

        cout << "Enter 1 to confirm payment or 0 to cancel: ";
        int confirm;
        while (!(cin >> confirm) || (confirm != 0 && confirm != 1)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Please enter 1 to confirm or 0 to cancel: ";
        }

        customer.payment_completed = (confirm == 1);
        if (customer.payment_completed) {
            cout << "Payment of $" << amount << " via " << customer.payment_method << " confirmed.\n";
        }
        else {
            cout << "Payment cancelled.\n";
        }
    }

public:
    HotelManager() : totalRooms(0) {
        time_t now = time(0);
        tm ltm;
        localtime_s(&ltm, &now);
        ostringstream oss;
        oss << ltm.tm_mday << "/" << (1 + ltm.tm_mon) << "/" << (1900 + ltm.tm_year);
        current_date = oss.str();

        for (int floor = 0; floor < 5; floor++) {
            addRoom(floor * 100 + 1, true, false, floor, 3000);
            addRoom(floor * 100 + 2, false, false, floor, 2500);
            addRoom(floor * 100 + 3, true, false, floor, 3500);
            addRoom(floor * 100 + 4, false, false, floor, 1500);
            addRoom(floor * 100 + 5, true, true, floor, 5500);
            addRoom(floor * 100 + 6, false, true, floor, 4000);
            addRoom(floor * 100 + 7, true, true, floor, 5000);
            addRoom(floor * 100 + 8, false, true, floor, 4500);
            addRoom(floor * 100 + 9, true, false, floor, 3000);
            addRoom(floor * 100 + 10, false, false, floor, 2000);
        }
    }

    void addRoom(int id, bool ac, bool large, int floor, int rent) {
        if (totalRooms < MAX_ROOMS) {
            rooms[totalRooms++] = { id, ac, large, floor, rent, false, Customer() };
        }
    }

    void showAvailableRooms() {
        
        ofstream outFile("SHOW ALL AVAILABLE ROOM.txt", ios::app);

        
        if (outFile.is_open()) {
            outFile << "\n=== Available Rooms Report ===\n";
            outFile << "Generated on: " << current_date << "\n";
            outFile << "ID\tType\tSize\tFloor\tRent\n";
            outFile << "--------------------------------\n";
        }

       
        for (int i = 0; i < totalRooms; i++) {
            
                
                if (outFile.is_open()) {
                    outFile << rooms[i].room_id << "\t"
                        << (rooms[i].is_ac ? "AC" : "Non-AC") << "\t"
                        << (rooms[i].is_large ? "Large" : "Small") << "\t"
                        << rooms[i].floor_number << "\t$"
                        << rooms[i].daily_rent << "\n";
                }
            }
        

       
        if (outFile.is_open()) {
            outFile << "--------------------------------\n";
            outFile << "End of report\n\n";
            outFile.close();
            cout << "\nAvailable rooms list saved to 'available_rooms.txt'\n";
        }
        else {
            cout << "\nWarning: Could not save to file (available_rooms.txt)\n";
        }
    }
    void bookRoom() {
        if (totalRooms == 0) {
            cout << "No rooms available in the hotel.\n";
            return;
        }

        Customer newCustomer;
        newCustomer.payment_completed = false;
        char acChoice = '\0', sizeChoice = '\0';
        int floorChoice = -1;
        int minRent = 0, maxRent = INT_MAX;
        bool filterByAC = false, filterBySize = false, filterByFloor = false, filterByRent = false;
        int chosenRoomId = -1;

        cout << "\nEnter customer ID: ";
        while (!(cin >> newCustomer.id)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Please enter a number: ";
        }
        cin.ignore();

        cout << "Enter customer name: ";
        getline(cin, newCustomer.name);

        cout << "Enter phone number: ";
        getline(cin, newCustomer.phone_number);

        cout << "Enter check-in date (DD/MM/YYYY): ";
        getline(cin, newCustomer.from_date);

        cout << "Enter check-out date (DD/MM/YYYY): ";
        getline(cin, newCustomer.to_date);

        cout << "\nRoom Preferences (press Enter to skip any option):\n";
        cout << "AC Preference (A for AC, N for Non-AC): ";
        string input;
        getline(cin, input);
        if (!input.empty()) {
            acChoice = toupper(input[0]);
            filterByAC = true;
        }

        cout << "Size Preference (L for Large, S for Small): ";
        getline(cin, input);
        if (!input.empty()) {
            sizeChoice = toupper(input[0]);
            filterBySize = true;
        }

        cout << "Floor Preference (0-4): ";
        getline(cin, input);
        if (!input.empty()) {
            floorChoice = stoi(input);
            filterByFloor = true;
        }

        cout << "Minimum Rent (press Enter to skip): ";
        getline(cin, input);
        if (!input.empty()) {
            minRent = stoi(input);
            filterByRent = true;
        }

        cout << "Maximum Rent (press Enter to skip): ";
        getline(cin, input);
        if (!input.empty()) {
            maxRent = stoi(input);
            filterByRent = true;
        }

        int matchingCount = 0;
        int matchingRoomIndices[MAX_ROOMS];

        cout << "\nAvailable Rooms Matching Your Preferences:\n";
        cout << "ID\tType\tSize\tFloor\tRent\n";
        cout << "--------------------------------\n";
        for (int i = 0; i < totalRooms; i++) {
            if (!rooms[i].is_occupied) {
                bool matches = true;

                if (filterByAC && ((acChoice == 'A') != rooms[i].is_ac)) {
                    matches = false;
                }
                if (filterBySize && ((sizeChoice == 'L') != rooms[i].is_large)) {
                    matches = false;
                }
                if (filterByFloor && (floorChoice != rooms[i].floor_number)) {
                    matches = false;
                }
                if (filterByRent && (rooms[i].daily_rent < minRent || rooms[i].daily_rent > maxRent)) {
                    matches = false;
                }

                if (matches) {
                    cout << rooms[i].room_id << "\t"
                        << (rooms[i].is_ac ? "AC" : "Non-AC") << "\t"
                        << (rooms[i].is_large ? "Large" : "Small") << "\t"
                        << rooms[i].floor_number << "\t$"
                        << rooms[i].daily_rent << "\n";
                    matchingRoomIndices[matchingCount++] = i;
                }
            }
        }

        if (matchingCount == 0) {
            cout << "No rooms match your preferences. Showing all available rooms:\n";
            showAvailableRooms();
            matchingCount = totalRooms;
            for (int i = 0; i < totalRooms; i++) {
                matchingRoomIndices[i] = i;
            }
        }

        cout << "\nEnter the Room ID you want to book: ";
        while (!(cin >> chosenRoomId)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Please enter a room ID: ";
        }

        for (int i = 0; i < matchingCount; i++) {
            int roomIndex = matchingRoomIndices[i];
            if (rooms[roomIndex].room_id == chosenRoomId && !rooms[roomIndex].is_occupied) {
                int totalCost = calculateStayCost(rooms[roomIndex], newCustomer.from_date, newCustomer.to_date);

                cout << "\nTotal amount to pay: $" << totalCost << endl;
                processPayment(totalCost, newCustomer);

                if (newCustomer.payment_completed) {
                    rooms[roomIndex].is_occupied = true;
                    rooms[roomIndex].customer = newCustomer;
                    cout << "\n=== Booking Confirmed ===\n";
                    cout << "Room ID: " << rooms[roomIndex].room_id << "\n";
                    cout << "Customer: " << rooms[roomIndex].customer.name << "\n";
                    cout << "Payment Method: " << rooms[roomIndex].customer.payment_method << "\n";
                    cout << "Total Paid: $" << totalCost << "\n";
                }
                else {
                    cout << "Booking not completed due to payment failure.\n";
                }
                return;
            }
        }
        cout << "Invalid Room ID selected or room is already occupied.\n";
    }
    // You already have bookRoom(), checkOut(), etc.
    void checkOut(int roomId) {
        // First find the room
        Room* room = nullptr;
        int roomIndex = -1;
        for (int i = 0; i < totalRooms; i++) {
            if (rooms[i].room_id == roomId && rooms[i].is_occupied) {
                room = &rooms[i];
                roomIndex = i;
                break;
            }
        }

        if (!room) {
            cout << "Room not found or not occupied.\n";
            return;
        }

        // Calculate stay duration and cost
        int days = calculateDaysBetween(room->customer.from_date, room->customer.to_date);
        if (days <= 0) days = 1;
        int totalCost = days * room->daily_rent;

        // Open file for writing
        ofstream myfile("CHECK_FOR_CUSTOMER.txt", ios::app);  // Use append mode
        if (!myfile.is_open()) {
            cout << "Error: Could not open file for writing check-out details.\n";
            // Continue with check-out process even if file writing fails
        }

        // Display and write check-out details
        
        if (myfile.is_open()) {
            myfile << "\n=== Check-out Details ===" << endl;
            myfile << "Date: " << current_date << endl;
            myfile << "Customer: " << room->customer.name << endl;
            myfile << "Phone: " << room->customer.phone_number << endl;
            myfile << "Room: " << room->room_id << endl;
            myfile << "From: " << room->customer.from_date << endl;
            myfile << "To: " << room->customer.to_date << endl;
            myfile << "Stay Duration: " << days << " days" << endl;
            myfile << "Daily Rate: $" << room->daily_rent << endl;
            myfile << "Total Charges: $" << totalCost << endl;
            myfile << "Payment Method: " << (room->customer.payment_completed ? room->customer.payment_method : "Cash") << endl;
            myfile << "----------------------------------------" << endl;
        }

        // Handle payment if not completed
        if (!room->customer.payment_completed) {
            cout << "\nOutstanding balance detected. Please complete payment.\n";
            processPayment(totalCost, room->customer);
        }

        // Complete check-out
        room->is_occupied = false;
        cout << "Check-out completed successfully.\n";

        if (myfile.is_open()) {
            myfile.close();
        }
     }
    
    void UpDateCostemerinfo(int id) {
        for (int i = 0; i < totalRooms; i++) {
            if (rooms[i].is_occupied && rooms[i].customer.id == id) {
                cout << "Updating info for " << rooms[i].customer.name << endl;

                cout << "New phone number (leave blank to keep): ";
                string phone; getline(cin, phone);
                if (!phone.empty()) rooms[i].customer.phone_number = phone;

                cout << "New check-in date (DD/MM/YYYY, blank to keep): ";
                string from; getline(cin, from);
                if (!from.empty()) rooms[i].customer.from_date = from;

                cout << "New check-out date (DD/MM/YYYY, blank to keep): ";
                string to; getline(cin, to);
                if (!to.empty()) rooms[i].customer.to_date = to;

                cout << "Info updated.\n";
                return;
            }
        }
        cout << "Customer ID not found.\n";
    }

    void changecostemroom() {
        int fromRoom, toRoom;
        cout << "Enter current room ID: "; cin >> fromRoom;
        cin.ignore();
        for (int i = 0; i < totalRooms; i++) {
            if (rooms[i].room_id == fromRoom && rooms[i].is_occupied) {
                Customer cust = rooms[i].customer;
                showAvailableRooms();
                cout << "Enter new room ID: "; cin >> toRoom; cin.ignore();

                for (int j = 0; j < totalRooms; j++) {
                    if (rooms[j].room_id == toRoom && !rooms[j].is_occupied) {
                        rooms[j].customer = cust;
                        rooms[j].is_occupied = true;
                        rooms[i].is_occupied = false;
                        cout << "Customer moved to Room " << toRoom << endl;
                        return;
                    }
                }

                cout << "Room not available.\n";
                return;
            }
        }
        cout << "Room not occupied.\n";
    }

    void deletroombeforeoutdate() {
        int id;
        cout << "Enter room ID to cancel: "; cin >> id;
        cin.ignore();
        for (int i = 0; i < totalRooms; i++) {
            if (rooms[i].room_id == id && rooms[i].is_occupied) {
                rooms[i].is_occupied = false;
                cout << "Room " << id << " booking cancelled.\n";
                return;
            }
        }
        cout << "Invalid or unoccupied room.\n";
    }

    void alltheacupiedroominformation() {
        cout << "\nOccupied Room Info:\n";
        for (int i = 0; i < totalRooms; i++) {
            if (rooms[i].is_occupied) {
                cout << "Room " << rooms[i].room_id << ", Customer: " << rooms[i].customer.name
                    << ", Phone: " << rooms[i].customer.phone_number << ", Stay: "
                    << rooms[i].customer.from_date << " to " << rooms[i].customer.to_date << "\n";
            }
        }
    }

    void howmanydaylift() {
        int id;
        cout << "Enter room ID: "; cin >> id;
        cin.ignore();
        for (int i = 0; i < totalRooms; i++) {
            if (rooms[i].room_id == id && rooms[i].is_occupied) {
                int left = calculateDaysBetween(current_date, rooms[i].customer.to_date);
                cout << "Days left: " << (left < 0 ? 0 : left) << endl;
                return;
            }
        }
        cout << "Room not found or not occupied.\n";
    }
    void setCurrentDate() {
        cout << "Current system date: " << current_date << endl;
        cout << "Enter new date (DD/MM/YYYY): ";
        string new_date;
        getline(cin, new_date);

        tm tm_test = { 0 };
        if (parseDate(new_date, tm_test)) {
            current_date = new_date;
            cout << "Date updated to: " << current_date << endl;
            checkForDueCheckouts();
        }
        else {
            cout << "Invalid date format. Date not changed." << endl;
        }
    }

    void checkForDueCheckouts() {
        int rooms_to_checkout[MAX_ROOMS];
        int count = 0;

        for (int i = 0; i < totalRooms; i++) {
            if (rooms[i].is_occupied) {
                if (isDateBeforeOrEqual(rooms[i].customer.to_date, current_date)) {
                    rooms_to_checkout[count++] = rooms[i].room_id;
                }
            }
        }

        if (count > 0) {
            cout << "\n=== The following rooms have reached their check-out date ===" << endl;
            for (int i = 0; i < count; i++) {
                cout << "Room " << rooms_to_checkout[i] << endl;
            }

            cout << "\nDo you want to process check-outs for these rooms? (Y/N): ";
            char choice;
            cin >> choice;
            cin.ignore();

            if (toupper(choice) == 'Y') {
                for (int i = 0; i < count; i++) {
                    cout << "\nProcessing check-out for room " << rooms_to_checkout[i] << "..." << endl;
                    checkOut(rooms_to_checkout[i]);
                }
            }
        }
        else {
            cout << "No rooms have reached their check-out date." << endl;
        }
    }

    void showCurrentDate() {
        cout << "Current system date: " << current_date << endl;
    }
};
int main() {
    HotelManager hotel;
    int mainChoice;

    do {
        cout << "\n=== Welcome to the Hotel Management System ===\n";
        cout << "1. Customer Access\n";
        cout << "2. Manager/Admin Access\n";
        cout << "3. Exit\n";
        cout << "Enter your choice (1-3): ";

        while (!(cin >> mainChoice) || mainChoice < 1 || mainChoice > 3) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid choice. Please enter 1-3: ";
        }
        cin.ignore();

        if (mainChoice == 1) {
            int custChoice;
            do {
                cout << "\n=== Customer Menu ===\n";
                cout << "1. Show Available Rooms\n";
                cout << "2. Book a Room\n";
                cout << "3. Check Out\n";
                cout << "4. Show Days Left for Checkout\n";
                cout << "5. Back to Main Menu\n";
                cout << "Enter your choice (1-5): ";

                while (!(cin >> custChoice) || custChoice < 1 || custChoice > 5) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Invalid choice. Please enter 1-5: ";
                }
                cin.ignore();

                switch (custChoice) {
                case 1:
                    hotel.showAvailableRooms();
                    break;
                case 2:
                    hotel.bookRoom();
                    break;
                case 3: {
                    int roomId;
                    cout << "Enter room ID to check out: ";
                    while (!(cin >> roomId)) {
                        cin.clear();
                        cin.ignore(10000, '\n');
                        cout << "Invalid input. Please enter a number: ";
                    }
                    cin.ignore();
                    hotel.checkOut(roomId);
                    break;
                }
                case 4:
                    hotel.howmanydaylift();
                    break;
                }
            } while (custChoice != 5);
        }

        else if (mainChoice == 2) {
            const string MANAGER_PASSWORD = "EYOSI123"; 
            string enteredPassword;

            cout << "Enter manager password: ";
            getline(cin, enteredPassword);

            if (enteredPassword != MANAGER_PASSWORD) {
                cout << "YOU ARE NOT MANAGER OF THE HOTEL.\n";
                continue; 
            }

            int managerChoice;
            do {
                cout << "\n=== Manager/Admin Menu ===\n";
                cout << "1. Update Customer Info\n";
                cout << "2. Change Customer Room\n";
                cout << "3. Cancel Booking Before Checkout\n";
                cout << "4. Show All Occupied Rooms\n";
                cout << "5. Set Current System Date\n";
                cout << "6. Check for Due Check-outs\n";
                cout << "7. Show Current System Date\n";
                cout << "8. Back to Main Menu\n";
                cout << "Enter your choice (1-8): ";

                while (!(cin >> managerChoice) || managerChoice < 1 || managerChoice > 8) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Invalid input. Enter a number 1-8: ";
                }
                cin.ignore();

                switch (managerChoice) {
                case 1: {
                    int id;
                    cout << "Enter customer ID to update: ";
                    while (!(cin >> id)) {
                        cin.clear();
                        cin.ignore(10000, '\n');
                        cout << "Invalid input. Enter a number: ";
                    }
                    cin.ignore();
                    hotel.UpDateCostemerinfo(id);
                    break;
                }
                case 2:
                    hotel.changecostemroom();
                    break;
                case 3:
                    hotel.deletroombeforeoutdate();
                    break;
                case 4:
                    hotel.alltheacupiedroominformation();
                    break;
                case 5:
                    hotel.setCurrentDate();
                    break;
                case 6:
                    hotel.checkForDueCheckouts();
                    break;
                case 7:
                    hotel.showCurrentDate();
                    break;
                }
            } while (managerChoice != 8);
        }

    } while (mainChoice != 3);

    cout << "Thank you for using the Hotel Management System!\n";
    return 0;
}
