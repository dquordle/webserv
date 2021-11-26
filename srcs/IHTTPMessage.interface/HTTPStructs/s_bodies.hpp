#ifndef S_BODIES_HPP
#define S_BODIES_HPP

#include <vector>
#include <string>
#include <iostream>

struct s_bodies {
	std::vector<std::string>	bodies;
	std::string					_full_request;
	std::string					_body;

	void print() const {
        std::vector<std::string>::const_iterator it = bodies.cbegin();
        std::vector<std::string>::const_iterator ite = bodies.cend();

        for (; it != ite; ++it) {
            std::cout << "|" << (*it);
        }
	}
};

#endif
