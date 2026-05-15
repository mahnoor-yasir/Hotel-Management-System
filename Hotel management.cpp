#include <iostream>
#include <string>
using namespace std;

class Room {
public:
    string room_type;
    string room_id;
    string status;
    int availability[30];

    Room(string type, string id, string initial_status) : room_type(type), room_id(id), status(initial_status) {
        for (int i = 0; i < 30; i++) {
            availability[i] = 0;
        }
    }

    bool is_available(int nights) {
        for (int i = 0; i < nights; i++) {
            if (availability[i] != 0) return false;
        }
        return true;
    }

    void book_room(int nights) {
        for (int i = 0; i < nights; i++) {
            availability[i] = 1;
        }
        status = "Booked";
    }

    void set_status(string new_status) {
        status = new_status;
    }

    void cancel_room(int nights) {
        for (int i = 0; i < nights; i++) {
            availability[i] = 0;
        }
        status = "Ready";
    }
};

class Floor {
public:
    int floor_number;
    Room* room_list[10];
    int room_count = 0;

    Floor(int number) : floor_number(number) {}

    void add_room(Room* room) {
        if (room_count < 10) {
            room_list[room_count++] = room;
        }
    }

    Room* get_available_room(string room_type, int nights) {
        for (int i = 0; i < room_count; i++) {
            if (room_list[i]->room_type == room_type && room_list[i]->status == "Ready" && room_list[i]->is_available(nights)) {
                return room_list[i];
            }
        }
        return nullptr;
    }

    void cancel_room(string room_id, int nights) {
        for (int i = 0; i < room_count; i++) {
            if (room_list[i]->room_id == room_id) {
                room_list[i]->cancel_room(nights);
                cout << "Booking for Room " << room_id << " has been canceled." << endl;
                return;
            }
        }
        cout << "No such room found to cancel." << endl;
    }
};

class BookingRequest {
public:
    string customer_name;
    string room_type;
    int nights;
    bool is_high_priority;

    BookingRequest(string name, string type, int nights, bool priority)
        : customer_name(name), room_type(type), nights(nights), is_high_priority(priority) {}
};

class BookingHistory {
public:
    string customer_name;
    string room_id;
    int nights;
    double cost;

    BookingHistory(string name, string id, int nights, double cost)
        : customer_name(name), room_id(id), nights(nights), cost(cost) {}
};

class Hotel {
public:
    string name;
    Floor* floor_list[5];
    int floor_count = 0;
    BookingRequest* request_queue[100];
    int request_count = 0;
    BookingHistory* history_stack[100];
    int history_count = 0;
    double total_revenue;

    Hotel(string hotel_name) : name(hotel_name), total_revenue(0) {}

    void add_floor(Floor* floor) {
        if (floor_count < 5) {
            floor_list[floor_count++] = floor;
        }
    }

    void process_booking_requests() {
        while (request_count > 0) {
            BookingRequest* request = request_queue[0];
            for (int i = 0; i < request_count - 1; i++) {
                request_queue[i] = request_queue[i + 1];
            }
            request_count--;

            bool booking_successful = false;

            for (int i = 0; i < floor_count; i++) {
                Room* room = floor_list[i]->get_available_room(request->room_type, request->nights);
                if (room != nullptr) {
                    room->book_room(request->nights);
                    double cost = request->nights * 10000;
                    history_stack[history_count++] = new BookingHistory(request->customer_name, room->room_id, request->nights, cost);
                    total_revenue += cost;
                    cout << "Booking Successful: " << request->customer_name << " in Room " << room->room_id << endl;
                    booking_successful = true;
                    break;
                }
            }

            if (!booking_successful) {
                cout << "Sorry, no available rooms for " << request->customer_name << " for the requested " << request->nights << " nights." << endl;
            }

            delete request;
        }
        cout << endl;
    }

    void cancel_booking(string customer_name, string room_id) {
        BookingHistory* temp_stack[100];
        int temp_count = 0;
        bool found = false;
        double canceled_cost = 0;

        while (history_count > 0) {
            BookingHistory* booking = history_stack[--history_count];

            if (booking->customer_name == customer_name && booking->room_id == room_id) {
                found = true;
                canceled_cost = booking->cost;
                total_revenue -= canceled_cost;
                cout << "Booking Cancelled: " << booking->customer_name << " in Room " << booking->room_id << endl;
                delete booking;
            } else {
                temp_stack[temp_count++] = booking;
            }
        }

        for (int i = 0; i < temp_count; i++) {
            history_stack[history_count++] = temp_stack[i];
        }

        if (!found) {
            cout << "No booking found for customer: " << customer_name << " in Room " << room_id << endl;
        }

        // Show updated revenue and number of bookings
        cout << "Updated Total Revenue: " << total_revenue << endl;
        cout << "Total Number of Bookings: " << history_count << endl;
        cout << endl;
    }

    void display_status() {
        cout << "Hotel: " << name << endl;
        for (int i = 0; i < floor_count; i++) {
            cout << "  Floor " << floor_list[i]->floor_number << endl;
            for (int j = 0; j < floor_list[i]->room_count; j++) {
                cout << "    Room " << floor_list[i]->room_list[j]->room_id << " (" << floor_list[i]->room_list[j]->room_type << ") - " << floor_list[i]->room_list[j]->status << endl;
            }
        }
        cout << endl;
    }

    void generate_report() {
        cout << "Total Revenue: " << total_revenue << endl;
        cout << "Total Number of Bookings: " << history_count << endl;
        cout << endl;
    }
};

string Cnin() {
    string c;
    cout << "\nEnter the Cnic_number (format XXXXX-XXXXXXX-X): ";

    while (true) {
        cin >> c;
        if (c.length() == 15 && c[5] == '-' && c[13] == '-') {
            bool valid = true;
            for (char ch : c) {
                if ((ch < '0' || ch > '9') && ch != '-') {
                    valid = false;
                    break;
                }
            }
            if (valid) {
                return c;
            } else {
                cout << "Invalid input. Try again: ";
            }
        } else {
            cout << "Invalid input format. Try again: ";
        }
    }
}

string namechk() {
    string name;
    cout << "\nEnter Your User Name: ";

    while (true) {
        cin >> name;
        bool valid = true;
        for (char c : name) {
            if (!isalpha(c)) {
                valid = false;
                break;
            }
        }
        if (valid) {
            return name;
        } else {
            cout << "Invalid name. Enter again: ";
        }
    }
}

class Hotel_entry {
public:
    string Name;
    string Cnic;

    Hotel_entry() : Name(""), Cnic("") {}

    void get_name() {
        Name = namechk();
    }

    void get_cnic() {
        Cnic = Cnin();
    }

    string name() {
        return Name;
    }
};

void book_room(Hotel& hotel_connect, Hotel_entry& person) {
    int nights;
    string type;
    bool vip;
    char again;


    while (true) {
        cout << "How many Nights do you want to Stay? ";
        cin >> nights;

        if (nights > 0) {
            break;
        } else {
            cout << "Invalid number of nights. Please enter a positive number of nights: ";
        }
    }

    cout << "What Type of Room Do you like (Single/Double/Suite)? ";
    cin >> type;

    cout << "Are you a VIP (1/0)? ";
    cin >> vip;

    hotel_connect.request_queue[hotel_connect.request_count++] = new BookingRequest(person.name(), type, nights, vip);

    hotel_connect.process_booking_requests();

    bool booking_found = false;
    double cost = nights * 10000;


    if (hotel_connect.history_count > 0) {
        booking_found = true;
        cout << "At the time of checkout, " << person.Name << ", you have to pay " << cost << endl;
    } else {
        cout << "No room was booked, so no payment is required." << endl;
    }

    char c;
    cout << "Do you want to see the Hotel Booking List (Y/N)? ";
    cin >> c;
    if (c == 'Y' || c == 'y') {
        hotel_connect.display_status();
    }

    cout << "Do you want to book another room? (Y/N): ";
    cin >> again;
    if (again == 'Y' || again == 'y') {

        book_room(hotel_connect, person);
    } else {
        cout << "Thank you for booking with us!" << endl;
    }
    cout << endl;
}

void cancel_booking_recursively(Hotel& hotel_connect) {
    string cancel_name, cancel_room_id;
    char cancel_choice;


    cout << "Do you want to cancel any booking? (Y/N): ";
    cin >> cancel_choice;

    if (cancel_choice == 'Y' || cancel_choice == 'y') {
        cout << "Enter your name: ";
        cin >> cancel_name;
        cout << "Enter your room ID: ";
        cin >> cancel_room_id;

        hotel_connect.cancel_booking(cancel_name, cancel_room_id);


        cancel_booking_recursively(hotel_connect);
    }
}

int main() {
    Hotel hotel_connect("Urban Nest");
    system("color 60");


    for (int i = 1; i <= 5; i++) {
        Floor* floor = new Floor(i);
        for (int j = 1; j <= 10; j++) {
            string room_id = "F" + to_string(i) + "R" + to_string(j);
            string room_type = (j % 3 == 0) ? "Suite" : (j % 2 == 0) ? "Double" : "Single";
            floor->add_room(new Room(room_type, room_id, "Ready"));
        }
        hotel_connect.add_floor(floor);
    }


    char anotherUser;
    do {
        Hotel_entry person;
        cout << ".....Enter Your Name......" << endl;
        person.get_name();
        cout << "......Enter Your Cnic........" << endl;
        person.get_cnic();

        cout << ".......Welcome to Urban Nest Hotel..........." << person.Name << endl;


        book_room(hotel_connect, person);

        cout << "Do you want another user to book a room? (Y/N): ";
        cin >> anotherUser;
    } while (anotherUser == 'Y' || anotherUser == 'y');


    cancel_booking_recursively(hotel_connect);


    hotel_connect.generate_report();

    return 0;
}
