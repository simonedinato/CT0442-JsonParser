#include "json.hpp"

struct list{
    json j;
    list* next;
};

struct dictionary{
    std::pair<std::string,json> j;
    dictionary* next;
};

typedef list* jlist;
typedef dictionary* jdictionary;

struct json::list_iterator{
    list_iterator(jlist p) : ptr(p) {};

    list_iterator& operator++(){
        if(ptr){
            ptr = ptr->next;
            return *this;
        }
        else{
            throw json_exception{"Error: iterator out of bounds - list_iterator"};
        }
    }

    list_iterator operator++(int i){
        json::list_iterator tmp = *this;
        ++(*this);
        return tmp;
    }

    bool operator==(json::list_iterator const& other) const{
        return ptr == other.ptr;
    }

    bool operator!=(list_iterator const& other) const{
        return ptr != other.ptr;
    }

    json& operator*() const{
        return ptr->j;
    }

    json* operator->() const{
        return &(ptr->j);
    }

    private:
        jlist ptr;
};

struct json::const_list_iterator{
    const_list_iterator(jlist p) : ptr(p) {};

    const_list_iterator& operator++(){
        if(ptr){
            ptr = ptr->next;
            return *this;
        }
        else{
            throw json_exception{"Error: iterator out of bounds - const_list_iterator"};
        }
    }

    const_list_iterator operator++(int i){
        const_list_iterator tmp = *this;
        ++(*this);
        return tmp;
    }

    bool operator==(const_list_iterator const& other) const{
        return ptr == other.ptr;
    }

    bool operator!=(const_list_iterator const& other) const{
        return ptr != other.ptr;
    }

    json const& operator*() const{
        return ptr->j;
    }

    json const* operator->() const{
        return &(ptr->j);
    }

    private:
        jlist ptr;
};

struct json::dictionary_iterator{
     dictionary_iterator(jdictionary p) : ptr(p) {};

     dictionary_iterator& operator++(){
         if(ptr){
                ptr = ptr->next;
                return *this;
            }
            else{
                throw json_exception{"Error: iterator out of bounds - dictionary_iterator"};
         }
     }

     dictionary_iterator operator++(int i){
         dictionary_iterator tmp = *this;
         ++(*this);
        return tmp;
     }

     bool operator==(dictionary_iterator const& other) const{
         return ptr == other.ptr;
     }

     bool operator!=(dictionary_iterator const& other) const{
         return ptr != other.ptr;
     }

     std::pair<std::string,json>& operator*() const{
         return ptr->j;
     }

     std::pair<std::string,json>* operator->() const{
         return &(ptr->j);
     }

    private:
        jdictionary ptr;
};

struct json::const_dictionary_iterator{
    const_dictionary_iterator(jdictionary p) : ptr(p) {};

    const_dictionary_iterator& operator++(){
        if(ptr){
            ptr = ptr->next;
            return *this;
        }
        else{
            throw json_exception{"Error: iterator out of bounds - const_dictionary_iterator"};
        }
    }

    const_dictionary_iterator operator++(int i){
        const_dictionary_iterator tmp = *this;
        ++(*this);
        return tmp;
    }

    bool operator==(const_dictionary_iterator const& other) const{
        return ptr == other.ptr;
    }

    bool operator!=(const_dictionary_iterator const& other) const{
        return ptr != other.ptr;
    }

    std::pair<std::string, json> const& operator*() const{
        return ptr->j;
    }

    std::pair<std::string, json> const* operator->() const{
        return &(ptr->j);
    }

    private:
        jdictionary ptr;
};

struct json::impl{
    std::string used;
    double number;
    std::string word;
    bool boolean;
    jlist listhead;
    jlist listtail;
    jdictionary dichead;
    jdictionary dictail;

    void deletelist(jlist& head, jlist& tail){
        jlist j = head;
        while(head){
            head = head->next;
            delete j;
            j = head;
        }
        head = nullptr;
        tail = nullptr;
    }

    void deletedic (jdictionary& head, jdictionary& tail){
        jdictionary j = head;
        while(head){
            head = head->next;
            delete j;
            j = head;
        }
        head = nullptr;
        tail = nullptr;
    }

    jlist copy(jlist h, jlist& tail){
        if(h == nullptr){
            return nullptr;
        }
        else{
            jlist supp = new list{h->j, copy(h->next, tail)};
            if(supp->next == nullptr) tail = supp;
            return supp;
        }
    }

    jdictionary copy2(jdictionary h, jdictionary& tail){
        if(h == nullptr){
            return nullptr;
        }
        else{
            jdictionary supp = new dictionary{h->j, copy2(h->next, tail)};
            if(supp->next == nullptr) tail = supp;
            return supp;
        }
    }

    ~impl(){
        deletelist(listhead, listtail);
        deletedic(dichead, dictail);
    }
};

json::json(){
    pimpl = new impl;
    pimpl->used = "null";
    pimpl->number = 0.0;
    pimpl->word = "";
    pimpl->boolean = false;
    pimpl->listhead = nullptr;
    pimpl->listtail = pimpl->listhead;
    pimpl->dichead = nullptr;
    pimpl->dictail = pimpl->dichead;
}

json::json(json const& other){
    pimpl = new impl;
    pimpl->used = other.pimpl->used;
    pimpl->number = other.pimpl->number;
    pimpl->word = other.pimpl->word;
    pimpl->boolean = other.pimpl->boolean;
    pimpl->listhead = pimpl->copy(other.pimpl->listhead, pimpl->listtail);
    pimpl->dichead = pimpl->copy2(other.pimpl->dichead, pimpl->dictail);
}

json::json(json&& other){
    pimpl = nullptr;
    std::swap(pimpl, other.pimpl);
}

json::~json(){
    delete pimpl;
}

json::list_iterator json::begin_list(){
    if(!is_list()) throw json_exception{"not a list - begin_list_iterator"};
    return list_iterator{pimpl->listhead};
}

json::const_list_iterator json::begin_list() const{
    if(!is_list()) throw json_exception{"not a list - begin_const_list_iterator"};
    return const_list_iterator{pimpl->listhead};
}

json::list_iterator json::end_list(){
    if(!is_list()) throw json_exception{"not a list - end_list_iterator"};
    return list_iterator{nullptr};
}

json::const_list_iterator json::end_list() const{
    if(!is_list()) throw json_exception{"not a list - end_const_list_iterator"};
    return const_list_iterator{nullptr};
}

json::dictionary_iterator json::begin_dictionary(){
    if(!is_dictionary()) throw json_exception{"not a dictionary - begin_dictionary_iterator"};
    return dictionary_iterator{pimpl->dichead};
}

json::const_dictionary_iterator json::begin_dictionary() const{
    if(!is_dictionary()) throw json_exception{"not a dictionary - begin_const_dictionary_iterator"};
    return const_dictionary_iterator{pimpl->dichead};
}

json::dictionary_iterator json::end_dictionary(){
    if(!is_dictionary()) throw json_exception{"not a dictionary - end_dictionary_iterator"};
    return dictionary_iterator{nullptr};
}

json::const_dictionary_iterator json::end_dictionary() const{
    if(!is_dictionary()) throw json_exception{"not a dictionary - end_const_dictionary_iterator"};
    return const_dictionary_iterator{nullptr};
}

json& json::operator=(json const& other){

    if(other.pimpl != this->pimpl){
        delete pimpl;
        pimpl = new impl;
        pimpl->used = other.pimpl->used;
        pimpl->number = other.pimpl->number;
        pimpl->word = other.pimpl->word;
        pimpl->boolean = other.pimpl->boolean;
        pimpl->listhead = pimpl->copy(other.pimpl->listhead, pimpl->listtail);
        pimpl->dichead = pimpl->copy2(other.pimpl->dichead, pimpl->dictail);
    }
    return *this;
}

json& json::operator=(json&& other){
    if(other.pimpl != pimpl){
        delete pimpl;
        pimpl = nullptr;
        std::swap(pimpl, other.pimpl);
    }
    return *this;
}

bool json::is_list() const{
    if(pimpl->used == "list")
        return true;
    else
        return false;
}
bool json::is_dictionary() const{
    if(pimpl->used == "dictionary")
        return true;
    else
        return false;
}
bool json::is_string() const{
    if(pimpl->used == "string")
        return true;
    else
        return false;
}
bool json::is_number() const{
    if(pimpl->used == "number")
        return true;
    else
        return false;
}
bool json::is_bool() const{
    if(pimpl->used == "bool")
        return true;
    else
        return false;
}
bool json::is_null() const{
    if(pimpl->used == "null")
        return true;
    else
        return false;
}

json const& json::operator[](std::string const& key) const{
    if(is_dictionary()){
        jdictionary supp = pimpl->dichead;
        bool found = false;
        while(supp && !found){
            if(supp->j.first == key){
                found = true;
            }
            else supp = supp->next;
        }
        if(found) return (json&)(supp->j.second);
        else throw std::runtime_error("Chiave non trovata - const operator[]");
    }
    else{
        throw std::runtime_error("L'oggetto non è un dizionario - const operator[]");
    }
}

json& json::operator[](std::string const& key){
    if(is_dictionary()){
        jdictionary supp = pimpl->dichead;
        bool found = false;
        while(supp && !found){
            if(supp->j.first == key){
                found = true;
            }
            else supp = supp->next;
        }
        if(found) return (json &)(supp->j.second);
        this->insert(std::pair<std::string, json>(key, json()));
        return (json &)(pimpl->dictail->j.second);
    }
    else{
        throw std::runtime_error("L'oggetto non è un dizionario - operator[]");
    }
}

double& json::get_number(){
    if(this->is_number()){
        return pimpl->number;
    }
    else throw json_exception{"L'oggetto non è un number - get_number"};
}
double const& json::get_number() const{
    if(this->is_number()){
        return pimpl->number;
    }
    else throw json_exception{"L'oggetto non è un number - const get_number"};
}

bool& json::get_bool(){
    if(this->is_bool()){
        return pimpl->boolean;
    }
    else throw json_exception{"L'oggetto non è un boolean - get_bool"};
}

bool const& json::get_bool() const{
    if(this->is_bool()){
        return pimpl->boolean;
    }
    else throw json_exception{"L'oggetto non è un boolean - const get_bool"};
}

std::string& json::get_string(){
    if(this->is_string()){
        return pimpl->word;
    }
    else throw json_exception{"L'oggetto non è una stringa - get_string"};
}

std::string const& json::get_string() const{
    if(this->is_string()){
        return pimpl->word;
    }
    else throw json_exception{"L'oggetto non è una stringa - const get_string"};
}

void json::set_string(std::string const& x){
    if(pimpl->used == "number"){
        pimpl->number = 0.0;
    }
    else if(pimpl->used == "bool"){
        pimpl->boolean = false;
    }
    else if(pimpl->used == "list"){
        pimpl->deletelist(pimpl->listhead, pimpl->listtail);
        pimpl->listhead = nullptr;
        pimpl->listtail = nullptr;
    }
    else if(pimpl->used == "dictionary"){
        pimpl->deletedic(pimpl->dichead, pimpl->dictail);
        pimpl->dichead = nullptr;
        pimpl->dictail = nullptr;
    }
    pimpl->word = x;
    pimpl->used = "string";
}

void json::set_bool(bool x){
    if(pimpl->used == "number"){
        pimpl->number = 0.0;
    }
    else if(pimpl->used == "string"){
        pimpl->word = "";
    }
    else if(pimpl->used == "list"){
        pimpl->deletelist(pimpl->listhead, pimpl->listtail);
        pimpl->listhead = nullptr;
        pimpl->listtail = nullptr;
    }
    else if(pimpl->used == "dictionary"){
        pimpl->deletedic(pimpl->dichead, pimpl->dictail);
        pimpl->dichead = nullptr;
        pimpl->dictail = nullptr;
    }
    pimpl->boolean = x;
    pimpl->used = "bool";
}

void json::set_number(double x){
    if(pimpl->used == "bool"){
        pimpl->boolean = false;
    }
    else if(pimpl->used == "string"){
        pimpl->word = "";
    }
    else if(pimpl->used == "list"){
        pimpl->deletelist(pimpl->listhead, pimpl->listtail);
        pimpl->listhead = nullptr;
        pimpl->listtail = nullptr;
    }
    else if(pimpl->used == "dictionary"){
        pimpl->deletedic(pimpl->dichead, pimpl->dictail);
        pimpl->dichead = nullptr;
        pimpl->dictail = nullptr;
    }
    pimpl->number = x;
    pimpl->used = "number";
}

void json::set_null(){
    if(pimpl->used == "number"){
        pimpl->number = 0.0;
    }
    else if(pimpl->used == "string"){
        pimpl->word = "";
    }
    else if(pimpl->used == "list"){
        pimpl->deletelist(pimpl->listhead, pimpl->listtail);
        pimpl->listhead = nullptr;
        pimpl->listtail = nullptr;
    }
    else if(pimpl->used == "dictionary"){
        pimpl->deletedic(pimpl->dichead, pimpl->dictail);
        pimpl->dichead = nullptr;
        pimpl->dictail = nullptr;
    }
    else if(pimpl->used == "bool"){
        pimpl->boolean = false;
    }
    pimpl->used = "null";
}

void json::set_list(){
    if(pimpl->used == "number"){
        pimpl->number = 0.0;
    }
    else if(pimpl->used == "string"){
        pimpl->word = "";
    }
    else if(pimpl->used == "list"){
        pimpl->deletelist(pimpl->listhead, pimpl->listtail);
    }
    else if(pimpl->used == "dictionary"){
        pimpl->deletedic(pimpl->dichead, pimpl->dictail);
        pimpl->dichead = nullptr;
        pimpl->dictail = nullptr;
    }
    else if(pimpl->used == "bool"){
        pimpl->boolean = false;
    }
    pimpl->listhead = nullptr;
    pimpl->listtail = nullptr;
    pimpl->used = "list";
}

void json::set_dictionary(){
    if(pimpl->used == "number"){
        pimpl->number = 0.0;
    }
    else if(pimpl->used == "string"){
        pimpl->word = "";
    }
    else if(pimpl->used == "list"){
        pimpl->deletelist(pimpl->listhead, pimpl->listtail);
        pimpl->listhead = nullptr;
        pimpl->listtail = nullptr;
    }
    else if(pimpl->used == "dictionary"){
        pimpl->deletedic(pimpl->dichead, pimpl->dictail);
    }
    else if(pimpl->used == "bool"){
        pimpl->boolean = false;
    }
    pimpl->dichead = nullptr;
    pimpl->dictail = nullptr;
    pimpl->used = "dictionary";
}

void json::push_front(json const& x){
    if(pimpl->used != "list"){
        throw json_exception{"L'oggetto non è una lista - push_front"};
    }
    pimpl->listhead = new list{x, pimpl->listhead};
    if(pimpl->listhead == nullptr){
        pimpl->listtail = pimpl->listhead;
    }
}

void json::push_back(json const& x){
    if(pimpl->used != "list"){
        throw json_exception{"L'oggetto non è una lista - push_back"};
    }

    if(pimpl->listhead == nullptr){
        pimpl->listhead = pimpl->listtail =  new list{x, nullptr};
    }
    else{
        if(pimpl->listtail == nullptr){
            jlist tmp = pimpl->listhead;
            while(tmp->next != nullptr){
                tmp = tmp->next;
            }
            pimpl->listtail = tmp;
        }
        pimpl->listtail->next = new list{x, nullptr};
        pimpl->listtail = pimpl->listtail->next;
    }
}

void json::insert(std::pair<std::string, json> const& x){
    if(pimpl->used != "dictionary"){
        throw json_exception{"L'oggetto non è un dizionario - insert"};
    }
    if(pimpl->dichead == nullptr){
        pimpl->dichead = pimpl->dictail = new dictionary{x, nullptr};
    }
    else{
        if(pimpl->dictail == nullptr){
            jdictionary tmp = pimpl->dichead;
            while(tmp->next != nullptr){
                tmp = tmp->next;
            }
            pimpl->dictail = tmp;
        }
        pimpl->dictail->next = new dictionary{x, nullptr};
        pimpl->dictail = pimpl->dictail->next;
    }
}

std::ostream& operator<<(std::ostream& lhs, json const& rhs){
    if(rhs.is_null()){
        lhs << "null";
    }
    else if(rhs.is_bool()){
        if(rhs.get_bool()){
            lhs << "true";
        }
        else{
            lhs << "false";
        }
    }
    else if(rhs.is_number()){
        lhs << rhs.get_number();
    }
    else if(rhs.is_string()){
        lhs << '"'<<rhs.get_string() <<'"';
    }
    else if(rhs.is_list()){
        lhs << "[";
        json::const_list_iterator supp = rhs.begin_list();
        json::const_list_iterator end = rhs.end_list();
        if(supp == end){
            if(supp != nullptr)
                lhs << *supp;
        }
        else{
            while(supp != end){
                lhs << *supp;
                supp++;
                if(supp != end){
                    lhs << ", ";
                }
            }
            if(supp != nullptr) {
                lhs << ", ";
                lhs << *supp;
            }
        }

        lhs << "]";
    }
    else if(rhs.is_dictionary()){
        lhs << "{";
        json::const_dictionary_iterator supp = rhs.begin_dictionary();
        json::const_dictionary_iterator end = rhs.end_dictionary();
        if(supp == end){
            if(supp != nullptr)
                lhs << '"' <<supp->first <<'"'<< " : " << supp->second;
        }
        else {
            while (supp != end) {
                lhs << '"' << supp->first << '"' << " : " << supp->second;
                supp++;
                if (supp != end) {
                    lhs << ", ";
                }
            }
            if (supp != nullptr) {
                lhs << ", ";
                lhs << '"' << supp->first << '"' << " : " << supp->second;
            }
        }
        lhs << "}";
    }
    return lhs;
}

json parse(std::string s){
    json j;
    if(s.empty()){
        throw json_exception{"L'input è vuoto o errato - Stringa vuota - parse"};
    }

    else if(s == "null"){
        j.set_null();
    }
    else if(s[0] >= '0' && s[0] <= '9' || s[0] == '-'){
        double supp = std::stod(s);
        j.set_number(supp);
    }
    else if(s == "true"){
        j.set_bool(true);
    }
    else if(s == "false"){
        j.set_bool(false);
    }
    else if(s[0] == '"'){
        s = s.substr(1, s.size()-2);
        j.set_string(s);
    }
    else if(s[0] == '['){
        j.set_list();
        if(s.size() == 1) {
            throw json_exception{"La lista non è ben formata - Lista non chiusa - parse"};
        }
        if(s[1] == ']'){
            return j;
        }
        if(s.size() == 2){
            throw json_exception{"La lista non è ben formata - Lista non chiusa - parse"};
        }
        std::string supp = s.substr(1, s.size()-1);
        if(supp[0] == ']'){
            return j;
        }
        int i = 0;
        while(i != supp.size()){
            if(supp[i] == ','){
                j.push_back(parse(supp.substr(0, i)));
                supp = supp.substr(i+1, supp.size()-i-1);
                i = 0;
            }
            else if(supp[i] == '['){
                int cont = 1;
                while(cont != 0 && i != supp.size()){
                    i++;
                    if(supp[i] == '"'){
                        bool found = false;
                        i++;
                        while(!found && i != supp.size()){
                            if(supp[i] == '"'){
                                if(supp[i - 1] != '\\') {
                                    found = true;
                                }
                            }
                            i++;
                        }
                    }
                    if(supp[i] == '['){
                        cont++;
                    }
                    else if(supp[i] == ']'){
                        cont--;
                    }
                }
                if(i == supp.size() && cont != 0){
                    throw json_exception{"La lista non è ben formata - Lista non chiusa - parse"};
                }
                i++;
            }
            else if(supp[i] == '{'){
                int cont = 1;
                while(cont != 0 && i != supp.size()){
                    i++;
                    if(supp[i] == '"'){
                        bool found = false;
                        i++;
                        while(!found && i != supp.size()){
                            if(supp[i] == '"'){
                                if(supp[i - 1] != '\\') {
                                    found = true;
                                }
                            }
                            i++;
                        }
                    }
                    if(supp[i] == '{'){
                        cont++;
                    }
                    else if(supp[i] == '}'){
                        cont--;
                    }
                }
                if(i == supp.size() && cont != 0){
                    throw json_exception{"La lista non è ben formata - Dizionario non chiuso - parse"};
                }
                i++;
            }
            else if(supp[i] == '"'){
                int cont = 1;
                while(cont != 0 && i != supp.size()){
                    i++;
                    if(supp[i] == '"'){
                        cont--;
                    }
                }
                if(i == supp.size() && cont != 0){
                    throw json_exception{"La lista non è ben formata - Stringa non chiusa - parse"};
                }
                i++;
            }
            else if(supp[i] == ']'){

                j.push_back(parse(supp.substr(0, i)));
                i = supp.size();
            }
            else{
                i++;
            }
        }
    }
    else if(s[0] == '{'){
        if(s.size() == 1) {
            throw json_exception{"Il dizionario non è ben formato - Chiave non trovata - parse"};
        }
        if(s[1] == '}'){
            return j;
        }
        if(s.size() == 2){
            throw json_exception{"Il dizionario non è ben formato - Chiave non trovata - parse"};
        }

        j.set_dictionary();
        std::string supp = s.substr(1, s.size()-2);
        int i = 0;
        while(i != supp.size()){
            if(supp[0] == ','){
                supp = supp.substr(1, supp.size()-1);
            }
            bool found = false;
            i++;
            while(!found && i != supp.size()){
                if(supp[i] == '"'){
                    if(supp[i - 1] != '\\') {
                        found = true;
                    }
                }
                i++;
            }
            if(i == supp.size()){
                throw json_exception{"Il dizionario non è ben formato - Chiave non trovata - parse"};
            }
            std::string key = supp.substr(1, i-2);
            json value;
            supp = supp.substr(i+1, supp.size()-i-1);
            i = 0;
            int x = 0;
            if(supp[i] == '['){
                int cont = 1;
                while(cont != 0 && x != supp.size()){
                    x++;
                    if(supp[x] == '"'){
                        bool found1 = false;
                        x++;
                        while(!found1 && x != supp.size()){
                            if(supp[x] == '"'){
                                if(supp[x - 1] != '\\') {
                                    found1 = true;
                                }
                            }
                            x++;
                        }
                    }
                    if(supp[x] == '['){
                        cont++;
                    }
                    else if(supp[x] == ']'){
                        cont--;
                    }
                }
                if(x - 1 == supp.size() && supp[x] != ']'){
                    throw json_exception{"Il dizionario non è ben formato - Lista non chiusa - parse"};
                }
                value = parse(supp.substr(i, x-i+1));
            }
            else if(supp[i] == '{'){
                int cont = 1;
                while(cont != 0 && x != supp.size()){
                    x++;
                    if(supp[x] == '"'){
                        bool found1 = false;
                        x++;
                        while(!found1 && x != supp.size()){
                            if(supp[x] == '"'){
                                if(supp[x - 1] != '\\') {
                                    found1 = true;
                                }
                            }
                            x++;
                        }
                    }
                    if(supp[x] == '{'){
                        cont++;
                    }
                    if(supp[x] == '}'){
                        cont--;
                    }
                }
                if(x - 1 == supp.size() && supp[x] != '}'){
                    throw json_exception{"Il dizionario non è ben formato - Dizionario non chiuso - parse"};
                }
                value = parse(supp.substr(i, x-i+1));
            }
            else if(supp[i] == '"'){
                x++;
                bool found1 = false;
                while(!found1 && x != supp.size()){
                    if(supp[x] == '"'){
                        if(supp[x - 1] != '\\') {
                            found1 = true;
                            x--;
                        }
                    }
                    x++;
                }
                if(x - 1 == supp.size() && supp[x] != '"'){
                    throw json_exception{"Il dizionario non è ben formato - Stringa non chiusa - parse"};
                }
                value = parse(supp.substr(0, x + 1));
            }
            else if(supp[i] == 't'){
                if(supp[i+1] != 'r' || supp[i+2] != 'u' || supp[i+3] != 'e'){
                    throw json_exception{"L'input non è valido - True non è true - parse"};
                }
                value = parse("true");
                x = 4;
            }
            else if(supp[i] == 'f'){
                if(supp[i+1] != 'a' || supp[i+2] != 'l' || supp[i+3] != 's' || supp[i+4] != 'e'){
                    throw json_exception{"L'input non è valido - False non è false - parse"};
                }
                value = parse("false");
                x = 5;
            }
            else if(supp[x] >= '0' && supp[x] <= '9' || supp[x] == '-'){
                while(supp[x] != ',' && supp[x] != '}' && supp[x] != ']' && x != supp.size()){
                    x++;
                }
                value = parse(supp.substr(i, x));
            }
            else if(supp[x] == 'n'){
                if(supp[x+1] != 'u' || supp[x+2] != 'l' || supp[x+3] != 'l'){
                    throw json_exception{"L'input non è valido - Null non è null - parse"};
                }
                value = parse("null");
                x = 4;
            }
            else{
                throw json_exception{"L'input non è valido - non appartiene a nessun tipo Dictionary - parse"};
            }
            std::pair<std::string, json> p = std::make_pair(key, value);
            j.insert(p);
            if(x == supp.size()){
                i = supp.size();
            }
            else{
                supp = supp.substr(x+1, supp.size()- x + 1);
                i = 0;
            }
        }
    }
    else{
        throw json_exception{"L'input non è valido - non appartiene a nessun tipo - parse"};
    }
    return j;
}

void clearspaces(std::string& s){
    int i = 0;
    while(i != s.size()){
        if(s[i] == '"'){
            i++;
            bool found = false;
            while(!found && i != s.size()){
                if(s[i] == '"'){
                    if(s[i - 1] != '\\') {
                        found = true;
                    }
                }
                i++;
            }
        }
        else if(s[i] == ' '){
            s.erase(i, 1);
        }
        else{
            i++;
        }
    }
}

std::istream& operator>>(std::istream& lhs, json& rhs){
    if(!rhs.is_null()){
        rhs.set_null();
    }

    std::string line;
    std::string s = "";

    while(std::getline(lhs, line)){
        s += line;
    }
    if(s.empty()){
        throw json_exception{"L'input è vuoto - operator>>"};
    }
    else{
        clearspaces(s);
        std::string filteredString;
        if(s.find_first_of("\r\n\t\b\f\v") != std::string::npos){
            for (char c : s) {
                if (c != '\r' && c != '\n' && c != '\t' && c != '\b' && c != '\f' && c != '\v') {
                    filteredString += c;
                }
            }
            s = filteredString;
        }

        rhs = parse(s);
        return lhs;
    }
    return lhs;
}