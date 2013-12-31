#include "sigcalc.h"

std::string addtrailer(const std::string & data, Tag2 * sig){
    std::string trailer = sig -> get_up_to_hashed();
    if (sig -> get_version() == 3){
        return data + trailer.substr(1, trailer.size() - 1) + unhexlify(makehex(sig -> get_time(), 8)); // remove version from trailer
    }
    else if (sig -> get_version() == 4){
        return data + trailer + "\x04\xff" + unhexlify(makehex(trailer.size(), 8));
    }
    else{
        std::cerr << "Error: addtrailer for version " << (int) sig -> get_version() << " not defined." << std::endl;
        throw(1);
    }
    return ""; // should never reach here; mainly just to remove compiler warnings
}

std::string overkey(Key * key){
    std::string key_str = key -> raw();
    // remove private data by copying over to Tag 6
    Tag6 tag6(key_str);
    key_str = tag6.raw();
    return "\x99" + unhexlify(makehex(key_str.size(), 4)) + key_str;
}

std::string certification(uint8_t version, ID * id){
    if (version == 3){
        return id -> raw();
    }
    else if (version == 4){
        std::string data = id -> raw();
        if (id -> get_tag() == 13){     // User ID packet
            return "\xb4" + unhexlify(makehex(data.size(), 8)) + data;
        }
        else if (id -> get_tag() == 17){// User Attribute packet
            return "\xd1" + unhexlify(makehex(data.size(), 8)) + data;
        }
    }
    else{
        std::cerr << "Error: Certification for version " << (int) version << " not defined." << std::endl;
        throw(1);
    }
    return ""; // should never reach here; mainly just to remove compiler warnings
}

std::string to_sign_00(const std::string & data, Tag2 * tag2){
    return use_hash(tag2 -> get_hash(), addtrailer(data, tag2));
}

std::string to_sign_01(const std::string & data, Tag2 * tag2){
    std::string out;
    // convert line endings ot <CR><LF>
    if (data[0] == '\n'){
        out = "\r\n";
    }
    else{
        out = std::string(1, data[0]);
    }
    for(unsigned int x = 1; x < data.size(); x++){
        if ((data[x] == '\n') && (data[x - 1] != '\r')){  // check to make sure lines aren't already <CR><LF>
            out += "\r";
        }
        out += std::string(1, data[x]);
    }
    return use_hash(tag2 -> get_hash(), addtrailer(out, tag2));
}

std::string to_sign_02(Tag2 * tag2){
    if (tag2 -> get_version() == 3){
        std::cerr << "Error: It does not make sense to have a V3 standalone signature." << std::endl;
        throw(1);
    }
    return use_hash(tag2 -> get_hash(), addtrailer("", tag2));
}

std::string to_sign_10(Key * key, ID * id, Tag2 * tag2){
    return use_hash(tag2 -> get_hash(), addtrailer(overkey(key) + certification(tag2 -> get_version(), id), tag2));
}

std::string to_sign_11(Key * key, ID * id, Tag2 * tag2){
    return to_sign_10(key, id, tag2);
}

std::string to_sign_12(Key * key, ID * id, Tag2 * tag2){
    return to_sign_10(key, id, tag2);
}

std::string to_sign_13(Key * key, ID * id, Tag2 * tag2){
    return to_sign_10(key, id, tag2);
}

std::string to_sign_18(Key * primary, Key * key, Tag2 * tag2){
    return use_hash(tag2 -> get_hash(), addtrailer(overkey(primary) + overkey(key), tag2));
}

std::string to_sign_19(Key * primary, Key * key, Tag2 * tag2){
    return use_hash(tag2 -> get_hash(), addtrailer(overkey(primary) + overkey(key), tag2));
}

std::string to_sign_1f(Tag2 * tag2){
    std::cerr << "Error: Signature directly on a key has not implemented." << std::endl;
    throw(1);
//    return use_hash(tag2 -> get_hash(), addtrailer("", tag2));
    return "";
}

std::string to_sign_20(Key * key, Tag2 * tag2){
    return use_hash(tag2 -> get_hash(), addtrailer(overkey(key), tag2));
}

std::string to_sign_28(Key * key, Tag2 * tag2){
    return use_hash(tag2 -> get_hash(), addtrailer(overkey(key), tag2));
}

std::string to_sign_30(Key * key, ID * id, Tag2 * tag2){
    return to_sign_10(key, id, tag2);
}

std::string to_sign_40(Tag2 * tag2){
    std::cerr << "Error: Signature directly on a key has not implemented." << std::endl;
    throw(1);
//    return use_hash(tag2 -> get_hash(), addtrailer("", tag2));
    return "";
}

std::string to_sign_50(Tag2 & sig, Tag2 * tag2){
    std::string data = sig.get_without_unhashed();
    return "\x88" + unhexlify(makehex(data.size(), 8)) + data;
}
