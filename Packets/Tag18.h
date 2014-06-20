// Sym. Encrypted and Integrity Protected Data Packet
#ifndef __TAG18__
#define __TAG18__

#include "packet.h"
class Tag18 : public Packet{
    private:
        std::string protected_data;

    public:
        typedef std::shared_ptr<Tag18> Ptr;

        Tag18();
        Tag18(std::string & data);
        void read(std::string & data);
        std::string show();
        std::string raw();

        std::string get_protected_data();

        void set_protected_data(const std::string & p);

        Packet::Ptr clone() const;
};
#endif
