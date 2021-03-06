// a1234567, Stark, Tony
// a2345678, Rogers, Steve
// a3456789, Banner, Bruce
// avengers
/*
created by Andrey Kan
andrey.kan@adelaide.edu.au
2021
*/
#include <iostream>
#include <fstream>
#include <deque>
#include <vector>

// std is a namespace: https://www.cplusplus.com/doc/oldtutorial/namespaces/
const int TIME_ALLOWANCE = 8;  // allow to use up to this number of time slots at once
const int PRIORITY_TIME = 30;
const int NON_PRI_TIME = 15;
const int PRINT_LOG = 0; // print detailed execution trace

class Customer
{
public:
    std::string name;
    int priority;
    int arrival_time;
    int slots_remaining; // how many time slots are still needed
    int playing_since;

    Customer(std::string par_name, int par_priority, int par_arrival_time, int par_slots_remaining)
    {
        name = par_name;
        priority = par_priority;
        arrival_time = par_arrival_time;
        slots_remaining = par_slots_remaining;
        playing_since = -1;
    }
};

class Event
{
public:
    int event_time;
    int customer_id;  // each event involes exactly one customer

    Event(int par_event_time, int par_customer_id)
    {
        event_time = par_event_time;
        customer_id = par_customer_id;
    }
};

void initialize_system(
    std::ifstream &in_file,
    std::deque<Event> &arrival_events,
    std::vector<Customer> &customers)
{
    std::string name;
    int priority, arrival_time, slots_requested;

    // read input file line by line
    // https://stackoverflow.com/questions/7868936/read-file-line-by-line-using-ifstream-in-c
    int customer_id = 0;
    while (in_file >> name >> priority >> arrival_time >> slots_requested)
    {
        Customer customer_from_file(name, priority, arrival_time, slots_requested);
        customers.push_back(customer_from_file);

        // new customer arrival event
        Event arrival_event(arrival_time, customer_id);
        arrival_events.push_back(arrival_event);

        customer_id++;
    }
}

void print_state(
    std::ofstream &out_file,
    int current_time,
    int current_id,
    const std::deque<Event> &arrival_events,
    const std::deque<int> &customer_queue)
{
    out_file << current_time << " " << current_id << '\n';
    if (PRINT_LOG == 0)
    {
        return;
    }
    std::cout << current_time << ", " << current_id << '\n';
    for (int i = 0; i < arrival_events.size(); i++)
    {
        std::cout << "\t" << arrival_events[i].event_time << ", " << arrival_events[i].customer_id << ", ";
    }
    std::cout << '\n';
    for (int i = 0; i < customer_queue.size(); i++)
    {
        std::cout << "\t" << customer_queue[i] << ", ";
    }
    std::cout << '\n';
}

// process command line arguments
// https://www.geeksforgeeks.org/command-line-arguments-in-c-cpp/
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Provide input and output file names." << std::endl;
        return -1;
    }
    std::ifstream in_file(argv[1]);
    std::ofstream out_file(argv[2]);
    if ((!in_file) || (!out_file))
    {
        std::cerr << "Cannot open one of the files." << std::endl;
        return -1;
    }

    // deque: https://www.geeksforgeeks.org/deque-cpp-stl/
    // vector: https://www.geeksforgeeks.org/vector-in-cpp-stl/
    std::deque<Event> arrival_events; // new customer arrivals
    std::vector<Customer> customers; // information about each customer

    // read information from file, initialize events queue
    initialize_system(in_file, arrival_events, customers);

    int current_id = -1; // who is using the machine now, -1 means nobody
    int time_out = -1; // time when current customer will be preempted
    std::deque<int> queue;
    std::deque<int> queue_0; // waiting queue
    std::deque<int> queue_1; // waiting queue
    int turn = 0;


    // step by step simulation of each time slot
    bool all_done = false;
    for (int current_time = 0; !all_done; current_time++)
    {
         std::cout << "CURRENT TIME: " << current_time << std::endl;
        // welcome newly arrived customers
        while (!arrival_events.empty() && (current_time == arrival_events[0].event_time))
        {
            /* MOD Put customer into the right queue */
            int customer_type = customers[arrival_events[0].customer_id].priority;
            //std::cout << "customer_type "<< customer_type;
            if (customer_type == 0) {
                queue_0.push_back(arrival_events[0].customer_id);
            } else {
                queue_1.push_back(arrival_events[0].customer_id);
            }
            ///////////////////
            queue.push_back(arrival_events[0].customer_id);
            arrival_events.pop_front();
        }
        // check if we need to take a customer off the machine
        if (current_id >= 0)
        {
            if (current_time == time_out)
            {
                int last_run = current_time - customers[current_id].playing_since;
                customers[current_id].slots_remaining -= last_run;
                if (customers[current_id].slots_remaining > 0)
                {
                    // customer is not done yet, waiting for the next chance to play

                    //MOD Push customer back to their queue
                    int customer_type = customers[current_id].priority;
                    //std::cout << "customer_type "<< customer_type;
                    if (customer_type == 0) {
                        queue_0.push_back(current_id);
                    } else {
                        queue_1.push_back(current_id);
                    }
                    ////////////

                    queue.push_back(current_id);
                }
                current_id = -1; // the machine is free now
            }
        }
        // if machine is empty, schedule a new customer
        if (current_id == -1)
        {     std::cout << "TURN: "<<turn<<'\n';
            // If turn is 0 or 1, RUN priority queue first
            if (turn > 0 && turn < 5) {
                 if (!queue_0.empty()) // is anyone waiting?
                    {
                        current_id = queue_0.front();
                        queue_0.pop_front();
                        /*
                        std::cout << "PRIORITY QUEUE_0" << std::endl;
                        std::cout << "current_id: " <<current_id << std::endl;
                        std::cout << "customers[current_id].slots_remaining: " << customers[current_id].slots_remaining << std::endl;
                        */
                        if (PRIORITY_TIME > customers[current_id].slots_remaining)
                        {
                            time_out = current_time + customers[current_id].slots_remaining;
                            std::cout << "inside IF: time_out: " << time_out << std::endl;

                        }
                        else
                        {
                            time_out = current_time + PRIORITY_TIME ;
                            std::cout << "else: time_out: " << time_out <<" (time_out= current_time + TIME_ALLOWANCE = "<< current_time << " + "<< TIME_ALLOWANCE<< std::endl;

                        }
                        customers[current_id].playing_since = current_time;
                    } else if (!queue_1.empty()) 
                    {
                        current_id = queue_1.front();
                        queue_1.pop_front();
                        std::cout << "PRIORITY QUEUE_1" << std::endl;
                        std::cout << "current_id: " <<current_id << std::endl;
                        std::cout << "customers[current_id].slots_remaining: " << customers[current_id].slots_remaining << std::endl;
                        if (NON_PRI_TIME > customers[current_id].slots_remaining)
                        {
                            time_out = current_time + customers[current_id].slots_remaining;
                            std::cout << "inside IF: time_out: " << time_out << std::endl;

                        }
                        else
                        {
                            time_out = current_time + NON_PRI_TIME;
                            std::cout << "else: time_out: " << time_out <<" (time_out= current_time + TIME_ALLOWANCE = "<< current_time << " + "<< TIME_ALLOWANCE<< std::endl;

                        }
                        customers[current_id].playing_since = current_time;
                    }
            } else if (turn > 5)  //Run non-priority queue
            {
                if (!queue_1.empty()) 
                    {
                        current_id = queue_1.front();
                        queue_1.pop_front();
                        std::cout << "PRIORITY QUEUE_1" << std::endl;
                        std::cout << "current_id: " <<current_id << std::endl;
                        std::cout << "customers[current_id].slots_remaining: " << customers[current_id].slots_remaining << std::endl;
                        if (NON_PRI_TIME > customers[current_id].slots_remaining)
                        {
                            time_out = current_time + customers[current_id].slots_remaining;
                            std::cout << "inside IF: time_out: " << time_out << std::endl;

                        }
                        else
                        {
                            time_out = current_time + NON_PRI_TIME;
                            std::cout << "else: time_out: " << time_out <<" (time_out= current_time + TIME_ALLOWANCE = "<< current_time << " + "<< TIME_ALLOWANCE<< std::endl;

                        }
                        customers[current_id].playing_since = current_time;
                    } 
                else if (!queue_0.empty()) 
                    {
                        current_id = queue_0.front();
                        queue_0.pop_front();
                        /*
                        std::cout << "PRIORITY QUEUE_0" << std::endl;
                        std::cout << "current_id: " <<current_id << std::endl;
                        std::cout << "customers[current_id].slots_remaining: " << customers[current_id].slots_remaining << std::endl;
                        */
                        if (PRIORITY_TIME > customers[current_id].slots_remaining)
                        {
                            time_out = current_time + customers[current_id].slots_remaining;
                            std::cout << "inside IF: time_out: " << time_out << std::endl;

                        }
                        else
                        {
                            time_out = current_time + PRIORITY_TIME;
                            std::cout << "else: time_out: " << time_out <<" (time_out= current_time + TIME_ALLOWANCE = "<< current_time << " + "<< TIME_ALLOWANCE<< std::endl;

                        }
                        customers[current_id].playing_since = current_time;
                    }
            }

            ////////////////////////////////////////          
            turn = turn + 1;
            turn = turn % 10;
            std::cout << "why turn is not on " <<turn<<'\n';
        }
        print_state(out_file, current_time, current_id, arrival_events, queue);

        // exit loop when there are no new arrivals, no waiting and no playing customers
        all_done = (arrival_events.empty() && queue_0.empty() && (current_id == -1) && queue_1.empty());
        std::cout << "END OF " << current_time << std::endl;

    }

    return 0;
}
