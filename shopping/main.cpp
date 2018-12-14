// project 2: 6.3 (P) Shopping
// Farid Khosravi - farid.khosravi@student.tut.fi - 267964
// This code receives a file name, Then read the file and if
// there is any abnormal information or error during opening
// the file or reading it happens shows error and Exit from
// executing the code.In the next step, it splits the information
// of the file line by line and splits each line by using ";" as
// delimiter. Then it stores the splitted information in a data structure
// map<map<struct>>. Then I implemented other functions to have access to
// the information based on different command features.

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>
#include <iomanip>

using namespace std;

// I used 100.0 to show an item is out of stock in my code
const double OUT_OF_STOCK = 100.0;
const string OUT_OF_STOCK_ID = "out-of-stock";

// The data structure of storing products and their prices in our data structure
struct Product {
         string product_name;
         double price;
     };

// These are memory types which I defined to use in my code easier and faster
// Selection is a vecotr of products with their prices
// Location is a map of location of the stores which contains Selection too
// Chains is map of different chains which consists of name of the chain
// store and Location map and Selection vectors
using Selection = vector<Product>;
using Location = map<string, Selection>;
using Chains = map<string, Location>;

// This function receives each line of informatin and splits it by specific delimiter we give it
// The inputs of this function are a string, a delimiter character, and ignore_empty to know should it
// ignore spaces or not and the output is a vector of strings. I used this function in previous exercises
vector<string> split(const string& s, const char delimiter, bool ignore_empty = false){
    vector<string> result;
    string tmp = s;

    while(tmp.find(delimiter) != string::npos)
    {
        string new_part = tmp.substr(0, tmp.find(delimiter));
        tmp = tmp.substr(tmp.find(delimiter)+1, tmp.size());
        if(not (ignore_empty and new_part.empty()))
        {
            result.push_back(new_part);
        }
    }
    if(not (ignore_empty and tmp.empty()))
    {
        result.push_back(tmp);
    }
    return result;
}


// This function is feeded with reference of a data structure which we designed to store the file data
// and a filename which will open by the function to read or generate error in case of
// inappropriate information or lack of some places which should contaion a type of data.
// The output of the function is boolean.The read and splitted data added to the referenced data structure.
// This function stores the information of file in a map<map<struct>> (chain_store<location<products and prices>>)
// Error handling part for this function implemented inside of it.
bool read_file(Chains& chains, string filename) {
    ifstream file_object(filename);

    if (not file_object) {
        cout << "Error: the input file cannot be opened" << endl;
        return false;
    }

    string line = "";
    while ( getline(file_object, line)) {
        vector<string> fields = split(line, ';');

        string chain = fields.at(0);
        string location = fields.at(1);
        string product = fields.at(2);

        if (fields.size() != 4 or chain == "" or chain == " " or location == "" or location == " " or product == "" or product == " "
                or fields.back() == "" or fields.back() == " ") {
            cout << "Error: the file has an erroneous line" << endl;
            return false;
        }

        double price = 0.00;
        if (fields.back() == OUT_OF_STOCK_ID) {
            price = OUT_OF_STOCK;
        } else {
            price = stod(fields.back());
        }


        if (chains.find(chain) == chains.end()) {
            chains.insert({ chain, {} });
        }

        if (chains.at(chain).find(location) == chains.at(chain).end()) {
            chains.at(chain).insert( {location, {}});
        }

        bool found = false;
        for (Product& prod: chains.at(chain).at(location)) {
            if ( prod.product_name == product ) {
                found = true;
                prod.price = price;
            }
        }

        if (not found) {
            chains.at(chain).at(location).push_back({ product, price });
        }
    }
    return true;
}


// The input of this function is our data structure and there is no output except printing
// the name of the chain stores
void chain_list(Chains chain) {
    for (auto chains: chain) {
        cout << chains.first << endl;
    }
}


// This function receives our data structure and a string name as chain store name as input and
// return a boolean value and prints the locations which has this chain store as output.
// if the location exists the function returns true else it returns false.
bool stores_list(Chains chain, string store) {
    Chains::iterator it;

    it  = chain.find(store);
    if (it != chain.end()) {
        Location stores = it->second;
        for(auto store: stores ) {
            cout << store.first << endl;
        }
        return true;
    }
    return false;
}


// This function receives our data structure, string as chain store name and another string as location
// as input and the output is an integer number (0,1,2) and printing the products and prices of that store.
// If the chain name is not found the output will be 1 and if the store location is not found the output will be 2
// if everything goes right the output will be 0.
int selection(Chains chain, string store, string location) {
    Chains::iterator it;
    Location::iterator it_1;

    it  = chain.find(store);
    if (it != chain.end()) {
        Location stores = it->second;
        it_1 = stores.find(location);
        if (it_1 != stores.end()) {
            Selection prod_list = it_1->second;
            
            // In this part I stored the name of the products and prices in a map from the struct we designed
            // in our data structure.
            map<string, double> product_list;
            for (Product prod: prod_list) {
                product_list.insert({prod.product_name, prod.price});
            }
            
            // Here I iterate over the map of products and prices and print the information.
            // instead of price 100 I print out of stock.
            map<string, double>::iterator it_prod;
            for (it_prod = product_list.begin(); it_prod != product_list.end(); it_prod++) {
                if (it_prod->second == 100) {
                    cout << it_prod->first << " out of stock" << endl;
                } else {
                    
                    // I used this part to be able to print the double value by precision of 2 digits after dot.
                    cout << fixed << setprecision(2);
                    cout << it_prod->first << " " << it_prod->second << endl;
                }
            }
        } else {
            return 2;
        }
    } else {
        return 1;
    }
    return 0;
}


// The inputs of this function are the data structure and the string name of a product and
// The output is an ineteger number (0, 1, 2) and if everything goes correct prints of the 
// price of the product and the chain store name and places which have the product.
// The output 0 means everything finished correctly, 1 means the product is out of stock and 2 means
// that product is not in our selection list.
// At first I iterate over my data structure to store the chain store and the store location and
// the price of the product in a map structure. Then I check the minimum price of the product.
// I assigned 100 to min variable because it is more than all prices.
int cheapest(Chains chain, string prod){
    map<string, double> product;
    for(auto chains: chain) {
        for(auto store: chains.second){
            for(Product pro: store.second) {
                if(pro.product_name == prod) {
                    product.insert({chains.first + " " + store.first, pro.price});
                }
            }
        }
    }

    // Finding minimum price initializing min
    double min = 100.00;
    for(auto pro: product) {
        if(pro.second < min) {
            min = pro.second;
        }
    }

    // Check to find out it is out of stock or not and do we have that product or not
    if(min == 100.00 && product.size() != 0) {
        return 1;
    }else if(min == 100.00 && product.size() == 0) {
        return 2;
    }

    // Print the minimum price and chain store name and location
    cout << fixed << setprecision(2);
    cout << min << " euros" << endl;
    for(auto pro: product) {
        if(pro.second == min)
            cout << pro.first << endl;
    }
    return 0;
}


// This function prints all products we have in all chain stores in all locations without repitition.
// The input is our data structure and the output is print of the products.
// I iterate over the data structure and push_back the products to my vector and then sort my vector and
// print it.
void products_print(Chains chain) {
    vector<string> products;
    for(auto chains: chain) {
        for(auto store: chains.second){
            for(Product pro: store.second) {
                vector<string>::iterator it;
                it = find(products.begin(), products.end(), pro.product_name);
                if(it == products.end() ) {
                    products.push_back(pro.product_name);
                }
            }
        }
    }
    sort(products.begin(),products.end());
    for(auto prod: products) {
        cout << prod << endl;
    }
}


int main() {
    // chain     location   product structure
    Chains chains;
    string file_name = "";
    cout << "Input file: ";
    getline(cin, file_name);

    if (not read_file(chains, file_name)) {
        return EXIT_FAILURE;
    }

    // In this part we have our UI and also the most part of error prints
    while(true){
        string line = "";
        cout << "> ";
        getline(cin, line);

        vector<string> parts = split(line, ' ', true);

        string command = parts.at(0);

        // command chains handler
        if(command == "chains"){
            if(parts.size() != 1) {
                cout << "Error: error in command chains" << endl;
            } else {
                chain_list(chains);
            }

         // command stores handler
        } else if(command == "stores") {
            if(parts.size() != 2) {
                cout << "Error: error in command stores" << endl;
            } else {
                string store = parts.at(1);
                if (not stores_list(chains, store)) {
                    cout << "Error: an " << store << " chain" << endl;
                }
            }

         // command selection handler
        } else if(command == "selection") {
            if(parts.size() != 3) {
                cout << "Error: error in command selection" << endl;
            } else {
                string store = parts.at(1);
                string location = parts.at(2);
                int result = selection(chains, store, location);
                if ( result == 1) {
                    cout << "Error: unknown chain" << endl;
                } else if (result == 2) {
                    cout << "Error: unknown store" << endl;
                }
            }

         // command cheapest handler
        } else if(command == "cheapest"){
            if(parts.size() != 2){
                cout << "Error: error in command cheapest" << endl;
            } else {
                string prod = parts.at(1);
                int result = cheapest(chains, prod);
                if (result == 1) {
                    cout << "The product is temporarily out of stock everywhere." << endl;
                } else if (result == 2) {
                    cout << "Product is not part of product selection." << endl;
                }
            }

         // command products handler
        } else if (command == "products"){
            if(parts.size() != 1){
                cout << "Error: error in command products" << endl;
            } else {
            products_print(chains);
            }

        // command quit handler
        } else if(command == "quit"){
           return EXIT_SUCCESS;

         // unknown command handler
        } else {
            cout << "Error: unknown command" << endl;
        }
    }

}
