
#include <iostream>
#include <vector>
#include <cassert>
#include "test2.pb.h"

int main()
{
    
    using namespace std;
    
    
    
    tutorial::AddressBook b;
    tutorial::Person* p =  b.add_people();

    p->set_email("yangbo");
    tutorial::Person::PhoneNumber* phone = p->add_phones();
    phone->set_number("702090924");
    phone->set_type(tutorial::Person::PhoneType::Person_PhoneType_MOBILE);

    
    string enc;
    b.SerializeToString(&enc);

    tutorial::AddressBook b2;
    assert(b2.IsInitialized());
    b2.ParseFromString(enc);
    cout << b2.people(0).email() << endl;
    
    
    

    cout << "protobuf demo" << endl;
}
// sudo apt-get install -y protobuf-compiler
// sudo apt-get install -y libprotobuf-dev
//protoc --cpp_out=. test2.proto
//g++ pb2.cc test2.pb.cc -std=c++17 -lprotobuf