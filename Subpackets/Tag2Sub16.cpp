#include "Tag2Sub16.h"

Tag2Sub16::Tag2Sub16() :
    Subpacket(16, 8)
{
}

Tag2Sub16::Tag2Sub16(std::string & data) :
    Tag2Sub16()
{
    read(data);
}

void Tag2Sub16::read(std::string & data){
    keyid = data;
}

std::string Tag2Sub16::show(){
    return "            Key ID: " + hexlify(keyid) + "\n";
}

std::string Tag2Sub16::raw(){
    return keyid;
}

std::string Tag2Sub16::get_keyid(){
    return keyid;
}

void Tag2Sub16::set_keyid(const std::string & k){
    if (k.size() != 8){
        throw std::runtime_error("Error: Key ID must be 8 octets.");
    }
    keyid = k;
}

Subpacket::Ptr Tag2Sub16::clone() const{
    return Ptr(new Tag2Sub16(*this));
}
